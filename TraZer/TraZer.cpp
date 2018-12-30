/*
#ifdef _MSC_VER
#include "GLEW/glew.h"
#include "FreeGLUT/freeglut.h"
#include <direct.h>
#else
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#include <unistd.h>
#endif
*/
#include "Common.h"
#include "ViewManager.h"

// headers
#include "../glView/include/tzGLWorldCentre.h"
#include "../Include/tzTool.h"

#include "Tracer\include\tzWorld.h"

using namespace glm;
using namespace std;

#define PI				3.14159265359f
#define RADIAN_TO_ANGLE (180.0f / PI)
#define ANGLE_TO_RADIAN (PI / 180.0f)

#define SHADOW_OTHO	15.0f
const float SHADOW_NEAR_PLANE = 0.01f;
const float SHADOW_FAR_PLANE = 50.0f;

struct
{
	GLint  model_matrix;
	GLint  view_matrix;
	GLint  projection_matrix;
	GLint  light_matrix;
	GLint  lightPos;
	GLint  viewPos;
	GLint  useShadowmap;
	GLint  useBias;
	GLint  pcfKernel;
} blinnPhongShaderUniforms;

struct
{
	GLint  model_matrix;
	GLint  view_matrix;
	GLint  projection_matrix;
} simpleShaderPrograms;

struct
{
	GLint model_matrix;
	GLint view_matrix;
	GLint projection_matrix;
	GLint light_matrix;
	GLint lightPos;
	GLint viewPos;
	GLint ambient;
	GLint diffuse;
	GLint specular;
	GLint shininess;

	GLint hasAmbientTex;
	GLint hasDiffuseTex;
	GLint hasSpecularTex;

} phongShaderPrograms;

struct
{
	GLint model_matrix;
	GLint view_matrix;
	GLint projection_matrix;
	GLint line_color;

} lineShaderPrograms;

mat4			proj_matrix;		//projection matrix
float			aspect;
vec3			m_lightPos = vec3(5, 5, 0);

//UI
//TwBar			*bar;

GLuint			m_mainWindow;
vec2			m_screenSize;
ViewManager		m_camera;


//material info
typedef struct
{
	string name;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 transmittance;
	vec3 emission;
	GLfloat shininess;
	GLfloat ior;      // index of refraction
	GLfloat dissolve; // 1 == opaque; 0 == fully transparent
					  // illumination model (see http://www.fileformat.info/format/material/)
	int illum;

	int dummy; // Suppress padding warning.

	GLuint m_tex_ambient;
	GLuint m_tex_diffuse;
	GLuint m_tex_specular;
	GLuint m_tex_specular_highlight;
	GLuint m_tex_bump;
	GLuint m_tex_displacement;
	GLuint m_tex_alpha;

	GLboolean hasAmbientTex;
	GLboolean hasDiffuseTex;
	GLboolean hasSpecularTex;

} Material;


GLuint			phongShaderProgram;
map<string, vector<Material>> m_materials;//mesh name


										  //model info
typedef struct
{
	string name;
	GLuint vao;
	GLuint vbo[3];
	GLuint ebo;
	int materialId;
	int indexCount;
	GLuint m_texture;

	vec3 position = vec3(0.0f);
	vec3 rotation = vec3(0.0f);
	vec3 scale = vec3(1.0f);

	string shaderName = "phong";

	mat4 getTransformationMatrix()
	{
		mat4 transformationMatrix = mat4(1.0f);
		transformationMatrix = glm::translate(transformationMatrix, position);
		transformationMatrix = glm::rotate(transformationMatrix, rotation[2] * ANGLE_TO_RADIAN, vec3(0.0, 0.0, 1.0));
		transformationMatrix = glm::rotate(transformationMatrix, rotation[0] * ANGLE_TO_RADIAN, vec3(1.0, 0.0, 0.0));
		transformationMatrix = glm::rotate(transformationMatrix, rotation[1] * ANGLE_TO_RADIAN, vec3(0.0, 1.0, 0.0));
		transformationMatrix = glm::scale(transformationMatrix, scale);
		return transformationMatrix;
	}

	void Draw()
	{
		map<string, vector<Material>>::iterator found = m_materials.find(name);
		if (found != m_materials.end())
		{
			Material mat = found->second[materialId];
			glBindVertexArray(vao);
			glUniformMatrix4fv(phongShaderPrograms.model_matrix, 1, GL_FALSE, value_ptr(getTransformationMatrix()));
			glUniform3fv(phongShaderPrograms.ambient, 1, value_ptr(mat.ambient));
			glUniform3fv(phongShaderPrograms.diffuse, 1, value_ptr(mat.diffuse));
			glUniform3fv(phongShaderPrograms.specular, 1, value_ptr(mat.specular));
			glUniform1f(phongShaderPrograms.shininess, (mat.shininess));

			glUniform1i(phongShaderPrograms.hasAmbientTex, (mat.hasAmbientTex));
			if (mat.hasAmbientTex)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mat.m_tex_ambient);
			}
			glUniform1i(phongShaderPrograms.hasDiffuseTex, (mat.hasDiffuseTex));
			if (mat.hasDiffuseTex)
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, mat.m_tex_diffuse);
			}
			glUniform1i(phongShaderPrograms.hasSpecularTex, (mat.hasSpecularTex));
			if (mat.hasSpecularTex)
			{
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, mat.m_tex_specular);
			}
			//glPolygonMode(GL_FRONT, GL_LINE);
			//glPolygonMode(GL_BACK, GL_LINE);
			//glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			glDrawArrays(GL_TRIANGLES, 0, indexCount);
			glBindVertexArray(0);
		}
		else
		{
			printf("Material name : %s Not Find!!\n", name.c_str() );
		}

	}

} Shape;

