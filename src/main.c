#include <time.h>
#include <stdio.h>

#include "raft.h"
#include "plant.h"
#include "player.h"
#include "kalipsos.h"

#define WIDTH 1280
#define HEIGHT 720
#define TITLE "Game Jam"

#define PLANTS_CAPACITY 128

#define GAME_OVER_FONT_SIZE 60

#define PLANT_COUNT 20

int main(void)
{
	InitWindow(WIDTH, HEIGHT, TITLE);

	while (!WindowShouldClose()) {
		bool should_restart = false;

		Rectangle ocean = {
			.x = WIDTH - 200.0f,
			.y = 0.0f,
			.width = 200.0f,
			.height = HEIGHT
		};

		Player player = player_create(WIDTH / 2.0f, HEIGHT / 2.0f);
		Kalipsos kalipsos = kalipsos_create(0, 0);

		Plant plants[128] = { 0 };
		uint32_t plants_count = 0;

		Raft raft = raft_create(WIDTH, HEIGHT, ocean);

		while (!WindowShouldClose() && !should_restart) {
			// UPDATE
			float time_step = GetFrameTime();

			if (kalipsos.suspicion >= SUSPICION_MAX) {
				if (IsKeyPressed(KEY_SPACE))
					should_restart = true;
			}
			else {
				player_update(&player, plants, &plants_count, &raft, ocean, HEIGHT, time_step);
				kalipsos_update(&kalipsos, &player, time_step);

				if (plants_count < PLANT_COUNT) {
					plants[plants_count++] = plant_create(PLANT_TREE, GetRandomValue(0, WIDTH - ocean.width - 50.0f), GetRandomValue(0, HEIGHT));
					plants[plants_count++] = plant_create(PLANT_WEED, GetRandomValue(0, WIDTH - ocean.width - 50.0f), GetRandomValue(0, HEIGHT));
				}
			}

			// RENDER
			ClearBackground(YELLOW);
			BeginDrawing();

			DrawRectangleRec(ocean, BLUE);

			for (uint32_t i = 0; i < plants_count; ++i)
				plant_render(&plants[i]);
			raft_render(&raft);
			player_render(&player, WIDTH, HEIGHT);
			kalipsos_render(&kalipsos);

			if (kalipsos.suspicion >= SUSPICION_MAX) {
				const char *game_over_caught_str = "Kalipsos caught you trying to escape!";
				DrawText(
					game_over_caught_str,
					WIDTH / 2.0f - MeasureText(game_over_caught_str, GAME_OVER_FONT_SIZE) / 2.0f,
					HEIGHT / 2.0f - GAME_OVER_FONT_SIZE / 2.0f,
					GAME_OVER_FONT_SIZE,
					RED
				);

				const char *game_over_space_str = "Press SPACE to restart";
				DrawText(
					game_over_space_str,
					WIDTH / 2.0f - MeasureText(game_over_space_str, GAME_OVER_FONT_SIZE) / 2.0f,
					HEIGHT / 2.0f + GAME_OVER_FONT_SIZE / 2.0f,
					GAME_OVER_FONT_SIZE,
					RED
				);
			}

			EndDrawing();
		}

		kalipsos_destroy(&kalipsos);
	}

	CloseWindow();

	return 0;
}
