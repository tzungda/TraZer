#include "../Include/ViewManager.h"

//
#include "tzMatrix.h"
#include "tzNormal.h"

using namespace glm;

ViewManager::ViewManager()
{
	mFace.x = 0.0f, mFace.y = 0.0f, mFace.z = -1.0f;
	mUp.x = 0.0f, mUp.y = 1.0f, mUp.z = 0.0f;
	mRight.x = 1.0f, mRight.y = 0.0f, mRight.z = 0.0f;

	SetCamera(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f));
	zoom = 1.0f;
}

mat4 ViewManager::GetModelMatrix() {
	return translationMatrix * rotationMatrix;
}

mat4 ViewManager::GetViewMatrix()
{
    return viewMatrix;
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
        rotateXAxis = (invrtRot * /*vec4(1, 0, 0, 0)*/vec4(mRight, 0.0f)).xyz;
		
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
		rotateZAxis = (invrtRot * /*vec4(1, 0, 0, 0)*/vec4(mFace, 0.0f)).xyz;
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
        float factor = 0.002f;
		if ( fabs( diff.x ) > fabs( diff.y ) )
		{
			//glm::vec3 up = mUp;//upDir();
			rotationMatrix = rotate(rotationMatrix,diff.x * factor, rotateYAxis);
			//mRight = mRight*glm::mat3(rotationMatrix), mRight = normalize(mRight );
			//mFace = mFace*glm::mat3(rotationMatrix), mFace = normalize(mFace);
		}
		else
		{
			//glm::vec3 right = mRight;//rightDir();
			rotationMatrix = rotate(rotationMatrix,diff.y * factor, rotateXAxis);
			//mUp *= rotationMatrix, mUp = normalize(mUp);
			//mFace *= rotationMatrix, mFace = normalize(mFace);
		}
        lmbDownCoord = coord;
    }
	else if(midDown)
	{
		vec2 coord = vec2(x,y);
		vec2 diff = coord - midDownCoord;

		vec4 up( upDir(), 0.0f ); //vec4(0, 1, 0, 0);
		vec4 right(rotateXAxis, 0.0f ); //vec4(1, 0, 0, 0);

		vec3 diffUp = up.xyz * diff.y / (float)w_height;
		vec3 diffRight = right.xyz * diff.x / (float)w_width;

        translationMatrix = translate(translationMatrix, (-diffUp + diffRight) * zoom * 3.0f);
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
		translationMatrix = translate(translationMatrix, rotateZAxis * d * 3.0f);
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
	mFace = normalize( focusPos - cameraPos );
	mRight = normalize( cross(mFace, mUp) );
	mUp = normalize(cross(mRight, mFace));
	//

	vec3 up = vec3(0, 1, 0);
	viewMatrix = lookAt(eyePosition, eyeLookPosition, up);
	viewVector = eyePosition - eyeLookPosition;
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

glm::vec3 ViewManager::faceDir() const
{
	glm::vec3 face = glm::mat3( rotationMatrix )*mFace;
	return normalize( face );
}

glm::vec3 ViewManager::upDir() const
{
	glm::vec3 up = glm::mat3(rotationMatrix)*mUp;
	return normalize(up);
}

glm::vec3 ViewManager::rightDir() const
{
	glm::vec3 right = glm::mat3(rotationMatrix)*mRight;
	return normalize(right);
}
