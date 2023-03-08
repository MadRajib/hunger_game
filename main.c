#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include <stdio.h> 
#include <assert.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#include "./style.h"
#include "utils.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

typedef struct{
	int p_count;
	SDL_Vertex vertices[3];
} Triangle_t;

struct Agent_t;
typedef struct{	
	Triangle_t shape;
	Vector2D_t speed;
	Vector2D_t acc;
	Vector2D_t head;
	Vector2D_t pivot;
} Agent_t;

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

void triangle(SDL_Renderer *renderer, SDL_FPoint points[3]) {

	Triangle_t tri;

	for (int i =0; i< 3; i++) {
		tri.vertices[i].position = points[i];
		tri.vertices[i].color = (struct SDL_Color){255,255,255};
		tri.vertices[i].tex_coord = (struct SDL_FPoint) {1};

	}
	
	SDL_RenderGeometry(renderer, NULL, tri.vertices, 3, NULL , 3);
}



void rotate_agent(Agent_t *agent, float angle) {
	Vector2D_t pos;
	
	for (int i =0; i< 3; i++) {
		pos.x = agent->shape.vertices[i].position.x;
		pos.y = agent->shape.vertices[i].position.y;

		rotate(&pos, angle, agent->pivot);

		agent->shape.vertices[i].position.x = pos.x;
		agent->shape.vertices[i].position.y = pos.y;

	}
}


void render_agent(SDL_Renderer *renderer, Agent_t *agent) {
	SDL_RenderGeometry(renderer, NULL, agent->shape.vertices, 3, NULL , 3);
}


void update_agent(Agent_t *agent) {


	vect_add(&agent->speed, &agent->acc);
	limit_mag(&(agent->speed) , 0.04);
	vect_add(&agent->pivot, &agent->speed);
	/*filp direction if reached boundary*/
	if(agent->pivot.x < 0.0001) {
		agent->speed.x = random_float_range(0.0001, 0.029);
		agent->speed.y = random_float_range(-0.028, 0.029);
	}else if(agent->pivot.x > SCREEN_WIDTH) {
		agent->speed.x = random_float_range(-0.028, 0.0001);
		agent->speed.y = random_float_range(-0.028, 0.029);
	}

	if(agent->pivot.y < 0.0001) {
		agent->speed.x = random_float_range(-0.028, 0.029);
		agent->speed.y = random_float_range(0.0001, 0.029);
	}else if(agent->pivot.y > SCREEN_HEIGHT) {
		agent->speed.x = random_float_range(-0.028, 0.029);
		agent->speed.y = random_float_range(-0.028, 0.0001);
	}
	
	agent->shape.vertices[0].position = (SDL_FPoint){ agent->pivot.x + 10, agent->pivot.y};
	agent->shape.vertices[1].position = (SDL_FPoint){ agent->pivot.x - 10, agent->pivot.y - 8};
	agent->shape.vertices[2].position = (SDL_FPoint){ agent->pivot.x - 10, agent->pivot.y + 8};
	
	rotate_agent(agent, get_angle(&agent->speed));	
	
	agent->acc.x = 0;
	agent->acc.y = 0;
}

void apply_force(Agent_t *agent, Vector2D_t force) {
	agent->acc.x = force.x;
	agent->acc.y = force.y;
}

Agent_t init_agent(Vector2D_t pos) {

	Agent_t agent;	
	
	agent.speed.x = random_float_range(-0.028, 0.029) ;
	agent.speed.y = random_float_range(-0.028, 0.029) ;
	agent.acc = (Vector2D_t){0,0};

	agent.pivot = pos;

	Triangle_t tri;

	tri.vertices[0].position = (SDL_FPoint){ agent.pivot.x + 10, agent.pivot.y};
	tri.vertices[1].position = (SDL_FPoint){ agent.pivot.x - 10, agent.pivot.y - 8};
	tri.vertices[2].position = (SDL_FPoint){ agent.pivot.x - 10, agent.pivot.y + 8};

	for (int i =0; i< 3; i++) {
		tri.vertices[i].color = (struct SDL_Color){255,255,255};
		tri.vertices[i].tex_coord = (struct SDL_FPoint) {1};
	}	

	agent.shape = tri;

	rotate_agent(&agent, get_angle(&agent.speed));


	return agent;
	
	//SDL_RenderGeometry(renderer, NULL, tri.vertices, 3, NULL , 3);
}

int mouse_inside_window(Vector2D_t *pos){
	if(pos->x <= 0 || pos->y <= 0 || pos->x >= (SCREEN_WIDTH *0.98) || pos->y >= (SCREEN_HEIGHT*0.98))
		return 0;
	return 1;
}

int main_gp() {
	
	Vector2D_t mouse_pos = {0, 0};
	Vector2D_t seek_force;

	int quit = 0;
	const int agent_count = 100;

	Vector2D_t tmp;

	Agent_t agents[agent_count];

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



	srand(time(0));
	for (int i =0; i< agent_count ; i++) {	
		vect_get_random(&tmp, 20, 600);
		agents[i] = init_agent(tmp);
	}
	
	
	while (!quit) {
		SDL_Event event;
		int pos_x  = 0;
		int pos_y  = 0;
		
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
		

		sdl_set_color_hex(renderer, BACKGROUND_CLR);
		scc(SDL_RenderClear(renderer));	

		SDL_GetMouseState(&pos_x, &pos_y);
		
		mouse_pos.x = pos_x;
		mouse_pos.y = pos_y;
	
		for (int i = 0; i< agent_count ; i++) {
			seek_force = sub_vect(&mouse_pos, &agents[i].pivot);

			if(mouse_inside_window(&mouse_pos) && get_mag(&seek_force) < 100){
				set_mag(&seek_force, 0.001);
				apply_force(&agents[i], scalar_mult_vect(&seek_force, -1));
			}
			
			render_agent(renderer, &agents[i]);
			update_agent(&agents[i]);

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
	//main_gp();
	//test_matrix_tanspose();
	test_matrix_multiply();
}
