#include <stdio.h>

#define JSIMPLON_IMPLEMENTATION
#include <jsimplon.h>

#include "player.h"
#include "kalipsos.h"
#include "plant.h"

#define WIDTH 1280
#define HEIGHT 720
#define TITLE "Game Jam"

#define DIALOGUE_FILE_PATH "assets/dialogue.json"

#define PLANTS_CAPACITY 128

int main(void)
{
	char *json_error;
	Jsimplon_Value *json_root_value = jsimplon_tree_from_file(&json_error, DIALOGUE_FILE_PATH);
	
	if (json_error != NULL) {
		fprintf(stderr, "error: %s\n", json_error);
		free(json_error);

		return 1;
	}

	Jsimplon_Object *json_root_object = jsimplon_value_get_object(json_root_value);

	Player player = player_create(WIDTH / 2.0f, HEIGHT / 2.0f);
	Kalipsos kalipsos = kalipsos_create(0, 0);

	Plant plants[128] = { 0 };
	uint32_t plants_count = 0;

	for (uint32_t i = 0; i < 5; ++i) {
		++plants_count;
		plants[i] = plant_create(PLANT_TREE, GetRandomValue(0, WIDTH), GetRandomValue(0, HEIGHT));
	}

	InitWindow(WIDTH, HEIGHT, TITLE);

	while (!WindowShouldClose()) {
		float time_step = GetFrameTime();

		// UPDATE
		player_update(&player, plants, &plants_count, time_step);
		kalipsos_update(&kalipsos, &player, json_root_object, time_step);

		// RENDER
		ClearBackground(GREEN);
		BeginDrawing();

		player_render(&player);
		kalipsos_render(&kalipsos);
		for (uint32_t i = 0; i < plants_count; ++i)
			plant_render(&plants[i]);

		EndDrawing();
	}

	CloseWindow();

	jsimplon_tree_destroy(json_root_value);

	return 0;
}
