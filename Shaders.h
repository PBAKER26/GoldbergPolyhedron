#include "goldberg.h"

extern GLuint MasterMatrixID;
extern GLuint WorldMatrixID;
extern GLuint EyePositionID;
extern GLuint SpecularIntensityLocation;
extern GLuint SpecularPowerLocation;
extern GLuint LightColorID;
extern GLuint LightAmbientIntensityID;
extern GLuint LightDirectionID;
extern GLuint LightDiffuseIntensityID;

class GLSL_ShaderManager
{
public:
	// Loads a vertex shader
	static void CompileShaders();
private:
	static void AddShader(GLuint ShaderProgram, std::string filename, GLenum ShaderType);
	static char *ReadFileToBuffer(const char *filename);
};
