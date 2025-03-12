#version 460

struct Ray {
	vec3 origin;
	vec3 direction;
};

struct HitPayload {
	float hitDistance;
	vec3 worldPos;
	vec3 worldNorm;

	int objInd;
};

out vec4 FragColor;

uniform vec2 Resolution;
uniform int Bounces;

uniform vec3 CamPosition;
uniform vec3 CamDirection;
uniform vec3 CamRight;
uniform vec3 CamUp;

uniform vec3 SpherePositions[2];
uniform vec3 SphereColors[2];
uniform float SphereRadii[2];

HitPayload TraceRay(Ray ray) {
	HitPayload payload;
	payload.hitDistance = -1;

	return payload;
}

void main() {
	vec2 uv = gl_FragCoord.xy / Resolution * 2.0f - 1.0f;
	int closestInd = -1;
	float hitDistance = 3.402823466e+38;
	vec3 rayDirection = normalize(CamDirection + uv.x * CamRight + uv.y * CamUp);
	vec3 color = vec3(0.0f);

	// Ray Creation
	Ray ray;
	ray.origin = CamPosition;
	ray.direction = rayDirection;
	
	for (int i = 0; i < Bounces; ++i) {
		HitPayload payload = TraceRay(ray);

		if (payload.hitDistance == -1) {
			color += vec4(0.6f, 0.7f, 0.9f, 1.0f) * 0.2;
			break;
		}
		ray.origin = payload.worldPos + payload.worldNorm * 0.0001f;
		ray.direction = reflect(ray.direction, payload.worldNorm);
		
	}

	for (int i = 0; i < 2; ++i) {
		vec3 origin = CamPosition - SpherePositions[i];

		float a = dot(rayDirection, rayDirection);
		float b = 2.0f * dot(origin, rayDirection);
		float c = dot(origin, origin) - SphereRadii[i] * SphereRadii[i];

		float discriminant = b * b - 4.0f * a * c;

		if (discriminant < 0.0f)
			continue;

		float closestT = (-b - sqrt(discriminant)) / (2.0f * a);
		if (closestT > 0.0f && closestT < hitDistance) {
			closestInd = i;
			hitDistance = closestT;
		}
	}

	if (closestInd == -1) {
		FragColor = vec4(0.6f, 0.7f, 0.9f, 1.0f);
		return;
	}

	vec3 rayOrigin = CamPosition - SpherePositions[closestInd];
	vec3 hitPoint = rayOrigin + rayDirection * hitDistance;
	vec3 lightDirection = normalize(vec3(-1.0f));

	vec3 normal = normalize(hitPoint); 

	float d = max(dot(normal, -lightDirection), 0.0f);

	vec3 reflectDirection = reflect(lightDirection, normal);

	vec3 diffuse = d * SphereColors[closestInd];
	vec3 specular = pow(max(dot(reflectDirection, rayOrigin), 0.0f), 1) * vec3(1.0f, 1.0f, 1.0f);
	
	FragColor = vec4(diffuse , 1.0f);
}
