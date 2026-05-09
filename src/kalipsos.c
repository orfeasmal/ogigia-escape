#include "kalipsos.h"

#include <raymath.h>

#define KALIPSOS_WIDTH 35.0f
#define KALIPSOS_HEIGHT 65.0f
#define KALIPSOS_COLOR WHITE

Kalipsos kalipsos_create(float x, float y)
{
	return (Kalipsos) {
		.body = {
			.x = x,
			.y = y,
			.width = KALIPSOS_WIDTH,
			.height = KALIPSOS_HEIGHT
		},
		.color = KALIPSOS_COLOR
	};
}

#define SPEED 100.0f

void kalipsos_update(Kalipsos *k, Player *player, Jsimplon_Object *json_root_object, float time_step)
{
	Vector2 player_minus_kalipsos_pos = (Vector2) {
		.x = (player->body.x + player->body.width / 2.0f) - (k->body.x + k->body.width / 2.0f),
		.y = (player->body.y + player->body.height / 2.0f) - (k->body.y + k->body.height / 2.0f)
	};
	float distance_from_player = Vector2Length(player_minus_kalipsos_pos);

	if (distance_from_player < 100.0f) {
		k->player_prev_state = player->state;

		k->state = KALIPSOS_QUESTIONING;
		player->state = PLAYER_BEING_QUESTIONED;

		k->vel = (Vector2) { 0.0f };
	}
	else {
		k->state = KALIPSOS_DOING_NOTHING;
		player->state = k->player_prev_state;
		k->vel = Vector2Scale(player_minus_kalipsos_pos, SPEED / distance_from_player);
	}

	k->body.x += k->vel.x * time_step;
	k->body.y += k->vel.y * time_step;

	if (k->state != KALIPSOS_QUESTIONING)
		return;

}

void kalipsos_render(const Kalipsos *k)
{
	DrawRectangleRec(k->body, k->color);
}
