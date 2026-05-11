#ifndef KALIPSO_H
#define KALIPSO_H

#include <stdint.h>

#include <raylib.h>
#include <jsimplon.h>

#include "player.h"

typedef struct {
	Rectangle body;
	const char *str;
	uint32_t suspiciousness;
} Answer;

typedef enum {
	KALIPSOS_CHASING_PLAYER,
	KALIPSOS_QUESTIONING
} KalipsosState;

#define SUSPICION_MAX 100
#define ANSWERS_MAX_COUNT 4

typedef struct {
	Rectangle body;
	Jsimplon_Value *json_root_value;
	const char *question;
	Answer answers[ANSWERS_MAX_COUNT];
	uint32_t answers_count;

	Vector2 vel;
	Color color;

	float timer;

	KalipsosState state;
	PlayerState player_prev_state;
	uint8_t suspicion;
} Kalipsos;

Kalipsos kalipsos_create(float x, float y);
void kalipsos_destroy(Kalipsos *kalipsos);
void kalipsos_update(Kalipsos *kalipsos, Player *player, float time_step);
void kalipsos_render(const Kalipsos *kalipsos);

#endif // KALIPSO_H
