#include <string.h>
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

static inline void *abuf_get_raw(abuf *abuf)
{
	assert(abuf);
	return abuf_get(abuf, 0);
}

static inline void *abuf_push(abuf *abuf)
{
	assert(abuf);
	assert(abuf->n < abuf->cap);
	return (char*)abuf + abuf->off + abuf->size * abuf->n++;
}

#define VBUF(VAR, T, CAP) u32 VAR ## _n ; T VAR [ CAP ]
#define VBUF_MK(VAR, T, CAP) u32 VAR ## _n = 0; T VAR [ CAP ]
#define VBUF_INIT(VAR) VAR ## _n = 0
#define VBUF_PUSH(VAR) \
	(assert(VAR ## _n < sizeof(VAR) / sizeof(VAR[0])), VAR + VAR ## _n++)

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
