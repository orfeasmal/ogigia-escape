#include <time.h>
#include <stdio.h>

#include "player.h"
#include "kalipsos.h"
#include "plant.h"

#define WIDTH 1280
#define HEIGHT 720
#define TITLE "Game Jam"

#define PLANTS_CAPACITY 128

int main(void)
{
	InitWindow(WIDTH, HEIGHT, TITLE);

	Player player = player_create(WIDTH / 2.0f, HEIGHT / 2.0f);
	Kalipsos kalipsos = kalipsos_create(0, 0);

	Plant plants[128] = { 0 };
	uint32_t plants_count = 0;

	for (uint32_t i = 0; i < 5; ++i) {
		++plants_count;
		plants[i] = plant_create(PLANT_TREE, GetRandomValue(0, WIDTH), GetRandomValue(0, HEIGHT));
	}

	while (!WindowShouldClose()) {
		float time_step = GetFrameTime();

		// UPDATE

		if (kalipsos.suspicion >= SUSPICION_MAX) {
			break;
		}
		else {
			player_update(&player, plants, &plants_count, time_step);
			kalipsos_update(&kalipsos, &player, time_step);
		}

		// RENDER
		ClearBackground(GREEN);
		BeginDrawing();

		for (uint32_t i = 0; i < plants_count; ++i)
			plant_render(&plants[i]);
		player_render(&player);
		kalipsos_render(&kalipsos);

		EndDrawing();
	}

	kalipsos_destroy(&kalipsos);

	CloseWindow();


	return 0;
}
