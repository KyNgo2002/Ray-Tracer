#version 460 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 ViewPos;

uniform Material material;

#define NUM_POINT_LIGHTS 4
uniform DirectionalLight dirLight;
uniform PointLight pointLights[NUM_POINT_LIGHTS];

vec3 calcDirectionalLights(DirectionalLight light, vec3 normal, vec3 viewDir) {

    vec3 lightDirection = normalize(-light.direction);
    // Diffuse lighting
    float diff = max(dot(lightDirection, normal), 0.0f);
    // Specular lighting
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    return ambient + diffuse + specular;
}

vec3 calculatePointLights(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));

    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return ambient + diffuse + specular;
}

void main() {

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(ViewPos - FragPos);

    vec3 result = calcDirectionalLights(dirLight, norm, viewDir);

    for (int i = 0; i < NUM_POINT_LIGHTS; ++i)
        result += calculatePointLights(pointLights[i], norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0f);

}