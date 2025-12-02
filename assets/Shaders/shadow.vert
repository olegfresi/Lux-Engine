#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

layout (location = 3) in vec4 i_col0;
layout (location = 4) in vec4 i_col1;
layout (location = 5) in vec4 i_col2;
layout (location = 6) in vec4 i_col3;


out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;
uniform bool useInstancing;

void main()
{
    mat4 m;
    if (useInstancing)
    {
        m = mat4(i_col0, i_col1, i_col2, i_col3);
    }
    else
    {
        m = model;
    }
    vec4 worldPos = m * vec4(aPos, 1.0);
    vs_out.FragPos = vec3(worldPos);
    vs_out.Normal = mat3(transpose(inverse(m))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace = lightSpaceMatrix * worldPos;
    gl_Position = projection * view * worldPos;
}