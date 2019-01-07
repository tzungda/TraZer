
#include "../include/tzWorld.h"
#include "../include/tzAmbientOccluder.h"

//===================================================================================
tzAmbientOccluder::tzAmbientOccluder(void)
	: 	tzILight(),
		ls(1.0),
		color(1.0),
		min_amount( 0.0f ),
		mSamplerPtr( NULL )
{}


//===================================================================================
tzAmbientOccluder::tzAmbientOccluder(const tzAmbientOccluder& a)
	: 	tzILight(a),
		ls(a.ls),
		color(a.color) ,
		min_amount(0.0f),
		mSamplerPtr(NULL)
{}


//===================================================================================
tzILight* tzAmbientOccluder::clone(void) const 
{
	return (new tzAmbientOccluder(*this));
}	


//===================================================================================
tzAmbientOccluder& tzAmbientOccluder::operator= (const tzAmbientOccluder& rhs)
{
	if (this == &rhs)
		return (*this);
			
	tzILight::operator= (rhs);
	
	ls 		= rhs.ls;
	color 	= rhs.color;
	
	return (*this);
}


//===================================================================================
tzAmbientOccluder::~tzAmbientOccluder(void) 
{
	if ( mSamplerPtr )
	{
		delete mSamplerPtr;
		mSamplerPtr = NULL;
	}
}


//===================================================================================
tzVector3D tzAmbientOccluder::getDirection(  tzShadeRec& s)
{
	tzPoint3D sp = mSamplerPtr->sample_hemisphere();
	return ( sp.x*u + sp.y*v + sp.z*w );
}


//===================================================================================
tzRGBColor tzAmbientOccluder::L( tzShadeRec& sr) 
{
	w = sr.mNormal;
	// jitter up vector in case normal is vertical
	v = w^tzVector3D(0.0072f, 1.0f, 0.0034f);
	v.normalize();
	u = v^w;

	tzRay shadowRay;
	shadowRay.o = sr.mHitPoint;
	shadowRay.d = getDirection( sr );

	if (inShadow( shadowRay, sr ) )
	{
		return (min_amount * ls * color);
	}
	
	return (ls*color);
}

//===================================================================================
bool tzAmbientOccluder::inShadow(const tzRay &ray, const tzShadeRec &sr) const
{
	float t;
	int numObjects = (int)sr.mWorld.mObjects.size();

	for ( int j = 0; j < numObjects; j++ )
	{
		if ( sr.mWorld.mObjects[j]->shadowHit( ray, t ) )
		{
			return true;
		}
	}

	return false;
}

// ================================================================================
void tzAmbientOccluder::setSampler( tzISampler *samplerPtr )
{
	if ( mSamplerPtr )
	{
		delete mSamplerPtr;
		mSamplerPtr = NULL;
	}

	mSamplerPtr = samplerPtr;
	mSamplerPtr->map_samples_to_hemisphere( 1 );
}

// ================================================================================
void tzAmbientOccluder::setMinAmount(float minAmount)
{
	min_amount = minAmount;
}


