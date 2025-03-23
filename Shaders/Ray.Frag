#version 460
#extension GL_NV_gpu_shader5 : enable

struct Ray {
	vec3 origin;
	vec3 direction;
};

struct HitPayload {
	float hitDistance;
	vec3 worldPos;
	vec3 worldNorm;

	int objInd;
	int objType;
};

struct Sphere {
    vec3 position;
    float radius;  
    vec3 color;
	float roughness;
	vec3 emissionColor;
	float emissionPower;
};

struct Plane {
    vec3 normal;
    float dist;
    vec3 color;
    float roughness;
    vec3 emissionColor;
    float emissionPower;
};

layout(std430, binding = 0) buffer SphereBuffer {
    Sphere spheres[];
};
  
layout(std430, binding = 1) buffer PlaneBuffer {
    Plane planes[];
};

in vec2 TexCoord;

out vec4 FragColor;

uniform int NumSpheres;
uniform int NumPlanes;
uniform vec2 Resolution;
uniform int Bounces;
uniform int Time;

uniform vec3 CamPosition;
uniform vec3 CamDirection;
uniform vec3 CamRight;
uniform vec3 CamUp;

uniform sampler2D AccumulationTexture;
uniform samplerCube Skybox;

// https://www.shadertoy.com/view/lldyDn
vec2 hash2d(vec2 p) {
	vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yzx+19.19);
    return fract((p3.xx+p3.yz)*p3.zy);
}

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
	float x = randomFloatNormalDistribution(seed);
	float y = randomFloatNormalDistribution(seed);
	float z = randomFloatNormalDistribution(seed);
	return normalize(vec3(x, y, z));

}

HitPayload TraceRay(Ray ray) {
	HitPayload payload;
	payload.hitDistance = -1;
	int objType = -1;

	int closestInd = -1;
	float hitDistance = 3.402823466e+38;

	for (int i = 0; i < NumSpheres; ++i) {
		vec3 origin = ray.origin - spheres[i].position;

		float a = dot(ray.direction, ray.direction);
		float b = 2.0f * dot(origin, ray.direction);
		float c = dot(origin, origin) - spheres[i].radius * spheres[i].radius;

		float discriminant = b * b - 4.0f * a * c;

		if (discriminant < 0.0f)
			continue;

		float closestT = (-b - sqrt(discriminant)) / (2.0f * a);
		if (closestT > 0.0f && closestT < hitDistance) {
			objType = 0;
			closestInd = i;
			hitDistance = closestT;
		}
	}

	for (int i = 0; i < NumPlanes; ++i) {
		float denominator = (dot(planes[i].normal, ray.direction));

		// No Intersection ray-plane
		if (denominator == 0) 
			FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);

		float t = -(dot(normalize(planes[i].normal), ray.origin) + planes[i].dist) / 
			denominator;

		if (t > 0.0f && t < hitDistance) {
			objType = 1;
			closestInd = i;
			hitDistance = t;
		}
	}

	// Ray Triangle Intersection;
	for (int i = 0; i < 0; ++i) {
		
	}
 
	if (closestInd < 0) {
		payload.hitDistance = -1.0f;
		payload.objType = -1;
		return payload;
	}

	payload.objInd = closestInd;
	payload.hitDistance = hitDistance;
	payload.objType = objType;

	if (objType == 0) {
		payload.worldPos = (ray.origin - spheres[closestInd].position) + ray.direction * hitDistance;
		payload.worldNorm = normalize(payload.worldPos);
		payload.worldPos += spheres[closestInd].position;
	}
	else if (objType == 1) {
		payload.worldPos = ray.origin + ray.direction * hitDistance;
		payload.worldNorm = planes[payload.objInd].normal;
	}
	return payload;
}

void main() {
	// Random number generation
	vec2 uv = (gl_FragCoord.xy / Resolution) * 2.0f - 1.0f;
	vec2 uvHash = hash2d(uv);
	uint32_t seed = (uint32_t)(uvHash.x * uvHash.y) + (uint32_t)gl_FragCoord.x * 
		(uint32_t)Resolution.y + (uint32_t)gl_FragCoord.y + Time;

	// Lighting variables
	vec3 light = vec3(0.0f);
	vec3 contribution = vec3(1.0f);
	vec3 diffuseDir = vec3(0.0f);
	vec3 specularDir = vec3(0.0f);
	vec3 emittedLight = vec3(0.0f);

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
			vec3 specularDir = reflect(ray.direction, payload.worldNorm);
			ray.origin = payload.worldPos + payload.worldNorm * 0.00001f;
			if (payload.objType == 0) {
				diffuseDir = normalize(payload.worldNorm + spheres[payload.objInd].roughness * generateRandVec(seed));
				emittedLight = spheres[payload.objInd].emissionColor * spheres[payload.objInd].emissionPower;
				ray.direction = mix(specularDir, diffuseDir, spheres[payload.objInd].roughness);
			}
			else if (payload.objType == 1) {
				diffuseDir = normalize(payload.worldNorm + planes[payload.objInd].roughness * generateRandVec(seed));
				emittedLight = planes[payload.objInd].emissionColor * planes[payload.objInd].emissionPower;
				ray.direction = mix(specularDir, diffuseDir, planes[payload.objInd].roughness);
			}

			light += emittedLight * contribution;
			contribution *= payload.objType == 0 ? spheres[payload.objInd].color : planes[payload.objInd].color;
		}
	}
	
	FragColor = texture(AccumulationTexture, TexCoord) + vec4(light, 1.0f);
}
