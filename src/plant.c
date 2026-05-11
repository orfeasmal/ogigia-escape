#include <stdio.h>
#include <stdlib.h>

#include "plant.h"

#define TREE_WIDTH 25.0f
#define TREE_HEIGHT 50.0f

#define WEED_WIDTH 20.0f
#define WEED_HEIGHT 20.0f

#define TREE_COLOR GREEN
#define WEED_COLOR LIME

#define TREE_TIME 5.0f
#define WEED_TIME 2.0f

Plant plant_create(PlantType type, float x, float y)
{
	Plant plant = {
		.body = {
			.x = x,
			.y = y,
		},
		.type = type
	};

	switch (type) {
		case PLANT_TREE:
			plant.body.width = TREE_WIDTH;
			plant.body.height = TREE_HEIGHT;
			plant.color = TREE_COLOR;
			plant.time_needed_to_pick = TREE_TIME;
			break;
		case PLANT_WEED:
			plant.body.width = WEED_WIDTH;
			plant.body.height = WEED_HEIGHT;
			plant.color = WEED_COLOR;
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
