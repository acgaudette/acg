struct eye {
	v4 rot;
	v3 pos;
	float fov;
	float asp;
};

// - Anchor ranges from -1 to +1
// - Scale: 1 unit = 1 "screen height"
static ttt screen_to_world(const ttt in, const ff anchor, struct eye eye)
{
	const ttt view = {
		.scale = 1.f,
		.pos = eye.pos,
		.rot = eye.rot,
	};

	const float scale = tanf(.5f * eye.fov * M_PI / 180.f);
	const ttt screen = {
		.scale = 2.f * scale,
		.pos = {
			anchor.x * scale * eye.asp,
			anchor.y * scale,
			1.f,
		},
		.rot = QT_ID,
	};

	return t3_app(view, t3_app(screen, in));
}
