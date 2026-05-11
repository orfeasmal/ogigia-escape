#include <stdlib.h>
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

#define COOLDOWN 3.0f

#define QUESTION_FONT_SIZE 50
#define ANSWER_FONT_SIZE 30
#define ANSWER_SEPARATION 10

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
		.color = KALIPSOS_COLOR,
		.timer = COOLDOWN
	};
}

void kalipsos_destroy(Kalipsos *k)
{
	jsimplon_tree_destroy(k->json_root_value);
	*k = (Kalipsos) { 0 };
}

#define SPEED 100.0f
#define QUESTION_TRIGGER_DISTANCE 125

void kalipsos_update(Kalipsos *k, Player *player, float time_step)
{
	if (k->state == KALIPSOS_CHASING_PLAYER && k->timer >= COOLDOWN) {
		Vector2 player_minus_kalipsos_pos = (Vector2) {
			.x = (player->body.x + player->body.width / 2.0f) - (k->body.x + k->body.width / 2.0f),
			.y = (player->body.y + player->body.height / 2.0f) - (k->body.y + k->body.height / 2.0f)
		};
		float distance_from_player = Vector2Length(player_minus_kalipsos_pos);

		k->vel = Vector2Scale(player_minus_kalipsos_pos, SPEED / distance_from_player);

		k->body.x += k->vel.x * time_step;
		k->body.y += k->vel.y * time_step;

		if (distance_from_player <= QUESTION_TRIGGER_DISTANCE) {
			k->player_prev_state = player->state;

			k->state = KALIPSOS_QUESTIONING;
			player->state = PLAYER_BEING_QUESTIONED;

			k->vel = (Vector2) { 0.0f };

			// This project made me realise how bad my JSimplON API is

			Jsimplon_Object *json_temp_object = NULL;
			json_temp_object = jsimplon_value_get_object(k->json_root_value);
			switch (k->player_prev_state) {
				case PLAYER_DOING_NOTHING:
					json_temp_object = jsimplon_object_member_get_object(json_temp_object, "general");
					break;
				case PLAYER_PICKING_PLANT:
					if (player->plant_being_picked.type == PLANT_TREE)
						json_temp_object = jsimplon_object_member_get_object(json_temp_object, "tree");
					else if (player->plant_being_picked.type == PLANT_WEED)
						json_temp_object = jsimplon_object_member_get_object(json_temp_object, "weed");
					else {
						fprintf(stderr, "internal error: unreachable plant type in kalipsos_update\n");
						exit(EXIT_FAILURE);
					}
					break;
				case PLAYER_BUILDING_RAFT:
					json_temp_object = jsimplon_object_member_get_object(json_temp_object, "raft");
					break;
				default:
					fprintf(stderr, "internal error: unreachable player state in kalipsos_update\n");
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

			if (json_question_array_count == 0) {
				fprintf(stderr, "internal error: no questions to ask\n");
				exit(EXIT_FAILURE);
			}
			Jsimplon_Value *json_random_question = jsimplon_array_get_value_at_index(json_question_array, GetRandomValue(0, json_question_array_count - 1));

			json_temp_object = jsimplon_value_get_object(json_random_question);

			k->question = jsimplon_object_member_get_str(json_temp_object, "question");
			
			Jsimplon_Array *json_answer_array = jsimplon_object_member_get_array(json_temp_object, "answers");
			k->answers_count = jsimplon_array_get_count(json_answer_array);
			if (k->answers_count > ANSWERS_MAX_COUNT) {
				fprintf(stderr, "internal error: dialogue per question answer limit exceeded %d\n", ANSWERS_MAX_COUNT);
				k->answers_count = ANSWERS_MAX_COUNT;
			}

			for (uint32_t i = 0; i < k->answers_count; ++i) {
				Jsimplon_Value *json_answer_value = jsimplon_array_get_value_at_index(json_answer_array, i);

				json_temp_object = jsimplon_value_get_object(json_answer_value);

				uint32_t random_index;
				do {
					random_index = GetRandomValue(0, k->answers_count - 1);
				} while (k->answers[random_index].str != NULL);

				Answer answer = {
					.suspiciousness = jsimplon_object_member_get_number(json_temp_object, "suspiciousness"),
					.str = jsimplon_object_member_get_str(json_temp_object, "answer")
				};

				answer.body = (Rectangle) {
					.x = ANSWER_SEPARATION,
					.y = ANSWER_SEPARATION + QUESTION_FONT_SIZE + random_index * (ANSWER_FONT_SIZE + ANSWER_SEPARATION * 2),
					.width = MeasureText(answer.str, ANSWER_FONT_SIZE) + ANSWER_SEPARATION,
					.height = ANSWER_FONT_SIZE + ANSWER_SEPARATION
				};

				k->answers[random_index] = answer;
			}
		}
	}
	else if (k->state == KALIPSOS_QUESTIONING) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			for (uint32_t i = 0; i < k->answers_count; ++i) {
				Answer answer = k->answers[i];

				if (CheckCollisionPointRec(GetMousePosition(), answer.body)) {
					k->suspicion += answer.suspiciousness;
					k->timer = 0.0f;

					memset(k->answers, 0, ANSWERS_MAX_COUNT * sizeof(*k->answers));

					k->state = KALIPSOS_CHASING_PLAYER;
					player->state = k->player_prev_state;
				}
			}
		}
	}

	k->timer += time_step;
}

void kalipsos_render(const Kalipsos *k)
{
	DrawRectangleRec(k->body, k->color);

	if (k->state != KALIPSOS_QUESTIONING)
		return;

	DrawText(k->question, ANSWER_SEPARATION, 0, QUESTION_FONT_SIZE, BLACK);

	for (uint32_t i = 0; i < k->answers_count; ++i) {
		Answer answer = k->answers[i];

		DrawRectangleRec(answer.body, BLACK);
		DrawText(answer.str, answer.body.x + ANSWER_SEPARATION / 2.0f, answer.body.y + ANSWER_SEPARATION / 2.0f, ANSWER_FONT_SIZE, WHITE);
	}
}
