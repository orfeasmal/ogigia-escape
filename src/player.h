#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

Entity player_create(float x, float y);
void player_update(Entity *player, uint32_t width, uint32_t height, float time_step);

#endif // PLAYER_H
