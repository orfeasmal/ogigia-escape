#ifndef KALIPSO_H
#define KALIPSO_H

#include <stdint.h>

#include <raylib.h>
#include <jsimplon.h>

#include "player.h"

typedef enum {
	KALIPSOS_DOING_NOTHING,
	KALIPSOS_QUESTIONING
} KalipsosState;

typedef struct {
	Rectangle body;
	Vector2 vel;
	Color color;

	KalipsosState state;
	PlayerState player_prev_state;
	uint8_t suspicion;
} Kalipsos;

Kalipsos kalipsos_create(float x, float y);
void kalipsos_update(Kalipsos *kalipsos, Player *player, Jsimplon_Object *json_root_object, float time_step);
void kalipsos_render(const Kalipsos *kalipsos);

#endif // KALIPSO_H
