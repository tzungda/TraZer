#include "../Include/ViewManager.h"

//
#include "tzMatrix.h"
#include "tzNormal.h"

using namespace glm;

const float degreeToRadian = 3.1415926f / 180.0f;

ViewManager::ViewManager()
{
	mFace.x = 0.0f, mFace.y = 0.0f, mFace.z = -1.0f;
	mUp.x = 0.0f, mUp.y = 1.0f, mUp.z = 0.0f;
	mRight.x = 1.0f, mRight.y = 0.0f, mRight.z = 0.0f;

	//
	mXAxisAngle = mYAxisAngle = mZAxisAngle = 0.0f;
	//

	SetCamera(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f));
	zoom = 1.0f;
}

mat4 ViewManager::GetModelMatrix() {
	return mat4();//translationMatrix * rotationMatrix;
}

mat4 ViewManager::GetViewMatrix()
{
	viewMatrix = inverse(mTransform );
	mat4 m = viewMatrix ;//*translationMatrix * rotationMatrix;
    return m;
}

mat4 ViewManager::GetProjectionMatrix(float aspect)
{
    mat4 projectionMatrix;
    float nearVal;
    float farVal;
    nearVal = 0.1f;
    farVal = 10000.0f;
    if(ortho) {
        float size = 1.5f * zoom;
        projectionMatrix = glm::ortho(-aspect * size, aspect * size, -size, size, nearVal, farVal);
    } 
	else {
        projectionMatrix = perspective(radians(45.0f * zoom), aspect, nearVal, farVal);		
    }
    return projectionMatrix;
}

mat4 ViewManager::GetViewProjectionMatrix(float aspect)
{
    return GetProjectionMatrix(aspect) * viewMatrix;
}

mat4 ViewManager::GetModelViewProjectionMatrix(float aspect)
{
    return GetViewProjectionMatrix(aspect) * GetModelMatrix();
}

vec3 ViewManager::GetWorldEyePosition() {
	vec4 wordEyePosition = vec4(eyePosition, 0) * GetModelMatrix();
	return wordEyePosition.xyz;
}

vec3 ViewManager::GetWorldViewVector() {
	vec4 wordLookVector = vec4(-viewVector, 0) * GetModelMatrix();
	return wordLookVector.xyz;
}

void ViewManager::keyEvents(unsigned char key) {
	switch (key)
	{
	case 'w': 
	case 'W':
		Translate(vec3(0, 0, moveSpeed));
		break;
	case 'a':
	case 'A':
		Translate(vec3(moveSpeed, 0, 0));
		break;
	case 's':
	case 'S':
		Translate(vec3(0, 0, -moveSpeed));
		break;
	case 'd':
	case 'D':
		Translate(vec3(-moveSpeed, 0, 0));
		break; 
	case 'q':
	case 'Q':
		Translate(vec3(0, -moveSpeed, 0));
		break;
	case 'e':
	case 'E':
		Translate(vec3(0, moveSpeed, 0));
		break;
	case '+':
		wheelEvent(-moveSpeed);
		break;
	case '-':
		wheelEvent(moveSpeed);
		break;
	default:
		break;
	}
}

void ViewManager::mouseEvents(int button, int state, int x, int y) {

	if (state == GLUT_UP)
	{
		mouseReleaseEvent(button, x, y);
	}
	else if (state == GLUT_DOWN) 
	{
		mousePressEvent(button, x, y);
	}
	if (button == 4) 
	{
		wheelEvent(1);
	}
	else if (button == 3)
	{
		wheelEvent(-1);
	}
}

void ViewManager::mousePressEvent(int button, int x, int y)
{
    if( button == GLUT_LEFT_BUTTON)
	{
        lmbDown = true;
        lmbDownCoord = vec2(x,y);
        mat4 invrtRot = inverse(rotationMatrix);
        rotateYAxis = (invrtRot * vec4(0, 1, 0, 0)).xyz;
        rotateXAxis = (invrtRot * /*vec4(1, 0, 0, 0)*/vec4(rightDir(), 0.0f)).xyz;
		
    } 
	else if(button == GLUT_MIDDLE_BUTTON)
	{
        midDown = true;
        midDownCoord = vec2(x, y);
    }
	else if ( button == GLUT_RIGHT_BUTTON )
	{
		rmbDown = true;
		rmbDownCoord = vec2(x, y);
		mat4 invrtRot = inverse(rotationMatrix);
		rotateZAxis = (invrtRot * /*vec4(1, 0, 0, 0)*/vec4(faceDir(), 0.0f)).xyz;
	}
}

