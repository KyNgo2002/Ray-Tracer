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

uniform int NumSpheres;
uniform vec2 Resolution;
uniform int Bounces;

uniform vec3 SpherePositions[2];
uniform vec3 SphereColors[2];
uniform float SphereRadii[2];

uniform vec3 CamPosition;
uniform vec3 CamDirection;
uniform vec3 CamRight;
uniform vec3 CamUp;

out vec4 FragColor;

HitPayload TraceRay(inout Ray ray) {
	HitPayload payload;
	payload.hitDistance = -1;

	int closestInd = -1;
	float hitDistance = 3.402823466e+38;

	for (int i = 0; i < NumSpheres; ++i) {
		vec3 origin = ray.origin - SpherePositions[i];

		float a = dot(ray.direction, ray.direction);
		float b = 2.0f * dot(origin, ray.direction);
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

	if (closestInd < 0) {
		payload.hitDistance = -1.0f;
		return payload;
	}
	else {
		payload.objInd = closestInd;
		payload.hitDistance = hitDistance;
		vec3 origin = ray.origin - SpherePositions[closestInd];
		payload.worldPos = origin + ray.direction * hitDistance;
		payload.worldNorm = normalize(payload.worldPos);
		payload.worldPos += SpherePositions[closestInd];
	}

	return payload;
}

void main() {
	vec2 uv = gl_FragCoord.xy / Resolution * 2.0f - 1.0f;
	//vec3 rayDirection = normalize(CamDirection + uv.x * CamRight + uv.y * CamUp);
	vec3 color = vec3(0.0f);
	float multiplier = 1.0f;

	// Ray Creation
	Ray ray;
	ray.origin = CamPosition;
	ray.direction = normalize(CamDirection + uv.x * CamRight + uv.y * CamUp);
	
	for (int i = 0; i < Bounces; ++i) {
		HitPayload payload = TraceRay(ray);

		if (payload.hitDistance == -1) {
			
			// color += vec3(0.6f, 0.7f, 0.9f) * multiplier;
			break;
		}
		// Add Color contribution
		vec3 lightDirection = normalize(vec3(-1.0f));
		float lightIntensity = max(dot(payload.worldNorm, -lightDirection), 0.0f);

		color += SphereColors[payload.objInd] * lightIntensity * multiplier;
		// Set new ray parameters
		ray.origin = payload.worldPos + payload.worldNorm * 0.0001f;
		ray.direction = reflect(ray.direction, payload.worldNorm);
		// Reduce multiplier
		multiplier *= 1.0;
	}

	//vec3 rayOrigin = CamPosition - SpherePositions[closestInd];
	//vec3 hitPoint = rayOrigin + rayDirection * hitDistance;
	//vec3 lightDirection = normalize(vec3(-1.0f));

	//vec3 normal = normalize(hitPoint); 

	//float d = max(dot(normal, -lightDirection), 0.0f);

	//vec3 reflectDirection = reflect(lightDirection, normal);

	//vec3 diffuse = d * SphereColors[closestInd];
	//vec3 specular = pow(max(dot(reflectDirection, rayOrigin), 0.0f), 1) * vec3(1.0f, 1.0f, 1.0f);
	
	FragColor = vec4(color , 1.0f);
}
