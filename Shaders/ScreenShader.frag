#version 460
#extension GL_NV_gpu_shader5 : enable

in vec2 TexCoord;

out vec4 FragColor;

uniform uint Frames;
uniform sampler2D ScreenTexture;

void main() {
	FragColor = texture(ScreenTexture, TexCoord) / float(Frames);
}