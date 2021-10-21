typedef enum {
	  AXIS_NONE = 0x0
	, AXIS_HL   = 0x1
	, AXIS_KJ   = 0x2
	, AXIS_HJKL = 0x3
	, AXIS_AD   = 0x4
	, AXIS_WS   = 0x8
	, AXIS_WASD = 0xc
	, AXIS_LR   = 0x10
	, AXIS_UD   = 0x20
	, AXIS_ARRW = 0x30
	, AXIS_ALL  = 0x3f
	, AXIS_HORZ = 0x15
	, AXIS_VERT = 0x2a
} axis_f;

static v2 control_axis(axis_f flags)
{
	const i8 hl = flags & AXIS_HL;
	const i8 kj = flags & AXIS_KJ;
	const i8 ad = flags & AXIS_AD;
	const i8 ws = flags & AXIS_WS;
	const i8 lr = flags & AXIS_LR;
	const i8 ud = flags & AXIS_UD;

	i8 lf = (KEY_HELD(H) && hl) | (KEY_HELD(A) && ad)
		| (lr && KEY_HELD(LEFT));
	i8 rt = (KEY_HELD(L) && hl) | (KEY_HELD(D) && ad)
		| (lr && KEY_HELD(RIGHT));
	i8 up = (KEY_HELD(K) && kj) | (KEY_HELD(W) && ws)
		| (ud && KEY_HELD(UP));
	i8 dn = (KEY_HELD(J) && kj) | (KEY_HELD(S) && ws)
		| (ud && KEY_HELD(DOWN));

	v2 axis = { rt - lf, up - dn };
	return axis.x || axis.y ? v2_norm(axis) : axis;
}

static i8 control_1(axis_f flags)
{
	const i8 hl = flags & AXIS_HL;
	const i8 kj = flags & AXIS_KJ;
	const i8 ad = flags & AXIS_AD;
	const i8 ws = flags & AXIS_WS;
	const i8 lr = flags & AXIS_LR;
	const i8 ud = flags & AXIS_UD;

	i8 pos = (KEY_HELD(L) && hl)
	       | (KEY_HELD(D) && ad)
	       | (KEY_HELD(K) && kj)
	       | (KEY_HELD(W) && ws)
	       | (lr && KEY_HELD(RIGHT))
	       | (ud && KEY_HELD(UP));
	i8 neg = (KEY_HELD(H) && hl)
	       | (KEY_HELD(A) && ad)
	       | (KEY_HELD(J) && kj)
	       | (KEY_HELD(S) && ws)
	       | (lr && KEY_HELD(LEFT))
	       | (ud && KEY_HELD(DOWN));

	return pos - neg;
}
