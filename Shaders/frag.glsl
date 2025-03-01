#version 460 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 LightPosition;

void main() {
    
    // Ambient Lighting
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor * objectColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(LightPosition - FragPos);

    float diff = max(dot(lightDirection, norm), 0.0f);
    vec3 diffuse = diff * lightColor;

    FragColor = vec4((ambient + diffuse) * objectColor, 1.0f);

}