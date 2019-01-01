#ifndef TZ_MESH
#define TZ_MESH

/*
 This class is for the triangulated object
*/

#include "../Include/tzCoreObject.h"

#include <vector>
#include "tzPoint3D.h"
#include "tzNormal.h"

using namespace std;

class tzCoreMesh : public tzCoreObject
{
public:
	tzCoreMesh( );
	virtual ~tzCoreMesh();
	
private: // member data
	vector<tzPoint3D> 			mVertices;				// mesh vertices 
	vector<tzNormal>			mVertexNormals;			// average normal at each vertex;
	vector<float>				mU;						// u texture coordinate at each vertex
	vector<float>				mV;						// v texture coordinate at each vertex
	vector<vector<int> > 		mVertexFaces;			// the triangles shared by each vertex
	int 						mNumVertices; 			// number of vertices
	int 						mNumTriangles; 			// number of triangles
	vector<int>					mIndices;				// indices of the vertices
	vector<float>				mPositions;				// 
	vector<vector<int>>			mFaceVertices;

public: // 

	// set data interfaces
	void							setVertices( const vector< tzPoint3D > &vertices);
	void							setVertices(const vector< float > &positions);
	void							setVertexNormals(const vector< tzNormal > &normals);
	void							setUs(const vector< float > &us);
	void							setVs(const vector< float > &vs);
	void							setVertexFaces( const vector<vector<int> > vertexFaces);
	void							setNumVertices( int numVertices );
	void							setNumTriangles(int numVertices);
	void							setIndices( const vector< int > &indices );
	void							setFaceVertices( const vector<vector<int>> &faceVertices );
	
	
	// get data interfaces
	const vector< float >&			floatPositions( ) const;
	const vector< tzPoint3D >&		vertices( ) const;
	const vector< tzNormal >&		vertexNormals( ) const;
	const vector< float >&			us( ) const;
	const vector< float >&			vs( ) const;
	const vector<vector<int> >		vertexFaces( ) const;
	int								numVertices( ) const;
	int								numTriangles( ) const;
	const vector< int >&			indices( ) const;
	const vector<vector<int>>&		faceVertices() const;
};

#endif
