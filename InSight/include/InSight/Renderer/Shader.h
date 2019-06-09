#pragma once

#include <string>

namespace InSight
{
	class Shader
	{
	public:
		Shader(const std::string& aVertexSrc, const std::string& aFragSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

	private:
		uint32_t mRednererID;
	};
}
