#include <stdio.h>

#include "player.h"

#define WIDTH 1280
#define HEIGHT 720
#define TITLE "Game Jam"

int main(void)
{
	InitWindow(WIDTH, HEIGHT, TITLE);

	Entity player = player_create(WIDTH / 2.0f, HEIGHT / 2.0f);

	while (!WindowShouldClose()) {
		float time_step = GetFrameTime();

		// UPDATE
		player_update(&player, WIDTH, HEIGHT, time_step);

		// RENDER
		ClearBackground(GREEN);
		BeginDrawing();
		entity_render(&player);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
