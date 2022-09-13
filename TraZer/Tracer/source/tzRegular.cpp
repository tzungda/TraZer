
#include "../include/tzRegular.h"

//===================================================================================
tzRegular::tzRegular(void)
	: tzISampler()
{}


//===================================================================================
tzRegular::tzRegular(const int num)
	: tzISampler(num)
{
	generateSamples();
}

//===================================================================================
tzRegular::tzRegular(const tzRegular& u)
	: tzISampler(u)
{
	generateSamples();
}

//===================================================================================
tzRegular& tzRegular::operator= (const tzRegular& rhs)	
{
	if (this == &rhs)
		return (*this);
		
	tzISampler::operator= (rhs);

	return (*this);
}

//===================================================================================
std::shared_ptr< tzISampler > tzRegular::clone(void) const
{
	return (std::make_shared< tzRegular >(*this));
}

//===================================================================================
tzRegular::~tzRegular(void) 
{
}

//===================================================================================
void tzRegular::generateSamples(void) 
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
