static fff hsv_to_rgb(const float h, const float s, const float v)
{
	// Gray
	if (s == 0.f)
		return (fff) { v, v, v };

	float r, g, b;

	// Six color sections
	const float range = h * 6.f * (1.f - __FLT_EPSILON__);
	unsigned section = floorf(range);
	const float t = range - section;

	// Lower color value
	const float bot = v * (1.f - s);

	// Interpolated values
	const float up = v * (1.f - s * t);
	const float dn = v * (1.f - s * (1.f - t));

	switch (section) {
	case 0: // Red to yellow
		r = v;
		g = dn;
		b = bot;
		break;
	case 1: // Yellow to green
		r = up;
		g = v;
		b = bot;
		break;
	case 2: // Green to cyan
		r = bot;
		g = v;
		b = dn;
		break;
	case 3: // Cyan to blue
		r = bot;
		g = up;
		b = v;
		break;
	case 4: // Blue to purple
		r = dn;
		g = bot;
		b = v;
		break;
	default: // Purple to red
		r = v;
		g = bot;
		b = up;
		break;
	}

	return (fff) { r, g, b };
}
