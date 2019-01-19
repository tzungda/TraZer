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
		setKr(const float k);
				
		void													
		setCr(const tzColor& c);
		
		void
		setCr(const float r, const float g, const float b);
		
		void
		setCr(const float c);
		
		virtual tzColor										
		shade(tzShadeRec& s);

		virtual tzColor pathShade(tzShadeRec& sr);
		
	private:
	
		tzPerfectSpecular* reflective_brdf;
};


// ---------------------------------------------------------------- setKr

inline void
tzReflective::setKr(const float k) {
	reflective_brdf->setKr(k);
}


// ---------------------------------------------------------------- setCr

inline void
tzReflective::setCr(const tzColor& c) {
	reflective_brdf->setCr(c);
	
}


// ---------------------------------------------------------------- setCr

inline void
tzReflective::setCr(const float r, const float g, const float b) {
	reflective_brdf->setCr(r, g, b);
}


// ---------------------------------------------------------------- setCr

inline void
tzReflective::setCr(const float c) {
	reflective_brdf->setCr(c);
}

#endif