//---------------------------------------------------------------------------------------------------
tzGLWorldCentre gWorldCentre;
GLuint			lineShaderProgram;
//---------------------------------------------------------------------------------------------------

vector<Shape>	m_shapes;

void drawaxes(void)
{
	glColor3ub(255, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(50.0, 0.0, 0.0);
	glVertex3f(0.75, 0.25, 0.0);
	glVertex3f(0.75, -0.25, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glVertex3f(0.75, 0.0, 0.25);
	glVertex3f(0.75, 0.0, -0.25);
	glVertex3f(1.0, 0.0, 0.0);
	glEnd();
	glColor3ub(0, 255, 0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 50.0, 0.0);
	glVertex3f(0.0, 0.75, 0.25);
	glVertex3f(0.0, 0.75, -0.25);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.25, 0.75, 0.0);
	glVertex3f(-0.25, 0.75, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glEnd();
	glColor3ub(0, 0, 255);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 50.0);
	glVertex3f(0.25, 0.0, 0.75);
	glVertex3f(-0.25, 0.0, 0.75);
	glVertex3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.25, 0.75);
	glVertex3f(0.0, -0.25, 0.75);
	glVertex3f(0.0, 0.0, 1.0);
	glEnd();

	glColor3ub(255, 255, 0);
	glRasterPos3f(50.1f, 0.0f, 0.0f);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'x');
	glRasterPos3f(0.0f, 50.1f, 0.0f);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'y');
	glRasterPos3f(0.0f, 0.0f, 50.1f);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'z');
}


GLuint loadShader(string vsPath, string fsPath)
{
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	char** vsSource = LoadShaderSource(vsPath.c_str());
	char** fsSource = LoadShaderSource(fsPath.c_str());
	glShaderSource(vs, 1, vsSource, NULL);
	glShaderSource(fs, 1, fsSource, NULL);
	FreeShaderSource(vsSource);
	FreeShaderSource(fsSource);
	glCompileShader(vs);
	glCompileShader(fs);
	ShaderLog(vs);
	ShaderLog(fs);
	//Attach Shader to program
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);
	return shaderProgram;
}


