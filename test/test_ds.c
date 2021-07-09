#include <stdio.h>
#include "acg/ds.h"

#define TEST_HDR(NAME) static void test_ ## NAME () \
{ \
	printf("\t%s\n\n", __func__);
#define TEST_FTR \
}

static void abuf_print(abuf *buf)
{
	printf(
		"size=%u\noff=%u\ncap=%u\nn=%u\n",
		buf->size,
		buf->off,
		buf->cap,
		buf->n
	);

	for (u32 i = 0; i < buf->n; ++i) {
		printf("buf[%u]=%d\n", i, *(int*)abuf_get(buf, i));
	}

	printf("\n");
}

TEST_HDR(abuf)
	abuf *buf;
	ABUF_MK(buf, int, 2, ALLOC_SYS);

	assert(buf->size == sizeof(int));
	assert(buf->off == sizeof(abuf));
	assert(buf->cap == 2);
	assert(!buf->n);

	*(int*)abuf_push(buf) = 87;

	assert(buf->size == sizeof(int));
	assert(buf->off == sizeof(abuf));
	assert(buf->cap == 2);
	assert(buf->n == 1);
	assert(*(int*)abuf_get(buf, 0) == 87);

	*(int*)abuf_push(buf) = 5;

	assert(buf->size == sizeof(int));
	assert(buf->off == sizeof(abuf));
	assert(buf->cap == 2);
	assert(buf->n == 2);
	assert(*(int*)abuf_get(buf, 0) == 87);
	assert(*(int*)abuf_get(buf, 1) == 5);

	abuf_print(buf);
TEST_FTR

struct pod {
	u32 a;
	float b;
	u64 c;
	double d;
};

TEST_HDR(abuf_vmem)
	abuf *buf;
	u32 size = 1024 * 2;
	ABUF_MK_MB(buf, struct pod, size, ALLOC_VMEM);

	struct pod *new = abuf_push(buf);
	new->a = 32;
	new->d = 16.f;

	assert(buf->size == sizeof(struct pod));
	assert(buf->off == buf->size);
	assert(buf->cap == (size * 1024 * 1024) / sizeof(struct pod));
	assert(buf->n == 1);
	assert(((struct pod*)abuf_get(buf, 0))->a == 32);
	assert(((struct pod*)abuf_get(buf, 0))->d == 16.f);

	abuf_print(buf);
TEST_FTR

static void vbuf_print(u64 *buf, u32 n)
{
	for (u32 i = 0; i < n; ++i) {
		printf("sizes[%u] = %lu\n", i, buf[i]);
	}

	printf("\n");
}

static u16 sizes_keyof(void *e)
{
	size_t size = *(size_t*)e;
	return (u16)size;
}

TEST_HDR(vbuf)
	VBUF_MK(sizes, u64, 64);

	*VBUF_PUSH(sizes) = 3;
	*VBUF_PUSH(sizes) = 1;
	*VBUF_PUSH(sizes) = 0;
	*VBUF_PUSH(sizes) = 3;
	*VBUF_PUSH(sizes) = 2;

	assert(sizes_n == 5);
	assert(sizes[0] == 3);
	assert(sizes[1] == 1);
	assert(sizes[2] == 0);
	assert(sizes[3] == 3);
	assert(sizes[4] == 2);

	vbuf_print(sizes, sizes_n);
	sort_count(sizes, 3, sizes_keyof, sizes_n, sizeof(u64), sizeof(u64));
	vbuf_print(sizes, sizes_n);
TEST_FTR

int main()
{
	test_abuf();
	test_abuf_vmem();
	test_vbuf();
}
