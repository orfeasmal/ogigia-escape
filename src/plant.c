#include "plant.h"

#define PLANT_WIDTH 25.0f
#define PLANT_HEIGHT 25.0f
#define PLANT_COLOR BLACK

Plant plant_create(PlantType type, float x, float y)
{
	return (Plant) {
		.body = {
			.x = x,
			.y = y,
			.width = PLANT_WIDTH,
			.height = PLANT_HEIGHT
		},
		.type = type,
		.color = PLANT_COLOR
	};
}

void plant_render(const Plant *p)
{
	DrawRectangleRec(p->body, p->color);
}
