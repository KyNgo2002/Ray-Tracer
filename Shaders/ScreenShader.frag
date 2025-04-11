#version 460

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ScreenTexture;

// Shader for rendering final result to the screen.
void main() {
	FragColor = texture(ScreenTexture, TexCoord);
}