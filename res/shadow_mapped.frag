#version 330 core

in vec4 shadowCoord;

out vec4 color;

uniform sampler2D textureShadow;
uniform vec4 colorize;

void main() {
    float illum = texture(textureShadow, shadowCoord.xy / shadowCoord.w).z < (shadowCoord.z / shadowCoord.w) - 0.0001 ? 0.2 : 1.0;

    color = vec4(vec3(illum), 1.0);
    //color = vec4(1.0) * texture(textureShadow, shadowCoord.xy).z;
    //color = vec4(vec3(1.0) * (shadowCoord.z * 0.5 + 0.5), 1.0);
    //color = vec4(vec3(1.0) * shadowCoord.z, 1.0);
    //color = vec4(1.0) * gl_FragCoord.z;
    //color = vec4(vec3(1.0) * (gl_FragCoord.z * 0.5 + 0.5), 1.0);
}
