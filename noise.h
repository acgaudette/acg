static inline float noise2(float x, float y)
{
	float a = 128;
	float b = 5363;
	float c = 4576;
	float s = sin(x * a + y * b) * c; // FIXME
	float f = fmodf(s, 1.f);
	return f;
}

static inline float noise3(float x, float y, float z)
{
	float a = 256;
	float b = 4576;
	float c = 5363;
	float d = 3451;
	float s = (1 + .5f * sin(x * a + y * b + z * c)) * d;
	float f = fmodf(s, 1.f);
	return f;
}

static inline float value(float x, float y)
{
	x *= .1f;
	y *= .1f;

	float i = floorf(x);
	float j = floorf(y);

	float xx = fmodf(x, 1.f);
	float yy = fmodf(y, 1.f);
	xx *= xx * (3.f - 2.f * xx);
	yy *= yy * (3.f - 2.f * yy);

	float a = noise2(i + 0, j + 0);
	float b = noise2(i + 0, j + 1);
	float c = noise2(i + 1, j + 1);
	float d = noise2(i + 1, j + 0);

	float ab = lerpf(a, b, yy);
	float dc = lerpf(d, c, yy);

	float r = lerpf(ab, dc, xx);
	return r;
}

static inline float fbm(
	float x, float y,
	float sum,
	float amp,
	float frq,
	float lac,
	float pwr,
	u8 oct
) {
	for (u8 i = 0; i < oct; ++i) {
		sum += amp * value(x * frq, y * frq);
		frq *= lac;
		amp *= pwr;
	}

	return sum;
}
