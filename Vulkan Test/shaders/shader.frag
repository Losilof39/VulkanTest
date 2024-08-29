#version 450

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform sampler2D lightmapSampler;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec2 fragLmCoord;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 baseColor = texture(texSampler, fragTexCoord);
    vec4 lightmapColor = texture(lightmapSampler, fragLmCoord);

    outColor = baseColor * lightmapColor;
}