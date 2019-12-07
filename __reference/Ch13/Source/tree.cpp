#include "tree.h"

Tree::Tree()
{

}
Tree::Tree(Shape stemShape, Shape appleShape) 
{
	initialize(stemShape, appleShape, true, false);
}
Tree::Tree(Shape stemShape, Shape appleShape, bool drawLeaves, bool drawApples)
{
	initialize(stemShape, appleShape, drawLeaves, drawApples);
}
Tree::~Tree()
{

}

void Tree::build(int depth)
{
	_depth = depth;
	_stems.clear();
	_leavies.clear();
	_apples.clear();

	buildRecursive(mat4(1.0), _depth, _params.initLengthFac * power(1.2, _depth), _params.initThinFac * power(1.6, _depth));

	bindInstancedModelMatrix(_stemBaseShape, _stems);
	bindInstancedModelMatrix(_leafBaseShape, _leavies);
	bindInstancedModelMatrix(_appleBaseShape, _apples);
}
void Tree::initialize(Shape stemShape, Shape appleShape, bool drawLeaves, bool drawApples)
{
	randomFloats = uniform_real_distribution<float>(0.0, 1.0);

	_stemBaseShape = stemShape;
	_appleBaseShape = appleShape;

	_drawLeaves = drawLeaves;
	_drawApples = drawApples;

	_params.angle = 60;
	_params.lengthMult = 0.78;
	_params.thinMult = 0.6;
	_params.initLengthFac = 0.8;
	_params.initThinFac = 0.016;
	_params.angleLimb = 30;
	_params.numLimbs = 3;
	_params.maxLeaves = 16;
	_params.stemFac = 0.6;
	_params.limbOffset = 0.52;
	_params.leafSize = 0.17;

	// Load stem and leaf textures

	TextureData texData = Common::Load_png("./cortex.png");
	glGenTextures(1, &_stemTex);
	glBindTexture(GL_TEXTURE_2D, _stemTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, texData.width, texData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texData = Common::Load_png("./leaf.png");
	glGenTextures(1, &_leafTex);
	glBindTexture(GL_TEXTURE_2D, _leafTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, texData.width, texData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texData = Common::Load_png("./apple.png");
	glGenTextures(1, &_appleTex);
	glBindTexture(GL_TEXTURE_2D, _appleTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, texData.width, texData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Setup leaf shape

	float leafVertices[] =
	{
		//position			    //texCoord        //normal
		0.0, 0.0, 0.0,			0.5, 0.0,		  0, 0, 1,
		0.167, 0.083, 0.0,		0.834, 0.083,	  0, 0, 1,
		0.225, 0.208, 0.0,		0.95, 0.208,	  0, 0, 1,
		0.25, 0.42,	0.0,		1.0, 0.42,		  0, 0, 1,
		0.23, 0.583, 0.0,		0.96, 0.583,	  0, 0, 1,
		0.167, 0.75, 0.0,		0.834, 0.75,	  0, 0, 1,
		0.0, 1.0, 0.0,			0.5, 1.0,		  0, 0, 1,
		-0.167, 0.75, 0.0,		0.166, 0.75,	  0, 0, 1,
		-0.23, 0.583, 0.0,		0.04, 0.583,	  0, 0, 1,
		-0.25, 0.42, 0.0,		0.0, 0.42,		  0, 0, 1,
		-0.225, 0.208, 0.0,		0.5, 0.208,		  0, 0, 1,
		-0.167, 0.083, 0.0,		0.166, 0.083,	  0, 0, 1
	};
	// setup plane VAO
	unsigned int vbo, vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(leafVertices), &leafVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	_leafBaseShape.vao = vao;
}
void Tree::buildRecursive(mat4 &parentMat, int depth, float length, float thin)
{
	length *= _params.lengthMult;
	float curLength = length * (randomFloats(generator) * 0.5 + 0.75); // vary the length of each stem by random
	thin *= _params.thinMult;

	// build stem
	{
		mat4 modelMatrix(1.0);
		modelMatrix = glm::scale(modelMatrix, vec3(thin, curLength, thin));
		_stems.push_back(parentMat * modelMatrix);
	}

	// build leaves
	int numOfLeaves = (_params.maxLeaves - depth * depth);
	for (int i = 0; i < numOfLeaves; i++)
	{
		float posLen = curLength * randomFloats(generator);
		float posAngle = 360 * randomFloats(generator);

		mat4 modelMatrix(1.0);
		modelMatrix = glm::rotate(modelMatrix, posAngle, vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, vec3(0, posLen, thin));
		modelMatrix = glm::rotate(modelMatrix, deg2rad(_params.angle), vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, vec3(_params.leafSize));

		_leavies.push_back(parentMat * modelMatrix);
	}
	
	// build apples
	if (depth > 0) 
	{
		int numOfApples = (5 - depth);
		for (int i = 0; i < numOfApples; i++) 
		{
			float posLength = curLength * randomFloats(generator);

			mat4 modelMatrix(1.0);
			modelMatrix = glm::translate(modelMatrix, vec3(0, posLength, thin));
			modelMatrix = glm::scale(modelMatrix, vec3(0.04));

			_apples.push_back(parentMat * modelMatrix);
		}
	}

	if (depth != 0) 
	{
		mat4 modelMatrix(1.0);
		modelMatrix = glm::rotate(modelMatrix, deg2rad(120.0f * (float)(randomFloats(generator) + 0.5)), vec3(0, 1, 0));

		// build one limb at the end of the stem
		{
			mat4 limbModelMatrix(1.0);
			limbModelMatrix = glm::translate(limbModelMatrix, vec3(0.0, curLength * 0.95, 0.0));
			limbModelMatrix = glm::rotate(limbModelMatrix, deg2rad(_params.angleLimb), vec3(0, 0, 1));
			buildRecursive(parentMat * modelMatrix * limbModelMatrix, depth - 1, length, thin);
		}

		// build two other limbs
		for (int i = 0; i < (_params.numLimbs - 1); i++)
		{
			modelMatrix = glm::rotate(modelMatrix, deg2rad((360.0f / _params.numLimbs) * (float)(randomFloats(generator) + 0.5)), vec3(0, 1, 0));
		
			mat4 limbModelMatrix(1.0);
			limbModelMatrix = glm::translate(limbModelMatrix, vec3(0.0, (randomFloats(generator) * (1 - _params.limbOffset) + _params.limbOffset) * curLength, 0.0));
			limbModelMatrix = glm::rotate(limbModelMatrix, deg2rad(_params.angleLimb), vec3(0, 0, 1));
			buildRecursive(parentMat * modelMatrix * limbModelMatrix, depth - 1, length, thin);
		}
	}
}
void Tree::bindInstancedModelMatrix(Shape &shape, vector<mat4> &modelMatrices)
{
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(mat4), &modelMatrices[0], GL_STATIC_DRAW);
	for (unsigned int i = 0; i < modelMatrices.size(); i++)
	{
		glBindVertexArray(shape.vao);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vec4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vec4), (void*)(sizeof(vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vec4), (void*)(2 * sizeof(vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vec4), (void*)(3 * sizeof(vec4)));
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glBindVertexArray(0);
	}
}
void Tree::draw(GLint modelMatrixUniform, GLint useInstancingUniform, GLint treeStemFactor)
{
#define USE_INSTANCING 1

	// draw stems
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _stemTex);
#if (USE_INSTANCING == 1)
	glUniform1i(useInstancingUniform, 1);
	if (treeStemFactor > 0) glUniform1f(treeStemFactor, _params.stemFac);
	glBindVertexArray(_stemBaseShape.vao);
	glDrawElementsInstanced(GL_TRIANGLES, _stemBaseShape.indexCount, GL_UNSIGNED_INT, 0, _stems.size());
	glBindVertexArray(0);
	glUniform1f(treeStemFactor, -1);
#else
	for (int i = 0; i < _stems.size(); ++i)
	{
		mat4 modelMatrix = _stems[i];
		glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, value_ptr(modelMatrix));
		glBindVertexArray(_stemBaseShape.vao);
		glDrawElements(GL_TRIANGLES, _stemBaseShape.indexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
#endif

	// draw leaves
	if (_drawLeaves)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _leafTex);
#if (USE_INSTANCING == 1)
		glUniform1i(useInstancingUniform, 1);
		glBindVertexArray(_leafBaseShape.vao);
		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 12, _leavies.size());
		glBindVertexArray(0);
#else
		for (int i = 0; i < _leavies.size(); ++i)
		{
			mat4 modelMatrix = _leavies[i];
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, value_ptr(modelMatrix));
			glBindVertexArray(_leafBaseShape.vao);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 12);
			glBindVertexArray(0);
		}
