#ifndef TZ_MULTI_JITTERED
#define TZ_MULTI_JITTERED


#include "../include/tzISampler.h"

class tzMultiJittered: public tzISampler {
	public:
		
		tzMultiJittered(void);
		
		tzMultiJittered(const int numSamples);
		
		tzMultiJittered(const int numSamples, const int m);

		tzMultiJittered(const tzMultiJittered& mjs);

		tzMultiJittered& operator= (const tzMultiJittered& rhs);

		virtual tzMultiJittered* clone(void) const;			

		virtual ~tzMultiJittered(void);
		
	private:
		
		virtual void generateSamples(void);		
};

#endif

