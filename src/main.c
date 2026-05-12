#include <time.h>
#include <stdio.h>

#include "util.h"
#include "raft.h"
#include "plant.h"
#include "player.h"
#include "kalipso.h"

#define SOUND_PLAYER_WALKING_PATH "assets/audio/player_walking.wav"
#define SOUND_TREE_BREAKING_PATH "assets/audio/tree_breaking.wav"
#define SOUND_TREE_BROKEN_PATH "assets/audio/tree_broken.wav"
#define SOUND_WEED_BREAKING_PATH "assets/audio/weed_breaking.wav"
#define SOUND_WEED_BROKEN_PATH "assets/audio/weed_broken.wav"

#define WIDTH 1280
#define HEIGHT 720
#define TITLE "Game Jam"

#define PLANTS_CAPACITY 128

#define GAME_OVER_FONT_SIZE 60

#define PLANT_COUNT 20

int main(void)
{
	InitWindow(WIDTH, HEIGHT, TITLE);
	InitAudioDevice();

	Sound sounds[SOUND_ENUM_COUNT] = { 0 };
	sounds[SOUND_PLAYER_WALKING] = LoadSound(SOUND_PLAYER_WALKING_PATH);
	sounds[SOUND_TREE_BREAKING] = LoadSound(SOUND_TREE_BREAKING_PATH);
	sounds[SOUND_TREE_BROKEN] = LoadSound(SOUND_TREE_BROKEN_PATH);
	sounds[SOUND_WEED_BREAKING] = LoadSound(SOUND_WEED_BREAKING_PATH);
	sounds[SOUND_WEED_BROKEN] = LoadSound(SOUND_WEED_BROKEN_PATH);

	while (!WindowShouldClose()) {
		bool should_restart = false;

		Rectangle ocean = {
			.x = WIDTH - 200.0f,
			.y = 0.0f,
			.width = 200.0f,
			.height = HEIGHT
		};

		Player player = player_create(WIDTH / 2.0f, HEIGHT / 2.0f);
		Kalipso kalipso = kalipso_create(0, 0);

		Plant plants[128] = { 0 };
		uint32_t plants_count = 0;

		Raft raft = raft_create(WIDTH, HEIGHT, ocean);

		while (!WindowShouldClose() && !should_restart) {
			// UPDATE
			float time_step = GetFrameTime();

			if (kalipso.suspicion >= SUSPICION_MAX) {
				if (IsKeyPressed(KEY_SPACE))
					should_restart = true;
			}
			else {
				player_update(&player, plants, &plants_count, &raft, ocean, sounds, HEIGHT, time_step);
				kalipso_update(&kalipso, &player, time_step);

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
			kalipso_render(&kalipso);

			if (kalipso.suspicion >= SUSPICION_MAX) {
				const char *game_over_caught_str = "Kalipso caught you trying to escape!";
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

		kalipso_destroy(&kalipso);
	}

	for (uint32_t i = 0; i < SOUND_ENUM_COUNT; ++i)
		UnloadSound(sounds[i]);

	CloseAudioDevice();
	CloseWindow();

	return 0;
}
