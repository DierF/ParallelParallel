#pragma once

#include <glad/glad.h>

#include "Render/Shader.h"

namespace PParallel
{
	class Renderer
	{
	public:
		Renderer() = default;

		~Renderer() = default;

		void clearBuffer()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		Shader& getShader()
		{
			return m_shader;
		}

	private:
		Shader m_shader;
	};
}