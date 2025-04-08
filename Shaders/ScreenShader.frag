#version 460

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D BrightnessTexture;

void main() {
	FragColor = texture(BrightnessTexture, TexCoord);
}