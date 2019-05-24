#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <stb_image.h>

#include "Error.h"

#include "Utilities.h"
#include "Application_Log.h"

static double s_prevTime = 0;
static float s_totalTime = 0;
static float s_deltaTime = 0;

void Utility::resetTimer()
{
	s_prevTime = glfwGetTime();
	s_totalTime = 0;
	s_deltaTime = 0;
}

float Utility::tickTimer()
{
	double currentTime = glfwGetTime();
	s_deltaTime = (float)(currentTime - s_prevTime);
	s_totalTime += s_deltaTime;
	s_prevTime = currentTime;
	return s_deltaTime;
}

float Utility::getDeltaTime()
{
	return s_deltaTime;
}

float Utility::getTotalTime()
{
	return s_totalTime;
}

// loads a shader from a file and creates it for the specified stage
unsigned int Utility::loadShader(const char* a_filename, unsigned int a_type)
{
	int success = GL_FALSE;

	if(a_filename == "")
	{
		return 0;
	}

	unsigned char* source = fileToBuffer(a_filename);
	unsigned int handle = glCreateShader(a_type);

	glShaderSource(handle, 1, (const char**)&source, 0);
	glCompileShader(handle);

	glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

	delete[] source;
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;		
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = DEBUG_NEW  char[infoLogLength];

		glGetShaderInfoLog(handle, infoLogLength, 0, infoLog);
		Application_Log* log = Application_Log::Create();
		if (log != nullptr)
		{

			log->addLog(LOG_LEVEL::LOG_ERROR, "Error: Failed to compile shader!\n : %s %s", infoLog, a_filename);
		}
		delete[] infoLog;
		return 0;
	}

	return handle;
}

// creates a shader program, links the specified shader stages to it, and binds the specified input/output attributes
unsigned int Utility::createProgram(unsigned int a_vertexShader, unsigned int a_controlShader, unsigned int a_evaluationShader, unsigned int a_geometryShader, unsigned int a_fragmentShader,
								  unsigned int a_inputAttributeCount /* = 0 */, const char** a_inputAttributes /* = nullptr */,
								  unsigned int a_outputAttributeCount /* = 0 */, const char** a_outputAttributes /* = nullptr */)
{
	int success = GL_FALSE;

	// create a shader program and attach the shaders to it
	unsigned int handle = glCreateProgram();
	glAttachShader(handle, a_vertexShader);
	glAttachShader(handle, a_controlShader);
	glAttachShader(handle, a_evaluationShader);
	glAttachShader(handle, a_geometryShader);
	glAttachShader(handle, a_fragmentShader);

	// specify vertex input attributes
	for ( unsigned int i = 0 ; i < a_inputAttributeCount ; ++i )
		glBindAttribLocation(handle, i, a_inputAttributes[i]);

	// specify fragment shader outputs
	for ( unsigned int i = 0 ; i < a_outputAttributeCount ; ++i )
		glBindFragDataLocation(handle, i, a_outputAttributes[i]);

	// link the program together and log errors
	glLinkProgram(handle);

	glGetProgramiv(handle, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;		
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = DEBUG_NEW  char[infoLogLength];

		glGetProgramInfoLog(handle, infoLogLength, 0, infoLog);

		Application_Log* log = Application_Log::Create();
		if (log != nullptr)
		{
			log->addLog(LOG_LEVEL::LOG_ERROR, "Error: Failed to link shader program!\n : %s", infoLog);
		}
		delete[] infoLog;
		return 0;
	}

	return handle;
}

unsigned char* Utility::fileToBuffer(const char* a_sPath)
{
	// open file for text reading
	FILE* pFile = fopen(a_sPath,"rb");
	if (pFile == nullptr)
	{
		printf("Error: Unable to open file '%s' for reading!\n",a_sPath);
		return nullptr;
	}

	// get number of bytes in file
	fseek(pFile, 0, SEEK_END);
	unsigned int uiLength = (unsigned int)ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	// allocate buffer and read file contents
	unsigned char* acBuffer = DEBUG_NEW  unsigned char[uiLength + 1];
	memset(acBuffer,0,uiLength + 1);
	fread(acBuffer, sizeof(unsigned char), uiLength, pFile);

	fclose(pFile);
	return acBuffer;
}

