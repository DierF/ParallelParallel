#version 450 core

in vec4 FragColor;

out vec4 FragColorOut;

void main()
{
    // Calculate the distance from the center of the particle
    //float dist = length(gl_PointCoord - vec2(0.5, 0.5));

    // Calculate brightness based on distance
    //float brightness = 1.0 - smoothstep(0.0, 0.5, dist);

    //FragColorOut = FragColor * brightness;

    FragColorOut = FragColor;
}
