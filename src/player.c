#include <math.h>
#include <stdio.h>

#include "player.h"

#include <raymath.h>

#define PLAYER_WIDTH 35.0f
#define PLAYER_HEIGHT 75.0f
#define PLAYER_COLOR BLUE

#define VEL 400.0f

Entity player_create(float x, float y)
{
	return (Entity) {
		.body = {
			.width = PLAYER_WIDTH,
			.height = PLAYER_HEIGHT,
			.x = x,
			.y = y
		},
		.type = ENTITY_PLAYER,
		.color = PLAYER_COLOR
	};
}

void player_update(Entity *p, Entity *materials, uint32_t *materials_count, float time_step)
{
	for (uint32_t i = 0; i < *materials_count; ++i) {
		Entity *material = &materials[i];

		float distance_from_material = Vector2Length(
			(Vector2) {
				.x = p->body.x - material->body.x,
				.y = p->body.y - material->body.y
			}
		);

		if (distance_from_material <= p->body.width * 2.0f) {
			if (IsKeyPressed(KEY_E)) {
				++p->materials_in_possesion[material->material_type];
				materials[i] = materials[--(*materials_count)];
				printf("%d\n", p->materials_in_possesion[0]);
			}
		}
	}

	if (IsKeyDown(KEY_W))
		p->body.y -= VEL * time_step;
	if (IsKeyDown(KEY_S))
		p->body.y += VEL * time_step;
	if (IsKeyDown(KEY_A))
		p->body.x -= VEL * time_step;
	if (IsKeyDown(KEY_D))
		p->body.x += VEL * time_step;

	entity_update(p, time_step);
}
