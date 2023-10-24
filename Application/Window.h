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
		Window()
		{
			s_instance = this;
		}

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
			glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

			m_window = glfwCreateWindow(s_width, s_height, "ParallelParallel", nullptr, nullptr);
			assert(m_window);

			glfwMakeContextCurrent(m_window);

			glfwSwapInterval(0);
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

		GLFWwindow* nativeWindow()
		{
			return m_window;
		}

	private:
		GLFWwindow* m_window;

	public:
		static Window& get()
		{
			return *s_instance;
		}

	private:
		static int const s_width  = 1600;
		static int const s_height = 900;
		static Window* s_instance;
	};
}