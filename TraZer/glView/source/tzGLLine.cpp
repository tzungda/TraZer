#include "../include/tzGLLine.h"

#include "../Utilities/tzVector3D.h"
/*
Constructor/Destructor
*/

//================================================================================
tzGLLine::tzGLLine()
{
	this->mInitialized = false;
	mShaderName = "line";
}

//================================================================================
tzGLLine::~tzGLLine()
{
	if ( mInitialized )
	{
		glDeleteBuffers( 1, &mVAObject );
		glDeleteBuffers( 1, &mVBObject );
		glDeleteShader( mShaderProgram );
	}
}


/*
my interfaces
*/

//================================================================================
void tzGLLine::setLineData( const tzVector3D& startPoint, const tzVector3D& endPoint)
{
	mLintPoints.push_back( startPoint );
	mLintPoints.push_back( endPoint );
}


/*
tzIGLDraw interfaces
*/

//================================================================================
void tzGLLine::init(GLuint shaderProgram)
{
	if (mInitialized )
		return;

	glGenVertexArrays(1, &mVAObject);
	glGenBuffers(1, &mVBObject);

	glBindVertexArray(mVAObject);
	glBindBuffer(GL_ARRAY_BUFFER, mVBObject);

	glBufferData(GL_ARRAY_BUFFER, mLintPoints.size() * sizeof(tzVector3D), &mLintPoints[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(tzVector3D), (GLvoid*)0);

	// clean
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// setup shader
	/*std::string commonDataPath = "./commonData/";
	mShaderProgram = loadShader(commonDataPath + "line.vs", commonDataPath + "line.fs");
	{
		glUseProgram(mShaderProgram);
		mLineShaderParams.model_matrix = glGetUniformLocation(mShaderProgram, "model");
		mLineShaderParams.view_matrix = glGetUniformLocation(mShaderProgram, "view");
		mLineShaderParams.projection_matrix = glGetUniformLocation(mShaderProgram, "projection");
		glUseProgram(0);
	}*/

	//
	this->mInitialized = true;
}

//================================================================================
void tzGLLine::draw()
{
	if (!mInitialized)
		return;
	//glm::mat4 mtx;

	//glUseProgram(lineShaderProgram);

	glBindVertexArray(mVAObject);

	//glUniformMatrix4fv(lineShaderPrograms.model_matrix, 1, GL_FALSE, value_ptr(mtx));

	glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)(sizeof(tzVector3D)*mLintPoints.size()));
	glBindVertexArray(0);
}