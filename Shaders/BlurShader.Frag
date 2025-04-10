#version 460

in vec2 TexCoord;

out vec4 FragColor;

uniform bool Horizantal;
uniform float Weights[5];
uniform sampler2D Texture;
uniform float TextureOffset;

void main() {
	vec2 offset = Horizantal ? vec2(TextureOffset, 0.0f) : vec2(0.0f, TextureOffset);
	vec3 result = texture(Texture, TexCoord).rgb * Weights[0];

	for (int i = 1; i < 5; ++i) {
		result += texture(Texture, TexCoord + (i * offset)).rgb * Weights[i];
		result += texture(Texture, TexCoord - (i * offset)).rgb * Weights[i];
	}

	FragColor = vec4(result, 1.0f);
}