#include <GL/glew.h>
#include "polyhedron.h"
#include "Shaders.h"
#include "matrix.h"
#include "texture.h"
#include <vector>

Texture*		hexTexture;
Vertex			icoV[12];
SDL_GLContext	glcontext;
Vector3f		You(0, 0, 0);
GLsizei			numV;

float ratio;
int width = 800;
int height = 800;
float FOV = 60.0f;
float nearDist = 0.1f;
float farDist = 200.0f;

void loadTextures() {
	hexTexture = new Texture(GL_TEXTURE_2D, "images/textures.bmp");
	hexTexture->Load();
}

bool goldberg::initRendering() {
	ratio = (float)width / (float)height;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
	if ((Window_Display = SDL_CreateWindow("Goldberg Polyhedron", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL)) == NULL) return false;
	glcontext = SDL_GL_CreateContext(Window_Display);

	glewInit();

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	initPerspective(FOV, width, height, nearDist, farDist);

	GLSL_ShaderManager::CompileShaders();

	loadTextures();
	polyModel = new Polyhedron(sizeM, sizeN);

	return true;
}

void goldberg::drawScene() {
	Matrix4f MasterMatrix;
	Matrix4f WorldMatrix;
	Plane maskPlane;

	maskPlane.setNormalAndPoint(Vector3f(sin(ToRadian(rotation.y)) * cos(ToRadian(rotation.x)),
										 sin(ToRadian(rotation.x)),
										 cos(ToRadian(rotation.x)) * cos(ToRadian(rotation.y))),
								Vector3f(sin(ToRadian(rotation.y)) * cos(ToRadian(rotation.x)),
										 sin(ToRadian(rotation.x)),
										 cos(ToRadian(rotation.x)) * cos(ToRadian(rotation.y))) * -0.7);

	cameraLook(You, Vector3f(0, 0, 1), Vector3f(0, 1, 0));
	
	MasterMatrix = matrixTransform(	Vector3f(1, 1, 1), rotation, Vector3f(0, 0, 10 + polyModel->radius));
	WorldMatrix  =  worldTransform(	Vector3f(1, 1, 1), rotation, Vector3f(0, 0, 0));

	glUniformMatrix4fv(MasterMatrixID,	1, GL_TRUE, &MasterMatrix.m[0][0]);
	glUniformMatrix4fv(WorldMatrixID,	1, GL_TRUE,  &WorldMatrix.m[0][0]);
	glUniform3f(EyePositionID, You.x, You.y, You.z);

	glUniform1f(SpecularIntensityLocation, 0);
	glUniform1f(SpecularPowerLocation, 32);
	glUniform1f(LightAmbientIntensityID, 0.2);
	glUniform3f(LightColorID, 1, 1, 1);
	Vector3f Direction = Vector3f(.6, .8, 1);
	Direction.Normalize();
	glUniform1f(LightDiffuseIntensityID, 0.8);
	glUniform3f(LightDirectionID, Direction.x, Direction.y, Direction.z);

	hexTexture->Bind(GL_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, polyModel->VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

	glDrawArrays(GL_TRIANGLES, 0, polyModel->numV);

}

void goldberg::OnRender() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	drawScene();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	SDL_GL_SwapWindow(Window_Display); //Send the 3D scene to the screen

	return;
}

