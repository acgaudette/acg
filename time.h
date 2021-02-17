#ifndef MIN_DT
#define MIN_DT 1e-4
#endif

enum _time_type {
	  _T_NONE
	, _T_REAL
	, _T_GAME
	, _T_FIXED
};

extern struct _time {
	struct {
		float real;
		float game;
	} el;

	struct {
		float real;
		float game;
	} dt;

#ifdef FIXED_DT
	struct {
		float acc;
		float rem;
	} fixed;
#endif
	float scale; // Remember to initialize!
} _time;

static int _time_tick(float *t, float *dt)
{
	_time.el.real = *t;
	_time.dt.real = *dt;

	*dt *= _time.scale;
	if (*dt < MIN_DT) *dt = 0.f;

	_time.dt.game  = *dt;
	_time.el.game += *dt;
	*t = _time.el.game;

#ifdef FIXED_DT
	_time.fixed.acc += _time.dt.game;
#endif
	return *dt > 0.f;
}

#ifdef FIXED_DT
static int _time_step()
{
	if (_time.fixed.acc < FIXED_DT) {
		_time.fixed.rem = _time.fixed.acc / FIXED_DT;
		assert(is01f(_time.fixed.rem));
		return 0;
	}

	_time.fixed.acc -= FIXED_DT;
	return 1;
}
#endif

static inline float _time_dt(enum _time_type e)
{
	switch (e) {
	case _T_REAL:
		return _time.dt.real;
	case _T_GAME:
		return _time.dt.game;
	case _T_FIXED:
#ifdef FIXED_DT
		return FIXED_DT;
#endif
	default:
		assert(0);
	}
}

static inline float _time_el(enum _time_type e)
{
	switch (e) {
	case _T_REAL:
		return _time.el.real;
	case _T_GAME:
		return _time.el.game;
	default:
		assert(0);
	}
}
