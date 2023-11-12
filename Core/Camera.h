#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Render/Shader.h"

namespace PParallel
{
    class Camera
    {
    public:
        Camera()
        {
            updateCameraOrientation();
        }

        Camera(glm::vec3 position, float yaw, float pitch)
            : m_position(position), m_yaw(yaw), m_pitch(pitch)
        {
            updateCameraOrientation();
        }

        void update(glm::vec3 const& deltaPosition, float deltaYaw, float deltaPitch)
        {
            m_position += deltaPosition;
            m_yaw      += deltaYaw;
            m_pitch    += deltaPitch;

            updateCameraOrientation();
        }

        void render(Shader& shader)
        {
            shader.updateUniformMat4(shader.getUniformLocation("u_viewProjection"), getViewProjectionMatrix());
        }

        glm::mat4 getViewMatrix()
        {
            return glm::lookAt(m_position, m_position + m_front, m_up);
        }

        glm::mat4 getProjectionMatrix()
        {
            return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
        }

        glm::mat4 getViewProjectionMatrix()
        {
            return getProjectionMatrix() * getViewMatrix();
        }

        glm::vec3 getFront()
        {
            return m_front;
        }

        glm::vec3 getRight()
        {
            return m_right;
        }

        glm::vec3 getUp()
        {
            return m_up;
        }

    private:
        void updateCameraOrientation()
        {
            glm::quat pitchQuat = glm::angleAxis(glm::radians(m_pitch), glm::vec3(1.0f, 0.0f, 0.0f));
            glm::quat yawQuat = glm::angleAxis(glm::radians(m_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
            m_orientation = yawQuat * pitchQuat;
            glm::mat4 rotationMatrix = glm::toMat4(m_orientation);
            m_front = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
            m_right = glm::normalize(glm::cross(m_front, glm::vec3(0.0f, 1.0f, 0.0f)));
            m_up = glm::normalize(glm::cross(m_right, m_front));
        }

    private:
        glm::vec3 m_position    = glm::vec3(0.0f, 5.0f, 0.0f);
        glm::quat m_orientation = glm::quat(0.0f, 0.0f, -1.0f, 0.0f);
        glm::vec3 m_front;
        glm::vec3 m_right;
        glm::vec3 m_up          = glm::vec3(0.0f, 1.0f, 0.0f);
        float m_yaw             = 0.0f;// horizontal
        float m_pitch           = 0.0f;// vertical
        float m_fov             = 45.0f;
        float m_aspectRatio     = 16.0f / 9.0f;
        float m_nearPlane       = 0.1f;
        float m_farPlane        = 1000.0f;
    };
}