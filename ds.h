#include "acg/alloc.h"

#define MAX(A, B) ((A) > (B) ? (A) : (B))

typedef struct {
	u32 size;
	u32  off;
	u32  cap;
	u32    n;
} abuf;

#define ABUF_CAP_STR(CAP) \
	CAP > 1e6f, \
	CAP > 1e6f ? CAP / 1e6f : (CAP > 1e4f ? CAP / 1e3f : CAP), \
	CAP > 1e6f ? "M" : (CAP > 1e4f ? "K" : "")
#define ABUF_MK_LOG(VAR, T) \
printf( \
	"abuf \"" #VAR "\" of type " #T ": %.*f%s elements, %.1f %s\n", \
	ABUF_CAP_STR(VAR->cap), \
	(VAR->off + VAR->size * VAR->cap) / (1024.f * 1024.f), "MiB"), \
dump_vmem()

#define ABUF_MK(VAR, T, CAP, ALLOCATOR) \
	VAR = abuf_mk(CAP, ALLOCATOR, sizeof(T), 1), \
	ABUF_MK_LOG(VAR, T)
#define ABUF_MK_MB(VAR, T, MB, ALLOCATOR) \
	assert(MB < 4096), \
	VAR = abuf_mk((MB * 1024 * 1024) / sizeof(T), ALLOCATOR, sizeof(T), 1), \
	ABUF_MK_LOG(VAR, T)

static abuf *abuf_mk(
	const u64 cap,
	allocator allocator,
	u64 size,
	const u64 align
) {
	assert(cap);
	size = align_up(size, align);
	assert(size);
	u64 off = MAX(size, sizeof(abuf));

	u64 total = size * cap + off;
	assert(total <= ~(u32)0);
	abuf *abuf = alloc(total, allocator);

	abuf->size = size;
	abuf->off = off;
	abuf->cap = cap;
	abuf->n = 0;

	return abuf;
}

static inline void *abuf_get(abuf *abuf, u32 i)
{
	assert(abuf);
	assert(i < abuf->n);
	return (char*)abuf + abuf->off + abuf->size * i;
}

static inline void *abuf_push(abuf *abuf)
{
	assert(abuf);
	assert(abuf->n < abuf->cap);
	return (char*)abuf + abuf->off + abuf->size * abuf->n++;
}
