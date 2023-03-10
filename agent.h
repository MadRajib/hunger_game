#ifndef _AGENT_H
#define _AGENT_H
#include <SDL2/SDL.h>
#include "utils.h"

typedef struct{
	int p_count;
	SDL_Vertex vertices[3];
} Triangle_t;

typedef struct{	
	Triangle_t shape;
	Vector2D_t speed;
	Vector2D_t acc;
	Vector2D_t head;
	Vector2D_t pivot;
} Agent_t;


void agent_rotate(Agent_t *agent, float angle);
void agent_render(SDL_Renderer *renderer, Agent_t *agent);

void agent_update(Agent_t *agent, float delta);
void agent_apply_force(Agent_t *agent, Vector2D_t force);
Agent_t agent_init(Vector2D_t pos);
Agent_t *agent_ptr_init(Vector2D_t pos);

#endif
