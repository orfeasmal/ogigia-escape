#include "material.h"

#define MATERIAL_WIDTH 25.0f
#define MATERIAL_HEIGHT 25.0f
#define MATERIAL_COLOR BLACK

Entity material_create(MaterialType type, float x, float y)
{
	return (Entity) {
		.body = {
			.x = x,
			.y = y,
			.width = MATERIAL_WIDTH,
			.height = MATERIAL_HEIGHT
		},
		.type = ENTITY_MATERIAL,
		.color = MATERIAL_COLOR
	};
}
