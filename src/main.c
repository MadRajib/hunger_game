#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h> 
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <time.h>

#include "style.h"
#include "utils.h"
#include "list.h"
#include "config.h"
#include "agent.h"
#include "food.h"

typedef union{
	Food_t *food;
	Agent_t *agent;
} item_type;

typedef  struct{
	item_type as;
	struct list_head node;
} list_item;

/* error handlers */
int scc(int code) {
	if (code < 0) {
		fprintf(stderr, "SDL error: %s\n", SDL_GetError());
		exit(1);
	}

	return code;
}

void *scp(void *ptr) {
	if (ptr == NULL) {
		fprintf(stderr, "SDL error: %s\n", SDL_GetError());
		exit(1);
	}

	return ptr;
}

void sdl_set_color_hex(SDL_Renderer *renderer, Uint32 hex) {

	scc(SDL_SetRenderDrawColor(
		renderer,
		(hex >> (3 * 8)) & 0XFF, 
		(hex >> (2 * 8)) & 0XFF, 
		(hex >> (1 * 8)) & 0XFF, 
		(hex >> (0 * 8)) & 0XFF));
}


int mouse_inside_window(Vector2D_t *pos){
	if(pos->x <= 0 || pos->y <= 0 || pos->x >= (SCREEN_WIDTH *0.98) || pos->y >= (SCREEN_HEIGHT*0.98))
		return 0;
	return 1;
}

int main_gp() {
	
	//Vector2D_t mouse_pos = {0, 0};

	int quit = 0;
	const int agent_count = 50;
	const int food_count = 5;

	clock_t p_clk, delta;
	float timescale = 1;
	char text[80];

	/* Init SDL */
	scc(SDL_Init(SDL_INIT_VIDEO) || TTF_Init());

	SDL_Window *window = scp(SDL_CreateWindow(
		"Hunger Game",
		0,0,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_RESIZABLE));
	
	SDL_Renderer *const renderer = scp(SDL_CreateRenderer(
		window, -1,
		SDL_RENDERER_ACCELERATED));

	scc(SDL_RenderSetLogicalSize(
		renderer,
		SCREEN_WIDTH,
		SCREEN_HEIGHT));

	TTF_Font* Sans = TTF_OpenFont("./assets/Sans.ttf", 20);

	/*done init*/

	srand(time(NULL));

	/**/
	LIST_HEAD(food_list);

	for (int i=0; i< food_count; i++) {
		Food_t *food = food_int(vect_get_random(20, 600), (Vector2D_t){10,10});
		list_item *item = (list_item *) malloc(sizeof(list_item));
		item->as.food = food;
		INIT_LIST_HEAD(&item->node);
		list_add(&item->node, &food_list);
	}

	LIST_HEAD(agent_list);


	for (int i=0; i< agent_count; i++) {
		list_item *item = (list_item *) malloc(sizeof(list_item));
		item->as.agent = agent_ptr_init(vect_get_random(20, 600));
		INIT_LIST_HEAD(&item->node);
		list_add(&item->node, &agent_list);
	}
	
	p_clk = clock();

	while (!quit) {
		SDL_Event event;
		//int pos_x  = 0;
		//int pos_y  = 0;
		
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					switch ( event.key.keysym.sym ) {
						case SDLK_q:
							quit = 1;
							break;	
					}
					break;
				case SDL_QUIT:
					quit = 1;
					break;
			}
		}

		delta = clock() - p_clk;
		p_clk = clock();

		sdl_set_color_hex(renderer, BACKGROUND_CLR);
		scc(SDL_RenderClear(renderer));	

		//SDL_GetMouseState(&pos_x, &pos_y);
		
		//mouse_pos.x = pos_x;
		//mouse_pos.y = pos_y;


		struct list_head *agent_iter, *food_iter, *tmp;
		list_item *food_item, *agent_item;

		/*Update elemetns*/
		list_for_each_safe(agent_iter, tmp, &agent_list) {
			agent_item = list_entry(agent_iter,list_item, node);
			float min_mag = 100;
			list_item *nearest_food_item = NULL;
			Vector2D_t min_seek_frc = {0, 0};

			__list_for_each(food_iter, &food_list) {
				food_item = list_entry(food_iter,list_item, node);	
				Vector2D_t seek_force = sub_vect(&(food_item->as.food->pos),	&(agent_item->as.agent->pivot));
				float mag = get_mag(&seek_force);
				if( mag < min_mag) {
					min_mag = mag;
					nearest_food_item = food_item;
					min_seek_frc.x = seek_force.x;
					min_seek_frc.y = seek_force.y;
				}
			}

			if(min_mag < 1 && nearest_food_item) {
				switch (nearest_food_item->as.food->as) {
					case ENERGY:
						agent_update_fitness(agent_item->as.agent, 20);
						break;
					case POISION:
						if(agent_update_fitness(agent_item->as.agent, -20) < 0){	
							list_del(&agent_item->node);
							free(agent_item->as.agent);
							agent_item->as.agent = NULL;
							free(agent_item);
							agent_item = NULL;
						}
				}

				food_random_int(nearest_food_item->as.food);

			}else if(nearest_food_item){
				set_mag(&min_seek_frc, 0.001);
				agent_apply_force(agent_item->as.agent, vect_scalar_multiply(&min_seek_frc, 1));
			}
			
			if(agent_item)	
				agent_update(agent_item->as.agent, delta*timescale);
		}
		
		/*end of update*/
		
		/*render elements*/
		__list_for_each(food_iter, &food_list) {
			food_item = list_entry(food_iter,list_item, node);
			food_render(renderer, food_item->as.food);
		}
		int count = 0;
		float avg_fitness = 0;
		__list_for_each(agent_iter, &agent_list) {
			agent_item = list_entry(agent_iter,list_item, node);
			agent_render(renderer, agent_item->as.agent);
			count++;
			avg_fitness +=agent_item->as.agent->fitness;
		}

		sprintf(text, "Agents: %d		Avg-fitness %f", count, avg_fitness/count);

		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, text, (SDL_Color){255,255,255});
		SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
		SDL_Rect Message_rect; //create a rect
		Message_rect.x = SCREEN_WIDTH/2 - 150;  //controls the rect's x coordinate 
		Message_rect.y = SCREEN_HEIGHT - 35; // controls the rect's y coordinte
		Message_rect.w = 300; // controls the width of the rect
		Message_rect.h = 40;

		SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

		/*end of render*/

		SDL_RenderPresent(renderer);
		
		SDL_FreeSurface(surfaceMessage);
		SDL_DestroyTexture(Message);
	}

	SDL_Quit();
	return 0;
} 


