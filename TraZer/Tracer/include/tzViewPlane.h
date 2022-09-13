#ifndef TZ_TRACER_VIEWPLANE
#define TZ_TRACER_VIEWPLANE

#include <memory>

class tzISampler;

class tzViewPlane
{
public:
	tzViewPlane();
	virtual ~tzViewPlane();

//private:
	int				mHeight;		//horizontal image resolution
	int				mWidth;		//vertical image resolution
	float			mS;			//pixel size
	float			mGamma;		// monitor gamma factor
	float			mInvGamma;	// one over gamma
	std::shared_ptr<tzISampler>		mSamplerPtr;
	int				mNumSamples;
	float			mMaxDepth;

public:
	void			setHeight( int hres );
	void			setWidth( int vres );
	void			setPixelSize( float s );
	void			setGamma( float gamma );
	void			setSampler(std::shared_ptr<tzISampler> sp);
	void			setSamples( const int n );
	void			setMaxDepth( float maxDepth );
};

#endif