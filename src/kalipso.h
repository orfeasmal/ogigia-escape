#ifndef KALIPSO_H
#define KALIPSO_H

#include <stdint.h>

#include <raylib.h>
#include <jsimplon.h>

#include "player.h"

#define ANSWERS_MAX_COUNT 4
#define SUSPICION_MAX 100

typedef struct {
	Rectangle body;
	const char *str;
	uint32_t suspiciousness;
} Answer;

typedef enum {
	KALIPSO_CHASING_PLAYER,
	KALIPSO_QUESTIONING
} KalipsoState;

typedef struct {
	Rectangle body;
	Jsimplon_Value *json_root_value;
	const char *question;
	Answer answers[ANSWERS_MAX_COUNT];
	uint32_t answers_count;

	Vector2 vel;
	Color color;

	float timer;

	KalipsoState state;
	PlayerState player_prev_state;
	uint8_t suspicion;
} Kalipso;

Kalipso kalipso_create(float x, float y);
void kalipso_destroy(Kalipso *kalipso);
void kalipso_update(Kalipso *kalipso, Player *player, float time_step);
void kalipso_render(const Kalipso *kalipso);

#endif // KALIPSO_H
