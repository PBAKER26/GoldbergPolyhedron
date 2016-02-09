#include "Shaders.h"

GLuint MasterMatrixID;
GLuint WorldMatrixID;
GLuint EyePositionID;
GLuint SpecularIntensityLocation;
GLuint SpecularPowerLocation;
GLuint LightColorID;
GLuint LightAmbientIntensityID;
GLuint LightDirectionID;
GLuint LightDiffuseIntensityID;

char * GLSL_ShaderManager::ReadFileToBuffer(const char *filename) {
	std::ifstream fin;
	std::string buffer;

	char line[256];
	char *ret_val = NULL;

	fin.open(filename, std::ios::in);

	if (fin.is_open()) {
		while (fin.good()) {
			fin.getline(line, 256);
			buffer.append(line);
			buffer.append("\n");
		}

		ret_val = new char[buffer.length() + 1];

#pragma warning( push )
#pragma warning( disable : 4996 )
		strncpy(ret_val, buffer.c_str(), buffer.length() + 1);
#pragma warning( pop ) 
		
	}

	return ret_val;
}

void GLSL_ShaderManager::AddShader(GLuint ShaderProgram, std::string filename, GLenum ShaderType) {
	GLuint ShaderObj = glCreateShader(ShaderType);
	const char *buffer;

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}

	buffer = filename.c_str();

	char *shaderSource = ReadFileToBuffer(buffer);

	glShaderSource(ShaderObj, 1, (const GLchar **)&shaderSource, 0);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}

	glAttachShader(ShaderProgram, ShaderObj);
}

void GLSL_ShaderManager::CompileShaders() {
	GLuint ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	AddShader(ShaderProgram, "vertexshader.glsl", GL_VERTEX_SHADER);
	AddShader(ShaderProgram, "fragmentshader.glsl", GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glUseProgram(ShaderProgram);

	MasterMatrixID = glGetUniformLocation(ShaderProgram, "MasterMatrix");
	WorldMatrixID = glGetUniformLocation(ShaderProgram, "WorldMatrix");
	LightColorID = glGetUniformLocation(ShaderProgram, "gDirectionalLight.Color");
	LightAmbientIntensityID = glGetUniformLocation(ShaderProgram, "gDirectionalLight.AmbientIntensity");
	LightDirectionID = glGetUniformLocation(ShaderProgram, "gDirectionalLight.Direction");
	LightDiffuseIntensityID = glGetUniformLocation(ShaderProgram, "gDirectionalLight.DiffuseIntensity");
	EyePositionID = glGetUniformLocation(ShaderProgram, "gEyeWorldPos");
	SpecularIntensityLocation = glGetUniformLocation(ShaderProgram, "gMatSpecularIntensity");
	SpecularPowerLocation = glGetUniformLocation(ShaderProgram, "gSpecularPower");

	return;
}
