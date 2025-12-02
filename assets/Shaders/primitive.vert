#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

layout(location = 3) in vec4 i_col0;
layout(location = 4) in vec4 i_col1;
layout(location = 5) in vec4 i_col2;
layout(location = 6) in vec4 i_col3;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool useInstancing;

void main()
{
    mat4 m;
    if(useInstancing)
    {
        m = model;
    }
    gl_Position = projection * view * m * vec4(aPos, 1.0);
}