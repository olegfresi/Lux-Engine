#version 410 core

#ifdef LUX_FORCE_EXPLICIT_LAYOUT
#    ifdef GL_ARB_explicit_uniform_location
#        extension GL_ARB_explicit_uniform_location : enable
#    endif
#    define LAYOUT_LOC(index) layout(location = index)
#else
#    define LAYOUT_LOC(index)
#endif

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

out vec2 TexCoords;

LAYOUT_LOC(0) uniform mat4 model;
LAYOUT_LOC(4) uniform mat4 view;
LAYOUT_LOC(8) uniform mat4 projection;

void main()
{
    TexCoords = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}


/*
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 outPosition;
layout(location = 1) out vec3 outNormal;

void main() {
    gl_Position = vec4(inPosition, 1.0);
    outPosition = inPosition;
    outNormal = inNormal;
}
*/

/*
layout(std140) uniform CameraUBO {
    mat4 viewProj;
    vec3 cameraPos;
};

layout(std140) uniform LightUBO {
    vec3 lightPos;
    vec3 lightColor;
};

layout(std140) uniform MaterialUBO {
    vec3 albedo;
    float shininess;
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec4 fragColor;

void main() {
    vec3 N = normalize(inNormal);
    vec3 L = normalize(lightPos - inPosition);
    vec3 V = normalize(cameraPos - inPosition);
    vec3 H = normalize(L + V);

    float NdotL = max(dot(N, L), 0.0);
    float NdotH = max(dot(N, H), 0.0);

    vec3 diffuse = albedo * NdotL;
    vec3 specular = vec3(1.0) * pow(NdotH, shininess);

    fragColor = vec4(diffuse + specular, 1.0);
}
*/