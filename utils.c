#include "utils.h"
#include <math.h>
#include <stdio.h>

void rotate(Vector2D_t *vect, float radians, Vector2D_t pivot) {
	float x = cosf(radians) * (vect->x - pivot.x) - sinf(radians) * (vect->y - pivot.y) + pivot.x ;
	float y = sinf(radians) * (vect->x - pivot.x) + cosf(radians) * (vect->y - pivot.y) + pivot.y ; 

	vect->x = x;
	vect->y = y;
}

float get_angle(Vector2D_t *vect) {
	//if (vect->x == 0) return (vect->y > 0)?90:-90;
	if (vect->y == 0 && vect->x ==0 ) return 0;
	return  atan2(vect->y, vect->x);
}

float get_mag(Vector2D_t *vect) {
	return sqrtf(pow(vect->x, 2) +  pow(vect->y, 2));
}

void set_mag(Vector2D_t *vect, float mag) {
	float cur_mag = get_mag(vect);
	vect->x = (vect->x / cur_mag ) * mag; 
	vect->y = (vect->y / cur_mag ) * mag; 
}

void limit_mag(Vector2D_t *vect, float mag) {
	if(get_mag(vect) >= mag) {
		set_mag(vect, mag);
	}
}

float angle_between(Vector2D_t *v1, Vector2D_t *v2) {	
	float dot = v1->x * v2->x + v1->y*v2->y;
	float det = v1->x*v2->y - v1->y*v2->x;
	return atan2(det, dot);
	
}

void vect_add(Vector2D_t *to, Vector2D_t *vec) {
   to->x += vec->x;
   to->y += vec->y;
}

void vect_sub(Vector2D_t *from, Vector2D_t *vec) {
   from->x -= vec->x;
   from->y -= vec->y;
}
