static float ease(const float s)
{
	assert(is01f(s));
	if (s < .5f) return s * s * s * 4.f;
	float a = -2.f * s + 2.f;
	return 1.f - a * a * a * .5f;
}

static float rvease(const float s)
{
	assert(is01f(s));
	float a = 1.6f;
	float b = a * s - .5f * a;
	return .5f + b * b * b;
}

// www.desmos.com/calculator/viem5x3qgo
static float bump(float t, const float height)
{
	t = clamp01f(t);

	float in = t - 1.f;
	float a = (height + 0.f) * in * in;
	float b = (height + 1.f) * in * in * in + 1.f;
	float c = a + b;
	return c;
}
