#ifndef TREE_H_
#define TREE_H_

#include <iostream>
#include <vector>
#include "../Include/Common.h"
#include <random>

using namespace std;
using namespace glm;

typedef struct
{
	float angle;
	float lengthMult;
	float thinMult;
	float initLengthFac;
	float initThinFac;
	float angleLimb;
	float stemFac;
	float limbOffset;
	float leafSize;
	int numLimbs;
	int maxLeaves;
} TreeParams;

class Tree
{
public:
	
	Tree();
	Tree(Shape stemShape, Shape appleShape);
	Tree(Shape stemShape, Shape appleShape, bool drawLeaves, bool drawApples);
	virtual ~Tree();

	void build(int depth);
	void draw(GLint modelMatrixUniform, GLint useInstancingUniform, GLint treeStemFactor = -1);
	
	void setDrawingOption(bool drawLeaves, bool drawApples);
	void setParams(TreeParams params);

private:

	void initialize(Shape stemShape, Shape appleShape, bool drawLeaves, bool drawApples);
	void buildRecursive(mat4 &parentMat, int depth, float len, float t);
	void bindInstancedModelMatrix(Shape &shape, vector<mat4> &modelMatrices);
	float power(float base, int exp);

	GLuint _stemTex;
	GLuint _leafTex;
	GLuint _appleTex;

	bool _drawLeaves;
	bool _drawApples;

	Shape _stemBaseShape;
	Shape _leafBaseShape;
	Shape _appleBaseShape;

	int _depth;
	vector<mat4> _stems;
	vector<mat4> _leavies;
	vector<mat4> _apples;

	uniform_real_distribution<float> randomFloats;
	default_random_engine generator;

	TreeParams _params;
};

#endif // !TREE_H_