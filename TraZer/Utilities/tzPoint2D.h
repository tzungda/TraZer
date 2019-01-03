#ifndef TZ_POINT_2D
#define TZ_POINT_2D


class tzPoint2D {	
	public:
	
		float x, y;
				
	public:
	
		tzPoint2D(void);
		tzPoint2D(const float arg);
		tzPoint2D(const float x1, const float y1);
		tzPoint2D(const tzPoint2D& p);
		~tzPoint2D(void);

		tzPoint2D&
		operator= (const tzPoint2D& rhs);
		
		tzPoint2D
		operator* (const float a);
};


//===================================================================================
inline tzPoint2D
tzPoint2D::operator* (const float a) {
	return (tzPoint2D(x * a, y * a));
}

#endif


