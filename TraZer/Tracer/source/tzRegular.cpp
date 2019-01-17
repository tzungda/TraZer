
#include "../include/tzRegular.h"

// ---------------------------------------------------------------- default constructor
	
tzRegular::tzRegular(void)
	: tzISampler()
{}


// ---------------------------------------------------------------- constructor

tzRegular::tzRegular(const int num)
	: tzISampler(num)
{
	generateSamples();
}


// ---------------------------------------------------------------- copy constructor

tzRegular::tzRegular(const tzRegular& u)
	: tzISampler(u)
{
	generateSamples();
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


// ---------------------------------------------------------------- generateSamples	

void
tzRegular::generateSamples(void) 
{
	int n = (int) sqrt((float)mNumSamples);

	for (int j = 0; j < mNumSets; j++)
	{
		for (int p = 0; p < n; p++)		
		{
			for (int q = 0; q < n; q++)
			{
				mSamples.push_back(tzPoint2D((q + 0.5f) / n, (p + 0.5f) / n));
			}
		}
	}
}
