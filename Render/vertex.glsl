#version 450 core

layout (location = 0) in vec4  a_color;
layout (location = 1) in vec3  a_position;
layout (location = 2) in float a_size;

uniform mat4 u_viewProjection;

out vec4 FragColor;

void main()
{
    gl_Position = u_viewProjection * vec4(a_position, 1.0);

    gl_PointSize = a_size;

    FragColor = a_color;
}
