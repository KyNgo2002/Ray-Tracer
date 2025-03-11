#version 460

out vec4 FragColor;

uniform float Radius;
uniform vec2 Resolution;

void main() {
	vec3 rayOrigin = vec3(0.0f, 0.0f, 1.0f);
	vec3 rayDirection = normalize(vec3(gl_FragCoord.x / Resolution.x * 2.0f - 1.0f, 
		gl_FragCoord.y / Resolution.y * 2.0f - 1.0f, -1.0f));
	
	vec3 lightDirection = normalize(vec3(-1.0f));

	float a = dot(rayDirection, rayDirection);
	float b = 2.0f * dot(rayOrigin, rayDirection);
	float c = dot(rayOrigin, rayOrigin) - Radius * Radius;

	float discriminant = b * b - 4.0f * a * c;

	float farthestT = (-b + sqrt(discriminant)) / (2.0f * a);
	float closestT = (-b - sqrt(discriminant)) / (2.0f * a);

	vec3 hitpoint = (rayOrigin + rayDirection * closestT);

	vec3 normal = normalize(hitpoint); 

	float d = max(dot(normal, -lightDirection), 0.0f);

	vec3 reflectDirection = reflect(lightDirection, normal);

	vec3 diffuse = d * vec3(1.0f, 0.0f, 1.0f);
	vec3 specular = pow(max(dot(reflectDirection, rayOrigin), 0.0f), 32) * vec3(1.0f, 1.0f, 1.0f);
	
	FragColor = discriminant >= 0.0f ? FragColor = vec4(diffuse + specular, 1.0f) : 
		FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
