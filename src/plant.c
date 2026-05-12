#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "plant.h"

#define TREE_WIDTH 60.0f
#define TREE_HEIGHT (TREE_WIDTH * 2.0f)

#define WEED_WIDTH 30.0f
#define WEED_HEIGHT WEED_WIDTH

#define TREE_COLOR GREEN
#define WEED_COLOR LIME

#define TREE_TIME 5.0f
#define WEED_TIME 2.0f

Plant plant_create(PlantType type, float x, float y)
{
	Plant p = {
		.body = {
			.x = x,
			.y = y,
		},
		.type = type
	};

	switch (type) {
		case PLANT_TREE:
			p.body.width = TREE_WIDTH;
			p.body.height = TREE_HEIGHT;
			p.color = TREE_COLOR;
			p.time_needed_to_pick = TREE_TIME;
			break;
		case PLANT_WEED:
			p.body.width = WEED_WIDTH;
			p.body.height = WEED_HEIGHT;
			p.color = WEED_COLOR;
			p.time_needed_to_pick = WEED_TIME;
			break;
		default:
			fprintf(stderr, "internal error: unreachable plant type in plant_create\n");
			exit(EXIT_FAILURE);
			break;
	}

	return p;
}

void plant_render(const Plant *p, const Texture *textures)
{
	const Texture *texture = NULL;

	if (p->type == PLANT_TREE)
		texture = &textures[TEXTURE_TREE];
	else if (p->type == PLANT_WEED)
		texture = &textures[TEXTURE_WEED];

	Rectangle texture_rect = {
		.width = texture->width,
		.height = texture->height
	};
	DrawTexturePro(*texture, texture_rect, p->body, (Vector2) { 0 }, 0.0f, WHITE);
}
