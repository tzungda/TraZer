#ifndef TZ_MATRIX
#define TZ_MATRIX

//----------------------------------------- class Matrix

class tzMatrix {
	
	public:
	
		double	m[4][4];								// elements
	
		
		tzMatrix(void);									// default constructor

		tzMatrix(const tzMatrix& mat);						// copy constructor
		
		~tzMatrix(void);									// destructor
			
		tzMatrix& 										// assignment operator
		operator= (const tzMatrix& rhs);
			
		tzMatrix 											// multiplication of two matrices
		operator* (const tzMatrix& mat) const;

		tzMatrix 											// divsion by a double
		operator/ (const double d);

		void											// set to the identity matrix
		set_identity(void);	
};

#endif



