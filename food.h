#ifndef _FOOD_H
#define _FOOD_H
#include <SDL2/SDL.h>
#include "utils.h"

typedef enum {ENERGY=0, POISION} Food_type_t;

typedef struct{
	int R,G,B,A;
} Color_t;

typedef struct{
	Vector2D_t dim;
} Food_shape;

typedef struct{
	Food_shape shape;
	Vector2D_t pos;
	Vector2D_t dim;
	Color_t color;
	Food_type_t as;
} Food_t;

Food_t *food_int(Vector2D_t , Vector2D_t , Color_t , Food_type_t);
void food_update(Food_t *);
void food_render(SDL_Renderer *, Food_t *);

#endif
