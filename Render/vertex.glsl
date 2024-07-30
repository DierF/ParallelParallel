#version 450 core

layout (location = 0) in vec4  a_color;
layout (location = 1) in vec3  a_position;
layout (location = 2) in float a_lifetime;
layout (location = 3) in float a_size;

uniform mat4 u_viewProjection;
uniform vec3 u_cameraPosition;

out vec4 FragColor;

void main()
{
    gl_Position = u_viewProjection * vec4(a_position, 1.0);

    //float dist = distance(u_cameraPosition, a_position) + 1.0;

    // less lifetime -> smaller size
    //gl_PointSize = min(a_size * a_lifetime * 100.0, 50.0);
    gl_PointSize = a_size;

    FragColor = a_color;
}
