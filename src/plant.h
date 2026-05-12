#ifndef MATERIAL_H
#define MATERIAL_H

#include <raylib.h>

typedef enum {
	PLANT_TREE,
	PLANT_WEED,
	PLANT_COUNT
} PlantType;

typedef struct {
	Rectangle body;
	PlantType type;
	Color color;
	float time_needed_to_pick;
} Plant;

Plant plant_create(PlantType type, float x, float y);
void plant_render(const Plant *plant, const Texture *textures);

#endif // MATERIAL_H
