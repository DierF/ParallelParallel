#version 450 core

layout (location = 0) in vec4  a_color;
layout (location = 1) in vec3  a_position;
layout (location = 2) in float a_size;
layout (location = 3) in float a_lifetime;

uniform mat4 u_viewProjection;
uniform vec3 u_cameraPosition;

out vec4 FragColor;

float decrease(float t)
{
    return t < 2000.0 ? t / 2000.0 : 1.0;
}

void main()
{
    gl_Position = u_viewProjection * vec4(a_position, 1.0);

    float dist = distance(u_cameraPosition, a_position) + 1.0;

    gl_PointSize = a_size / dist * decrease(a_lifetime);

    FragColor = a_color;
}
