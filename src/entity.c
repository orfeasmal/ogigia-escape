#include "entity.h"

#include <raymath.h>

void entity_update(Entity *e, float time_step)
{
	e->body.x += e->vel.x * time_step;
	e->body.y += e->vel.y * time_step;
}

void entity_render(const Entity *e)
{
	DrawRectangleRec(e->body, e->color);
}
