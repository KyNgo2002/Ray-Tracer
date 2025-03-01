#version 460 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 ObjectColor;

uniform vec3 ViewPos;

uniform Material material;
uniform Light light;

void main() {
    
    // Ambient Lighting
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(light.position - FragPos);

    float diff = max(dot(lightDirection, norm), 0.0f);
    vec3 diffuse = diff * material.diffuse * light.diffuse;

    // Specular lighting
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = material.specular * spec * light.specular;

    FragColor = vec4((ambient + diffuse + specular) * ObjectColor, 1.0f);

}