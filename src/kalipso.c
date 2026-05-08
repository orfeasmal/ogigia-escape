#include "kalipso.h"

#define KALIPSO_WIDTH 35.0f
#define KALIPSO_HEIGHT 65.0f
#define KALIPSO_COLOR WHITE

Entity kalipso_create(float x, float y)
{
	return (Entity) {
		.body = {
			.x = x,
			.y = y,
			.width = KALIPSO_WIDTH,
			.height = KALIPSO_HEIGHT
		},
		.type = ENTITY_KALIPSO,
		.color = KALIPSO_COLOR
	};
}

void kalipso_update(Entity *k, float time_step)
{
	entity_update(k, time_step);
}
