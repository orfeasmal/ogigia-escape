#include <time.h>
#include <stdio.h>

#include "util.h"
#include "raft.h"
#include "plant.h"
#include "player.h"
#include "kalipso.h"

#define SOUND_BACKGROUND_PATH "assets/audio/background.wav"
#define SOUND_PLAYER_TALKING_PATH "assets/audio/player_talking.wav"
#define SOUND_KALIPSO_TALKING_LOW_SUSPICION_PATH "assets/audio/kalipso_talking_low_suspicion.wav"
#define SOUND_KALIPSO_TALKING_MEDIUM_SUSPICION_PATH "assets/audio/kalipso_talking_medium_suspicion.wav"
#define SOUND_KALIPSO_TALKING_HIGH_SUSPICION_PATH "assets/audio/kalipso_talking_high_suspicion.wav"
#define SOUND_PLAYER_WALKING_PATH "assets/audio/player_walking.wav"
#define SOUND_TREE_BREAKING_PATH "assets/audio/tree_breaking.wav"
#define SOUND_TREE_BROKEN_PATH "assets/audio/tree_broken.wav"
#define SOUND_WEED_BREAKING_PATH "assets/audio/weed_breaking.wav"
#define SOUND_WEED_BROKEN_PATH "assets/audio/weed_broken.wav"
#define SOUND_DEPOSITED_TO_RAFT_PATH "assets/audio/deposited_to_raft.wav"

#define WIDTH 1280
#define HEIGHT 720
#define TITLE "Ogigia Escape"

#define PLANTS_CAPACITY 128

#define GAME_OVER_FONT_SIZE 55

#define PLANT_COUNT 20

int main(void)
{
	InitWindow(WIDTH, HEIGHT, TITLE);
	InitAudioDevice();

	Sound sounds[SOUND_ENUM_COUNT] = { 0 };
	sounds[SOUND_PLAYER_TALKING] = LoadSound(SOUND_PLAYER_TALKING_PATH);
	sounds[SOUND_KALIPSO_TALKING_LOW_SUSPICION] = LoadSound(SOUND_KALIPSO_TALKING_LOW_SUSPICION_PATH);
	sounds[SOUND_KALIPSO_TALKING_MEDIUM_SUSPICION] = LoadSound(SOUND_KALIPSO_TALKING_MEDIUM_SUSPICION_PATH);
	sounds[SOUND_KALIPSO_TALKING_HIGH_SUSPICION] = LoadSound(SOUND_KALIPSO_TALKING_HIGH_SUSPICION_PATH);
	sounds[SOUND_BACKGROUND] = LoadSound(SOUND_BACKGROUND_PATH);
	sounds[SOUND_PLAYER_WALKING] = LoadSound(SOUND_PLAYER_WALKING_PATH);
	sounds[SOUND_TREE_BREAKING] = LoadSound(SOUND_TREE_BREAKING_PATH);
	sounds[SOUND_TREE_BROKEN] = LoadSound(SOUND_TREE_BROKEN_PATH);
	sounds[SOUND_WEED_BREAKING] = LoadSound(SOUND_WEED_BREAKING_PATH);
	sounds[SOUND_WEED_BROKEN] = LoadSound(SOUND_WEED_BROKEN_PATH);
	sounds[SOUND_DEPOSITED_TO_RAFT] = LoadSound(SOUND_DEPOSITED_TO_RAFT_PATH);

	SetSoundVolume(sounds[SOUND_BACKGROUND], 0.2f);

	while (!WindowShouldClose()) {
		float time_elapsed = 0.0f;
		bool should_restart = false;

		Rectangle ocean = {
			.x = WIDTH - 200.0f,
			.y = 0.0f,
			.width = 200.0f,
			.height = HEIGHT
		};

		Player player = player_create(WIDTH / 2.0f, HEIGHT / 2.0f);
		Kalipso kalipso = kalipso_create(10, HEIGHT / 2.0f);

		Plant plants[128] = { 0 };
		uint32_t plants_count = 0;

		Raft raft = raft_create(HEIGHT, ocean);

		while (!WindowShouldClose() && !should_restart) {
			// UPDATE
			float time_step = GetFrameTime();

			if (!IsSoundPlaying(sounds[SOUND_BACKGROUND]))
				PlaySound(sounds[SOUND_BACKGROUND]);

			if (player.state == PLAYER_ESCAPING) {
				player_update(&player, plants, &plants_count, &raft, ocean, sounds, WIDTH, HEIGHT, time_step);
			}
			else if (player.state != PLAYER_ESCAPED && kalipso.suspicion < SUSPICION_MAX) {
				time_elapsed += time_step;

				player_update(&player, plants, &plants_count, &raft, ocean, sounds, WIDTH, HEIGHT, time_step);
				kalipso_update(&kalipso, &player, sounds, time_step);

				if (plants_count < PLANT_COUNT) {
					plants[plants_count++] = plant_create(PLANT_TREE, GetRandomValue(0, WIDTH - ocean.width - 50.0f), GetRandomValue(0, HEIGHT));
					plants[plants_count++] = plant_create(PLANT_WEED, GetRandomValue(0, WIDTH - ocean.width - 50.0f), GetRandomValue(0, HEIGHT));
				}
			}
			else {
				if (IsKeyPressed(KEY_SPACE))
					should_restart = true;
			}

			// RENDER
			ClearBackground(YELLOW);
			BeginDrawing();

			DrawRectangleRec(ocean, BLUE);

#define TIME_FONT_SIZE 22
			const char *time_str = TextFormat("%.1fs ELAPSED", time_elapsed);
			DrawText(time_str, WIDTH - MeasureText(time_str, TIME_FONT_SIZE) - 10, 10, TIME_FONT_SIZE, BLACK);

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
			else if (player.state == PLAYER_ESCAPED) {
				const char *win_str = TextFormat("You managed to escape in %.1f seconds!", time_elapsed);
				DrawText(
					win_str,
					WIDTH / 2.0f - MeasureText(win_str, GAME_OVER_FONT_SIZE) / 2.0f,
					HEIGHT / 2.0f - GAME_OVER_FONT_SIZE / 2.0f,
					GAME_OVER_FONT_SIZE,
					VIOLET
				);

				const char *win_space_str = "Press SPACE to restart";
				DrawText(
					win_space_str,
					WIDTH / 2.0f - MeasureText(win_space_str, GAME_OVER_FONT_SIZE) / 2.0f,
					HEIGHT / 2.0f + GAME_OVER_FONT_SIZE / 2.0f,
					GAME_OVER_FONT_SIZE,
					VIOLET
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
