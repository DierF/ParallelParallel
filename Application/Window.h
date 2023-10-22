#pragma once

#include <cassert>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace PParallel
{
	class Window
	{
	public:
		static int const s_width = 1600;
		static int const s_height = 900;

	public:
		Window() = default;

		~Window()
		{
			glfwTerminate();
		}

		void init()
		{
			if (not glfwInit())
			{
				throw std::runtime_error("glfwInit failed");
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			m_window = glfwCreateWindow(s_width, s_height, "ParallelParallel", nullptr, nullptr);
			assert(m_window);

			glfwMakeContextCurrent(m_window);
		}

		void update()
		{
			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}

		bool shouldClose()
		{
			return glfwWindowShouldClose(m_window);
		}

		GLFWwindow* get()
		{
			return m_window;
		}

	private:
		GLFWwindow* m_window;
	};
}