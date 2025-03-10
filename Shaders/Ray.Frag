#version 460

out vec4 FragColor;

uniform float Radius;
uniform vec2 Resolution;

void main() {
	vec3 rayOrigin = vec3(0.0f, 0.0f, 2.0f);
	vec3 rayDirection = (vec3(gl_FragCoord.x / Resolution.x * 2.0f - 1.0f, gl_FragCoord.y / Resolution.y * 2.0f - 1.0f, -1.0f));
	
	if (rayDirection.y <= 0) 
		FragColor = vec4(1.0f);
	else
		FragColor = vec4(0.0f);

	float a = dot(rayDirection, rayDirection);
	float b = 2.0f * dot(rayOrigin, rayDirection);
	float c = dot(rayOrigin, rayOrigin) - Radius * Radius;

	float discriminant = b * b - 4.0f * a * c;

	if (discriminant >= 0) 
		FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
	else if (discriminant < 0) 
		FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);

	// FragColor = vec4(1.0f);
}
