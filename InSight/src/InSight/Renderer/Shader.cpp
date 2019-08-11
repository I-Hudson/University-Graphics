#include "InSight/Renderer/Shader.h"
#include "Log.h"
#include<vector>

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>



namespace InSight
{
	Shader::Shader(const std::string & aVertexSrc, const std::string & aFragSrc)
	{
		// Create an empty vertex BaseShader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex BaseShader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar *)aVertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex BaseShader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the BaseShader anymore.
			glDeleteShader(vertexShader);

			EN_CORE_ERROR("{0}", infoLog.data());
			EN_CORE_ASSERT(false, "Vertex BaseShader compilation failure!");
			return;
		}

		// Create an empty fragment BaseShader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment BaseShader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = aFragSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment BaseShader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the BaseShader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			EN_CORE_ERROR("{0}", infoLog.data());
			EN_CORE_ASSERT(false, "Fragment BaseShader compilation failure!");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		mRednererID = glCreateProgram();
		GLuint program = mRednererID;

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			EN_CORE_ERROR("{0}", infoLog.data());
			EN_CORE_ASSERT(false, "BaseShader link failure!");

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(mRednererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(mRednererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::UploadUniformMat4(const std::string& a_name, const glm::mat4 & a_matrix)
	{
		unsigned int location = glGetUniformLocation(mRednererID, a_name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(a_matrix));
	}
}