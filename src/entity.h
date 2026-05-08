#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <stdbool.h>

#include <raylib.h>

typedef struct {
	Rectangle body;
	Vector2 vel, acc;
	Color color;

	bool on_ground;
	bool gravity;
} Entity;

void entity_update(Entity *entity, uint32_t width, uint32_t height, float time_step);
void entity_render(const Entity *entity);

#endif // ENTITY_H
