#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

layout( push_constant ) uniform constants
{
	mat4 model;
	mat4 view;
	mat4 projection;
} pushConst;

void main() {
    gl_Position = pushConst.projection * pushConst.view  * pushConst.model * vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
}