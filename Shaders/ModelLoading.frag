#version 460 core
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal;
};

in vec2 TexCoords;

//uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;
//uniform sampler2D texture_normal;

uniform Material material;

out vec4 FragColor;


void main() {    
    FragColor = texture(material.texture_diffuse1, TexCoords) + texture(material.texture_specular1, TexCoords) + texture(material.texture_normal, TexCoords);
}