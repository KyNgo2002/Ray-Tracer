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
	vec3 padding;
	int materialInd;
};

struct Triangle {
	vec4 x;
    vec4 y;
    vec4 z;
    vec3 normal;
    int materialInd;
};

struct Material {
	vec3 color;
	float roughness;
	vec3 emissionColor;
	float emissionPower;
};

layout(std430, binding = 0) buffer SphereBuffer {
    Sphere spheres[];
};

layout(std430, binding = 1) buffer TriangleBuffer {
    Triangle triangles[];
};

layout(std430, binding = 2) buffer MaterialBuffer {
    Material materials[];
};

in vec2 TexCoord;

out vec4 FragColor;

uniform int NumSpheres;
uniform int NumTriangles;

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
// https://www.shadertoy.com/view/MlGcDz
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

	vec3 triangleNormal;

	// Ray-Sphere intersection
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

	// Ray-Triangle Intersection;
	for (int i = 0; i < NumTriangles; ++i) {
		// Mollere Trumbore Triangle Intersection
		// https://stackoverflow.com/a/42752998
		Triangle triangle = triangles[i];
		vec3 v1v0 = triangle.y.xyz - triangle.x.xyz;
		vec3 v2v0 = triangle.z.xyz - triangle.x.xyz;
		vec3 rov0 =  ray.origin - triangle.x.xyz;
		vec3 normal = cross(v1v0, v2v0);
		vec3 q = cross(rov0, ray.direction);
		float d = 1.0 / dot(ray.direction, normal);
		float u = d * dot(-q, v2v0);
		float v = d * dot(q, v1v0);
		float t = d * dot(-normal, rov0);

		if(u < 0.0f || v < 0.0f || (u + v) > 1.0f) t = -1.0;
		if (t > 0.0f && t < hitDistance) {
			objType = 1;
			closestInd = i;
			hitDistance = t;
		}

	}
	
	if (closestInd < 0) {
		payload.hitDistance = -1.0f;
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
	else {
		payload.worldPos = ray.origin + ray.direction * hitDistance;
		payload.worldNorm = triangles[closestInd].normal.xyz;
	}
	
	return payload;
}

void main() {
	// Random number generation
	vec2 uv = (gl_FragCoord.xy / Resolution) * 2.0f - 1.0f;
	vec2 uvHash = hash2d(uv);
	uint32_t seed = (uint32_t)(uvHash.x * uvHash.y) + (uint32_t)gl_FragCoord.x * 
		(uint32_t)Resolution.y + (uint32_t)gl_FragCoord.y * Time;

	// Lighting variables
	vec3 light = vec3(0.0f);
	vec3 rayColor = vec3(1.0f);
	vec3 diffuseDir = vec3(0.0f);
	vec3 specularDir = vec3(0.0f);
	vec3 emittedLight = vec3(0.0f);

	// Ray Creation
	Ray ray;
	ray.origin = CamPosition;
	ray.direction = normalize(CamDirection + uv.x * CamRight + uv.y * CamUp);
	
	for (int i = 0; i <= Bounces; ++i) {
		seed += Time;
		HitPayload payload = TraceRay(ray);
		
		if (payload.hitDistance < 0.0f) {
			light += texture(Skybox, ray.direction).rgb * rayColor;
			break;
		}
		else {
			Material material;
			vec3 specularDir = reflect(ray.direction, payload.worldNorm);
			ray.origin = payload.worldPos + payload.worldNorm * 0.00001f;
			diffuseDir = normalize(payload.worldNorm + generateRandVec(seed));

			material = payload.objType == 0 ? materials[spheres[payload.objInd].materialInd] : 
				materials[triangles[payload.objInd].materialInd];
			
			ray.direction = mix(specularDir, diffuseDir, material.roughness);
			emittedLight = material.emissionColor * material.emissionPower;
			light += emittedLight * rayColor;
			rayColor *= material.color;
		}
	}
	
	FragColor = texture(AccumulationTexture, TexCoord) + vec4(light, 1.0f);
}
