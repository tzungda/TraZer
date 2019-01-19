
#include "../include/tzRectangle.h"

const float tzRectangle::mEpsilon = 0.001f;

//===================================================================================
tzRectangle::tzRectangle(void)
	: 	tzIGeometricObject(),
		mP0(-1, 0, -1), 
		mSideA(0, 0, 2), mSideB(2, 0, 0), 
		mLenSquaredA(4.0), 
		mLenSquaredB(4.0),
		mNormal(0, 1, 0),
		mArea(4.0),
		mInvArea(0.25),
		mSamplerPtr(NULL)
{}


//===================================================================================
tzRectangle::tzRectangle(const tzPoint3D& _p0, const tzVector3D& _a, const tzVector3D& _b)
	:	tzIGeometricObject(),
		mP0(_p0),
		mSideA(_a),
		mSideB(_b),
		mLenSquaredA((float)(mSideA.len_squared())),
		mLenSquaredB((float)(mSideB.len_squared())),
		mArea((float)(mSideA.length() * mSideB.length())),
		mInvArea(1.0f / (float)mArea),
		mSamplerPtr(NULL)		
{
	mNormal = mSideA ^ mSideB;
	mNormal.normalize();
}

//===================================================================================
tzRectangle::tzRectangle(const tzPoint3D& _p0, const tzVector3D& _a, const tzVector3D& _b, const tzNormal& n)
	:	tzIGeometricObject(),
		mP0(_p0),
		mSideA(_a),
		mSideB(_b),
		mLenSquaredA(mSideA.len_squared()),
		mLenSquaredB(mSideB.len_squared()),
		mArea((float)(mSideA.length() * mSideB.length())),
		mInvArea(1.0f / (float)mArea),
		mNormal(n),
		mSamplerPtr(NULL)
{
	mNormal.normalize();
}


//===================================================================================
tzRectangle* tzRectangle::clone(void) const
{
	return (new tzRectangle(*this));
}

//===================================================================================
tzRectangle::tzRectangle(const tzRectangle& r)
	:	tzIGeometricObject(r),
		mP0(r.mP0), 
		mSideA(r.mSideA),
		mSideB(r.mSideB),
		mLenSquaredA(r.mLenSquaredA), 
		mLenSquaredB(r.mLenSquaredB),	
		mNormal(r.mNormal),
		mArea(r.mArea),
		mInvArea(r.mInvArea)
{
	if(r.mSamplerPtr)
		mSamplerPtr	= r.mSamplerPtr->clone(); 
	else  mSamplerPtr = NULL;
}


//===================================================================================
tzRectangle& tzRectangle::operator= (const tzRectangle& rhs)
{
	if (this == &rhs)
		return (*this);

	tzIGeometricObject::operator=(rhs);
	
	mP0				= rhs.mP0;
	mSideA = rhs.mSideA;
	mSideB = rhs.mSideB;
	mLenSquaredA	= rhs.mLenSquaredA; 
	mLenSquaredB	= rhs.mLenSquaredB;
	mArea			= rhs.mArea;	
	mInvArea		= rhs.mInvArea;
	mNormal			= rhs.mNormal;
	
	if (mSamplerPtr) {
		delete mSamplerPtr;
		mSamplerPtr = NULL;
	}

	if (rhs.mSamplerPtr)
		mSamplerPtr= rhs.mSamplerPtr->clone();

	return (*this);
}

//===================================================================================
tzRectangle::~tzRectangle(void)
{

	if (mSamplerPtr) {
		delete mSamplerPtr;
		mSamplerPtr = NULL;
	}
}

//===================================================================================
tzBBox tzRectangle::getBoundingBox(void) 
{
	float delta = 0.0001f;

	return(tzBBox(fmin(mP0.x, mP0.x + mSideA.x + mSideB.x) - delta, fmax(mP0.x, mP0.x + mSideA.x + mSideB.x) + delta,
				fmin(mP0.y, mP0.y + mSideA.y + mSideB.y) - delta, fmax(mP0.y, mP0.y + mSideA.y + mSideB.y) + delta,
				fmin(mP0.z, mP0.z + mSideA.z + mSideB.z) - delta, fmax(mP0.z, mP0.z + mSideA.z + mSideB.z) + delta));
}
																			

//===================================================================================
bool tzRectangle::hit(const tzRay& ray, float& tmin, tzShadeRec& sr) const 
{
	
	float t = (mP0 - ray.mOrigin) * mNormal / (ray.mDirection * mNormal);
	
	if (t <= kEpsilon)
		return (false);
			
	tzPoint3D p = ray.mOrigin + t * ray.mDirection;
	tzVector3D d = p - mSideB;
	
	float ddota = d * mSideA;
	
	if (ddota < 0.0 || ddota > mLenSquaredA)
		return (false);
		
	float ddotb = d * mSideB;
	
	if (ddotb < 0.0 || ddotb > mLenSquaredB)
		return (false);
		
	tmin 				= t;
	sr.mNormal 			= mNormal;
	sr.mLocalHitPoint 	= p;
	
	return (true);
}


//===================================================================================
void tzRectangle::setSampler(tzISampler* sampler) 
{
	mSamplerPtr = sampler;
}


//===================================================================================
tzPoint3D tzRectangle::sample(const tzShadeRec& sr) 
{
	tzPoint2D samplePoint = mSamplerPtr->sampleUnitSquare( sr.mRay );
	return (mP0 + samplePoint.x * mSideA + samplePoint.y * mSideB);
}


//===================================================================================					 
tzNormal tzRectangle::getNormal(const tzPoint3D& p) 
{
	return (mNormal);
}


//===================================================================================
float tzRectangle::pdf(tzShadeRec& sr) 
{
	return (mInvArea);
} 





