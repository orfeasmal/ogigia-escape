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
} Plant;

Plant plant_create(PlantType type, float x, float y);
void plant_render(const Plant *plant);

#endif // MATERIAL_H
