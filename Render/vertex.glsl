#version 450 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_translation;
layout (location = 2) in vec3 a_color;

uniform mat4 u_viewProjection;

out vec3 FragColor;

mat4 translate(mat4 m, vec3 v)
{
    mat4 result = m;
    result[3] = m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3];
    return result;
}

void main()
{
    mat4 model = translate(mat4(1.0), vec3(0.0, -1.0, 0.0));

    gl_Position = u_viewProjection * model * vec4(a_position, 1.0);

    FragColor = a_color;
}
