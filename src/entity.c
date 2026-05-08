#include "entity.h"

#include <raymath.h>

void entity_update(Entity *e, uint32_t width, uint32_t height, float time_step)
{
	e->vel = Vector2Add(
		e->vel,
		Vector2Scale(e->acc, time_step)
	);

	e->body.x += e->vel.x * time_step;
	e->body.y += e->vel.y * time_step;
}

void entity_render(const Entity *e)
{
	DrawRectangleRec(e->body, e->color);
}
