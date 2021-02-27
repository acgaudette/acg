struct fx_pull {
	/* Config */

	float damp;
	const enum _time_type type;

	/* Internal */

	float t;
};

static void pullf(struct fx_pull *pull, const float target)
{
	pull->t = lerpf_clamp(
		pull->t,
		target,
		_time_dt(pull->type) * pull->damp
	);
}

static void decayf(float *s, float amt)
{
	float f = *s;
	float mag = fabsf(f);

	if (mag < amt) {
		*s = 0.f;
		return;
	}

	*s = f > 0.f ? f - amt : f + amt;
}

static void decay_v2(v2 *v, float amt)
{
	if (!amt) return;

	v2 f = *v;
	float mag = v2_mag(f);

	if (mag < amt) {
		*v = V2_ZERO;
		return;
	}

	v2 dir = v2_norm(f);
	*v = v2_add(f, v2_mul(dir, -amt));
}

static void decay_v2xz(v3 *v, float amt)
{
	ff prox = v3_xz(*v);
	decay_v2(&prox, amt);
	v->x = prox.x;
	v->z = prox.y;
}
