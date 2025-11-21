
            #version 330 core
            in vec3 Normal;
            in vec2 TexCoords;
            
            out vec4 FragColor;
            
            uniform sampler2D texture1;
            uniform vec3 lightPos;
            uniform vec3 viewPos;
            
            void main() {
                vec3 norm = normalize(Normal);
                vec3 lightDir = normalize(lightPos - FragPos);
                float diff = max(dot(norm, lightDir), 0.0);
                vec3 diffuse = diff * vec3(1.0);
                vec4 texColor = texture(texture1, TexCoords);
                FragColor = vec4(diffuse, 1.0) * texColor;
            }
        