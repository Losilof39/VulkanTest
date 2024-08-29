#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(push_constant) uniform pc {
   float scale;
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec2 inLightmapCoord;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out vec2 fragLmCoord;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition.x /scale, inPosition.y / scale, inPosition.z / scale, 1.0);
    fragTexCoord = inTexCoord;
    fragLmCoord = inLightmapCoord;
}