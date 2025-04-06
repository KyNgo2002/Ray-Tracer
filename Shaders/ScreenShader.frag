#version 460
#extension GL_NV_gpu_shader5 : enable

in vec2 TexCoord;

out vec4 FragColor;

uniform uint Frames;
uniform float BrightnessThreshold;
uniform sampler2D ScreenTexture;

void main() {
	vec4 color = texture(ScreenTexture, TexCoord) / float(Frames);
	float brightness = (color.r * 0.2126) + (color.g * 0.7152) + (color.b * 0.0722);

	FragColor = color * brightness * brightness;
}