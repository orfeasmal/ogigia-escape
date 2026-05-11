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
			.x = ocean.x + 10.0f,
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
}