void test_matrix_tanspose() {
	Matrix_t mat;
	mat.row = 1;
	mat.col = 2;

	mat.data = calloc(2, sizeof(float));
	mat.data[0] = 1;
	mat.data[1] = 2;

	matrix_print(mat);
	matrix_print(matrix_transpose(&mat));

	free(mat.data);
	mat.data = NULL;


	mat.row = 2;
	mat.col = 3;

	mat.data = calloc(2*3, sizeof(float));
	mat.data[0] = 1;
	mat.data[1] = 2;
	mat.data[2] = 3;
	mat.data[3] = 4;
	mat.data[4] = 5;
	mat.data[5] = 6;

	matrix_print(mat);
	matrix_print(matrix_transpose(&mat));

	free(mat.data);
	mat.data = NULL;
}

void test_matrix_multiply(){

	Matrix_t mat_1, mat_2;
	mat_1 = matrix_init_data(1, 2, (float []){1,2}, 2);
	mat_2 = matrix_init_data(2, 1, (float []){3,4}, 2);

	matrix_print(mat_1);
	matrix_print(mat_2);

	free(mat_1.data);
	free(mat_2.data);
	mat_1.data = NULL;
	mat_2.data = NULL;

	mat_1 = matrix_init_data(2, 2, (float []){1,2, 3, 4} ,4);
	mat_2 = matrix_init_data(2, 2, (float []){5,6, 7, 8}, 4);

	matrix_print(mat_1);
	matrix_print(mat_2);
	
	Matrix_t ret = matrix_multiply(&mat_1, &mat_2);

	matrix_print(ret);

	free(mat_1.data);
	free(mat_2.data);
	mat_1.data = NULL;
	mat_2.data = NULL;
	}


int main(int c, char **argv) {
	main_gp();
	//test_matrix_tanspose();
	//test_matrix_multiply();
}
