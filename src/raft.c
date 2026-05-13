#include "raft.h"
#include "util.h"

#include <raymath.h>

#define RAFT_WIDTH 100.0f
#define RAFT_HEIGHT 100.0f

#define RAFT_COLOR BROWN

#define RAFT_TREES_NEEDED_INITIAL 10
#define RAFT_WEEDS_NEEDED_INITIAL 20

#define RAFT_TIME_REQUIRED_TO_DEPOSIT 3.0f

Raft raft_create(uint32_t window_height, Rectangle ocean)
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

void raft_render(const Raft *r, const Texture *textures)
{
	int32_t plants_needed_to_be_built_total = r->plants_needed_to_be_built[PLANT_TREE] + r->plants_needed_to_be_built[PLANT_WEED];

	float width_multiplier = fmaxf(0.0f, 1.0f - (float)plants_needed_to_be_built_total / (RAFT_TREES_NEEDED_INITIAL + RAFT_WEEDS_NEEDED_INITIAL));


	const Texture *raft_texture = &textures[TEXTURE_RAFT];
	Rectangle texture_rect = {
		.width = raft_texture->width,
		.height = raft_texture->height
	};

	//DrawRectangleLinesEx(texture_rect, 5.0f, r->color);

	Rectangle partial_body = r->body;
	partial_body.width *= width_multiplier;
	texture_rect.width *= width_multiplier;

	DrawTexturePro(*raft_texture, texture_rect, partial_body, (Vector2) { 0 }, 0.0f, WHITE);

	if (plants_needed_to_be_built_total <= 0)
		return;

	const char *wood_str = TextFormat("Wood needed: %d", r->plants_needed_to_be_built[PLANT_TREE]);
	DrawText(wood_str, r->body.x + r->body.width / 2.0f - MeasureText(wood_str, MATERIAL_REQUIREMENT_FONT_SIZE) / 2.0f, r->body.y - MATERIAL_REQUIREMENT_FONT_SIZE - 10, MATERIAL_REQUIREMENT_FONT_SIZE, BLACK);

	const char *string_str = TextFormat("String needed: %d", r->plants_needed_to_be_built[PLANT_WEED]);
	DrawText(string_str, r->body.x + r->body.width / 2.0f - MeasureText(string_str, MATERIAL_REQUIREMENT_FONT_SIZE) / 2.0f, r->body.y + r->body.height + 10, MATERIAL_REQUIREMENT_FONT_SIZE, BLACK);
}