void setupShaders()
{
	string dirPath = "./commonData/";

	phongShaderProgram = loadShader(dirPath + "phong.vs", dirPath + "phong.fs");
	{
		glUseProgram(phongShaderProgram);

		//Cache uniform variable id
		phongShaderPrograms.model_matrix = glGetUniformLocation(phongShaderProgram, "model");
		phongShaderPrograms.view_matrix = glGetUniformLocation(phongShaderProgram, "view");
		phongShaderPrograms.projection_matrix = glGetUniformLocation(phongShaderProgram, "projection");
		phongShaderPrograms.light_matrix = glGetUniformLocation(phongShaderProgram, "lightSpaceMatrix");
		phongShaderPrograms.lightPos = glGetUniformLocation(phongShaderProgram, "lightPos");
		phongShaderPrograms.viewPos = glGetUniformLocation(phongShaderProgram, "viewPos");
		phongShaderPrograms.ambient = glGetUniformLocation(phongShaderProgram, "ambient");
		phongShaderPrograms.diffuse = glGetUniformLocation(phongShaderProgram, "diffuse");
		phongShaderPrograms.specular = glGetUniformLocation(phongShaderProgram, "specular");
		phongShaderPrograms.shininess = glGetUniformLocation(phongShaderProgram, "shininess");

		phongShaderPrograms.hasAmbientTex = glGetUniformLocation(phongShaderProgram, "hasAmbientTexture");
		phongShaderPrograms.hasDiffuseTex = glGetUniformLocation(phongShaderProgram, "hasDiffuseTexture");
		phongShaderPrograms.hasSpecularTex = glGetUniformLocation(phongShaderProgram, "hasSpecularTexture");

		glUniform1i(glGetUniformLocation(phongShaderProgram, "ambientTexture"), 0);
		glUniform1i(glGetUniformLocation(phongShaderProgram, "diffuseTexture"), 1);
		glUniform1i(glGetUniformLocation(phongShaderProgram, "specularTexture"), 2);
	}

	// line shader
	glUseProgram(0);
	lineShaderProgram = loadShader(dirPath + "line.vs", dirPath + "line.fs");
	{
		glUseProgram(lineShaderProgram);
		lineShaderPrograms.model_matrix = glGetUniformLocation(lineShaderProgram, "model");
		lineShaderPrograms.view_matrix = glGetUniformLocation(lineShaderProgram, "view");
		lineShaderPrograms.projection_matrix = glGetUniformLocation(lineShaderProgram, "projection");
		lineShaderPrograms.line_color = glGetUniformLocation(lineShaderProgram, "lineColor");
	}
}




void setupModels()
{
	// initialize world axes buffers
	gWorldCentre.init();
	/*line1.setLineData( glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f, 0.0f, 0.0f) );
	line2.setLineData(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 20.0f, 0.0f));
	line1.init();
	line2.init();*/

	// path
	string dirPath = "./commonData/";

	//
	vector<string> model_names =
	{
		dirPath+"torus.obj",//"sponza.obj",
	};

	map<string, vector<tinyobj::shape_t>> model_cache;
	map<string, vector<tinyobj::material_t>> material_cache;

	for (int i = 0; i < model_names.size(); i++)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		map<string, vector<tinyobj::shape_t>>::iterator found = model_cache.find(model_names[i]);
		if (found != model_cache.end())
		{
			shapes = found->second;
		}
		else
		{
			std::string err;

			bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, model_names[i].c_str(), dirPath.c_str() );
			if (err.size() > 0)
			{
				printf("Load Models Fail: %s\n", err.c_str());
				continue;
			}
			model_cache.insert(pair<string, vector<tinyobj::shape_t>>(model_names[i], shapes));
			material_cache.insert(pair<string, vector<tinyobj::material_t>>(model_names[i], materials));

			printf("Load Models Success ! Shapes size %d Material size %d\n", (int)shapes.size(), (int)materials.size());
		}
		vector<Material> temp_materials;
		for (int j = 0; j < materials.size(); ++j)
		{
			Material m_material;
			m_material.ambient = glm::vec3(materials[j].ambient[0], materials[j].ambient[1], materials[j].ambient[2]);
			m_material.diffuse = glm::vec3(materials[j].diffuse[0], materials[j].diffuse[1], materials[j].diffuse[2]);
			m_material.specular = glm::vec3(materials[j].specular[0], materials[j].specular[1], materials[j].specular[2]);
			m_material.shininess = materials[j].shininess;
			m_material.transmittance = glm::vec3(materials[j].transmittance[0], materials[j].transmittance[1], materials[j].transmittance[2]);
			m_material.emission = glm::vec3(materials[j].emission[0], materials[j].emission[1], materials[j].emission[2]);
			m_material.ior = materials[j].ior;      // index of refraction
			m_material.dissolve = materials[j].dissolve; // 1 == opaque; 0 == fully transparent
														 // illumination model (see http://www.fileformat.info/format/material/)
			m_material.illum = materials[j].illum;

			m_material.dummy = materials[j].dummy; // Suppress padding warning.

			TextureData textureData;
			textureData = Load_png(materials[j].ambient_texname.c_str());
			if (m_material.hasAmbientTex = (textureData.data != NULL))
			{
				glGenTextures(1, &m_material.m_tex_ambient);
				glBindTexture(GL_TEXTURE_2D, m_material.m_tex_ambient);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textureData.width, textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data);
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}

			textureData = Load_png(materials[j].diffuse_texname.c_str());
			if (m_material.hasDiffuseTex = (textureData.data != NULL))
			{
				glGenTextures(1, &m_material.m_tex_diffuse);
				glBindTexture(GL_TEXTURE_2D, m_material.m_tex_diffuse);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textureData.width, textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data);
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}

			textureData = Load_png(materials[j].specular_texname.c_str());
			if (m_material.hasSpecularTex = (textureData.data != NULL))
			{
				glGenTextures(1, &m_material.m_tex_specular);
				glBindTexture(GL_TEXTURE_2D, m_material.m_tex_specular);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textureData.width, textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data);
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}

			glBindTexture(GL_TEXTURE_2D, 0);

			printf("Add material %s\n", materials[j].name.c_str());

			temp_materials.push_back(m_material);
		}
		m_materials.insert(pair<string, vector<Material>>(model_names[i], temp_materials));

		for (int j = 0; j < shapes.size(); ++j)
		{
			Shape m_shape;
			m_shape.name = model_names[i];
			m_shape.indexCount = (int)shapes[j].mesh.indices.size();

			std::vector<float> positions;
			std::vector<float> texcoords;
			std::vector<float> normals;
			positions.resize(3 * shapes[j].mesh.indices.size());
			texcoords.resize(2 * shapes[j].mesh.indices.size());
			normals.resize(3 * shapes[j].mesh.indices.size());
			for (int idx = 0; idx < m_shape.indexCount; idx++)
			{
				for (int channel = 0; channel < 3; channel++)
				{
					positions[3 * idx + channel] = attrib.vertices[3 * shapes[j].mesh.indices[idx].vertex_index + channel];
					normals[3 * idx + channel] = attrib.normals[3 * shapes[j].mesh.indices[idx].normal_index + channel];
				}
				for (int channel = 0; channel < 2; channel++)
					texcoords[2 * idx + channel] = attrib.texcoords[2 * shapes[j].mesh.indices[idx].texcoord_index + channel];
			}
			glGenVertexArrays(1, &m_shape.vao);
			glBindVertexArray(m_shape.vao);

			glGenBuffers(3, m_shape.vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_shape.vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, m_shape.vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(float), texcoords.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, m_shape.vbo[2]);
			glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

			m_shape.materialId = shapes[j].mesh.material_ids[0];
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			printf("Add object %s ,material ID %d\n", shapes[j].name.c_str(), m_shape.materialId);
			m_shapes.push_back(m_shape);
		}
	}

	m_shapes[0].position = vec3(0, 0, 0);
}

