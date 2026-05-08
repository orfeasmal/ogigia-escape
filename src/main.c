#include <stdio.h>

#include "player.h"
#include "kalipso.h"
#include "material.h"

#define WIDTH 1280
#define HEIGHT 720
#define TITLE "Game Jam"

#define MATERIALS_CAPACITY 128

int main(void)
{
	InitWindow(WIDTH, HEIGHT, TITLE);

	Entity player = player_create(WIDTH / 2.0f, HEIGHT / 2.0f);
	Entity kalipso = kalipso_create(0, 0);

	Entity materials[128] = { 0 };
	uint32_t materials_count = 0;

	for (uint32_t i = 0; i < 5; ++i) {
		++materials_count;
		materials[i] = material_create(MATERIAL_TREE, GetRandomValue(0, WIDTH), GetRandomValue(0, HEIGHT));
	}

	while (!WindowShouldClose()) {
		float time_step = GetFrameTime();

		// UPDATE
		player_update(&player, materials, &materials_count, time_step);

		// RENDER
		ClearBackground(GREEN);
		BeginDrawing();

		entity_render(&player);
		entity_render(&kalipso);
		for (uint32_t i = 0; i < materials_count; ++i)
			entity_render(&materials[i]);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
