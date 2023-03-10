#include "food.h"
#include "utils.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdlib.h>


Food_t *food_int(Vector2D_t pos, Vector2D_t dim) {
	Food_t *food = (Food_t *) malloc(sizeof(Food_t));
	
	food->pos = pos;
	food->dim = dim;

	food_random_int(food);

	return food;
}

void food_random_int(Food_t *food){	
	food->pos = vect_get_random(20 , 600);
	if(random_int_range(1 , 100) < 10){
		food->as = POISION;
		food->color = (Color_t){255,0,0};
	}else{
		food->as = ENERGY;
		food->color = (Color_t){0,255,0};
	}
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