void ViewManager::mouseReleaseEvent(int button, int x, int y)
{
    if( button == GLUT_LEFT_BUTTON) // rotation
	{
        lmbDown = false;
    }
	else if(button == GLUT_MIDDLE_BUTTON || button == 3 || button == 4) { // pan
        midDown = false;
    }
	else if(button == GLUT_RIGHT_BUTTON) // zoomn
	{
		rmbDown = false;
	}
}

void ViewManager::mouseMoveEvent(int x,int y)
{
    if(lmbDown)
	{
        vec2 coord = vec2(x, y);
		vec2 diff = coord - lmbDownCoord;
        float factor = 0.06f;
		glm::mat4 rotM;
		if ( fabs( diff.x ) > fabs( diff.y ) )
		{
			rotM = rotateMatrixAlongVector(diff.x*factor, upDir());
		}
		else
		{
			rotM = rotateMatrixAlongVector(diff.y*factor, rightDir());
		}
		float angleX = atan2(rotM[1][2], rotM[2][2]) / degreeToRadian;
		float angleY = atan2(-rotM[0][2], sqrt(rotM[1][2] * rotM[1][2] + rotM[2][2] * rotM[2][2])) / degreeToRadian;
		float angleZ = atan2(rotM[0][1], rotM[0][0]) / degreeToRadian;
		roatateX( angleX );
		roatateY( angleY );
		roatateZ( angleZ );
		updateTransformMatrix();
			//glm::vec3 up = mUp;//upDir();
			//rotationMatrix = rotate( rotationMatrix ,diff.x * factor, /* rotateYAxis*/ upDir() );
			//vec3 dir = normalize(viewVector*mat3(rotationMatrix) );
			//float len = ( eyeLookPosition - eyePosition ).length();
			//viewMatrix = lookAt( eyePosition, eyePosition - dir*len, upDir() );

			//mRight = mRight*glm::mat3(rotationMatrix), mRight = normalize(mRight );
			//mFace = mFace*glm::mat3(rotationMatrix), mFace = normalize(mFace);
		
		
        lmbDownCoord = coord;
    }
	else if(midDown)
	{
		vec2 coord = vec2(x,y);
		vec2 diff = coord - midDownCoord;

		mPosition += upDir() * diff.y * 10.0f / (float)w_height;
		mPosition -= rightDir() * diff.x * 10.0f / (float)w_height;
		updateTransformMatrix();

		//vec4 up( upDir(), 0.0f ); //vec4(0, 1, 0, 0);
		//vec4 right( rightDir(), 0.0f ); //vec4(1, 0, 0, 0);

		//vec3 diffUp = up.xyz * diff.y / (float)w_height;
		//vec3 diffRight = right.xyz * diff.x / (float)w_width;

		//eyeLookPosition += ( diffUp - diffRight )*3.0f;
		//eyePosition += (diffUp - diffRight)*3.0f;

		//viewMatrix = lookAt(eyePosition, eyeLookPosition, vec3( 0.0f, 1.0f, 0.0f ) );
        //translationMatrix = translate(translationMatrix, (-diffUp + diffRight) * zoom * 3.0f);
        midDownCoord = coord;
    }
	else if( rmbDown )
	{
		vec2 coord = vec2(x, y);
		vec2 diff = coord - rmbDownCoord;
		rmbDownCoord = coord;
		float d = 0.02f;
		if (fabs(diff.x) > fabs(diff.y))
		{
			d *= diff.x;
		}
		else
		{
			d *= diff.y;
		}
		mPosition += faceDir()*d*3.0f;
		updateTransformMatrix();
		//translationMatrix = translate(translationMatrix, rotateZAxis * d * 3.0f);
		//eyeLookPosition += faceDir()*d*3.0f;
		//eyePosition += faceDir()*d*3.0f;

		//viewMatrix = lookAt(eyePosition, eyeLookPosition, vec3(0.0f, 1.0f, 0.0f));
	}
}

