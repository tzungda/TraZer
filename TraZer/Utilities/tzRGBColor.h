#ifndef TZ_RGB_COLOR
#define TZ_RGB_COLOR

//------------------------------------------------------------ class RGBColor

class tzRGBColor {
	
	public:
	
		float	r, g, b;									
				
	public:
	
		tzRGBColor(void);										// default constructor
		tzRGBColor(float c);									// constructor
		tzRGBColor(float _r, float _g, float _b);				// constructor
		tzRGBColor(const tzRGBColor& c); 						// copy constructor
		
		~tzRGBColor(void);									// destructor
		
		tzRGBColor& 											// assignment operator
		operator= (const tzRGBColor& rhs);

		tzRGBColor 											// addition
		operator+ (const tzRGBColor& c) const;

		tzRGBColor& 											// compound addition
		operator+= (const tzRGBColor& c);
		
		tzRGBColor 											// multiplication by a float on the right
		operator* (const float a) const;
		
		tzRGBColor& 											// compound multiplication by a float on the right
		operator*= (const float a);					
				
		tzRGBColor 											// division by a float
		operator/ (const float a) const;
		
		tzRGBColor& 											// compound division by a float
		operator/= (const float a); 
				
		tzRGBColor 											// component-wise multiplication
		operator* (const tzRGBColor& c) const;
		
		bool												// are two RGBColours the same?
		operator== (const tzRGBColor& c) const;

		tzRGBColor											// raise components to a power
		powc(float p) const;
		
		float												// the average of the components
		average(void) const;
};



// inlined member functions

// ----------------------------------------------------------------------- operator+
// addition of two colors

inline tzRGBColor
tzRGBColor::operator+ (const tzRGBColor& c) const {
	return (tzRGBColor(r + c.r, g + c.g, b + c.b));
}


// ----------------------------------------------------------------------- operator+=
// compound addition of two colors

inline tzRGBColor&
tzRGBColor::operator+= (const tzRGBColor& c) {
	r += c.r; g += c.g; b += c.b;
    return (*this);
}


// ----------------------------------------------------------------------- operator*
// multiplication by a float on the right

inline tzRGBColor
tzRGBColor::operator* (const float a) const {
	return (tzRGBColor(r * a, g * a, b * a));
}


// ----------------------------------------------------------------------- operator*=
// compound multiplication by a float on the right

inline tzRGBColor&
tzRGBColor::operator*= (const float a) {
	r *= a; g *= a; b *= a;
	return (*this);
}


// ----------------------------------------------------------------------- operator/
// division by float

inline tzRGBColor
tzRGBColor::operator/ (const float a) const {
	return (tzRGBColor(r / a, g / a, b / a));
}


// ----------------------------------------------------------------------- operator/=
// compound division by float

inline tzRGBColor&
tzRGBColor::operator/= (const float a) {
	r /= a; g /= a; b /= a;
	return (*this);
}



// ----------------------------------------------------------------------- operator*
// component-wise multiplication of two colors

inline tzRGBColor
tzRGBColor::operator* (const tzRGBColor& c) const {
	return (tzRGBColor(r * c.r, g * c.g, b * c.b));
} 


// ----------------------------------------------------------------------- operator==
// are two RGBColors the same?

inline bool
tzRGBColor::operator== (const tzRGBColor& c) const {
	return (r == c.r && g == c.g && b == c.b);
}


// ----------------------------------------------------------------------- average
// the average of the three components

inline float											
tzRGBColor::average(void) const {
	return (0.333333333333f * (r + g + b));
}



// inlined non-member function

// ----------------------------------------------------------------------- operator*
// multiplication by a float on the left

tzRGBColor
operator* (const float a, const tzRGBColor& c);

inline tzRGBColor
operator* (const float a, const tzRGBColor& c) {
	return (tzRGBColor(a * c.r, a * c.g, a * c.b));
}


#endif

