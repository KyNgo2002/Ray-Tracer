#version 460

in vec2 TexCoord;

out vec4 FragColor;

uniform uint Frames;
uniform sampler2D AccumulationTexture;
uniform sampler2D BlurTexture;

// Shader that applies bloom effect to original texture.
void main() {
	vec3 result = texture(AccumulationTexture, TexCoord).rgb / float(Frames) + texture(BlurTexture, TexCoord).rgb;
	vec3 gammaCorrection = pow(result, vec3(1.0f / 1.4f));
	FragColor = vec4(gammaCorrection, 1.0f);
}