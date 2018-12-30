
#include "tzMatrix.h"

// ----------------------------------------------------------------------- default constructor
// a default matrix is an identity matrix

tzMatrix::tzMatrix(void) {
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			if (x == y)
				m[x][y] = 1.0;
			else
				m[x][y] = 0.0;
		}
}


// ----------------------------------------------------------------------- copy constructor

tzMatrix::tzMatrix(const tzMatrix& mat) {
	for (int x = 0; x < 4; x++)				
		for (int y = 0; y < 4; y++)			
			m[x][y] = mat.m[x][y];	
}


// ----------------------------------------------------------------------- destructor

tzMatrix::~tzMatrix(void) {}


// ----------------------------------------------------------------------- assignment operator

tzMatrix&
tzMatrix::operator= (const tzMatrix& rhs) {
	if (this == &rhs)
		return (*this);

	for (int x = 0; x < 4; x++)				
		for (int y = 0; y < 4; y++)			
			m[x][y] = rhs.m[x][y];	

	return (*this);
}


// ----------------------------------------------------------------------- operator*
// multiplication of two matrices

tzMatrix
tzMatrix::operator* (const tzMatrix& mat) const {
	tzMatrix 	product;
	
	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++) {
			double sum = 0.0;

			for (int j = 0; j < 4; j++)
				sum += m[x][j] * mat.m[j][y];
 
			product.m[x][y] = sum;			
		}
	
	return (product);
}


// ----------------------------------------------------------------------- operator/
// division by a double

tzMatrix
tzMatrix::operator/ (const double d) {
	for (int x = 0; x < 4; x++)				
		for (int y = 0; y < 4; y++)			
			m[x][y] = m[x][y] / d;	

	return (*this);
}



// ----------------------------------------------------------------------- set_identity
// set matrix to the identity matrix

void											
tzMatrix::set_identity(void) {
    for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			if (x == y)
				m[x][y] = 1.0;
			else
				m[x][y] = 0.0;
		}
}







