#include "utils.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int random_int_range(int low, int high) {
	return rand() % (high - low) + low;
}

float random_float_range(float LO, float HI) {
	return LO + (float) (rand()) /( (float) (RAND_MAX/(HI-LO)));
}

void rotate(Vector2D_t *vect, float radians, Vector2D_t pivot) {
	float x = cosf(radians) * (vect->x - pivot.x) - sinf(radians) * (vect->y - pivot.y) + pivot.x ;
	float y = sinf(radians) * (vect->x - pivot.x) + cosf(radians) * (vect->y - pivot.y) + pivot.y ; 

	vect->x = x;
	vect->y = y;
}

float get_angle(Vector2D_t *vect) {
	//if (vect->x == 0) return (vect->y > 0)?90:-90;
	if (vect->y == 0 && vect->x ==0 ) return 0;
	return  atan2(100 * vect->y, 100 * vect->x);
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

Vector2D_t add_vect(Vector2D_t *v1, Vector2D_t *v2) {
	return (Vector2D_t){v1->x + v2->x, v1->y + v2->y};
}

void vect_sub(Vector2D_t *from, Vector2D_t *vec) {
   from->x -= vec->x;
   from->y -= vec->y;
}

Vector2D_t sub_vect(Vector2D_t *v1, Vector2D_t *v2) {
	return (Vector2D_t){v1->x - v2->x, v1->y - v2->y};
}

Vector2D_t dot_vect(Vector2D_t *v1, Vector2D_t *v2) {
	return (Vector2D_t){v1->x * v2->x, v1->y * v2->y};
}

Vector2D_t vect_scalar_multiply(Vector2D_t *v1, float val) {
	return (Vector2D_t){v1->x * val, v1->y * val};
}

void vect_scalar_multiply_inplace(Vector2D_t *v1, float val) {
	v1->x *= val;
	v1->y *= val;
}

void vect_set_random(Vector2D_t *vec, int _min, int _max) {
	vec->x = random_int_range(_min, _max);	
	vec->y = random_int_range(_min, _max);
}

Vector2D_t vect_get_random(int _min, int _max) {
	Vector2D_t vec;
	vec.x = random_int_range(_min, _max);	
	vec.y = random_int_range(_min, _max);
	return vec;
}
/*
	Matrix_t mat;
	mat.row = 2;
	mat.col = 2;

	mat.data = calloc(mat.row * mat.col, sizeof(float));
	mat.data[3] = mat.data[0] = cosf(radian);	
	mat.data[2] = sinf(radian);	
	mat.data[1] = -1 * mat.data[3];


	free(mat.data);
	mat.data = NULL;
*/

Matrix_t matrix_init(int rows, int cols) {
	Matrix_t mat;

	mat.row = rows;
	mat.col = cols;
	mat.count = mat.row * mat.col;
	mat.data = calloc(mat.count, sizeof(float));

	return mat;
}


Matrix_t matrix_init_data(int rows, int cols, float *data, int data_len) {
	Matrix_t mat;

	mat = matrix_init(rows, cols);

	for (int i = 0; (i < mat.count && i < data_len) ; i++) {
		mat.data[i] = data[i];
	}

	return mat;
}
Matrix_t matrix_transpose(Matrix_t *m_1) {
	Matrix_t mat;
	mat.row = m_1->col;
	mat.col = m_1->row;

	mat.data = calloc(mat.row * mat.col, sizeof(float));
	size_t count = 0;
	for (int c=0; c < m_1->col ; c++) {
		for( int r= 0; r < m_1->row; r++) {
			int indx = c + (r*m_1->col);
			mat.data[count] = m_1->data[indx];
			count++;
		}
	}

	return mat;
}

Matrix_t matrix_multiply(Matrix_t *m1, Matrix_t *m2) {
	Matrix_t mat;
	mat.row = mat.col = -1; 

	if(m1->col != m2->row)
		return mat;
	mat.row = m1->row;
	mat.col = m2->col;

	mat.data = calloc(mat.row * mat.col, sizeof(float));
	size_t count = 0;
	for(int i = 0; i < m1->row; i++ ) {
		for(int j = 0; j < m2->col; j++ ) {
			int sum = 0;
			for (int k =0; k < m2->row ; k++) {
				int m1_indx = k + (m1->col * i);
				int m2_indx = k + (m2->row * j);
				sum += m1->data[m1_indx] * m2->data[m2_indx]; 
			}
			mat.data[count++] = sum;
		}
	}
	return mat;

}

void matrix_print(Matrix_t m) {

	for(int i =0; i< m.row ;i++) {
		for(int j = 0; j< m.col;j++) {
			int indx = i*m.col + j;
			printf("%f ",m.data[indx]);
		}
		printf("\n");
	}

	printf("\n");
}



