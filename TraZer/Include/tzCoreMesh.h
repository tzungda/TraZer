#ifndef TZ_MESH
#define TZ_MESH

/*
 This class is for the triangulated object
*/

#include "../Include/tzCoreObject.h"
#include "../Include/tzCoreTransform.h"
#include "../Include/tzCoreMaterial.h"

#include <vector>
#include "tzPoint3D.h"
#include "tzNormal.h"

typedef struct {
	std::vector<float> vertices;   // 'v'
	std::vector<float> normals;    // 'vn'
	std::vector<float> texcoords;  // 'vt'
} tzMeshAttr;

class tzCoreMesh : public tzCoreObject,
				   public tzCoreTransform
{
public:
	tzCoreMesh( );
	virtual ~tzCoreMesh();

	typedef struct {
		int vertex_index;
		int normal_index;
		int texcoord_index;
	} index;
	
private: // member data
	std::vector<tzPoint3D> 			mVertices;				// mesh vertices 
	std::vector<float>				mFloatNormals;			// float array of the normal
	std::vector<tzNormal>			mVertexNormals;			// average normal at each vertex;
	std::vector<float>				mTexcoords;				// float array of the texture coords
	std::vector<float>				mU;						// u texture coordinate at each vertex
	std::vector<float>				mV;						// v texture coordinate at each vertex
	std::vector<std::vector<int> > 	mVertexFaces;			// the triangles shared by each vertex
	int 							mNumVertices; 			// number of vertices
	int 							mNumTriangles; 			// number of triangles
	std::vector<index>				mIndices;				// indices of the vertices
	std::vector<float>				mPositions;				// 
	std::vector<std::vector<int>>	mFaceVertices;
	std::vector<int>				mMaterialIds;

	tzCoreMaterial					*mPtrCoreMaterial;

public: // 

	// set data interfaces
	void							setVertices( const std::vector< tzPoint3D > &vertices);
	void							setVertices(const std::vector< float > &positions);
	void							setVertexNormals(const std::vector< tzNormal > &normals);
	void							updateFloatNormals(const std::vector< float >* normals = NULL);
	void							setUs(const std::vector< float > &us);
	void							setVs(const std::vector< float > &vs);
	void							setMaterialIds( const std::vector<int> &matIds );
	void							updateTexcoords(const std::vector< float >* texcoord = NULL);
	void							setVertexFaces( const std::vector<std::vector<int> > vertexFaces);
	void							setNumVertices( int numVertices );
	void							setNumTriangles(int numVertices);
	void							setIndices( const std::vector< index > &indices );
	void							setFaceVertices( const std::vector<std::vector<int>> &faceVertices );
	void							setMaterial( tzCoreMaterial *mat );
	
	
	// get data interfaces
	const std::vector< float >&			floatPositions( ) const;
	const std::vector< tzPoint3D >&		vertices( ) const;
	const std::vector< tzNormal >&		vertexNormals( ) const;
	const std::vector< float > &			floatNormals() ;
	const std::vector< float >&			us( ) const;
	const std::vector< float >&			vs( ) const;
	const std::vector< float >&			texcoords( );
	const std::vector<std::vector<int> >		vertexFaces( ) const;
	int								numVertices( ) const;
	int								numTriangles( ) const;
	const std::vector< index >&			indices( ) const;
	const std::vector<std::vector<int>>&		faceVertices() const;
	const std::vector<int> &		materialIds( ) const;
	tzCoreMaterial* material( ) const;
};

#endif
