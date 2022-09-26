typedef struct {
	ff pos;
	float asp;
	float rot;
	float scale;
} cam2;

#define CAM2_DEFAULT ((cam2) { .asp = 1.f, .scale = 1.f })

static inline m4 cam2_conv(const cam2 cam)
{
	const m4 view = m4_view(
		v3_padxy(cam.pos, 0.f),
		qt_axis_angle(V3_FWD, cam.rot)
	);

	assert(cam.asp);
	assert(cam.scale);

	const m4 proj = m4_ortho(cam.scale, cam.asp, 0.f, 1.f);
	const m4 vp = m4_mul(proj, view);
	return vp;
}

typedef struct {
	v4 rot;
	v3 pos;
	float asp;
	float fov;
	float near;
	float far;
} cam3;

#define CAM3_DEFAULT ((cam3) { \
	.rot = QT_ID,          \
	.asp = 1.f,            \
	.fov = 60.f,           \
	.near = 1.f,           \
	.far = 1024.f })

static inline m4 cam3_conv_10(const cam3 cam)
{
	const m4 view = m4_view(cam.pos, cam.rot);
	const m4 proj = m4_persp_10(cam.fov, cam.asp, cam.near, cam.far);
	return m4_mul(proj, view);
}

static inline m4 cam3_conv_10_inf(const cam3 cam)
{
	const m4 view = m4_view(cam.pos, cam.rot);
	const m4 proj = m4_persp_10_inf(cam.fov, cam.asp, cam.near);
	return m4_mul(proj, view);
}
