#include <string.h>
#include "acg/alloc.h"

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))

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

#define ABUF_GET_UNSAFE(VAR, I) ((void*)((char*)VAR + VAR->off + VAR->size * I))
static inline void *abuf_get(abuf *abuf, u32 i)
{
	assert(abuf);
	assert(i < abuf->n);
	return ABUF_GET_UNSAFE(abuf, i);
}

#define ABUF_HEAD_UNSAFE(VAR) ((void*)((char*)VAR + VAR->off))
#define ABUF_HEAD(VAR) abuf_get(VAR, 0)
#define ABUF_TAIL(VAR) abuf_get(VAR, VAR->n - 1)
#define ABUF_I(VAR, E) (((char*)E - (char*)ABUF_HEAD(VAR)) / VAR->size)
#define ABUF_NPTR(VAR) ABUF_GET_UNSAFE(VAR, VAR->n)
#define ABUF_NEXT_UNSAFE(VAR, E) ((void*)((char*)E + VAR->size))
#define ABUF_PREV_UNSAFE(VAR, E) ((void*)((char*)E - VAR->size))

static inline void *abuf_next(abuf *abuf, void *entry)
{
	assert(abuf);
	if (!abuf->n)
		return NULL;
	if (!entry)
		return ABUF_HEAD(abuf);

	char *end = entry;
	char *beg = ABUF_HEAD_UNSAFE(abuf);

	return (u64)(end - beg) / abuf->size < abuf->n - 1?
			(void*)(end + abuf->size) : NULL;
}

static inline void *abuf_push(abuf *abuf)
{
	assert(abuf);
	assert(abuf->n < abuf->cap);
	return ABUF_HEAD_UNSAFE(abuf) + abuf->size * abuf->n++;
}

#define ABUF_RMSWAP(VAR, E) abuf_rmswap(VAR, E, sizeof(*E))
static inline void abuf_rmswap(abuf *abuf, void *entry, u32 size)
{
	assert(abuf);

	switch (abuf->n) {
	case 0:
		panic();
	default:
		memcpy(entry, abuf_get(abuf, abuf->n - 1), size);
	case 1:
		break;
	}

	--abuf->n;
}

static inline void *abuf_pop(abuf *abuf)
{
	assert(abuf);
	assert(abuf->n);
	return ABUF_HEAD_UNSAFE(abuf) + abuf->size * abuf->n--;
}

static void abuf_clear(abuf *abuf)
{
	assert(abuf);
	abuf->n = 0;
}

#define ABUF_ITER(VAR) for ( \
	struct { u32 i; void *val; } iter = { .val = ABUF_HEAD(VAR) }; \
	iter.i < VAR->n; \
	iter.val = ABUF_GET_UNSAFE(VAR, ++iter.i))
#define ABUF_FOREACH(VAR, T) \
	for (T *T = abuf_next(VAR, NULL); T; T = abuf_next(VAR, T))

#define VBUF(VAR, T, CAP) u32 VAR ## _n ; T VAR [ CAP ]
#define VBUF_MK(VAR, T, CAP) u32 VAR ## _n = 0; T VAR [ CAP ]
#define VBUF_INIT(VAR) VAR ## _n = 0

#define VBUF_I(VAR, ENTRY) (ENTRY - VAR)
#define VBUF_CAP(VAR) (sizeof(VAR) / sizeof(VAR[0]))

#define VBUF_PUSH(VAR) \
	(assert(VAR ## _n < VBUF_CAP(VAR)), VAR + VAR ## _n++)
#define VBUF_RMSWAP(VAR, ENTRY) \
	(assert(VBUF_I(VAR, ENTRY) < VAR ## _n), *ENTRY = VAR[--VAR ## _n])

#define VBUF_ITER(VAR) for ( \
	struct { u32 i; void *val; } iter = { .val = VAR }; \
	iter.i < VAR ## _n; \
	iter.val = ++iter.i + VAR)
#define VBUF_ENUM(VAR, E) \
	E = VAR ; for (u32 i = 0; i < VAR ## _n; E = ++i + VAR)
#define VBUF_FOREACH(VAR, T) \
	for (T *T = VAR; T - VAR < VAR ## _n; ++T)

// In-place counting sort, where k is the maximum value returned by keyof()
typedef u16 (*keyof)(void*);
static void sort_count(
	void *buf,
	u16 k,
	keyof keyof,
	const u32 n,
	const u32 size,
	const u32 align
) {
	assert(buf);
	assert(k);
	assert(size);
	assert(align);

	// Values of k should be small (otherwise, why are you using this?)
	// and therefore the stack should be fine.
	// This way, we're reetrant.
	u16 len = k + 1;
	u32 end[len * 2], *beg = end + len;
	char swap[size];

	memset(end, 0, sizeof(u32) * len);
	memset(beg, 0, sizeof(u32) * len);

	for (u32 i = 0; i < align * n; i += align) {
		void *entry = (char*)buf + i;
		u16 key = keyof(entry);
		++end[key];
	}

	for (u16 i = 1; i < len; ++i) {
		end[i] += end[i - 1];
		beg[i]  = end[i - 1];
	}

	for (u32 i = 0; i < align * n; i += align) {
		void *entry = (char*)buf + i;
		u16 key = keyof(entry);

		while (end[key] != beg[key]) {
			void *cmp = (char*)buf + align * --end[key];
			if (key == keyof(cmp))
				continue;

			// Swap
			void *target = (char*)buf + align * end[key];
			memcpy(  swap, target, size);
			memcpy(target,  entry, size);
			memcpy( entry,   swap, size);
			key = keyof(entry);
		}
	}
}

#define ABUF_SORT_COUNT(VAR, T, K, FN) \
	sort_count(ABUF_HEAD_UNSAFE(VAR), K, FN, VAR->n, sizeof(T), VAR->size)