void My_Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GLUT_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);

	//m_camera.SetCamera(vec3(-800.0f, 100.0f, -40.0f), vec3(10.0f, 100.0f, -40.0f));
	m_camera.SetCamera(vec3(0.0f, 0.0f, -200.0f), vec3(0.0f, 0.0f, 0.0f));
	m_camera.Zoom(3.0f);

	setupShaders();
	setupModels();
}

// GLUT callback. Called to draw the scene.
void My_Display()
{
	glClearColor(0.243f, 0.36f, 0.459f, 1.0f); // 124, 184, 234
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*glUseProgram(0);
	drawaxes();*/

	mat4 lightProjection, lightView, lightSpaceMatrix;
	lightProjection = glm::ortho(-SHADOW_OTHO, SHADOW_OTHO, -SHADOW_OTHO, SHADOW_OTHO, SHADOW_NEAR_PLANE, SHADOW_FAR_PLANE);
	lightView = glm::lookAt(m_lightPos, vec3(0.0f), vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	glUseProgram(phongShaderProgram);
	{
		glUniform3fv(phongShaderPrograms.lightPos, 1, value_ptr(m_lightPos));
		glUniform3fv(phongShaderPrograms.viewPos, 1, value_ptr(m_camera.GetWorldEyePosition()));
		glUniformMatrix4fv(phongShaderPrograms.light_matrix, 1, GL_FALSE, value_ptr(lightSpaceMatrix));
		glUniformMatrix4fv(phongShaderPrograms.view_matrix, 1, GL_FALSE, value_ptr(m_camera.GetViewMatrix() * m_camera.GetModelMatrix()));
		glUniformMatrix4fv(phongShaderPrograms.projection_matrix, 1, GL_FALSE, value_ptr(m_camera.GetProjectionMatrix(aspect)));
		//mesh->Draw();


		for (int i = 0; i < m_shapes.size(); i++)
		{
			m_shapes[i].Draw();
		}

	}

	// line
	glUseProgram(lineShaderProgram);
	{
		glm::mat4 mtx; // identical matrix
		glUniformMatrix4fv(lineShaderPrograms.view_matrix, 1, GL_FALSE, value_ptr(m_camera.GetViewMatrix() * m_camera.GetModelMatrix()));
		glUniformMatrix4fv(lineShaderPrograms.projection_matrix, 1, GL_FALSE, value_ptr(m_camera.GetProjectionMatrix(aspect)));
		glUniformMatrix4fv(lineShaderPrograms.model_matrix, 1, GL_FALSE, value_ptr(mtx));

		//glutSolidTeapot(30.0);
		/*
		glUniform3fv(lineShaderPrograms.line_color, 1, value_ptr(vec3(0.0f, 1.0f, 0.0f)));
		line1.draw();
		glUniform3fv(lineShaderPrograms.line_color, 1, value_ptr(vec3(1.0f, 0.0f, 0.0f)));
		line2.draw();
		*/
		gWorldCentre.draw(lineShaderPrograms.line_color);

		//drawLine();
	}

	/////////////////////////////////////////////////////////////////////////////////
	//TwDraw();



	glutSwapBuffers();
	glutPostRedisplay();
}

//Call to resize the window
void My_Reshape(int width, int height)
{
	m_screenSize = vec2(width, height);

	aspect = width * 1.0f / height;
	m_camera.SetWindowSize(width, height);
	glViewport(0, 0, width, height);

	//TwWindowSize(width, height);
}

//Timer event
void My_Timer(int val)
{
	glutPostRedisplay();
	glutTimerFunc(16, My_Timer, val);
}

//Mouse event
void My_Mouse(int button, int state, int x, int y)
{
	//if (!TwEventMouseButtonGLUT(button, state, x, y))
	{
		m_camera.mouseEvents(button, state, x, y);

		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				//printf("Mouse %d is pressed at (%d, %d)\n", button, x, y);
			}
			else if (state == GLUT_UP)
			{
				//printf("Mouse %d is released at (%d, %d)\n", button, x, y);
			}
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			//printf("Mouse %d is pressed\n", button);
		}
		//printf("%d %d %d %d\n",button,state,x,y);
	}
}

