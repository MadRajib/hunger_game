#include "agent.h"
#include "config.h"
#include <stdlib.h>

void triangle(SDL_Renderer *renderer, SDL_FPoint points[3]) {

	Triangle_t tri;

	for (int i =0; i< 3; i++) {
		tri.vertices[i].position = points[i];
		tri.vertices[i].color = (struct SDL_Color){255,255,255};
		tri.vertices[i].tex_coord = (struct SDL_FPoint) {1};

	}
	
	SDL_RenderGeometry(renderer, NULL, tri.vertices, 3, NULL , 3);
}


void agent_rotate(Agent_t *agent, float angle) {
	Vector2D_t pos;
	
	for (int i =0; i< 3; i++) {
		pos.x = agent->shape.vertices[i].position.x;
		pos.y = agent->shape.vertices[i].position.y;

		rotate(&pos, angle, agent->pivot);

		agent->shape.vertices[i].position.x = pos.x;
		agent->shape.vertices[i].position.y = pos.y;

	}
}


void agent_render(SDL_Renderer *renderer, Agent_t *agent) {
	for (int i =0; i< 3; i++) {
		agent->shape.vertices[i].color = (struct SDL_Color){255 - agent->fitness,agent->fitness, 0};
		agent->shape.vertices[i].tex_coord = (struct SDL_FPoint) {1};
	}

	SDL_RenderGeometry(renderer, NULL, agent->shape.vertices, 3, NULL , 3);
}

int agent_update_fitness(Agent_t *agent, int fitness){
	agent->fitness += fitness;
	if(agent->fitness < 0)
		return -1;

	if(agent->fitness > 255)
		agent->fitness = 255;

	return 0;
}

void agent_update(Agent_t *agent, float delta) {

	vect_add(&agent->pivot, &agent->speed);

	/*filp direction if reached boundary*/
	if(agent->pivot.x < 0.0001) {
		agent->speed.x = random_float_range(0.0001, AGENT_MAX_SPEED);
		agent->speed.y = random_float_range(-AGENT_MAX_SPEED, AGENT_MAX_SPEED);
		agent->pivot.x = 1;
	}else if(agent->pivot.x > SCREEN_WIDTH) {
		agent->speed.x = random_float_range(-AGENT_MAX_SPEED, 0.0001);
		agent->speed.y = random_float_range(-AGENT_MAX_SPEED, AGENT_MAX_SPEED);
		agent->pivot.x = SCREEN_WIDTH -1;
	}

	if(agent->pivot.y < 0.0001) {
		agent->speed.x = random_float_range(-AGENT_MAX_SPEED, AGENT_MAX_SPEED);
		agent->speed.y = random_float_range(0.0001, AGENT_MAX_SPEED);	
		agent->pivot.y = 1;
	}else if(agent->pivot.y > SCREEN_HEIGHT) {
		agent->speed.x = random_float_range(-AGENT_MAX_SPEED, AGENT_MAX_SPEED);
		agent->speed.y = random_float_range(-AGENT_MAX_SPEED, 0.0001);
		agent->pivot.y = SCREEN_HEIGHT -1;
	}
	
	agent->shape.vertices[0].position = (SDL_FPoint){ agent->pivot.x + 10, agent->pivot.y};
	agent->shape.vertices[1].position = (SDL_FPoint){ agent->pivot.x - 10, agent->pivot.y - 8};
	agent->shape.vertices[2].position = (SDL_FPoint){ agent->pivot.x - 10, agent->pivot.y + 8};
	
	agent_rotate(agent, get_angle(&agent->speed));	
}

void agent_apply_force(Agent_t *agent, Vector2D_t force) {
	agent->acc.x = force.x;
	agent->acc.y = force.y;
	
	vect_add(&agent->speed, &agent->acc);
	limit_mag(&(agent->speed) , AGENT_MAX_SPEED_LIMIT);
}

Agent_t agent_init(Vector2D_t pos) {

	Agent_t agent;	

	agent.fitness = 0;
	
	agent.speed.x = random_float_range(-AGENT_MAX_SPEED, AGENT_MAX_SPEED) ;
	agent.speed.y = random_float_range(-AGENT_MAX_SPEED, AGENT_MAX_SPEED) ;
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

	agent_rotate(&agent, get_angle(&agent.speed));

	return agent;
	
	//SDL_RenderGeometry(renderer, NULL, tri.vertices, 3, NULL , 3);
}

Agent_t * agent_ptr_init(Vector2D_t pos) {

	Agent_t *agent = (Agent_t *)malloc(sizeof(Agent_t));

	agent->fitness = 0;
	agent->speed.x = random_float_range(-AGENT_MAX_SPEED, AGENT_MAX_SPEED) ;
	agent->speed.y = random_float_range(-AGENT_MAX_SPEED, AGENT_MAX_SPEED) ;
	agent->acc = (Vector2D_t){0,0};

	agent->pivot = pos;

	Triangle_t tri;

	tri.vertices[0].position = (SDL_FPoint){ agent->pivot.x + 10, agent->pivot.y};
	tri.vertices[1].position = (SDL_FPoint){ agent->pivot.x - 10, agent->pivot.y - 8};
	tri.vertices[2].position = (SDL_FPoint){ agent->pivot.x - 10, agent->pivot.y + 8};

	for (int i =0; i< 3; i++) {
		tri.vertices[i].tex_coord = (struct SDL_FPoint) {1};
	}	

	agent->shape = tri;

	agent_rotate(agent, get_angle(&agent->speed));

	return agent;
	
	//SDL_RenderGeometry(renderer, NULL, tri.vertices, 3, NULL , 3);
}
