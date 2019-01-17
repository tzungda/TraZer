
#include "../include/tzMultiJittered.h"
#include <math.h>

//===================================================================================	
tzMultiJittered::tzMultiJittered(void)
	: tzISampler()
{}


//===================================================================================
tzMultiJittered::tzMultiJittered(const int numSamples)
	: tzISampler(numSamples)
{
//	count = 0;
//	jump = 0;
	generateSamples();
}

//===================================================================================
tzMultiJittered::tzMultiJittered(const int numSamples, const int m)
	: tzISampler(numSamples, m)
{
	generateSamples();
}


//===================================================================================
tzMultiJittered::tzMultiJittered(const tzMultiJittered& mjs)
	: tzISampler(mjs)
{
	generateSamples();
}

//===================================================================================
tzMultiJittered& tzMultiJittered::operator= (const tzMultiJittered& rhs)	
{
	if (this == &rhs)
		return (*this);
		
	tzISampler::operator=(rhs);

	return (*this);
}

//===================================================================================
tzMultiJittered* tzMultiJittered::clone(void) const 
{
	return (new tzMultiJittered(*this));
}

//===================================================================================
tzMultiJittered::~tzMultiJittered(void) {}


// ---------------------------------------------------------------- shuffleXCoordinates

/*

void
MultiJittered::shuffleXCoordinates(void) {
	for (int p = 0; p < mNumSets; p++) {
			for (int i = 0; i < n; i++)		
				for (int j = 0; j < n; j++) {
					int k = rand_int(j, n - 1);
					float t = samples[i * n + j + p * mNumSamples].x;
					samples[i * n + j + p * mNumSamples].x = samples[i * n + k + p * mNumSamples].x;
					samples[i * n + k + p * mNumSamples].x = t;
				}
		}
}

*/


//===================================================================================
void tzMultiJittered::generateSamples(void) 
{
	// mNumSamples needs to be a perfect square
			
	int n = (int)sqrt((float)mNumSamples);
	float subcell_width = 1.0f / ((float) mNumSamples);
	
	// fill the samples array with dummy points to allow us to use the [ ] notation when we set the 
	// initial patterns
	
	tzPoint2D fill_point;
	for (int j = 0; j < mNumSamples * mNumSets; j++)
		mSamples.push_back(fill_point);
		
	// distribute points in the initial patterns
	
	for (int p = 0; p < mNumSets; p++) 
		for (int i = 0; i < n; i++)		
			for (int j = 0; j < n; j++) {
				mSamples[i * n + j + p * mNumSamples].x = (i * n + j) * subcell_width + rand_float(0, subcell_width);
				mSamples[i * n + j + p * mNumSamples].y = (j * n + i) * subcell_width + rand_float(0, subcell_width);
			}
	
	// shuffle x coordinates
	
	for (int p = 0; p < mNumSets; p++) 
		for (int i = 0; i < n; i++)		
			for (int j = 0; j < n; j++) {
				int k = (int)fmin( n-1, rand_int(j, n - 1) );
				float t = mSamples[i * n + j + p * mNumSamples].x;
				mSamples[i * n + j + p * mNumSamples].x = mSamples[i * n + k + p * mNumSamples].x;
				mSamples[i * n + k + p * mNumSamples].x = t;
			}

	// shuffle y coordinates
	
	for (int p = 0; p < mNumSets; p++)
		for (int i = 0; i < n; i++)		
			for (int j = 0; j < n; j++) {
				int k = (int)fmin( n-1, rand_int(j, n - 1) );
				float t = mSamples[j * n + i + p * mNumSamples].y;
				mSamples[j * n + i + p * mNumSamples].y = mSamples[k * n + i + p * mNumSamples].y;
				mSamples[k * n + i + p * mNumSamples].y = t;
		}
}


