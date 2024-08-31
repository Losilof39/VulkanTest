#version 450

layout(set = 0, binding = 1) uniform sampler2D texSampler;
layout(set = 1, binding = 0) uniform sampler2D lightmapSampler;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec2 fragLmCoord;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 baseColor = texture(texSampler, fragTexCoord);
    vec4 lightmapColor = texture(lightmapSampler, fragLmCoord);

    outColor = baseColor * lightmapColor;
}