#include <math.h>
#include <stdio.h>

#include "player.h"

#include <raymath.h>

#define PLAYER_WIDTH 35.0f
#define PLAYER_HEIGHT 75.0f
#define PLAYER_COLOR BLUE

#define VEL 400.0f

Player player_create(float x, float y)
{
	return (Player) {
		.body = {
			.width = PLAYER_WIDTH,
			.height = PLAYER_HEIGHT,
			.x = x,
			.y = y
		},
		.color = PLAYER_COLOR,
	};
}

void player_update(Player *p, Plant *plants, uint32_t *plants_count, float time_step)
{
	for (uint32_t i = 0; i < *plants_count; ++i) {
		Plant *plant = &plants[i];

		float distance_from_plant = Vector2Length(
			(Vector2) {
				.x = (p->body.x + p->body.width / 2.0f) - (plant->body.x + plant->body.width / 2.0f),
				.y = (p->body.y + p->body.height / 2.0f) - (plant->body.y + plant->body.height / 2.0f)
			}
		);

		if (distance_from_plant <= p->body.width * 2.5f) {
			if (IsKeyPressed(KEY_E)) {
				++p->plants_in_possesion[plant->type];
				plants[i] = plants[--(*plants_count)];
				printf("%d\n", p->plants_in_possesion[0]);
			}
		}
	}

	if (IsKeyDown(KEY_W))
		p->body.y -= VEL * time_step;
	if (IsKeyDown(KEY_S))
		p->body.y += VEL * time_step;
	if (IsKeyDown(KEY_A))
		p->body.x -= VEL * time_step;
	if (IsKeyDown(KEY_D))
		p->body.x += VEL * time_step;
}

void player_render(const Player *p)
{
	DrawRectangleRec(p->body, p->color);
}
