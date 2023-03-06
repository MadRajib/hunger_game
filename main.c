#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h> 
#include <assert.h>
#include <SDL2/SDL.h>

#include "./style.h"
#include "utils.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 10

#define CELL_WIDTH ( (float) SCREEN_WIDTH/ BOARD_WIDTH)
#define CELL_HEIGHT ( (float) SCREEN_HEIGHT/ BOARD_HEIGHT)

#define AGENT_PADDING 10
#define AGENT_HEAD_HEIGHT 20
#define AGENT_HEAD_WIDTH 20
#define AGENTS_COUNT 10
#define AGENT_SPEED 0.01

typedef enum {
	DIR_RIGHT = 0,
	DIR_UP,
	DIR_LEFT,
	DIR_DOWN,
} Dir;

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

int random_int_range(int low, int high) {

	return rand() % (high - low) + low;
}

Dir random_dir(void) {
	return (Dir) random_int_range(0, 4);
}


void triangle(SDL_Renderer *renderer, SDL_FPoint points[3]) {

	Triangle_t tri;

	float g_x = 0;
	float g_y = 0;

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

	Vector2D_t prev_speed;
	prev_speed = agent->speed;
	
	vect_add(&agent->speed, &agent->acc);
	limit_mag(&(agent->speed) , 0.04);

	vect_add(&agent->pivot, &agent->speed);
	
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
	agent.speed.x = -1;
	agent.speed.y = 1;

	agent.pivot = pos;

	Triangle_t tri;

	float g_x = 0;
	float g_y = 0;

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


int main(int c, char **argv) {
	
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

	int quit = 0;
	Agent_t a1 = init_agent((Vector2D_t){400, 400});
	while (!quit) {
		SDL_Event event;
		Vector2D_t agent_speed = {0, 0};
		Vector2D_t mouse_pos = {0, 0};
		Vector2D_t seek_force = {0, 0};
		int pos_x  = 0;
		int pos_y  = 0;

		
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					agent_speed.x = 0;
					agent_speed.y = 0;

					switch ( event.key.keysym.sym ) {
					case SDLK_LEFT:
						agent_speed.x -= AGENT_SPEED;
						break;
					case SDLK_RIGHT:
						agent_speed.x += AGENT_SPEED;
						break;
					case SDLK_UP:
						agent_speed.y -= AGENT_SPEED;
						break;
					case SDLK_DOWN:
						agent_speed.y += AGENT_SPEED;
						break;
					default:
						break;

					}
					//apply_force(&a1, agent_speed);
					break;
				case SDL_QUIT:
					quit = 1;
					break;
			}
		}
		

		SDL_GetMouseState(&pos_x, &pos_y);
		//printf("%d %d %f %f\n", pos_x, pos_y, a1.shape.vertices[0].position.x, a1.shape.vertices[0].position.y);
		mouse_pos.x = pos_x;
		mouse_pos.y = pos_y;
		
		seek_force.x = mouse_pos.x - a1.pivot.x;
		seek_force.y = mouse_pos.y - a1.pivot.y; 
		
	    //printf("mouse "V_FMT, V_ARG(mouse_pos));
	    //printf("frce "V_FMT, V_ARG(seek_force));
		set_mag(&seek_force, 0.001);
		apply_force(&a1, seek_force);


		sdl_set_color_hex(renderer, BACKGROUND_CLR);
		scc(SDL_RenderClear(renderer));	

		render_agent(renderer, &a1);
		update_agent(&a1);

		SDL_RenderPresent(renderer);
	}

	SDL_Quit();
	return 0;
} 
