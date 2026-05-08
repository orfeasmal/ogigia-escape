#include <stdio.h>

#include "raylib.h"

int main(void)
{
	InitWindow(1280, 720, "Game Jam");

	while (!WindowShouldClose()) {
		ClearBackground(RED);
		BeginDrawing();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
