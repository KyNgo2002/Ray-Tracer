#version 460

out vec4 FragColor;

uniform float radius;
uniform vec2 Resolution;

void main() {
	vec3 rayOrigin = vec3(0.0f, 0.0f, 0.0f);
	vec3 rayDirection = vec3(gl_FragCoord.x / Resolution.x, gl_FragCoord.y / Resolution.y, -1.0f);
	
	float a = dot(rayDirection, rayDirection);
	float b = 2.0f * dot(rayOrigin, rayDirection);
	float c = dot(rayOrigin, rayOrigin) - radius * radius;


	FragColor = vec4(1.0f);
}
