#ifndef TZ_TRACER_SAMPLER
#define TZ_TRACER_SAMPLER

#include <vector>
#include <math.h>

#include "tzPoint2D.h"
#include "tzPoint3D.h"
#include "../include/tzMaths.h"

using namespace std;

class tzISampler {	
	public:	
		
		tzISampler(void);
		
		tzISampler(const int num);
		
		tzISampler(const int num, const int numSets);

		tzISampler(const tzISampler& s);

		tzISampler& operator= (const tzISampler& rhs);

		virtual tzISampler* clone(void) const = 0;			

		virtual ~tzISampler(void);
		
		void setNumSets(const int np);					
		
		virtual void generate_samples(void) = 0;

		int getNumSamples(void);							
		
		void shuffleXCoordinates(void);
		
		void shuffleYCoordinates(void);
			
		void setupShuffledIndices(void);
		
		void mapSamplesToUnitDisk(void);
		
		void mapSamplesToHemisphere(const float p);		
		
		void mapSamplesToSphere(void);					
		
		
		// the following functions are not const because they change count and jump
		
		tzPoint2D sampleUnitSquare(void);
		
		tzPoint2D sampleUnitDisk(void);
		
		tzPoint3D sampleHemisphere(void);
		
		tzPoint3D sampleSphere(void);
		
		tzPoint2D sampleOneSet(void);							// this is not discussed in the book, but see the
														// file LatticeNoise.cpp in Chapter 31
		
	protected:
	
		int 					mNumSamples;     		// the number of sample points in a set
		int 					mNumSets;				// the number of sample sets
		vector<tzPoint2D>		mSamples;				// sample points on a unit square
		vector<int>				mShuffledIndices;		// shuffled samples array indices
		vector<tzPoint2D>		mDiskSamples;			// sample points on a unit disk
		vector<tzPoint3D> 		mHemisphereSamples;		// sample points on a unit hemisphere
		vector<tzPoint3D> 		mSphereSamples;			// sample points on a unit sphere
		unsigned long 			mCount;					// the current number of sample points used
		int 					mJump;					// random index jump
};

#endif

