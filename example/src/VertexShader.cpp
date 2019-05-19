#include "VertexShader.h"
#include "Utilities.h"

VertexShader::VertexShader()
{
}

VertexShader::VertexShader(const char * aVertexPath, const char * aControlpath, const char * aEvaluationpath, 
	const char * aGeometryPath, const char * aFragmentPath, unsigned int aInputCount,
	const char * aInputs[], unsigned int aOutputCount, const char * aOutputs[])
{
	unsigned int vs = Utility::loadShader(aVertexPath,GL_VERTEX_SHADER);
	unsigned int fs = Utility::loadShader(aFragmentPath, GL_FRAGMENT_SHADER);

	const char* szInputs[] = { "Position"};

	mShaderID = glCreateProgram();
	glAttachShader(mShaderID, vs);
	glAttachShader(mShaderID, fs);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(mShaderID);

	glGetProgramiv(mShaderID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(mShaderID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
	}

	glValidateProgram(mShaderID);
	glGetProgramiv(mShaderID, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(mShaderID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		//   return false;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
}

VertexShader::~VertexShader()
{
}

void VertexShader::useShader(bool aClear)
{
	glUseProgram(mShaderID);
	setUniforms();
}
