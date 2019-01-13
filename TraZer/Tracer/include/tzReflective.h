#ifndef TZ_TRACER_REFLECTIVE
#define TZ_TRACER_REFLECTIVE


#include "../include/tzPhong.h"
#include "../include/tzPerfectSpecular.h"

class tzReflective: public tzPhong {	
	public:
	
		tzReflective(void);

		tzReflective(const tzReflective& rm);

		tzReflective&
		operator= (const tzReflective& rhs);

		virtual tzReflective*
		clone(void) const;				

		~tzReflective(void);
		
		void
		set_kr(const float k);
				
		void													
		set_cr(const tzRGBColor& c);
		
		void
		set_cr(const float r, const float g, const float b);
		
		void
		set_cr(const float c);
		
		virtual tzRGBColor										
		shade(tzShadeRec& s);

		virtual tzRGBColor pathShade(tzShadeRec& sr);
		
	private:
	
		tzPerfectSpecular* reflective_brdf;
};


// ---------------------------------------------------------------- set_kr

inline void
tzReflective::set_kr(const float k) {
	reflective_brdf->set_kr(k);
}


// ---------------------------------------------------------------- set_cr

inline void
tzReflective::set_cr(const tzRGBColor& c) {
	reflective_brdf->set_cr(c);
	
}


// ---------------------------------------------------------------- set_cr

inline void
tzReflective::set_cr(const float r, const float g, const float b) {
	reflective_brdf->set_cr(r, g, b);
}


// ---------------------------------------------------------------- set_cr

inline void
tzReflective::set_cr(const float c) {
	reflective_brdf->set_cr(c);
}

#endif
