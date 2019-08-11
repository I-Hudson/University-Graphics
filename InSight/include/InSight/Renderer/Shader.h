#pragma once

#include <string>
#include <glm/glm.hpp>

namespace InSight
{
	class Shader
	{
	public:
		Shader(const std::string& aVertexSrc, const std::string& aFragSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void UploadUniformMat4(const std::string& a_name, const glm::mat4& a_matrix);

	private:
		uint32_t mRednererID;
	};
}
