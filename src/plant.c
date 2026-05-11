#include <stdio.h>
#include <stdlib.h>

#include "plant.h"

#define PLANT_WIDTH 25.0f
#define PLANT_HEIGHT 25.0f
#define PLANT_COLOR BLACK

#define TREE_TIME 5.0f
#define WEED_TIME 2.0f

Plant plant_create(PlantType type, float x, float y)
{
	Plant plant = {
		.body = {
			.x = x,
			.y = y,
			.width = PLANT_WIDTH,
			.height = PLANT_HEIGHT
		},
		.type = type,
		.color = PLANT_COLOR
	};

	switch (type) {
		case PLANT_TREE:
			plant.time_needed_to_pick = TREE_TIME;
			break;
		case PLANT_WEED:
			plant.time_needed_to_pick = WEED_TIME;
			break;
		default:
			fprintf(stderr, "internal error: unreachable plant type in plant_create\n");
			exit(EXIT_FAILURE);
			break;
	}

	return plant;
}

void plant_render(const Plant *p)
{
	DrawRectangleRec(p->body, p->color);
}
