
#include "../include/tzRegular.h"

// ---------------------------------------------------------------- default constructor
	
tzRegular::tzRegular(void)
	: tzISampler()
{}


// ---------------------------------------------------------------- constructor

tzRegular::tzRegular(const int num)
	: tzISampler(num)
{
	generate_samples();
}


// ---------------------------------------------------------------- copy constructor

tzRegular::tzRegular(const tzRegular& u)
	: tzISampler(u)
{
	generate_samples();
}

// ---------------------------------------------------------------- assignment operator

tzRegular&
tzRegular::operator= (const tzRegular& rhs)	{
	if (this == &rhs)
		return (*this);
		
	tzISampler::operator= (rhs);

	return (*this);
}

// ---------------------------------------------------------------- clone

tzRegular*
tzRegular::clone(void) const {
	return (new tzRegular(*this));
}

// ---------------------------------------------------------------- destructor			

tzRegular::~tzRegular(void) {}


// ---------------------------------------------------------------- generate_samples	

void
tzRegular::generate_samples(void) {
	int n = (int) sqrt((float)num_samples);

	for (int j = 0; j < num_sets; j++)
		for (int p = 0; p < n; p++)		
			for (int q = 0; q < n; q++)
				samples.push_back(tzPoint2D((q + 0.5f) / n, (p + 0.5f) / n));
}
