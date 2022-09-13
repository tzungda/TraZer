#ifndef TZ_CONSTANTS
#define TZ_CONSTANTS


#include <math.h>
#include <stdlib.h>			// for RAND_MAX
#include "tzColor.h"


const float 	PI 			= 3.1415926535897932384f;
const float 	TWO_PI 		= 6.2831853071795864769f;
const float 	PI_ON_180 	= 0.0174532925199432957f;
const float 	invPI 		= 0.3183098861837906715f;
const float 	invTWO_PI 	= 0.1591549430918953358f;

const float		degreeToRadian = PI / 180.0f;

const float 	kEpsilon 	= 0.0001f;
const float	kHugeValue	= 1.0E10f;

const tzColor	black(0.0f);
const tzColor	white(1.0f);
const tzColor	red(1.0f, 0.0f, 0.0f);

const float 	invRAND_MAX = 1.0f / (float)RAND_MAX;

const unsigned int MAX_THREADS = 64;

#endif
