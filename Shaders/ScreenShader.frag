#version 460

in vec2 TexCoord;

out vec4 FragColor;

uniform bool DoBloom;
uniform float Frames;
uniform sampler2D ScreenTexture;

// Shader for rendering final result to the screen.
void main() {
	if (DoBloom)
		FragColor = texture(ScreenTexture, TexCoord);
	else {
		vec3 result = texture(ScreenTexture, TexCoord).rgb / float(Frames);
		vec3 gammaCorrection = pow(result, vec3(1.0f / 2.2f));
		FragColor = vec4(gammaCorrection, 1.0f);
	}
}