#version 330 core

layout (location = 0) in vec3 inPosition;

uniform mat4 u_viewProjection;
uniform mat4 u_model;

void main()
{
    vec4 modelSpacePosition = u_model * vec4(inPosition, 1.0);
    gl_Position = u_viewProjection * modelSpacePosition;
}