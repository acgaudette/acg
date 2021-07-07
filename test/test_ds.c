#include <stdio.h>
#include "acg/ds.h"

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

static void test_1()
{
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
}

struct pod {
	u32 a;
	float b;
	u64 c;
	double d;
};

static void test_2()
{
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
}

int main()
{
	test_1();
	test_2();
}
