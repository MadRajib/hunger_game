#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h> 
#include <assert.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#include "./style.h"
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
	const int agent_count = 10;
	const int food_count = 5;

	Vector2D_t tmp;

	Agent_t agents[agent_count];
	clock_t p_clk, delta;
	float timescale = 1;

	/* Init SDL */
	scc(SDL_Init(SDL_INIT_VIDEO));

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

	/*done init*/

	srand(time(NULL));

	/**/
	LIST_HEAD(food_list);

	for (int i=0; i< food_count; i++) {
		Food_t *food = food_int(vect_get_random(20, 600), (Vector2D_t){10,10}, (Color_t){0,255,0, 255}, ENERGY);
		list_item *item = (list_item *) malloc(sizeof(list_item));
		item->as.food = food;
		INIT_LIST_HEAD(&item->node);
		list_add(&item->node, &food_list);
	}

	struct list_head *iter;
	list_item *item;


	for (int i =0; i< agent_count ; i++) {	
		vect_set_random(&tmp, 20, 600);
		agents[i] = agent_init(tmp);
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


		__list_for_each(iter, &food_list) {
			item = list_entry(iter,list_item, node);
			food_render(renderer, item->as.food);
		}
	
		for (int i = 0; i< agent_count ; i++) {
			
			float min_mag = 100;
			list_item *food_item = NULL;
			Vector2D_t min_seek_frc = {0, 0};

			__list_for_each(iter, &food_list) {
				item = list_entry(iter,list_item, node);	
				Vector2D_t seek_force = sub_vect(&item->as.food->pos, &agents[i].pivot);
				float mag = get_mag(&seek_force);
				if( mag < min_mag) {
					min_mag = mag;
					food_item = item;
					min_seek_frc.x = seek_force.x;
					min_seek_frc.y = seek_force.y;
				}
			}

			if(min_mag < 1 && food_item) {
				food_item->as.food->pos =  vect_get_random(20 , 600);	

			}else if(food_item){
				set_mag(&min_seek_frc, 0.001);
				agent_apply_force(&agents[i], vect_scalar_multiply(&min_seek_frc, 1));
			}
			
			agent_update(&agents[i], delta*timescale);
		}
		
		for (int i = 0; i< agent_count ; i++) {
			agent_render(renderer, &agents[i]);
		}

		SDL_RenderPresent(renderer);
		
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
