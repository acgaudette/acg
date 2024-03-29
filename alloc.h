#ifndef ALLOC_H
#define ALLOC_H

#include <assert.h>
#include <errno.h>

#ifndef _WIN32
	#ifdef __linux__
		#define _DEFAULT_SOURCE 1
		#include <linux/mman.h>
	#endif
	#include <sys/mman.h>
#endif

#include "acg/types.h"
#include "acg/sys.h"

typedef enum {
	  ALLOC_NONE
	, ALLOC_SYS
	, ALLOC_VMEM
	, ALLOC_BUMP
} allocator;

static u32 align_up(u32 a, u32 to)
{
	assert(to);
	--to;
	return (a + to) & ~to;
}

static void *align_up_ptr(void *ptr, u32 to)
{
	assert(to);
	const uintptr_t a = (uintptr_t)ptr;
	const uintptr_t b = to - 1;
	return (void*)((a + b) & ~b);
}

static u32 next_pow(u32 a)
{
	--a;
	a |= a >> 1;
	a |= a >> 2;
	a |= a >> 4;
	a |= a >> 8;
	a |= a >> 16;
	++a;

	return a;
}

static void dump_vmem()
{
#ifdef __linux__
	FILE *smaps = fopen("/proc/self/smaps", "r");
	assert(smaps);

	char *str = NULL;
	size_t n;
	unsigned total = 0;

	while (-1 != getline(&str, &n, smaps)) {
		unsigned size;
		if (1 != sscanf(str, "Size: %u kB", &size))
			continue;
		total += size;
	}

	if (str) free(str);
	fclose(smaps);

	printf(
		"addressable vmem usage: %.1f MiB (~%.1f GiB)\n",
		total / (1024.f),
		total / (1024.f * 1024.f)
	);
#endif
}

#ifndef _WIN32
static void *alloc_vmem(u32 n)
{
	int flags = MAP_ANON | MAP_PRIVATE;
#ifdef __linux__
	flags += MAP_UNINITIALIZED;
#endif
	errno = 0;
	void *mem = mmap(
		NULL, n,
		PROT_READ | PROT_WRITE,
		flags,
		-1, 0
	);

	if (mem == (void*)-1) {
		assert(errno);
		perror(__func__);
		dump_vmem();
		panic();
	}

	return mem;
}
#endif

extern struct bump_mem {
	u32 i, size;
	char *src;
} bump_mem;

static void *alloc(u32, allocator);
static void bump_init(void *src, const u32 size)
{
	assert(size);
	bump_mem.size = size;

	if (src) {
		bump_mem.src = src;
		return;
	}

	bump_mem.src = alloc(size, ALLOC_SYS);
}

static void *bump_alloc(const u32 n)
{
	assert(bump_mem.src);
	assert(n);

	const u32 size = align_up(n, next_pow(n));
	assert(size);
	assert(size < UINT_MAX - bump_mem.i);

	char *result = bump_mem.src + bump_mem.i;
	bump_mem.i += size;

	assert(bump_mem.i <= bump_mem.size);
	assert(result);
	return result;
}

static void bump_clear()
{
	bump_mem.i = 0;
}

static void *alloc(u32 n, allocator allocator)
{
	assert(n);
	void *result;

	switch (allocator) {
	case ALLOC_SYS:
		result = malloc(n);
		assert(result);
		return result;
#ifndef _WIN32
	case ALLOC_VMEM:
		return alloc_vmem(n);
#endif
	case ALLOC_BUMP:
		return bump_alloc(n);
	default:
		panic();
	}
}

#endif
