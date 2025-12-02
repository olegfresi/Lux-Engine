#version 410 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 lightSpaceMatrix;
uniform bool useInstancing;

void main()
{
    bool instanced = useInstancing;
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}