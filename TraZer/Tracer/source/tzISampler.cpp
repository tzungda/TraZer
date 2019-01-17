

#include <algorithm>   // for random_shuffle in Sampler::setupShuffledIndices

#include "../include/tzConstants.h"
#include "../include/tzISampler.h"

//===================================================================================
tzISampler::tzISampler(void)
	: 	mNumSamples(1),
	mNumSets(83)
{
	mSamples.reserve(mNumSamples * mNumSets);
	setupShuffledIndices();

	for ( int i = 0; i < MAX_THREADS; i++ )
	{
		mCount[i] = 0;
		mJump[i] = 0;
	}
}


//===================================================================================
tzISampler::tzISampler(const int ns)
	: 	mNumSamples(ns),
	mNumSets(83)
{
	mSamples.reserve(mNumSamples * mNumSets);
	setupShuffledIndices();

	for (int i = 0; i < MAX_THREADS; i++)
	{
		mCount[i] = 0;
		mJump[i] = 0;
	}
}

//===================================================================================
tzISampler::tzISampler(const int ns, const int n_sets)
	: 	mNumSamples(ns),
		mNumSets(n_sets)
{
	mSamples.reserve(mNumSamples * mNumSets);
	setupShuffledIndices();

	for (int i = 0; i < MAX_THREADS; i++)
	{
		mCount[i] = 0;
		mJump[i] = 0;
	}
}


//===================================================================================
tzISampler::tzISampler(const tzISampler& s)
	: 	mNumSamples(s.mNumSamples),
		mNumSets(s.mNumSets),
		mSamples(s.mSamples),
		mShuffledIndices(s.mShuffledIndices),
		mDiskSamples(s.mDiskSamples),
		mHemisphereSamples(s.mHemisphereSamples),
		mSphereSamples(s.mSphereSamples)
{
	for (int i = 0; i < MAX_THREADS; i++)
	{
		mCount[i] = s.mCount[i];
		mJump[i] = s.mJump[i];
	}
}


//===================================================================================
tzISampler& tzISampler::operator= (const tzISampler& rhs)	
{
	if (this == &rhs)
		return (*this);
	
	mNumSamples 		= rhs.mNumSamples;
	mNumSets			= rhs.mNumSets;
	mSamples = rhs.mSamples;
	mShuffledIndices	= rhs.mShuffledIndices;
	mDiskSamples		= rhs.mDiskSamples;
	mHemisphereSamples	= rhs.mHemisphereSamples;
	mSphereSamples		= rhs.mSphereSamples;
	
	for (int i = 0; i < MAX_THREADS; i++)
	{
		mCount[i] = rhs.mCount[i];
		mJump[i] = rhs.mJump[i];
	}
	
	return (*this);
}

tzISampler::~tzISampler(void) {}


//===================================================================================
void tzISampler::setNumSets(const int np) 
{
	mNumSets = np;
}


//===================================================================================
int tzISampler::getNumSamples(void) 
{
	return mNumSamples;
}


//===================================================================================
void tzISampler::shuffleXCoordinates(void)
{
	for (int p = 0; p < mNumSets; p++)
	{
		for (int i = 0; i <  mNumSamples - 1; i++) 
		{
			int target = rand_int() % mNumSamples + p * mNumSamples;
			float temp = mSamples[i + p * mNumSamples + 1].x;
			mSamples[i + p * mNumSamples + 1].x = mSamples[target].x;
			mSamples[target].x = temp;
		}
	}
}


//===================================================================================
void tzISampler::shuffleYCoordinates(void) 
{
	for (int p = 0; p < mNumSets; p++)
	{
		for (int i = 0; i <  mNumSamples - 1; i++) {
			int target = rand_int() % mNumSamples + p * mNumSamples;
			float temp = mSamples[i + p * mNumSamples + 1].y;
			mSamples[i + p * mNumSamples + 1].y = mSamples[target].y;
			mSamples[target].y = temp;
		}	
	}
}


//===================================================================================
void tzISampler::setupShuffledIndices(void) 
{
	mShuffledIndices.reserve(mNumSamples * mNumSets);
	vector<int> indices;
	
	for (int j = 0; j < mNumSamples; j++)
		indices.push_back(j);
	
	for (int p = 0; p < mNumSets; p++) { 
		random_shuffle(indices.begin(), indices.end());	
		
		for (int j = 0; j < mNumSamples; j++)
			mShuffledIndices.push_back(indices[j]);
	}	
}


