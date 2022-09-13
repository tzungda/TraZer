#ifndef TZ_COLOR
#define TZ_COLOR


class tzColor {
	
	public:
	
		float	r, g, b, a;									
				
	public:
	
		tzColor(void);										// default constructor
		tzColor(float c);									// constructor
		tzColor(float _r, float _g, float _b);				// constructor
		tzColor(float _r, float _g, float _b, float _a);		// constructor
		tzColor(const tzColor& c); 						// copy constructor
		
		~tzColor(void);									// destructor
		
		tzColor& operator= (const tzColor& rhs);

		tzColor operator+ (const tzColor& c) const;

		tzColor& operator+= (const tzColor& c);
		
		tzColor operator* (const float a) const;
		
		tzColor& operator*= (const float a);					
				
		tzColor operator/ (const float a) const;
		
		tzColor& operator/= (const float a); 
				
		tzColor operator* (const tzColor& c) const;
		
		bool operator== (const tzColor& c) const;

		tzColor powc(float p) const;
		
		float average(void) const;
};



// inlined member functions
//
//===================================================================================
inline tzColor tzColor::operator+ (const tzColor& c) const 
{
	return (tzColor(r + c.r, g + c.g, b + c.b));
}

//===================================================================================
inline tzColor& tzColor::operator+= (const tzColor& c) 
{
	r += c.r; g += c.g; b += c.b;
    return (*this);
}

//===================================================================================
inline tzColor tzColor::operator* (const float a) const 
{
	return (tzColor(r * a, g * a, b * a));
}

//===================================================================================
inline tzColor& tzColor::operator*= (const float a) 
{
	r *= a; g *= a; b *= a;
	return (*this);
}

//===================================================================================
inline tzColor tzColor::operator/ (const float a) const 
{
	return (tzColor(r / a, g / a, b / a));
}

//===================================================================================
inline tzColor& tzColor::operator/= (const float a) 
{
	r /= a; g /= a; b /= a;
	return (*this);
}

//===================================================================================
inline tzColor tzColor::operator* (const tzColor& c) const 
{
	return (tzColor(r * c.r, g * c.g, b * c.b));
} 

//===================================================================================
inline bool tzColor::operator== (const tzColor& c) const 
{
	return (r == c.r && g == c.g && b == c.b);
}

//===================================================================================
inline float tzColor::average(void) const 
{
	return (0.333333333333f * (r + g + b));
}

//===================================================================================
tzColor operator* (const float a, const tzColor& c);

//===================================================================================
inline tzColor operator* (const float a, const tzColor& c) 
{
	return (tzColor(a * c.r, a * c.g, a * c.b));
}


#endif

