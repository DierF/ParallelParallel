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

			if (not glfwInit())
			{
				throw std::runtime_error("glfwInit failed");
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

			m_window = glfwCreateWindow(m_width, m_height, "ParallelParallel", nullptr, nullptr);
			assert(m_window);

			glfwMakeContextCurrent(m_window);

			glfwSetKeyCallback(m_window, keyCallback);
			glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

			glfwSwapInterval(1);

			if (not gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				throw std::runtime_error("failed to initialize GLAD");
			}

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_PROGRAM_POINT_SIZE);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		~Window()
		{
			glfwTerminate();
		}

		void update()
		{
			if (m_escapeKeyPressed)
			{
				glfwSetWindowShouldClose(m_window, GLFW_TRUE);
			}
			glfwSwapBuffers(m_window);
			glfwPollEvents();
		}

		bool isSpaceKeyPressed()
		{
			if (m_spaceKeyPressed)
			{
				m_spaceKeyPressed = false;
				return true;
			}
			return false;
		}

		bool isJKeyPressed()
		{
			if (m_jKeyPressed)
			{
				m_jKeyPressed = false;
				return true;
			}
			return false;
		}

		bool isKKeyPressed()
		{
			if (m_kKeyPressed)
			{
				m_kKeyPressed = false;
				return true;
			}
			return false;
		}

		bool isOKeyPressed()
		{
			if (m_oKeyPressed)
			{
				m_oKeyPressed = false;
				return true;
			}
			return false;
		}

		bool shouldClose()
		{
			return glfwWindowShouldClose(m_window);
		}

		GLFWwindow* nativeWindow()
		{
			return m_window;
		}

		int getWidth() const
		{
			return m_width;
		}

		int getHeight() const
		{
			return m_height;
		}

	private:
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (action == GLFW_PRESS)
			{
				if (key == GLFW_KEY_SPACE)
				{
					s_instance->m_spaceKeyPressed = true;
				}
				else if (key == GLFW_KEY_ESCAPE)
				{
					s_instance->m_escapeKeyPressed = true;
				}
				else if (key == GLFW_KEY_J)
				{
					s_instance->m_jKeyPressed = true;
				}
				else if (key == GLFW_KEY_K)
				{
					s_instance->m_kKeyPressed = true;
				}
				else if (key == GLFW_KEY_O)
				{
					s_instance->m_oKeyPressed = true;
				}
			}
		}

		static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
			s_instance->m_width  = width;
			s_instance->m_height = height;
		}

	private:
		GLFWwindow* m_window           = nullptr;
		bool        m_spaceKeyPressed  = false;
		bool        m_escapeKeyPressed = false;
		bool        m_kKeyPressed      = false;
		bool        m_jKeyPressed      = false;
		bool        m_oKeyPressed      = false;
		int         m_width            = 1600;
		int         m_height           = 900;

	public:
		static Window& get()
		{
			return *s_instance;
		}

	private:
		static Window* s_instance;
	};
}