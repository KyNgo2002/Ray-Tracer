#version 460
#extension GL_NV_gpu_shader5 : enable

in vec2 TexCoord;

out vec4 FragColor;

uniform uint Frames;
uniform sampler2D AccumulationTexture;

void main() {
	vec4 color = texture(AccumulationTexture, TexCoord) / float(Frames);
	float brightness = (color.r * 0.2126) + (color.g * 0.7152) + (color.b * 0.0722);

	FragColor = color * brightness * brightness;
}