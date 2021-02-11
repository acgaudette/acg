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
