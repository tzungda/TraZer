#ifndef TZ_MATHS
#define TZ_MATHS


#include <stdlib.h>

#include "../include/tzConstants.h"
#include "tzColor.h"


// prototypes

float
_min(float x0, float x1);

float
_max(float x0, float x1);


int							
rand_int(void);

float
rand_float(void);

void
set_rand_seed(const int seed);

float 						// for multi-jittered sampling
rand_float(int l, float h);	

int							// for multi-jittered sampling
rand_int(int l, int h);   

float
clamp(const float x, const float min, const float max);

int
SolveQuadric(float c[3], float s[2]);

int 
SolveCubic(float c[4], float s[3]);

int 
SolveQuartic(float c[5], float s[4]);



// inlined functions
//
//===================================================================================
inline float _min(float x0, float x1) 
{
	return ((x0 < x1) ? x0 : x1);
}

//===================================================================================
inline float _max(float x0, float x1) 
{
	return ((x0 > x1) ? x0 : x1);
}

//===================================================================================
inline int rand_int(void) 
{
	return(rand());
}

//===================================================================================
inline float rand_float(void) 
{
	return((float)rand_int() * invRAND_MAX);
}

//===================================================================================
inline void set_rand_seed(const int seed) 
{
	srand(seed);
}

//===================================================================================
inline float rand_float(int l, float h) 
{
	return (rand_float() * (h - l) + l);
}

//===================================================================================
inline int rand_int(int l, int h) 
{
	return ((int) (rand_float(0, (float)(h - l + 1)) + l));
}

//===================================================================================
inline float clamp(const float x, const float min, const float max) 
{
	return (x < min ? min : (x > max ? max : x));
}

#endif
