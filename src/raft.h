#ifndef RAFT_H
#define RAFT_H

#include <stdint.h>

#include <raylib.h>

#include "plant.h"

typedef struct {
	Rectangle body;
	Color color;

	float time_needed_to_deposit; // for consistency with plants

	int32_t plants_needed_to_be_built[PLANT_COUNT];
} Raft;

Raft raft_create(uint32_t window_width, uint32_t window_height, Rectangle ocean);
void raft_render(const Raft *raft);

#endif // RAFT_H
