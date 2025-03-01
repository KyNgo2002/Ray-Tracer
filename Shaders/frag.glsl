#version 460 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 ObjectColor;
uniform vec3 LightColor;
uniform vec3 LightPosition;

uniform vec3 ViewPos;

void main() {
    
    // Ambient Lighting
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * LightColor * ObjectColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(LightPosition - FragPos);

    float diff = max(dot(lightDirection, norm), 0.0f);
    vec3 diffuse = diff * LightColor;

    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * LightColor;

    FragColor = vec4((ambient + diffuse + specular) * ObjectColor, 1.0f);

}