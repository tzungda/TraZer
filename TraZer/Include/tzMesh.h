#ifndef TZ_MESH
#define TZ_MESH

/*
 This class is for the triangulated object
*/

#include "../Include/tzObject.h"

#include <vector>
#include "GLM/glm/glm.hpp"
using namespace std;

class tzMesh: public tzObject
{
public:
	tzMesh( );
	virtual ~tzMesh();
	
private: // member data
	vector< float >				mFloatPositions;
	vector< glm::vec3 >			mVertices;
	vector< glm::vec3 >			mVertexNormals;
	vector< unsigned int >		mIndices;
	unsigned int				mNumFaces;


public: // 

	// set data interfaces
	void							setPositions( const vector< float > &positions);
	void							setNumFaces(unsigned int numFaces);
	void							setIndices( const vector< unsigned int > &indices );
	void							setVertexNormals(const vector< glm::vec3 > &vertexNormals);
	
	// get data interfaces
	const vector< float >&			floatPositions( ) const;
	const vector< glm::vec3 >&		vertices( ) const;
	const unsigned int				numFaces( ) const;
	const vector< unsigned int >&	indices( ) const;
};

#endif
