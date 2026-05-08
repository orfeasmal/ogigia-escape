#include "entity.h"

#include <raymath.h>

#define G 500.0f

void entity_update(Entity *e, uint32_t width, uint32_t height, float time_step)
{
	if (e->gravity) {
		if (!e->on_ground)
			e->vel.y += G * time_step;
	}

	e->vel = Vector2Add(
		e->vel,
		Vector2Scale(e->acc, time_step)
	);

	e->body.x += e->vel.x * time_step;
	e->body.y += e->vel.y * time_step;

	if (e->body.y + e->body.height >= height) {
		e->body.y = height - e->body.height;
		e->vel.y = 0.0f;

		e->on_ground = true;
	}
	else
		e->on_ground = false;
}

void entity_render(const Entity *e)
{
	DrawRectangleRec(e->body, e->color);
}
