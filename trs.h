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

#define T3_V(V) ((t3) { .scale = 1.f, .pos = V, .rot = QT_ID })
#define T3_ID   ((t3) { .scale = 1.f,           .rot = QT_ID })

static inline t3 t3_app(const t3 parent, const t3 child)
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

static inline v3 t3_app_v(const t3 parent, const v3 v)
{
	return t3_app(parent, T3_V(v)).pos;
}

static inline m4 t3_to_m4(const t3 trs)
{
	return m4_model(trs.pos, trs.rot, trs.scale);
}
