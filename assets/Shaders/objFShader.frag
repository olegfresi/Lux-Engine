#version 410 core
/*
out vec4 fragColor;

in Vertex
{
    vec3 Position;
    vec3 Normal;
} vertex;

uniform float uTime;

uniform vec3 albedo;
uniform float roughness;
uniform float metallic;

void main()
{
    // Luce orbitante attorno all'oggetto
    float radius = 1.5;
    float speed = 1.0;
    vec3 lightPos = vec3(
        radius * cos(uTime * speed),
        0.5,
        radius * sin(uTime * speed)
    );

    vec3 lightRad = vec3(1.0); // luce bianca
    vec3 viewPos = vec3(0.0, 0.0, 2.0);

    vec3 N = normalize(vertex.Normal);
    vec3 L = normalize(lightPos - vertex.Position);
    vec3 V = normalize(viewPos - vertex.Position);
    vec3 H = normalize(L + V);

    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * lightRad;

    float NDotH = max(dot(N, H), 0.0);
    float roughness2 = roughness * roughness;
    float denom = (NDotH * NDotH) * (roughness2 - 1.0) + 1.0;
    float D = roughness2 / (3.1415 * denom * denom);
    float ks = metallic;
    vec3 specular = lightRad * (ks * D);

    vec3 ambient = 0.1 * albedo;

    vec3 rim = vec3(0.0);
    float rimFactor = 1.0 - max(dot(N, V), 0.0);
    rimFactor = pow(rimFactor, 2.0);
    rim += rimFactor * vec3(0.5, 0.3, 1.0); // viola-azzurro ai bordi

    vec3 color = ambient + albedo * (diffuse + specular) + rim;

    // gamma correction
    fragColor = vec4(pow(color, vec3(1.0 / 2.2)), 1.0);
}

*/

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform sampler2D tex;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 textureColor = texture(tex, TexCoords).rgb;
    vec3 result = (ambient + diffuse + specular) * textureColor;

    FragColor = vec4(result, 1.0);
}
