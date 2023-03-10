#include "food.h"
#include "utils.h"


Food_t food_int(Vector2D_t pos, Vector2D_t dim, Color_t color, Food_type_t type) {
	Food_t food = {0};
	
	food.pos = pos;
	food.dim = dim;
	food.color = color;
	food.as = type;

	return food;
}

void food_update(Food_t *food) {
	
}

void food_render(SDL_Renderer *renderer, Food_t *food) {	
}
