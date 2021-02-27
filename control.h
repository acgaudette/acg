typedef enum {
	  AXIS_NONE = 0x0
	, AXIS_HJKL = 0x1
	, AXIS_WASD = 0x2
	, AXIS_ARRW = 0x4
	, AXIS_ALL  = 0x7
} axis_f;

static v2 control_axis(axis_f flags)
{
	assert(flags);
	i8 hjkl = flags & AXIS_HJKL;
	i8 wasd = flags & AXIS_WASD;
	i8 arrw = flags & AXIS_ARRW;

	i8 lf = (KEY_HELD(H) && hjkl) | (KEY_HELD(A) && wasd)
		| (arrw && KEY_HELD(LEFT));
	i8 dn = (KEY_HELD(J) && hjkl) | (KEY_HELD(S) && wasd)
		| (arrw && KEY_HELD(DOWN));
	i8 up = (KEY_HELD(K) && hjkl) | (KEY_HELD(W) && wasd)
		| (arrw && KEY_HELD(UP));
	i8 rt = (KEY_HELD(L) && hjkl) | (KEY_HELD(D) && wasd)
		| (arrw && KEY_HELD(RIGHT));

	v2 axis = { rt - lf, up - dn };
	return axis.x || axis.y ? v2_norm(axis) : axis;
}
