#ifndef TZ_REGULAR_SAMPLER
#define TZ_REGULAR_SAMPLER

#include "../include/tzISampler.h"

class tzRegular: public tzISampler {
	public:
		
		tzRegular(void);
		
		tzRegular(const int num);

		tzRegular(const tzRegular& u);

		tzRegular& operator= (const tzRegular& rhs);

		virtual tzRegular* clone(void) const;			

		virtual ~tzRegular(void);
		
		virtual void generateSamples(void);
};

#endif

