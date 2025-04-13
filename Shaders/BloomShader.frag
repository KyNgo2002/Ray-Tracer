#version 460

in vec2 TexCoord;

out vec4 FragColor;

uniform uint Frames;
uniform sampler2D AccumulationTexture;
uniform sampler2D BlurTexture;

// Shader that applies a brightness filter to accumulated texture
void main() {
	vec3 result = texture(AccumulationTexture, TexCoord).rgb / float(Frames) + texture(BlurTexture, TexCoord).rgb;
	vec3 gammaCorrection = pow(result, vec3(1.0f / 2.2f));
	FragColor = vec4(gammaCorrection, 1.0f);
}