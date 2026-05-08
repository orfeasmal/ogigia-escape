#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <stdbool.h>

#include <raylib.h>

// I put it here because I want to do something cool
typedef enum {
	MATERIAL_TREE,
	MATERIAL_PLANT,
	MATERIAL_COUNT
} MaterialType;

typedef enum {
	ENTITY_PLAYER,
	ENTITY_KALIPSO,
	ENTITY_MATERIAL
} EntityType;

typedef struct entity {
	Rectangle body;

	EntityType type;
	Color color;

	Vector2 vel;

	MaterialType material_type; // for materials
	uint32_t materials_in_possesion[MATERIAL_COUNT]; // for the player
} Entity;

void entity_update(Entity *entity, float time_step);
void entity_render(const Entity *entity);

#endif // ENTITY_H
