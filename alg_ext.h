static inline fff v2_dirdist(const ff from, const ff to)
{
	const ff diff = v2_sub(to, from);
	const float dist = v2_mag(diff);
	const ff dir = dist > __FLT_MIN__ ? v2_mul(diff, 1.f / dist) : V2_UP;
	return (fff) { .xy = dir, dist };
}
