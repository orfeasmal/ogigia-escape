#include <stdlib.h>

#define JSIMPLON_STATIC
#define JSIMPLON_IMPLEMENTATION
#include <jsimplon.h>

#include "kalipsos.h"

#include <raymath.h>

#define DIALOGUE_FILE_PATH "assets/dialogue.json"

#define KALIPSOS_WIDTH 35.0f
#define KALIPSOS_HEIGHT 65.0f
#define KALIPSOS_COLOR WHITE

#define COOLDOWN 5.0f

Kalipsos kalipsos_create(float x, float y)
{
	char *json_error;
	Jsimplon_Value *json_root_value = jsimplon_tree_from_file(&json_error, DIALOGUE_FILE_PATH);

	if (json_error != NULL) {
		fprintf(stderr, "error: %s\n", json_error);
		free(json_error);

		exit(EXIT_FAILURE);
	}

	return (Kalipsos) {
		.body = {
			.x = x,
			.y = y,
			.width = KALIPSOS_WIDTH,
			.height = KALIPSOS_HEIGHT
		},
		.json_root_value = json_root_value,
		.color = KALIPSOS_COLOR
	};
}

void kalipsos_destroy(Kalipsos *k)
{
	jsimplon_tree_destroy(k->json_root_value);
	*k = (Kalipsos) { 0 };
}

#define SPEED 50.0f

void kalipsos_update(Kalipsos *k, Player *player, float time_step)
{
	if (k->state == KALIPSOS_DOING_NOTHING) {
		Vector2 player_minus_kalipsos_pos = (Vector2) {
			.x = (player->body.x + player->body.width / 2.0f) - (k->body.x + k->body.width / 2.0f),
			.y = (player->body.y + player->body.height / 2.0f) - (k->body.y + k->body.height / 2.0f)
		};
		float distance_from_player = Vector2Length(player_minus_kalipsos_pos);

		k->vel = Vector2Scale(player_minus_kalipsos_pos, SPEED / distance_from_player);

		k->body.x += k->vel.x * time_step;
		k->body.y += k->vel.y * time_step;

		if (k->timer >= COOLDOWN && distance_from_player <= 100.0f) {
			k->player_prev_state = player->state;

			k->state = KALIPSOS_QUESTIONING;
			player->state = PLAYER_BEING_QUESTIONED;

			k->vel = (Vector2) { 0.0f };

			Jsimplon_Object *json_temp_object = NULL;
			json_temp_object = jsimplon_value_get_object(k->json_root_value);
			switch (k->player_prev_state) {
				case PLAYER_DOING_NOTHING:
					json_temp_object = jsimplon_object_member_get_object(json_temp_object, "general");
					break;
				case PLAYER_CUTTING_TREE:
					json_temp_object = jsimplon_object_member_get_object(json_temp_object, "tree");
					break;
				case PLAYER_PICKING_WEEDS:
					json_temp_object = jsimplon_object_member_get_object(json_temp_object, "weed");
					break;
				case PLAYER_BUILDING_RAFT:
					json_temp_object = jsimplon_object_member_get_object(json_temp_object, "raft");
					break;
				case PLAYER_BEING_QUESTIONED:
					fprintf(stderr, "PLAYER_BEING_QUESTIONED is unreachable\n");
					break;
			}

			Jsimplon_Array *json_question_array = NULL;

			if (k->suspicion <= 33)
				json_question_array = jsimplon_object_member_get_array(json_temp_object, "suspicion <= 33");
			else if (k->suspicion <= 66)
				json_question_array = jsimplon_object_member_get_array(json_temp_object, "suspicion <= 66");
			else if (k->suspicion <= 99)
				json_question_array = jsimplon_object_member_get_array(json_temp_object, "suspicion <= 99");

			size_t json_question_array_count = jsimplon_array_get_count(json_question_array);

			Jsimplon_Value *json_random_question = NULL;
			if (json_question_array_count > 0)
				json_random_question = jsimplon_array_get_value_at_index(json_question_array, GetRandomValue(0, json_question_array_count - 1));

			json_temp_object = jsimplon_value_get_object(json_random_question);

			k->question = jsimplon_object_member_get_str(json_temp_object, "question");
			
			Jsimplon_Array *json_answer_array = jsimplon_object_member_get_array(json_temp_object, "answers");
			k->answers_count = jsimplon_array_get_count(json_answer_array);

			for (uint32_t i = 0; i < k->answers_count && i < MAX_ANSWER_COUNT; ++i) {
				Jsimplon_Value *json_answer_value = jsimplon_array_get_value_at_index(json_answer_array, i);

				json_temp_object = jsimplon_value_get_object(json_answer_value);

				k->answers[i] = (Answer) {
					.suspiciousness = jsimplon_object_member_get_number(json_temp_object, "suspiciousness"),
					.str = jsimplon_object_member_get_str(json_temp_object, "answer")
				};

				printf("%s\n", jsimplon_object_member_get_str(json_temp_object, "answer"));
			}
		}

		k->timer += time_step;
	}
	else if (k->state == KALIPSOS_QUESTIONING) {
		if (IsKeyPressed(KEY_B)) {
			k->state = KALIPSOS_DOING_NOTHING;
			player->state = k->player_prev_state;
			k->timer = 0.0f;
		}
	}
}

void kalipsos_render(const Kalipsos *k)
{
	DrawRectangleRec(k->body, k->color);

	if (k->state != KALIPSOS_QUESTIONING)
		return;

	DrawText(k->question, 10, 0, 50, BLACK);

	for (uint32_t i = 0; i < k->answers_count; ++i)
		DrawText(k->answers[i].str, 10, 50 + i * 30, 30, BLACK);
}
