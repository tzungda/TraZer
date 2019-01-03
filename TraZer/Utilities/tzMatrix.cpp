
#include "tzMatrix.h"
#include "tzVector3D.h"
#include "tzPoint3D.h"

//===================================================================================
tzMatrix::tzMatrix(void) 
{
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++) 
		{
			if (x == y)
				m[x][y] = 1.0;
			else
				m[x][y] = 0.0;
		}
	}
}


//===================================================================================
tzMatrix::tzMatrix(const tzMatrix& mat) 
{
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			m[x][y] = mat.m[x][y];
		}
	}
}


//===================================================================================
tzMatrix::~tzMatrix(void) 
{
}


//===================================================================================
tzMatrix& tzMatrix::operator= (const tzMatrix& rhs) 
{
	if (this == &rhs)
		return (*this);

	for (int x = 0; x < 4; x++) 
	{
		for (int y = 0; y < 4; y++)	
		{
			m[x][y] = rhs.m[x][y];	
		}
	}

	return (*this);
}


//===================================================================================
tzMatrix tzMatrix::operator* (const tzMatrix& mat) const 
{
	tzMatrix 	product;	
	
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++) 
		{
			double sum = 0.0;

			for (int j = 0; j < 4; j++)
				sum += m[x][j] * mat.m[j][y];
 
			product.m[x][y] = sum;			
		}
	}
	
	return (product);
}

//===================================================================================
tzVector3D tzMatrix::operator* (const tzVector3D& v) const
{
	return tzVector3D(	m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z,
						m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z,
						m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z );
}

//===================================================================================
tzPoint3D tzMatrix::operator* (const tzPoint3D& p) const
{
	return (tzPoint3D(	m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3],
						m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3],
						m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3]));
}

//===================================================================================
tzMatrix tzMatrix::operator/ (const double d) 
{
	for (int x = 0; x < 4; x++)				
		for (int y = 0; y < 4; y++)			
			m[x][y] = m[x][y] / d;	

	return (*this);
}


//===================================================================================
void tzMatrix::setIdentity(void) 
{
    for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			if (x == y)
				m[x][y] = 1.0;
			else
				m[x][y] = 0.0;
		}
}

//===================================================================================
void tzMatrix::initializeWithAValue(double v)
{
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			m[x][y] = v;
		}
}






