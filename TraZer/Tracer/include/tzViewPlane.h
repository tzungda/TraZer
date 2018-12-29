#ifndef TZ_TRACER_VIEWPLANE
#define TZ_TRACER_VIEWPLANE

class tzISampler;

class tzViewPlane
{
public:
	tzViewPlane();
	virtual ~tzViewPlane();

//private:
	int				mHres;		//horizontal image resolution
	int				mVres;		//vertical image resolution
	float			mS;			//pixel size
	float			mGamma;		// monitor gamma factor
	float			mInvGamma;	// one over gamma
	tzISampler		*mSamplerPtr;
	int				mNumSamples;
	float			mMaxDepth;

public:
	void			setHres( int hres );
	void			setVres( int vres );
	void			setPixelSize( float s );
	void			setGamma( float gamma );
	void			setSampler(tzISampler* sp);
	void			setSamples( const int n );
	void			setMaxDepth( float maxDepth );
};

#endif