void ViewManager::wheelEvent(int direction)
{
	wheel_val = direction * 15.0f;
    Zoom(wheel_val / 120.0f);
}

void ViewManager::Zoom(float distance)
{
    zoom *= (1.0f + 0.05f * distance);
    zoom = clamp(0.1f, zoom, 3.0f);
}

void ViewManager::SetCamera(glm::vec3 cameraPos, glm::vec3 focusPos) {
	eyePosition = cameraPos;
	eyeLookPosition = focusPos;

	//
	//mFace = normalize( focusPos - cameraPos );
	//mRight = normalize( cross(mFace, mUp) );
	//mUp = normalize(cross(mRight, mFace));
	//
	viewVector = eyePosition - eyeLookPosition;
	viewVector = normalize(viewVector);

	vec3 up = vec3(0, 1, 0);
	viewMatrix = lookAt(eyePosition, eyePosition - viewVector, up);

	/*
	tmat4x4(
	T const & x0, T const & y0, T const & z0, T const & w0,
	T const & x1, T const & y1, T const & z1, T const & w1,
	T const & x2, T const & y2, T const & z2, T const & w2,
	T const & x3, T const & y3, T const & z3, T const & w3);
	*/
	setPosition(vec3(50.0f, 70.0f, 50.0f));
	mXAxisAngle = mYAxisAngle = mZAxisAngle = 0.0f;
	mat4 rotXMatrixX = roatateX( -45.0f );
	mat4 r0 = rotateMatrixAlongVector( -45.0f, vec3( 1.0f, 0.0f, 0.0f ) );
	mat4 rotYXMatrix = roatateY(45.0f);
	mat4 r1 = rotateMatrixAlongVector(45.0f, vec3(0.0f, 1.0f, 0.0f));
	updateTransformMatrix();

	float angleX = atan2( mTransform[1][2], mTransform[2][2] )/degreeToRadian;
	float angleY = atan2( -mTransform[0][2], sqrt(mTransform[1][2]* mTransform[1][2] + mTransform[2][2]* mTransform[2][2]) ) / degreeToRadian;
	float angleZ = atan2( mTransform[0][1], mTransform[0][0] ) / degreeToRadian;
	/*
	const float ff = 3.1415926f/180.0f;
	mat4 viewMatrixTest(	1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, cos(-45.0f*ff), sin(-45.0f*ff), 0.0f,
							0.0f, -sin(-45.0f*ff), cos(-45.0f*ff), 0.0f,
							0.0f, 50.0f, 50.0f, 1.0f );
	*/
	
	//viewMatrix = inverse(mTransform);

	// test------------------------------------------------------------
	//vec4 test = vec4( 0, 1, 0, 0 );
	//test = test*viewMatrix;
	// test------------------------------------------------------------

	//viewVector = eyePosition - eyeLookPosition;
	viewVector = normalize(viewVector);
}

void ViewManager::SetWindowSize(int width,int height) {
	w_width = width;
	w_height = height;
}

/*
void ViewManager::SetRotation(float theta, float phi)
{
    rotationMatrix = mat4(1.0);
    rotationMatrix = rotate(rotationMatrix, theta, vec3(0, 1, 0));
    rotationMatrix = rotate(rotationMatrix, phi, vec3(1, 0, 0));
}

void ViewManager::SetRotation(float x, float y, float z)
{
	vec3 v(x, y, z);
    v = normalize(v);
	vec3 o(0, 0, 1);
    double angle = acos(dot(v, o));
    rotationMatrix = mat4(1.0);
    rotationMatrix = rotate(rotationMatrix, (float)angle, cross(o, v));
}
*/

void ViewManager::Reset()
{
	wheel_val = 0.0f;
    zoom = 1.0f;
    translationMatrix = mat4(1.0);
    rotationMatrix = mat4(1.0);
}

void ViewManager::Translate(vec3 vec) {
	vec3 diff = vec;

	vec4 up = vec4(0, 1, 0, 0);
	vec4 right = vec4(1, 0, 0, 0);
	vec4 front = vec4(0, 0, 1, 0);

	vec3 diffUp = up.xyz * diff.y;
	vec3 diffRight = right.xyz * diff.x;
	vec3 diffFront = front.xyz * diff.z;

	translationMatrix = translate(translationMatrix, (-diffUp + diffRight + diffFront) * zoom * 3.0f);
}

