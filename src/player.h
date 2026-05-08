#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

Entity player_create(float x, float y);
void player_update(Entity *player, Entity *materials, uint32_t *materials_count, float time_step);

#endif // PLAYER_H
