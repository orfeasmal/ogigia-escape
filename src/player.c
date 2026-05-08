#include <math.h>

#include "player.h"

#define PLAYER_WIDTH 35.0f
#define PLAYER_HEIGHT 75.0f
#define PLAYER_COLOR BLUE

#define VEL 400.0f

Entity player_create(float x, float y)
{
	return (Entity) {
		.body = {
			.width = PLAYER_WIDTH,
			.height = PLAYER_HEIGHT,
			.x = x,
			.y = y
		},
		.color = PLAYER_COLOR
	};
}

void player_update(Entity *p, uint32_t width, uint32_t height, float time_step)
{
	if (IsKeyDown(KEY_W))
		p->body.y -= VEL * time_step;

	if (IsKeyDown(KEY_S))
		p->body.y += VEL * time_step;

	if (IsKeyDown(KEY_A))
		p->body.x -= VEL * time_step;

	if (IsKeyDown(KEY_D))
		p->body.x += VEL * time_step;

	entity_update(p, width, height, time_step);
}
