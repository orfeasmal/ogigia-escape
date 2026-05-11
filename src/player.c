#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "player.h"

#include <raymath.h>

#define PLAYER_WIDTH 35.0f
#define PLAYER_HEIGHT 75.0f
#define PLAYER_COLOR BLUE

#define SPEED 200.0f

#define TREE_TIME 5.0f
#define WEED_TIME 2.0f

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
	if (p->state == PLAYER_DOING_NOTHING) {
		if (IsKeyPressed(KEY_E)) {
			for (uint32_t i = 0; i < *plants_count; ++i) {
				Plant *plant = &plants[i];

				float distance_from_plant = Vector2Length(
					(Vector2) {
						.x = (p->body.x + p->body.width / 2.0f) - (plant->body.x + plant->body.width / 2.0f),
						.y = (p->body.y + p->body.height / 2.0f) - (plant->body.y + plant->body.height / 2.0f)
					}
				);

				if (distance_from_plant <= p->body.width * 2.5f) {
					p->plant_being_picked = *plant;
					p->plant_being_picked_index = i;
					p->timer = 0.0f;

					p->state = PLAYER_PICKING_PLANT;

					break;
				}
			}
		}

		if (IsKeyDown(KEY_W))
			p->body.y -= SPEED * time_step;
		if (IsKeyDown(KEY_S))
			p->body.y += SPEED * time_step;
		if (IsKeyDown(KEY_A))
			p->body.x -= SPEED * time_step;
		if (IsKeyDown(KEY_D))
			p->body.x += SPEED * time_step;

	}
	else if (p->state == PLAYER_PICKING_PLANT) {
		if (p->timer >= p->plant_being_picked.time_needed_to_pick) {
			++p->plants_in_possesion[p->plant_being_picked.type];
			plants[p->plant_being_picked_index] = plants[--(*plants_count)];

			p->state = PLAYER_DOING_NOTHING;
		}
	}

	if (p->state != PLAYER_BEING_QUESTIONED)
		p->timer += time_step;
}

void player_render(const Player *p)
{
	if (p->state == PLAYER_PICKING_PLANT) {
		Rectangle timer_bar_outline = {
			.x = p->body.x - p->body.width / 2.0f,
			.y = p->body.y - p->body.height / 2.0f,
			.width = p->body.height,
			.height = p->body.width / 1.5f
		};

		Rectangle timer_bar = timer_bar_outline;
		timer_bar.width *= p->timer / p->plant_being_picked.time_needed_to_pick;

		DrawRectangleRec(timer_bar, BLUE);
		DrawRectangleLinesEx(timer_bar_outline, 2.0f, BLACK);
	}

	DrawRectangleRec(p->body, p->color);
}
