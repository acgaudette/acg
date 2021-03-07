#ifndef WHEEL_RATE_MAX
#define WHEEL_RATE_MAX .2f
#endif

#ifndef WHEEL_RATE_MIN
#define WHEEL_RATE_MIN .01f
#endif

#ifndef WHEEL_RAMP
#define WHEEL_RAMP .5f
#endif

typedef enum {
	  WHEEL_NONE  = 0x0
	, WHEEL_HL    = 0x1
	, WHEEL_AD    = 0x2
	, WHEEL_LR    = 0x4
	, WHEEL_HORIZ = 0x7
	, WHEEL_JK    = 0x8
	, WHEEL_SW    = 0x10
	, WHEEL_DU    = 0x20
	, WHEEL_VERT  = 0x38
	, WHEEL_BOTH  = 0x3f
	, WHEEL_QE    = 0x40
	, WHEEL_DPADH = 0x80
	, WHEEL_DPADV = 0x100
} wheel_f;

typedef struct {
	float rate;
	float timer;
	float pulse;
} wheel_rep;

i8 wheel_update(wheel_rep *rep, wheel_f flags)
{
	const float t = clamp01f(rep->timer / WHEEL_RAMP);
	rep->rate = lerpf(WHEEL_RATE_MAX, WHEEL_RATE_MIN, t);
	rep->pulse -= _time.dt.real;
	rep->timer += _time.dt.real;

	const u8 hl = flags & WHEEL_HL ? -1 : 0;
	const u8 ad = flags & WHEEL_AD ? -1 : 0;
	const u8 lr = flags & WHEEL_LR ? -1 : 0;
	const u8 jk = flags & WHEEL_JK ? -1 : 0;
	const u8 sw = flags & WHEEL_SW ? -1 : 0;
	const u8 du = flags & WHEEL_DU ? -1 : 0;
	const u8 qe = flags & WHEEL_QE ? -1 : 0;
	const u8 dh = flags & WHEEL_DPADH ? -1 : 0;
	const u8 dv = flags & WHEEL_DPADV ? -1 : 0;

	i8 single =
		  ((KEY_DOWN(L) - KEY_DOWN(H)) & hl)
		| ((KEY_DOWN(K) - KEY_DOWN(J)) & jk)
		| ((KEY_DOWN(D) - KEY_DOWN(A)) & ad)
		| ((KEY_DOWN(W) - KEY_DOWN(S)) & sw)
		| ((KEY_DOWN(E) - KEY_DOWN(Q)) & qe)
		| ((KEY_DOWN(RIGHT) - KEY_DOWN(LEFT)) & lr)
		| ((KEY_DOWN(UP)    - KEY_DOWN(DOWN)) & du)
		| ((PAD_DOWN(DPAD_RIGHT) - PAD_DOWN(DPAD_LEFT)) & dh)
		| ((PAD_DOWN(DPAD_UP)    - PAD_DOWN(DPAD_DOWN)) & dv);
	i8 multi =
		  ((KEY_HELD(L) - KEY_HELD(H)) & hl)
		| ((KEY_HELD(K) - KEY_HELD(J)) & jk)
		| ((KEY_HELD(D) - KEY_HELD(A)) & ad)
		| ((KEY_HELD(W) - KEY_HELD(S)) & sw)
		| ((KEY_HELD(E) - KEY_HELD(Q)) & qe)
		| ((KEY_HELD(RIGHT) - KEY_HELD(LEFT)) & lr)
		| ((KEY_HELD(UP)    - KEY_HELD(DOWN)) & du)
		| ((PAD_HELD(DPAD_RIGHT) - PAD_HELD(DPAD_LEFT)) & dh)
		| ((PAD_HELD(DPAD_UP)    - PAD_HELD(DPAD_DOWN)) & dv);

	i8 wheel = single;

	if (wheel) {
		rep->pulse = WHEEL_RATE_MAX;
		rep->timer = 0.f;
	} else if (rep->pulse < 0) {
		wheel = multi;
		rep->pulse = rep->rate;
	}

	return wheel;
}
