#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "player.h"

#include <raymath.h>

#define PLAYER_WIDTH 35.0f
#define PLAYER_HEIGHT 75.0f
#define PLAYER_COLOR VIOLET

#define MAX_ITEMS_IN_POSSESION 5

#define SPEED 200.0f

#define TREE_TIME 5.0f
#define WEED_TIME 2.0f

static int32_t max(int32_t a, int32_t b);

Player player_create(float x, float y)
{
	Player p = {
		.body = {
			.width = PLAYER_WIDTH,
			.height = PLAYER_HEIGHT,
			.x = x,
			.y = y
		},
		.color = PLAYER_COLOR,
	};

	return p;
}

void player_update(
	Player *p,
	Plant *plants,
	uint32_t *plants_count,
	Raft *raft,
	Rectangle ocean,
	Sound *sounds,
	uint32_t window_height,
	float time_step
)
{
	if (p->state == PLAYER_DOING_NOTHING) {
		p->plants_that_can_be_interacted_with = 0;

		for (uint32_t i = 0; i < *plants_count; ++i) {
			Plant *plant = &plants[i];

			float distance_from_plant = Vector2Length(
				(Vector2) {
					.x = (p->body.x + p->body.width / 2.0f) - (plant->body.x + plant->body.width / 2.0f),
					.y = (p->body.y + p->body.height / 2.0f) - (plant->body.y + plant->body.height / 2.0f)
				}
			);

			if (distance_from_plant <= p->body.width * 2.5f) {
				++p->plants_that_can_be_interacted_with;

				uint32_t items_in_possesion_total = 0;
				for (uint32_t i = 0; i < PLANT_COUNT; ++i)
					items_in_possesion_total += p->plants_in_possesion[i];

				if (IsKeyPressed(KEY_E) && items_in_possesion_total < MAX_ITEMS_IN_POSSESION) {
					p->plant_being_picked_type = plant->type;
					p->plant_being_picked_index = i;

					p->timer = 0.0f;
					p->current_cooldown = plant->time_needed_to_pick;

					p->state = PLAYER_PICKING_PLANT;

					break;
				}
			}
		}

		float distance_from_raft = Vector2Length(
			(Vector2) {
				.x = (p->body.x + p->body.width / 2.0f) - (raft->body.x + raft->body.width / 2.0f),
				.y = (p->body.y + p->body.height / 2.0f) - (raft->body.y + raft->body.height / 2.0f)
			}
		);

		if (distance_from_raft <= p->body.width * 5.0f) {
			p->can_interact_with_raft = true;

			if (IsKeyPressed(KEY_E)) {
				p->timer = 0.0f;
				p->current_cooldown = raft->time_needed_to_deposit;
				p->state = PLAYER_DEPOSITING_TO_RAFT;
			}
		}
		else
			p->can_interact_with_raft = false;

		Vector2 velocity = { 0 };

		if (IsKeyDown(KEY_W) && p->body.y >= 0)
			velocity.y = -SPEED;
		if (IsKeyDown(KEY_S) && p->body.y + p->body.height < window_height)
			velocity.y = SPEED;
		if (IsKeyDown(KEY_A) && p->body.x >= 0)
			velocity.x = -SPEED;
		if (IsKeyDown(KEY_D) && p->body.x + p->body.width < ocean.x)
			velocity.x = SPEED;

		p->body.x += velocity.x * time_step;
		p->body.y += velocity.y * time_step;

		if (memcmp(&velocity, &(Vector2) { 0 }, sizeof(velocity)) != 0) {
			if (!IsSoundPlaying(sounds[SOUND_PLAYER_WALKING]))
				PlaySound(sounds[SOUND_PLAYER_WALKING]);
		}
		else
			StopSound(sounds[SOUND_PLAYER_WALKING]);
	}
	else if (p->state == PLAYER_PICKING_PLANT) {
		StopSound(sounds[SOUND_PLAYER_WALKING]);

		if (p->timer >= p->current_cooldown) {
			if (p->plant_being_picked_type == PLANT_TREE) {
				StopSound(sounds[SOUND_TREE_BREAKING]);
				PlaySound(sounds[SOUND_TREE_BROKEN]);
			}
			else if (p->plant_being_picked_type == PLANT_WEED) {
				StopSound(sounds[SOUND_WEED_BREAKING]);
				PlaySound(sounds[SOUND_WEED_BROKEN]);
			}

			++p->plants_in_possesion[p->plant_being_picked_type];
			plants[p->plant_being_picked_index] = plants[--(*plants_count)];

			p->state = PLAYER_DOING_NOTHING;
		}
		else {
			if (p->plant_being_picked_type == PLANT_TREE) {
				if (!IsSoundPlaying(sounds[SOUND_TREE_BREAKING]))
					PlaySound(sounds[SOUND_TREE_BREAKING]);
			}
			else if (p->plant_being_picked_type == PLANT_WEED) {
				if (!IsSoundPlaying(sounds[SOUND_WEED_BREAKING]))
					PlaySound(sounds[SOUND_WEED_BREAKING]);
			}
		}
	}
	else if (p->state == PLAYER_DEPOSITING_TO_RAFT) {
		StopSound(sounds[SOUND_PLAYER_WALKING]);

		if (p->timer >= p->current_cooldown) {
			for (uint32_t i = 0; i < PLANT_COUNT; ++i) {
				raft->plants_needed_to_be_built[i] -= p->plants_in_possesion[i];
				raft->plants_needed_to_be_built[i] = max(raft->plants_needed_to_be_built[i], 0);
				p->plants_in_possesion[i] = 0;
			}

			p->state = PLAYER_DOING_NOTHING;
		}
	}
	else if (p->state == PLAYER_BEING_QUESTIONED) {
		StopSound(sounds[SOUND_PLAYER_WALKING]);
		StopSound(sounds[SOUND_TREE_BREAKING]);
		StopSound(sounds[SOUND_WEED_BREAKING]);
	}

	if (p->state != PLAYER_BEING_QUESTIONED)
		p->timer += time_step;
}

