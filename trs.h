typedef struct {
	float scale;
	v3 pos;
	v4 rot;
} t3;
typedef t3 ttt;

typedef struct {
	v2 pos;
	float scale;
	float angle;
} t2;
typedef t2 tt;

static inline t3 t3_app(t3 parent, t3 child)
{
	return (t3) {
		.scale = parent.scale * child.scale,
		.pos = v3_add(
			parent.pos,
			qt_app(parent.rot, v3_mul(child.pos, parent.scale))
		),
		.rot = qt_mul(parent.rot, child.rot),
	};
}
