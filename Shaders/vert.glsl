#version 460 core

layout(location = 0) in vec3 aPos;   
layout(location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;

void main() {
	gl_Position = model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	ourColor = aColor;
}
