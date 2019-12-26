#ifndef TZ_MATRIX
#define TZ_MATRIX

class tzVector3D;
class tzPoint3D;

class tzMatrix {
	
	public:
	
		float	m[4][4];								
	
		
		tzMatrix(void);									

		tzMatrix(const tzMatrix& mat);			

		tzMatrix(	float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
					float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);
		
		~tzMatrix(void);
			
		tzMatrix& operator= (const tzMatrix& rhs);
			
		tzMatrix operator* (const tzMatrix& mat) const;

		tzVector3D operator* (const tzVector3D& v) const; // m*vector

		tzPoint3D operator* (const tzPoint3D& p) const; // m*vector

		tzMatrix operator/ (const float d);

		bool	invert( tzMatrix &invertMatrix ) const;

		void	setIdentity(void);	

		void	initializeWithAValue( float v );

		tzMatrix rotationMatrix( ) const;
};

#endif