//===================================================================================
void tzISampler::mapSamplesToUnitDisk(void) 
{
	int size = (int)mSamples.size();
	float r, phi;		// polar coordinates
	tzPoint2D sp; 		// sample point on unit disk
	
	mDiskSamples.reserve(size);
		
	for (int j = 0; j < size; j++) {
		 // map sample point to [-1, 1] X [-1,1]
		 	
		sp.x = 2.0f * mSamples[j].x - 1.0f;
		sp.y = 2.0f * mSamples[j].y - 1.0f;
			
		if (sp.x > -sp.y) {			// sectors 1 and 2
			if (sp.x > sp.y) {		// sector 1
				r = sp.x;
				phi = sp.y / sp.x;
			}
			else {					// sector 2
				r = sp.y;
				phi = 2 - sp.x / sp.y;
			}
		}
		else {						// sectors 3 and 4
			if (sp.x < sp.y) {		// sector 3
				r = -sp.x;
				phi = 4 + sp.y / sp.x;
			}
			else {					// sector 4
				r = -sp.y;
				if (sp.y != 0.0)	// avoid division by zero at origin
					phi = 6 - sp.x / sp.y;
				else
					phi  = 0.0;
			}
		}
		
		phi *= ((float)PI) / 4.0f;
				
		mDiskSamples[j].x = r * cos(phi);
		mDiskSamples[j].y = r * sin(phi);
	}
	
	mSamples.erase(mSamples.begin(), mSamples.end());
}


//===================================================================================
void tzISampler::mapSamplesToHemisphere(const float exp) 
{
	int size = (int)mSamples.size();
	mHemisphereSamples.reserve(mNumSamples * mNumSets);
		
	for (int j = 0; j < size; j++) {
		float cos_phi = cos(2.0f * (float)PI * mSamples[j].x);
		float sin_phi = sin(2.0f * (float)PI * mSamples[j].x);
		float cos_theta = pow((1.0f - mSamples[j].y), 1.0f / (exp + 1.0f));
		float sin_theta = sqrt (1.0f - cos_theta * cos_theta);
		float pu = sin_theta * cos_phi;
		float pv = sin_theta * sin_phi;
		float pw = cos_theta;
		mHemisphereSamples.push_back(tzPoint3D(pu, pv, pw));
	}
}


//===================================================================================
void tzISampler::mapSamplesToSphere(void) 
{
	float r1, r2;
	float x, y, z;
	float r, phi;
		
	mSphereSamples.reserve(mNumSamples * mNumSets);   
		
	for (int j = 0; j < mNumSamples * mNumSets; j++) {
		r1 	= mSamples[j].x;
    	r2 	= mSamples[j].y;
    	z 	= 1.0f - 2.0f * r1;
    	r 	= sqrt(1.0f - z * z);
    	phi = (float)TWO_PI * r2;
    	x 	= r * cos(phi);
    	y 	= r * sin(phi);
		mSphereSamples.push_back(tzPoint3D(x, y, z));
	}
}


//===================================================================================
tzPoint2D tzISampler::sampleUnitSquare(const tzRay &ray) //ooxx
{
	if (mCount[ray.mThreadId] % mNumSamples == 0)  									// start of a new pixel
		mJump[ray.mThreadId] = (rand_int() % mNumSets) * mNumSamples;				// random index jump initialised to zero in constructor

	return (mSamples[mJump[ray.mThreadId] + mShuffledIndices[mJump[ray.mThreadId] + mCount[ray.mThreadId]++ % mNumSamples]]);
}




/*

// ------------------------------------------------------------------- sampleUnitSquare
// the first revised version in Listing in Listing 5.8

Point2D
Sampler::sampleUnitSquare(void) {
	if (count % mNumSamples == 0)  									// start of a new pixel
		jump = (rand_int() % mNumSets) * mNumSamples;				// random index jump initialised to zero in constructor
	
	return (samples[jump + count++ % mNumSamples]);	
}

*/



/*

// ------------------------------------------------------------------- sampleUnitSquare
// the original version in Listing 5.7

Point2D
Sampler::sampleUnitSquare(void) {
	return (samples[count++ % (mNumSamples * mNumSets)]);
}

*/



//===================================================================================
tzPoint2D tzISampler::sampleUnitDisk(const tzRay &ray)
{
	if (mCount[ray.mThreadId] % mNumSamples == 0)  									// start of a new pixel
		mJump[ray.mThreadId] = (rand_int() % mNumSets) * mNumSamples;
	
	return (mDiskSamples[mJump[ray.mThreadId] + mShuffledIndices[mJump[ray.mThreadId] + mCount[ray.mThreadId]++ % mNumSamples]]);
}

//===================================================================================
tzPoint3D tzISampler::sampleHemisphere(const tzRay &ray)
{
	if (mCount[ray.mThreadId] % mNumSamples == 0)  									// start of a new pixel
		mJump[ray.mThreadId] = (rand_int() % mNumSets) * mNumSamples;
		
	return (mHemisphereSamples[mJump[ray.mThreadId] + mShuffledIndices[mJump[ray.mThreadId] + mCount[ray.mThreadId]++ % mNumSamples]]);
}


//===================================================================================
tzPoint3D tzISampler::sampleSphere(const tzRay &ray)
{
	if (mCount[ray.mThreadId] % mNumSamples == 0)  									// start of a new pixel
		mJump[ray.mThreadId] = (rand_int() % mNumSets) * mNumSamples;
		
	return (mSphereSamples[mJump[ray.mThreadId] + mShuffledIndices[mJump[ray.mThreadId] + mCount[ray.mThreadId]++ % mNumSamples]]);
}


//===================================================================================
tzPoint2D tzISampler::sampleOneSet(const tzRay &ray)
{
	return(mSamples[mCount[ray.mThreadId]++ % mNumSamples]);
}