#endif
	}

	// draw apples
	if (_drawApples)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _appleTex);
		
		//{
		//	glUniform1i(useInstancingUniform, 0);
		//	mat4 m(1.0);
		//	m = translate(m, vec3(2, 0, 0));
		//	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, value_ptr(m));
		//	glBindVertexArray(_appleBaseShape.vao);
		//	glDrawElements(GL_TRIANGLES, _appleBaseShape.indexCount, GL_UNSIGNED_INT, 0);
		//	glBindVertexArray(0);
		//	glUniform1i(useInstancingUniform, 1);
		//}
		//
		//{
		//	glUniform1i(useInstancingUniform, 0);
		//	mat4 m(1.0);
		//	m = translate(m, vec3(0, 2, 0));
		//	m = glm::rotate(m, deg2rad(90), vec3(1, 0, 0));
		//	m = glm::rotate(m, deg2rad(90), vec3(1, 0, 0));
		//	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, value_ptr(m));
		//	glBindVertexArray(_appleBaseShape.vao);
		//	glDrawElements(GL_TRIANGLES, _appleBaseShape.indexCount, GL_UNSIGNED_INT, 0);
		//	glBindVertexArray(0);
		//	glUniform1i(useInstancingUniform, 1);
		//}

#if (USE_INSTANCING == 1)
		glUniform1i(useInstancingUniform, 1);
		glBindVertexArray(_appleBaseShape.vao);
		glDrawElementsInstanced(GL_TRIANGLES, _appleBaseShape.indexCount, GL_UNSIGNED_INT, 0, _apples.size());
		glBindVertexArray(0);
#else
		glUniform1i(useInstancingUniform, 0);
		for (int i = 0; i < _apples.size(); ++i)
		{
			mat4 modelMatrix = _apples[i];
			glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, value_ptr(modelMatrix));
			glBindVertexArray(_appleBaseShape.vao);
			glDrawElements(GL_TRIANGLES, _appleBaseShape.indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		glUniform1i(useInstancingUniform, 1);
#endif
	}

	glUniform1i(useInstancingUniform, 0);
}
void Tree::setDrawingOption(bool drawLeaves, bool drawApples)
{
	_drawLeaves = drawLeaves;
	_drawApples = drawApples;
}
void Tree::setParams(TreeParams params)
{
	_params = params;
}
float Tree::power(float base, int exp)
{
	if (exp == 1)
	{
		return base;
	}
	else if (exp == 0)
	{
		return 1.0;
	}
	else if (exp > -1)
	{
		exp--;
	}
	return base * (power(base, exp));
}