//Keyboard event
void My_Keyboard(unsigned char key, int x, int y)
{
//	if (!TwEventKeyboardGLUT(key, x, y))
	{
		m_camera.keyEvents(key);
		char k = 'r';
		//printf("Key %c is pressed at (%d, %d)\n", key, x, y);
		if ( key == k )
		{
			tzWorld w;
			w.build();
			w.setOutputPath( "C:\\Users\\User\\Desktop\\TraZer\\TraZer\\testImages\\Horse2K_01.png" );
			w.renderScene();
			int a = 0;
			a = 1;
		}
	}
}

//Special key event
void My_SpecialKeys(int key, int x, int y)
{
//	if (!TwEventSpecialGLUT(key, x, y))
	{
		switch (key)
		{
		case GLUT_KEY_F1:
			printf("F1 is pressed at (%d, %d)\n", x, y);
			break;
		case GLUT_KEY_PAGE_UP:
			printf("Page up is pressed at (%d, %d)\n", x, y);
			break;
		case GLUT_KEY_LEFT:
			printf("Left arrow is pressed at (%d, %d)\n", x, y);
			break;
		default:
			printf("Other special key is pressed at (%d, %d)\n", x, y);
			break;
		}
	}
}

void My_Mouse_Moving(int x, int y)
{
//	if (!TwEventMouseMotionGLUT(x, y))
	{
		m_camera.mouseMoveEvent(x, y);
	}
}

/*void setupGUI()
{
	// Initialize AntTweakBar

#ifdef _MSC_VER
	TwInit(TW_OPENGL, NULL);
#else
	TwInit(TW_OPENGL_CORE, NULL);
#endif

	TwGLUTModifiersFunc(glutGetModifiers);
	//bar = TwNewBar("Properties");
}*/

int main(int argc, char *argv[])
{
	// Initialize GLUT and GLEW, then create a window.
	////////////////////
	glutInit(&argc, argv);
#ifdef _MSC_VER
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
#else
	glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	m_mainWindow = glutCreateWindow("TraZer - GL View"); // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
#ifdef _MSC_VER
	glewInit();
#endif

	//Print debug information 
	DumpInfo();
	////////////////////

	//Call custom initialize function
	My_Init();
	//setupGUI();

	//Register GLUT callback functions
	////////////////////
	glutDisplayFunc(My_Display);
	glutReshapeFunc(My_Reshape);
	glutMouseFunc(My_Mouse);
	glutKeyboardFunc(My_Keyboard);
	glutSpecialFunc(My_SpecialKeys);
	glutTimerFunc(16, My_Timer, 0);
	glutPassiveMotionFunc(My_Mouse_Moving);
	glutMotionFunc(My_Mouse_Moving);
	////////////////////

	// Enter main event loop.
	glutMainLoop();
	//TwTerminate();
	return 0;
}
