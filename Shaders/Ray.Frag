#version 460
#extension GL_NV_gpu_shader5 : enable

// https://www.shadertoy.com/view/lldyDn
vec2 hash2d(vec2 p) {
	vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yzx+19.19);
    return fract((p3.xx+p3.yz)*p3.zy);
}

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

in vec2 TexCoord;

out vec4 FragColor;

uniform int NumSpheres;
uniform vec2 Resolution;
uniform int Bounces;
uniform int Time;

uniform vec3 SpherePositions[4];
uniform vec3 SphereColors[4];
uniform float SphereRadii[4];
uniform float Roughness[4];
uniform vec3 EmissionColor[4];
uniform float EmissionPower[4];

uniform vec3 CamPosition;
uniform vec3 CamDirection;
uniform vec3 CamRight;
uniform vec3 CamUp;

uniform sampler2D AccumulationTexture;
uniform samplerCube Skybox;

float RandomFloat(inout uint32_t seed) {
	seed = seed * 747796405 + 2891336453;
	uint32_t result = ((seed >> (( seed >> (28)) + (4))) ^ seed) * (277803737);
	result = (result >> (22)) ^ result;
	return result / (4294967295.0f);
}

// Sebastian Lague : Coding Adventure Ray Tracing
float randomFloatNormalDistribution(inout uint32_t seed) {
	float theta = 2 * 3.1415926 * RandomFloat(seed);
	float rho = sqrt(-2 * log(RandomFloat(seed)));
	return rho * cos(theta);
}

vec3 generateRandVec(inout uint32_t seed) {
	float x = randomFloatNormalDistribution(seed) * 2.0f - 1.0f;
	float y = randomFloatNormalDistribution(seed) * 2.0f - 1.0f;
	float z = randomFloatNormalDistribution(seed) * 2.0f - 1.0f;
	return normalize(vec3(x, y, z));

}

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
	// Random number generation
	vec2 uv = (gl_FragCoord.xy / Resolution) * 2.0f - 1.0f;
	vec2 uvHash = hash2d(uv);
	uint32_t seed = (uint32_t)gl_FragCoord.x * (uint32_t)Resolution.y + (uint32_t)gl_FragCoord.y + Time;

	// Lighting variables
	vec3 light = vec3(0.0f);
	vec3 contribution = vec3(1.0f);

	// Ray Creation
	Ray ray;
	ray.origin = CamPosition;
	ray.direction = normalize(CamDirection + uv.x * CamRight + uv.y * CamUp);
	
	for (int i = 0; i < Bounces; ++i) {
		seed += Time;
		HitPayload payload = TraceRay(ray);
		
		if (payload.hitDistance < 0.0f) {
			light += texture(Skybox, ray.direction).rgb * contribution;
			break;
		}
		else {
			// Set new ray parameters
			//ray.origin = payload.worldPos + payload.worldNorm * 0.0001f;
			ray.origin = payload.worldPos;
			ray.direction = normalize(payload.worldNorm + Roughness[payload.objInd] * generateRandVec(seed));

			vec3 emittedLight = EmissionColor[payload.objInd] * EmissionPower[payload.objInd];
			light += emittedLight * contribution;
			contribution *= SphereColors[payload.objInd];

			
		}
	}
	
	FragColor = texture(AccumulationTexture, TexCoord) + vec4(light, 1.0f);
}