#define TIMER_BAR_WIDTH 75.0f
#define TIMER_BAR_HEIGHT 20.0f

#define ITEM_COUNTER_FONT_SIZE 30

void player_render(const Player *p, uint32_t window_width, uint32_t window_height)
{
	if (p->state == PLAYER_PICKING_PLANT || p->state == PLAYER_DEPOSITING_TO_RAFT) {
		Rectangle timer_bar_outline = {
			.x = (p->body.x + p->body.width / 2.0f) - TIMER_BAR_WIDTH / 2.0f,
			.y = p->body.y - TIMER_BAR_HEIGHT * 1.5f,
			.width = TIMER_BAR_WIDTH,
			.height = TIMER_BAR_HEIGHT
		};

		Rectangle timer_bar = timer_bar_outline;
		timer_bar.width *= p->timer / p->current_cooldown;

		DrawRectangleRec(timer_bar, BLUE);
		DrawRectangleLinesEx(timer_bar_outline, 2.0f, BLACK);
	}
	else if (p->state == PLAYER_DOING_NOTHING) {
		if (p->can_interact_with_raft) {
			const char *str = "Press 'E' to deposit";
			DrawText(str, p->body.x + p->body.width / 2.0f - MeasureText(str, TIMER_BAR_HEIGHT) / 2.0f, p->body.y + p->body.height + 10, TIMER_BAR_HEIGHT, BLACK);
		}
		else if (p->plants_that_can_be_interacted_with > 0) {
			uint32_t items_in_possesion_total = 0;
			for (uint32_t i = 0; i < PLANT_COUNT; ++i)
				items_in_possesion_total += p->plants_in_possesion[i];

			const char *str;
			if (items_in_possesion_total < MAX_ITEMS_IN_POSSESION)
				str = "Press 'E' to collect";
			else
				str = "Reached inventory limit, deposit to raft";

			DrawText(str, p->body.x + p->body.width / 2.0f - MeasureText(str, TIMER_BAR_HEIGHT) / 2.0f, p->body.y + p->body.height + 10, TIMER_BAR_HEIGHT, BLACK);
		}
	}

	DrawRectangleRec(p->body, p->color);

	const char *wood_counter_str = TextFormat("WOOD: %u", p->plants_in_possesion[PLANT_TREE]);
	DrawText(wood_counter_str, 10, window_height - (ITEM_COUNTER_FONT_SIZE + 10) * 2, ITEM_COUNTER_FONT_SIZE, BLACK);

	const char *string_counter_str = TextFormat("STRING: %u", p->plants_in_possesion[PLANT_WEED]);
	DrawText(string_counter_str, 10, window_height - ITEM_COUNTER_FONT_SIZE - 10, ITEM_COUNTER_FONT_SIZE, BLACK);
}

static int32_t max(int32_t a, int32_t b)
{
	if (a >= b)
		return a;
	else
		return b;
}
