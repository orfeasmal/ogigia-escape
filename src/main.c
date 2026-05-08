#include <stdio.h>

#include "player.h"

#define WIDTH 1280
#define HEIGHT 720
#define TITLE "Game Jam"

int main(void)
{
	Entity player = {
		.body = {
			.width = 50.0f,
			.height = 100.0f
		},
		.color = BLUE,

		.gravity = true
	};

	InitWindow(WIDTH, HEIGHT, TITLE);

	while (!WindowShouldClose()) {
		float time_step = GetFrameTime();

		// UPDATE
		player_update(&player, WIDTH, HEIGHT, time_step);

		// RENDER
		ClearBackground(WHITE);
		BeginDrawing();
		entity_render(&player);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
