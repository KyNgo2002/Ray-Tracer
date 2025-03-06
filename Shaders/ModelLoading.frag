#version 460 core
//struct Material {
    // sampler2D texture_diffuse1;
    // sampler2D texture_specular1;
    // sampler2D texture_normal;
//};

//uniform Material material;

float near = 0.1; 
float far  = 100.0; 

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

out vec4 FragColor;

float LinearizeDepth(float depth)  {
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main() {   
    float depth = LinearizeDepth(gl_FragCoord.z) / far;
    FragColor = vec4(vec3(depth), 1.0);
    // FragColor = texture(material.texture_diffuse1, TexCoords) + texture(material.texture_specular1, TexCoords) + texture(material.texture_normal, TexCoords);
    //FragColor = texture(texture_diffuse1, TexCoords);
}
