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
