#version 460

in vec2 TexCoord;

out vec4 FragColor;

uniform uint Frames;
uniform sampler2D screenTexture;

void main() {
	FragColor = texture(screenTexture, TexCoord) / Frames;
}