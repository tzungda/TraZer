#ifndef TZ_COLOR
#define TZ_COLOR

//------------------------------------------------------------ class RGBColor

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
		
		tzColor& 											// assignment operator
		operator= (const tzColor& rhs);

		tzColor 											// addition
		operator+ (const tzColor& c) const;

		tzColor& 											// compound addition
		operator+= (const tzColor& c);
		
		tzColor 											// multiplication by a float on the right
		operator* (const float a) const;
		
		tzColor& 											// compound multiplication by a float on the right
		operator*= (const float a);					
				
		tzColor 											// division by a float
		operator/ (const float a) const;
		
		tzColor& 											// compound division by a float
		operator/= (const float a); 
				
		tzColor 											// component-wise multiplication
		operator* (const tzColor& c) const;
		
		bool												// are two RGBColours the same?
		operator== (const tzColor& c) const;

		tzColor											// raise components to a power
		powc(float p) const;
		
		float												// the average of the components
		average(void) const;
};



// inlined member functions

// ----------------------------------------------------------------------- operator+
// addition of two colors

inline tzColor
tzColor::operator+ (const tzColor& c) const {
	return (tzColor(r + c.r, g + c.g, b + c.b));
}


// ----------------------------------------------------------------------- operator+=
// compound addition of two colors

inline tzColor&
tzColor::operator+= (const tzColor& c) {
	r += c.r; g += c.g; b += c.b;
    return (*this);
}


// ----------------------------------------------------------------------- operator*
// multiplication by a float on the right

inline tzColor
tzColor::operator* (const float a) const {
	return (tzColor(r * a, g * a, b * a));
}


// ----------------------------------------------------------------------- operator*=
// compound multiplication by a float on the right

inline tzColor&
tzColor::operator*= (const float a) {
	r *= a; g *= a; b *= a;
	return (*this);
}


// ----------------------------------------------------------------------- operator/
// division by float

inline tzColor
tzColor::operator/ (const float a) const {
	return (tzColor(r / a, g / a, b / a));
}


// ----------------------------------------------------------------------- operator/=
// compound division by float

inline tzColor&
tzColor::operator/= (const float a) {
	r /= a; g /= a; b /= a;
	return (*this);
}



// ----------------------------------------------------------------------- operator*
// component-wise multiplication of two colors

inline tzColor
tzColor::operator* (const tzColor& c) const {
	return (tzColor(r * c.r, g * c.g, b * c.b));
} 


// ----------------------------------------------------------------------- operator==
// are two RGBColors the same?

inline bool
tzColor::operator== (const tzColor& c) const {
	return (r == c.r && g == c.g && b == c.b);
}


// ----------------------------------------------------------------------- average
// the average of the three components

inline float											
tzColor::average(void) const {
	return (0.333333333333f * (r + g + b));
}



// inlined non-member function

// ----------------------------------------------------------------------- operator*
// multiplication by a float on the left

tzColor
operator* (const float a, const tzColor& c);

inline tzColor
operator* (const float a, const tzColor& c) {
	return (tzColor(a * c.r, a * c.g, a * c.b));
}


#endif

