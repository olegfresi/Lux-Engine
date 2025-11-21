#version 410 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexCoord;
layout (location=2) in vec3 aNormal;

/*
out Vertex
{
    vec3 Position;
    vec3 Normal;
} vertex;


mat4 model = mat4(1.0);
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vertex.Normal = mat3(model) * aNormal;
    vertex.Position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
*/


out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoord;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = projection * view * model * vec4(FragPos, 1.0);
}