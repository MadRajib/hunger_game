#ifndef UITLS_H
#define UITLS_H

typedef struct{
	float x;
	float y;
}Vector2D_t;

#define V_FMT "%f %f\n"
#define V_ARG(arg) arg.x, arg.y
int random_int_range(int low, int high);
float random_float_range(float LO, float HI);

void rotate(Vector2D_t *vect, float angle, Vector2D_t pivot);
float get_angle(Vector2D_t *vect);
float get_mag(Vector2D_t *vect);
void set_mag(Vector2D_t *vect, float mag);
void limit_mag(Vector2D_t *vect, float mag);
float angle_between(Vector2D_t *v1, Vector2D_t *v2); 

void vect_add(Vector2D_t *to, Vector2D_t *vec);
void vect_sub(Vector2D_t *from, Vector2D_t *vec);
void vect_get_random(Vector2D_t *vec, int _min, int _max); 
void vect_sub(Vector2D_t *from, Vector2D_t *vec);
Vector2D_t sub_vect(Vector2D_t *v1, Vector2D_t *v2);
#endif
