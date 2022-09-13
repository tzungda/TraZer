#ifndef TZ_TRACER_SAMPLER
#define TZ_TRACER_SAMPLER

#include <vector>
#include <math.h>
#include <memory>

#include "tzPoint2D.h"
#include "tzPoint3D.h"
#include "../include/tzMaths.h"
#include "../include/tzShadeRec.h"


class tzISampler {	
	public:	
		
		tzISampler(void);
		
		tzISampler(const int num);
		
		tzISampler(const int num, const int numSets);

		tzISampler(const tzISampler& s);

		tzISampler& operator= (const tzISampler& rhs);

		virtual std::shared_ptr< tzISampler > clone(void) const = 0;			

		virtual ~tzISampler(void);
		
		void setNumSets(const int np);					
		
		virtual void generateSamples(void) = 0;

		int getNumSamples(void);							
		
		void shuffleXCoordinates(void);
		
		void shuffleYCoordinates(void);
			
		void setupShuffledIndices(void);
		
		void mapSamplesToUnitDisk(void);
		
		void mapSamplesToHemisphere(const float p);		
		
		void mapSamplesToSphere(void);					
		
		
		// the following functions are not const because they change count and jump
		
		tzPoint2D sampleUnitSquare(const tzRay &ray);
		
		tzPoint2D sampleUnitDisk(const tzRay &rayr);
		
		tzPoint3D sampleHemisphere(const tzRay &ray);
		
		tzPoint3D sampleSphere(const tzRay &ray);
		
		tzPoint2D sampleOneSet(const tzRay &ray);							// this is not discussed in the book, but see the
														// file LatticeNoise.cpp in Chapter 31
		
	protected:
	
		int 					mNumSamples;     		// the number of sample points in a set
		int 					mNumSets;				// the number of sample sets
		std::vector<tzPoint2D>		mSamples;				// sample points on a unit square
		std::vector<int>				mShuffledIndices;		// shuffled samples array indices
		std::vector<tzPoint2D>		mDiskSamples;			// sample points on a unit disk
		std::vector<tzPoint3D> 		mHemisphereSamples;		// sample points on a unit hemisphere
		std::vector<tzPoint3D> 		mSphereSamples;			// sample points on a unit sphere
		unsigned long 			mCount[MAX_THREADS];					// the current number of sample points used
		int 					mJump[MAX_THREADS];					// random index jump
};

#endif

