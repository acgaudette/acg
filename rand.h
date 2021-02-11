static float randf()
{
	return clamp01f(rand() / (float)RAND_MAX);
}

static float srandf()
{
	return 2.f * randf() - 1.f;
}

static float randr(const float min, const float max)
{
	return randf() * (max - min) + min;
}

static char randc()
{
	return 'A' + 25 * randf();
}

#define RAND(N) static v ## N randv ## N () \
{ \
	v ## N v; \
	for (u8 i = 0; i < N; ++i) \
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
	for (u8 i = 0; i < N; ++i) \
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
	for (u8 i = 0; i < N; ++i) \
		v.s[i] = srandf(); \
	return v ## N ## _norm(v); \
}

DRAND(2)
DRAND(3)
DRAND(4)
#undef DRAND
