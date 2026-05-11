#include "raft.h"

#include <raymath.h>

#define RAFT_WIDTH 100.0f
#define RAFT_HEIGHT 65.0f

#define RAFT_COLOR BROWN

#define RAFT_TREES_NEEDED_INITIAL 10
#define RAFT_WEEDS_NEEDED_INITIAL 25

#define RAFT_TIME_REQUIRED_TO_DEPOSIT 3.0f

Raft raft_create(uint32_t window_width, uint32_t window_height, Rectangle ocean)
{
	return (Raft) {
		.body = {
			.x = ocean.x + RAFT_WIDTH / 2.0f,
			.y = window_height / 2.0f - RAFT_HEIGHT / 2.0f,
			.width = RAFT_WIDTH,
			.height = RAFT_HEIGHT
		},
		.color = RAFT_COLOR,
		.time_needed_to_deposit = RAFT_TIME_REQUIRED_TO_DEPOSIT,
		.plants_needed_to_be_built = {
			[PLANT_TREE] = RAFT_TREES_NEEDED_INITIAL,
			[PLANT_WEED] = RAFT_WEEDS_NEEDED_INITIAL
		}
	};
}

#define MATERIAL_REQUIREMENT_FONT_SIZE 20.0f

void raft_render(const Raft *r)
{
	Rectangle partial_raft_body = r->body;
	partial_raft_body.width *= fmaxf(
		0.0f, 
		1.0f -
		(float)(r->plants_needed_to_be_built[PLANT_TREE] + r->plants_needed_to_be_built[PLANT_WEED]) /
		(RAFT_TREES_NEEDED_INITIAL + RAFT_WEEDS_NEEDED_INITIAL)
	);

	DrawRectangleLinesEx(r->body, 5.0f, r->color);
	DrawRectangleRec(partial_raft_body, r->color);

	const char *wood_str = TextFormat("Wood needed: %d", r->plants_needed_to_be_built[PLANT_TREE]);
	DrawText(wood_str, r->body.x + r->body.width / 2.0f - MeasureText(wood_str, MATERIAL_REQUIREMENT_FONT_SIZE) / 2.0f, r->body.y - MATERIAL_REQUIREMENT_FONT_SIZE - 10, MATERIAL_REQUIREMENT_FONT_SIZE, BLACK);

	const char *string_str = TextFormat("String needed: %d", r->plants_needed_to_be_built[PLANT_WEED]);
	DrawText(string_str, r->body.x + r->body.width / 2.0f - MeasureText(string_str, MATERIAL_REQUIREMENT_FONT_SIZE) / 2.0f, r->body.y + r->body.height + 10, MATERIAL_REQUIREMENT_FONT_SIZE, BLACK);
}