void Utility::freeMovement(glm::mat4& a_transform, float a_deltaTime, float a_speed, const glm::vec3& a_up /* = glm::vec3(0,1,0) */)
{
	GLFWwindow* window = glfwGetCurrentContext();

	// Get the camera's forward, right, up, and location vectors
	glm::vec4 vForward = a_transform[2];
	glm::vec4 vRight = a_transform[0];
	glm::vec4 vUp = a_transform[1];
	glm::vec4 vTranslation = a_transform[3];

	float frameSpeed = glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? a_deltaTime * a_speed * 2 : a_deltaTime * a_speed;	

	// Translate camera
	if (glfwGetKey(window,'W') == GLFW_PRESS)
	{
		vTranslation -= vForward * frameSpeed;
	}
	if (glfwGetKey(window,'S') == GLFW_PRESS)
	{
		vTranslation += vForward * frameSpeed;
	}
	if (glfwGetKey(window,'D') == GLFW_PRESS)
	{
		vTranslation += vRight * frameSpeed;
	}
	if (glfwGetKey(window,'A') == GLFW_PRESS)
	{
		vTranslation -= vRight * frameSpeed;
	}
	if (glfwGetKey(window,'Q') == GLFW_PRESS)
	{
		vTranslation += vUp * frameSpeed;
	}
	if (glfwGetKey(window,'E') == GLFW_PRESS)
	{
		vTranslation -= vUp * frameSpeed;
	}

	a_transform[3] = vTranslation;

	// check for camera rotation
	static bool sbMouseButtonDown = false;
	if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		static double siPrevMouseX = 0;
		static double siPrevMouseY = 0;

		if (sbMouseButtonDown == false)
		{
			sbMouseButtonDown = true;
			glfwGetCursorPos(window,&siPrevMouseX,&siPrevMouseY);
		}

		double mouseX = 0, mouseY = 0;
		glfwGetCursorPos(window,&mouseX,&mouseY);

		double iDeltaX = mouseX - siPrevMouseX;
		double iDeltaY = mouseY - siPrevMouseY;

		siPrevMouseX = mouseX;
		siPrevMouseY = mouseY;

		glm::mat4 mMat;
		
		// pitch
		if (iDeltaY != 0)
		{
			mMat = glm::axisAngleMatrix( vRight.xyz(), (float)-iDeltaY / 150.0f );
			vRight = mMat * vRight;
			vUp = mMat * vUp;
			vForward = mMat * vForward;
		}

		// yaw
		if (iDeltaX != 0)
		{
			mMat = glm::axisAngleMatrix( a_up, (float)-iDeltaX / 150.0f );
			vRight = mMat * vRight;
			vUp = mMat * vUp;
			vForward = mMat * vForward;
		}

		a_transform[0] = vRight;
		a_transform[1] = vUp;
		a_transform[2] = vForward;
	}
	else
	{
		sbMouseButtonDown = false;
	}
}

void Utility::TestShaderStatus(const unsigned int& a_uiShaderId, unsigned int a_uiStatus, int& a_iSuccessFlag, const char* a_shaderType)
{
	if (a_uiStatus == GL_LINK_STATUS)
	{
		glGetProgramiv(a_uiShaderId, a_uiStatus, &a_iSuccessFlag);
	}
	else
	{
		glGetShaderiv(a_uiShaderId, a_uiStatus, &a_iSuccessFlag);
	}

	if (a_iSuccessFlag == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetShaderiv(a_uiShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *log = (GLchar *)malloc(infoLogLength);
		glGetProgramInfoLog(a_uiShaderId, infoLogLength, &infoLogLength, log);
		std::ostringstream ssError;
		if (a_uiStatus == GL_LINK_STATUS)
		{
			ssError << "Error: Failed to  link shader!\n" << log;
		}
		else
		{
			ssError << "Error: Failed to compile " << a_shaderType << "shader!\n" << log;
		}
		Error::RuntimeError(ssError.str());
		free(log);
	}
}

unsigned int& Utility::createTexture(int aLevel, int aInternalFormat,
									 int aWidth, int aHeight,
									 int aBoarder, GLenum aFormat,
									 GLenum aType, const void* aPixels)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, aLevel, aInternalFormat, aWidth, aHeight, aBoarder, aFormat, aType, aPixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

void Utility::loadTextureFromFile(const char* aFileName, unsigned int& aTexture)
{
	int width = 0, height = 0, channels = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imageData = stbi_load(aFileName, &width, &height, &channels, 0);

	if (imageData != nullptr)
	{
		aTexture = 0;
		glGenTextures(1, &aTexture); //Generate a DEBUG_NEW texture

		glBindTexture(GL_TEXTURE_2D, aTexture); //Bind a texture 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Set a texture parameter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); //Set a texture parameter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Set a texture parameter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //Set a texture parameter

		if (channels == 1)//if the texture has one channel
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R, width, height, 0, GL_R, GL_UNSIGNED_BYTE, imageData);
		}
		else if (channels == 2)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, imageData);
		}
		else if (channels == 3)//If the texture has three channels
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		}

		glGenerateMipmap(GL_TEXTURE_2D);//Generate mipmaps
		glBindTexture(GL_TEXTURE_2D, 0);//Bind no texture

		stbi_image_free(imageData);
		Application_Log* log = Application_Log::Get();
		if (log != nullptr)
		{
			log->addLog(LOG_LEVEL::LOG_ERROR, "Successfully loaded texture: %s", aFileName);
		}
	}
	else
	{
		Application_Log* log = Application_Log::Get();
		if (log != nullptr)
		{
			log->addLog(LOG_LEVEL::LOG_ERROR, "Failed to load texture: %s", aFileName);
		}
	}
}
