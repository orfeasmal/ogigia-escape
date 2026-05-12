#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>

#include <raylib.h>

#include "raft.h"
#include "plant.h"

typedef enum {
	PLAYER_DOING_NOTHING,
	PLAYER_PICKING_PLANT,
	PLAYER_DEPOSITING_TO_RAFT,
	PLAYER_BEING_QUESTIONED,
} PlayerState;

typedef struct {
	uint32_t plants_in_possesion[PLANT_COUNT];

	Rectangle body;

	PlayerState state;
	Color color;

	float timer;
	float current_cooldown;

	PlantType plant_being_picked_type;
	uint32_t plant_being_picked_index;

	uint32_t plants_that_can_be_interacted_with;
	bool can_interact_with_raft;
} Player;

Player player_create(float x, float y);
void player_update(
	Player *player,
	Plant *plants,
	uint32_t *plants_count,
	Raft *raft,
	Rectangle ocean,
	Sound *sounds,
	uint32_t window_height,
	float time_step
);
void player_render(const Player *player, uint32_t window_width, uint32_t window_height);

#endif // PLAYER_H
