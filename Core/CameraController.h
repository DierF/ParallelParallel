#pragma once

#include <numbers>

#include <glm/glm.hpp>

#include "Application/Window.h"
#include "Application/Input.h"
#include "Application/KeyCode.h"
#include "Application/Time.h"
#include "Core/Camera.h"
#include "Render/Shader.h"

namespace PParallel
{
    static float s_moveSpeed = 0.01f;
    static float s_rotateSpeed = 0.01f;

	class CameraController
	{
    public:
        CameraController() = default;

        ~CameraController() = default;

        void update(float deltaTime)
        {
            if (Window::get().isOKeyPressed())
            {
                m_surrounding = not m_surrounding;
                m_rotating = false;
            }
            if (m_surrounding)
            {
                m_surroundAngle += deltaTime * std::numbers::pi_v<float> * 0.00003f;
                if (   Input::IsKeyPressed(PP_KEY_W)
                    or Input::IsKeyPressed(PP_KEY_A)
                    or Input::IsKeyPressed(PP_KEY_S)
                    or Input::IsKeyPressed(PP_KEY_D))
                {
                    m_surrounding = false;
                }
            }
            glm::vec3 deltaPosition = getDeltaPosition(deltaTime);
            auto [deltaYaw, deltaPitch] = getDeltaYawPitch(deltaTime);
            m_camera.update(deltaPosition, deltaYaw, deltaPitch);
            updateViewport(Window::get().getWidth(), Window::get().getHeight());
        }

        glm::vec3 getDeltaPosition(float deltaTime)
        {
            if (Input::IsKeyPressed(PP_KEY_LEFT_SHIFT) || Input::IsKeyPressed(PP_KEY_RIGHT_SHIFT))
            {
                deltaTime *= 10.0f;
            }
            if (m_surrounding)
            {
                glm::vec3 newPosition(500.0f * std::cos(m_surroundAngle), 100.0f, 500.0f * std::sin(m_surroundAngle));
                return newPosition - m_camera.getPosition();
            }
            else
            {
                glm::vec3 deltaPosition(0.0f);
                float distance = deltaTime * s_moveSpeed;
                if (Input::IsKeyPressed(PP_KEY_W))
                {
                    deltaPosition += m_camera.getFront() * distance;
                }
                if (Input::IsKeyPressed(PP_KEY_A))
                {
                    deltaPosition += -m_camera.getRight() * distance;
                }
                if (Input::IsKeyPressed(PP_KEY_S))
                {
                    deltaPosition += -m_camera.getFront() * distance;
                }
                if (Input::IsKeyPressed(PP_KEY_D))
                {
                    deltaPosition += m_camera.getRight() * distance;
                }
                return deltaPosition;
            }
        }

        std::pair<float, float> getDeltaYawPitch(float deltaTime)
        {
            float deltaYaw = 0.0f;
            float deltaPitch = 0.0f;
            if (m_surrounding)
            {
                deltaYaw = glm::degrees(std::numbers::pi_v<float> / 2.0f - m_surroundAngle) - m_camera.getYaw();
                deltaPitch = -m_camera.getPitch();
            }
            else
            {
                float angle = deltaTime * s_rotateSpeed;
                if (Input::IsMousePressed(PP_MOUSE_BUTTON_LEFT))
                {
                    glm::vec2 pos = Input::GetMousePosition();
                    if (m_rotating)
                    {
                        deltaYaw = (pos.x - m_lastPos.x) * angle;
                        deltaPitch = (pos.y - m_lastPos.y) * angle;
                    }
                    m_rotating = true;
                    m_lastPos = pos;
                }
                else
                {
                    m_rotating = false;
                }
            }
            return { deltaYaw, deltaPitch };
        }

        void updateViewport(int width, int height)
        {
            m_camera.resetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
        }

        void render(Shader& shader)
        {
            m_camera.render(shader);
        }

	private:
        glm::vec2 m_lastPos = glm::vec2(0.0f);
        bool m_rotating = false;
        float m_surroundAngle = 0.0f; // in radian
        bool m_surrounding = true;
		Camera m_camera;
	};
}