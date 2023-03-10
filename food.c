#include "food.h"
#include "utils.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdlib.h>


Food_t *food_int(Vector2D_t pos, Vector2D_t dim, Color_t color, Food_type_t type) {
	Food_t *food = (Food_t *) malloc(sizeof(Food_t));
	
	food->pos = pos;
	food->dim = dim;
	food->color = color;
	food->as = type;

	return food;
}

void food_update(Food_t *food) {
	
}

void food_render(SDL_Renderer *renderer, Food_t *food) {	
	SDL_Rect rect;
	rect.x = food->pos.x;
	rect.y = food->pos.y;
	rect.w = food->dim.x;
	rect.h = food->dim.y;
	SDL_SetRenderDrawColor( renderer, food->color.R, food->color.G, food->color.B, food->color.A);
	SDL_RenderFillRect(renderer, &rect);
}
