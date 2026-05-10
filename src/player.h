#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>

#include <raylib.h>

#include "plant.h"

typedef enum {
	PLAYER_DOING_NOTHING,
	PLAYER_CUTTING_TREE,
	PLAYER_PICKING_WEEDS,
	PLAYER_BUILDING_RAFT,
	PLAYER_BEING_QUESTIONED,
} PlayerState;

typedef struct {
	Rectangle body;
	Vector2 vel;

	PlayerState state;
	Color color;

	float timer;

	uint32_t plants_in_possesion[PLANT_COUNT]; // for the player
} Player;

Player player_create(float x, float y);
void player_update(Player *player, Plant *plants, uint32_t *plants_count, float time_step);
void player_render(const Player *player);

#endif // PLAYER_H
