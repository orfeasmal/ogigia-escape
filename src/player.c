#include <math.h>

#include "player.h"

#define ACC 750.0f
#define VELX_MAX 400.0f
#define JUMP_VEL 350.0f

void player_update(Entity *p, uint32_t width, uint32_t height, float time_step)
{
	if (p->on_ground && IsKeyDown(KEY_SPACE))
		p->vel.y -= JUMP_VEL;

	if (IsKeyDown(KEY_A))
		p->vel.x -= ACC * time_step;
	if (IsKeyDown(KEY_D))
		p->vel.x += ACC * time_step;

	float velx_abs = fabs(p->vel.x);
	if (velx_abs >= VELX_MAX)
		p->vel.x = (p->vel.x / velx_abs) * VELX_MAX;

	entity_update(p, width, height, time_step);
}