//===================================================================================
glm::vec3 ViewManager::faceDir() const
{
	glm::vec3 face = glm::mat3(mZRotationMatrix*mYRotationMatrix*mXRotationMatrix)*mFace;
	return normalize( face );
}

//===================================================================================
glm::vec3 ViewManager::upDir() const
{
	glm::vec3 up = glm::mat3(mZRotationMatrix*mYRotationMatrix*mXRotationMatrix)*mUp;
	return normalize(up);
}

//===================================================================================
glm::vec3 ViewManager::rightDir() const
{
	glm::vec3 right = glm::mat3(mZRotationMatrix*mYRotationMatrix*mXRotationMatrix)*mRight;
	return normalize(right);
}

//===================================================================================
glm::mat4 ViewManager::rotateMatrixAlongVector(float angle, const glm::vec3 &v)
{
	float a = angle*degreeToRadian;
	glm::vec3 u = normalize( v );

	//
	glm::mat4 m(	cos(a) + u.x*u.x*(1.0f - cos(a) )   , u.y*u.x*(1.0f - cos(a)) + u.z*sin(a), u.z*u.x*(1.0f - cos(a)) - u.y*sin(a), 0.0f,
					u.x*u.y*(1.0f - cos(a)) - u.z*sin(a), cos(a) + u.y*u.y*(1.0f - cos(a))	  , u.z*u.y*(1.0f - cos(a)) + u.x*sin(a), 0.0f,
					u.x*u.z*(1.0f - cos(a)) + u.y*sin(a), u.y*u.z*(1.0f - cos(a)) - u.x*sin(a), cos(a) + u.z*u.z*(1.0f - cos(a))    , 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f );
	return m;
}

//===================================================================================
glm::mat4  ViewManager::roatateX(float deltaAngle)
{
	mXAxisAngle += deltaAngle;
	mXRotationMatrix = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos(mXAxisAngle*degreeToRadian), sin(mXAxisAngle*degreeToRadian), 0.0f,
		0.0f, -sin(mXAxisAngle*degreeToRadian), cos(mXAxisAngle*degreeToRadian), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return mXRotationMatrix;
}

//===================================================================================
glm::mat4  ViewManager::roatateY(float deltaAngle)
{
	mYAxisAngle += deltaAngle;
	mYRotationMatrix = glm::mat4(cos(mYAxisAngle*degreeToRadian), 0.0f, -sin(mYAxisAngle*degreeToRadian), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin(mYAxisAngle*degreeToRadian), 0.0f, cos(mYAxisAngle*degreeToRadian), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return mYRotationMatrix;
}

//===================================================================================
glm::mat4  ViewManager::roatateZ(float deltaAngle)
{
	mZAxisAngle += deltaAngle;
	mZRotationMatrix = glm::mat4(cos(mZAxisAngle*degreeToRadian), sin(mZAxisAngle*degreeToRadian), 0.0f, 0.0f,
		-sin(mZAxisAngle*degreeToRadian), cos(mZAxisAngle*degreeToRadian), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return mZRotationMatrix;
}

//===================================================================================
glm::mat4  ViewManager::updateTransformMatrix()
{
	/*
	viewMatrix = mat4(	1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
					mPosition.x, mPosition.y, mPosition.z, 1.0f);
	*/
	
	mTransform = mZRotationMatrix*mYRotationMatrix*mXRotationMatrix ;
	mTransform[3][0] = mPosition.x;
	mTransform[3][1] = mPosition.y;
	mTransform[3][2] = mPosition.z;
	return mTransform;
}

//===================================================================================
void ViewManager::setPosition( const glm::vec3 &position)
{
	mPosition = position;
}

//===================================================================================
glm::vec3  ViewManager::move(float deltaX, float deltaY, float deltaZ)
{
	glm::vec3 up = upDir( );
	glm::vec3 face = faceDir( );
	glm::vec3 right = rightDir( );
	mPosition += right*deltaX + up*deltaY + face*deltaZ ;

	return mPosition;
}