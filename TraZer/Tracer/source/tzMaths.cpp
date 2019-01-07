
#include <math.h>
#include "../include/tzMaths.h"

 
#ifndef M_PI
#define M_PI PI
#endif

// You may have to experiment with EQN_EPS
// The original was 1e-9, but I use 1e-90  KS Dec 3, 2007

//#define     EQN_EPS     1e-9  
//#define     EQN_EPS     1e-30
//#define     EQN_EPS     1e-60
#define     EQN_EPS     1e-90

#define	IsZero(x)	((x) > -EQN_EPS && (x) < EQN_EPS)

#ifndef CBRT
#define     cbrt(x)  ((x) > 0.0 ? pow((float)(x), 1.0/3.0) : \
			  		 ((x) < 0.0 ? -pow((float)-(x), 1.0/3.0) : 0.0))
#endif


int 
SolveQuadric(float c[3], float s[2]) {
	float p, q, D;

    /* normal form: x^2 + px + q = 0 */

    p = c[ 1 ] / (2 * c[ 2 ]);
    q = c[ 0 ] / c[ 2 ];

    D = p * p - q;

    if (IsZero(D)) {
	s[ 0 ] = - p;
	return 1;
    }
    else if (D > 0) {
		float sqrt_D = sqrtf(D);

	s[ 0 ] =   sqrt_D - p;
	s[ 1 ] = - sqrt_D - p;
	return 2;
    }
    else /* if (D < 0) */
        return 0;
}


int 
SolveCubic(float c[4], float s[3]) {
    int     i, num;
	float  sub;
	float  A, B, C;
	float  sq_A, p, q;
	float  cb_p, D;

    /* normal form: x^3 + Ax^2 + Bx + C = 0 */

    A = c[ 2 ] / c[ 3 ];
    B = c[ 1 ] / c[ 3 ];
    C = c[ 0 ] / c[ 3 ];

    /*  substitute x = y - A/3 to eliminate quadric term:
	x^3 +px + q = 0 */

    sq_A = A * A;
    p = 1.0f/3.0f * (- 1.0f/3.0f * sq_A + B);
    q = 1.0f/2.0f * (2.0f/27.0f * A * sq_A - 1.0f/3.0f * A * B + C);

    /* use Cardano's formula */

    cb_p = p * p * p;
    D = q * q + cb_p;

    if (IsZero(D)) {
		if (IsZero(q)) { /* one triple solution */
		    s[ 0 ] = 0;
		    num = 1;
		}
	else { /* one single and one double solution */
		float u = cbrtf(-q);
	    s[ 0 ] = 2 * u;
	    s[ 1 ] = - u;
	    num = 2;
	}
    }
    else if (D < 0) { /* Casus irreducibilis: three real solutions */
		float phi = 1.0f/3.0f * acosf(-q / sqrtf(-cb_p));
		float t = 2.0f * sqrtf(-p);

		s[ 0 ] =   t * cosf(phi);
		s[ 1 ] = - t * cosf(phi + M_PI / 3.0f);
		s[ 2 ] = - t * cosf(phi - M_PI / 3.0f);
		num = 3;
    }
    else { /* one real solution */
		float sqrt_D = sqrtf(D);
		float u = cbrtf(sqrt_D - q);
		float v = - cbrtf(sqrt_D + q);

		s[ 0 ] = u + v;
		num = 1;
    }

    /* resubstitute */

    sub = 1.0f/3.0f * A;

    for (i = 0; i < num; ++i)
	s[ i ] -= sub;

    return num;
}



int 
SolveQuartic(float c[5], float s[4]) {
	float  coeffs[4];
	float  z, u, v, sub;
	float  A, B, C, D;
	float  sq_A, p, q, r;
    int     i, num;

    /* normal form: x^4 + Ax^3 + Bx^2 + Cx + D = 0 */

    A = c[ 3 ] / c[ 4 ];
    B = c[ 2 ] / c[ 4 ];
    C = c[ 1 ] / c[ 4 ];
    D = c[ 0 ] / c[ 4 ];

    /*  substitute x = y - A/4 to eliminate cubic term:
	x^4 + px^2 + qx + r = 0 */

    sq_A = A * A;
    p = - 3.0f/8.0f * sq_A + B;
    q = 1.0f/8.0f * sq_A * A - 1.0f/2.0f * A * B + C;
    r = - 3.0f/256.0f*sq_A*sq_A + 1.0f/16.0f*sq_A*B - 1.0f/4.0f*A*C + D;

    if (IsZero(r)) {
		/* no absolute term: y(y^3 + py + q) = 0 */

		coeffs[ 0 ] = q;
		coeffs[ 1 ] = p;
		coeffs[ 2 ] = 0;
		coeffs[ 3 ] = 1;

		num = SolveCubic(coeffs, s);

		s[ num++ ] = 0;
    }
    else {
		/* solve the resolvent cubic ... */

		coeffs[ 0 ] = 1.0f/2.0f * r * p - 1.0f/8.0f * q * q;
		coeffs[ 1 ] = - r;
		coeffs[ 2 ] = - 1.0f/2.0f * p;
		coeffs[ 3 ] = 1.0f;
		
		(void) SolveCubic(coeffs, s);

		/* ... and take the one real solution ... */

		z = s[ 0 ];

		/* ... to build two quadric equations */

		u = z * z - r;
		v = 2 * z - p;

		if (IsZero(u))
		    u = 0;
		else if (u > 0)
		    u = sqrtf(u);
		else
		    return 0;

		if (IsZero(v))
		    v = 0;
		else if (v > 0)
		    v = sqrtf(v);
		else
		    return 0;

		coeffs[ 0 ] = z - u;
		coeffs[ 1 ] = q < 0 ? -v : v;
		coeffs[ 2 ] = 1;

		num = SolveQuadric(coeffs, s);

		coeffs[ 0 ]= z + u;
		coeffs[ 1 ] = q < 0 ? v : -v;
		coeffs[ 2 ] = 1;

		num += SolveQuadric(coeffs, s + num);
	}

    /* resubstitute */

    sub = 1.0f/4.0f * A;

    for (i = 0; i < num; ++i)
		s[ i ] -= sub;

    return num;
}


