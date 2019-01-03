#ifndef TZ_MATRIX
#define TZ_MATRIX

class tzVector3D;
class tzPoint3D;

class tzMatrix {
	
	public:
	
		double	m[4][4];								
	
		
		tzMatrix(void);									

		tzMatrix(const tzMatrix& mat);			
		
		~tzMatrix(void);
			
		tzMatrix& operator= (const tzMatrix& rhs);
			
		tzMatrix operator* (const tzMatrix& mat) const;

		tzVector3D operator* (const tzVector3D& v) const; // m*vector

		tzPoint3D operator* (const tzPoint3D& p) const; // m*vector

		tzMatrix operator/ (const double d);

		void	setIdentity(void);	

		void	initializeWithAValue( double v );
};

#endif



