/*	geometry.c
	
	all mathematical and vector oparations will stay here
	called by rendering functions (from files boat_model.c and landscape.c)
*/

#include "open.h"

GLdouble cosInterpolate(GLdouble v1, GLdouble v2, GLdouble x)
{
	GLfloat ftemp = x * PI, f;
	f = (1 - cos(ftemp)) * 0.5;
	return v1 * (1 - f) + v2 * f;
}

GLvoid normalise(GLfloat vert[])
{
	/*
	CORRECT IDENTITY VECTOR CALCULATOR!!!
	//*/
	GLfloat d;
	int i;

	d = sqrt(vert[0] * vert[0] + vert[1] * vert[1] + vert[2] * vert[2]);
	if (d > 0.0)
		for (i = 0; i < 3; i++) vert[i] /= d;
	
}

GLvoid cross(GLfloat vertA[], GLfloat vertB[], GLfloat vertC[], GLfloat n[])
{
	n[0] = (vertB[1] - vertA[1]) * (vertC[2] - vertA[2]) - (vertB[2] - vertA[2]) * (vertC[1] - vertA[1]);
	n[1] = (vertB[2] - vertA[2]) * (vertC[0] - vertA[0]) - (vertB[0] - vertA[0]) * (vertC[2] - vertA[2]);
	n[2] = (vertB[0] - vertA[0]) * (vertC[1] - vertA[1]) - (vertB[1] - vertA[1]) * (vertC[0] - vertA[0]);
}

/////////////////////////////
//some additionals primitive functions
/////////////////////////////
//modulus
GLfloat mod(GLfloat x)
{
	return (x >= 0.0) ? x : -x;
}

//random number generator
GLdouble drand48(void)
{
	return (double) rand() / (double) RAND_MAX;
}

//square root
GLdouble sqr(GLdouble x)
{
	return x * x;
}
