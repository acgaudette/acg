static float randf() // Inclusive
{
#ifndef ACG_RAND_NO_ALG
	return clamp01f(rand() / (float)RAND_MAX);
#else
	const float result = rand() / (float)RAND_MAX;
	return result > 1.f ? 1.f :
	      (result < 0.f ? 0.f : result);
#endif
}

static float randfx() // Exclusive at max
{
	return fmodf(randf(), 1.f);
}

static int randb()
{
	return randf() > .5f;
}

static float srandf()
{
	return 2.f * randf() - 1.f;
}

static float randr(const float min, const float max) // Inclusive
{
	assert(max >= min);
	return randf() * (max - min) + min;
}

static char randc()
{
	return 'A' + 25 * randf();
}

static u32 randu32(
	  const u32 min // Inclusive
	, const u32 max // Exclusive
) {
	assert(max > min);
	return rand() % (max - min) + min;
}

#ifndef ACG_RAND_NO_ALG
#define RAND(N) static v ## N randv ## N () \
{ \
	v ## N v; \
	for (size_t i = 0; i < N; ++i) \
		v.s[i] = randf(); \
	return v; \
}

RAND(2)
RAND(3)
RAND(4)
#undef RAND

#define SRAND(N) static v ## N srandv ## N () \
{ \
	v ## N v; \
	for (size_t i = 0; i < N; ++i) \
		v.s[i] = srandf(); \
	return v; \
}

SRAND(2)
SRAND(3)
SRAND(4)
#undef SRAND

#define DRAND(N) static v ## N drandv ## N () \
{ \
	v ## N v; \
	for (size_t i = 0; i < N; ++i) \
		v.s[i] = srandf(); \
	return v ## N ## _norm(v); \
}

DRAND(2)
DRAND(3)
DRAND(4)
#undef DRAND
#endif
