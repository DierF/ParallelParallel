#pragma once

#include <glm/glm.hpp>

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

            float deltaYaw   = 0.0f;
            float deltaPitch = 0.0f;
            float angle      = deltaTime * s_rotateSpeed;
            if (Input::IsMousePressed(PP_MOUSE_BUTTON_LEFT))
            {
                glm::vec2 pos = Input::GetMousePosition();
                if (m_rotating)
                {
                    deltaYaw   = (pos.x - m_lastPos.x) * angle;
                    deltaPitch = (pos.y - m_lastPos.y) * angle;
                }
                m_rotating = true;
                m_lastPos = pos;
            }
            else
            {
                m_rotating = false;
            }

            m_camera.update(deltaPosition, deltaYaw, deltaPitch);
        }

        void render(Shader& shader)
        {
            m_camera.render(shader);
        }

	private:
        glm::vec2 m_lastPos = glm::vec2(0.0f);
        bool m_rotating = false;
		Camera m_camera;
	};
}