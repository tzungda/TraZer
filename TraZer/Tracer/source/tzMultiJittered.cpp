
#include "../include/tzMultiJittered.h"
#include <math.h>

// ---------------------------------------------------------------- default constructor
	
tzMultiJittered::tzMultiJittered(void)
	: tzISampler()
{}


// ---------------------------------------------------------------- constructor

tzMultiJittered::tzMultiJittered(const int num_samples)
	: tzISampler(num_samples) {
//	count = 0;
//	jump = 0;
	generate_samples();
}


// ---------------------------------------------------------------- constructor

tzMultiJittered::tzMultiJittered(const int num_samples, const int m)
	: tzISampler(num_samples, m) {
	generate_samples();
}


// ---------------------------------------------------------------- copy constructor

tzMultiJittered::tzMultiJittered(const tzMultiJittered& mjs)
	: tzISampler(mjs)
{
	generate_samples();
}

// ---------------------------------------------------------------- assignment operator

tzMultiJittered&
tzMultiJittered::operator= (const tzMultiJittered& rhs)	{
	if (this == &rhs)
		return (*this);
		
	tzISampler::operator=(rhs);

	return (*this);
}

// ---------------------------------------------------------------- clone

tzMultiJittered*
tzMultiJittered::clone(void) const {
	return (new tzMultiJittered(*this));
}

// ---------------------------------------------------------------- destructor			

tzMultiJittered::~tzMultiJittered(void) {}


// ---------------------------------------------------------------- shuffle_x_coordinates

/*

void
MultiJittered::shuffle_x_coordinates(void) {
	for (int p = 0; p < num_sets; p++) {
			for (int i = 0; i < n; i++)		
				for (int j = 0; j < n; j++) {
					int k = rand_int(j, n - 1);
					float t = samples[i * n + j + p * num_samples].x;
					samples[i * n + j + p * num_samples].x = samples[i * n + k + p * num_samples].x;
					samples[i * n + k + p * num_samples].x = t;
				}
		}
}

*/


// ---------------------------------------------------------------- generate_samples

// This is based on code in Chui et al. (1994), cited in the references	
// The overloaded functions rand_int and rand_float (called from rand_int), which take arguments,
// are defined in Maths.h
// They should be defined here, as this is the only place they are usedm but I couldn't get them to compile

void
tzMultiJittered::generate_samples(void) {
	// num_samples needs to be a perfect square
			
	int n = (int)sqrt((float)num_samples);
	float subcell_width = 1.0f / ((float) num_samples);
	
	// fill the samples array with dummy points to allow us to use the [ ] notation when we set the 
	// initial patterns
	
	tzPoint2D fill_point;
	for (int j = 0; j < num_samples * num_sets; j++)
		samples.push_back(fill_point);
		
	// distribute points in the initial patterns
	
	for (int p = 0; p < num_sets; p++) 
		for (int i = 0; i < n; i++)		
			for (int j = 0; j < n; j++) {
				samples[i * n + j + p * num_samples].x = (i * n + j) * subcell_width + rand_float(0, subcell_width);
				samples[i * n + j + p * num_samples].y = (j * n + i) * subcell_width + rand_float(0, subcell_width);
			}
	
	// shuffle x coordinates
	
	for (int p = 0; p < num_sets; p++) 
		for (int i = 0; i < n; i++)		
			for (int j = 0; j < n; j++) {
				int k = (int)fmin( n-1, rand_int(j, n - 1) );
				float t = samples[i * n + j + p * num_samples].x;
				samples[i * n + j + p * num_samples].x = samples[i * n + k + p * num_samples].x;
				samples[i * n + k + p * num_samples].x = t;
			}

	// shuffle y coordinates
	
	for (int p = 0; p < num_sets; p++)
		for (int i = 0; i < n; i++)		
			for (int j = 0; j < n; j++) {
				int k = (int)fmin( n-1, rand_int(j, n - 1) );
				float t = samples[j * n + i + p * num_samples].y;
				samples[j * n + i + p * num_samples].y = samples[k * n + i + p * num_samples].y;
				samples[k * n + i + p * num_samples].y = t;
		}
}


