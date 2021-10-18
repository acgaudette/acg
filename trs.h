typedef struct {
	v2 pos;
	float scale;
	float angle;
} t2;
typedef t2 tt;

#define T2_ID ((tt) { .scale = 1.f })

typedef struct {
	float scale;
	v3 pos;
	v4 rot;
} t3;
typedef t3 ttt;

#define T3_ID ((ttt) { .scale = 1.f, .rot = QT_ID })

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

static inline m4 t3_to_m4(t3 trs)
{
	return m4_model(trs.pos, trs.rot, trs.scale);
}
