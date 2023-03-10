#ifndef UITLS_H
#define UITLS_H

typedef struct{
	float x;
	float y;
}Vector2D_t;

typedef struct{
	unsigned int row, col, count;
	float *data;
}Matrix_t;

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
void vect_set_random(Vector2D_t *vec, int _min, int _max); 
Vector2D_t vect_get_random(int _min, int _max); 
Vector2D_t sub_vect(Vector2D_t *v1, Vector2D_t *v2);
Vector2D_t add_vect(Vector2D_t *v1, Vector2D_t *v2);
Vector2D_t dot_vect(Vector2D_t *v1, Vector2D_t *v2);
Vector2D_t scalar_mult_vect(Vector2D_t *v1, float val);
Vector2D_t vect_scalar_multiply(Vector2D_t *v1, float val);
void vect_scalar_multiply_inplace(Vector2D_t *v1, float val);

/*Matrix apis*/
Matrix_t matrix_init(int rows, int cols);
Matrix_t matrix_init_data(int rows, int cols, float *data, int data_len);
Matrix_t matrix_transpose(Matrix_t *m_1);
Matrix_t matrix_multiply(Matrix_t *m1, Matrix_t *m2);
void matrix_print(Matrix_t m);
#endif
