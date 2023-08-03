#version 330 core

in vec3 lVertex;
in vec3 dSceneVertex;
in vec3 dSceneLight;
in vec3 nSceneVertex;
in vec4 shadowCoord;

out vec4 color;

uniform sampler2D textureShadow;
uniform vec4 colorize;

void main() {
    // Light intensity calculation
    vec3 n = normalize(nSceneVertex);
    vec3 l = normalize(dSceneLight);
    float cosTheta = clamp(dot(n, l), 0.0, 1.0);

    // Changes the shadow map aliasing offset for edge-on light hits
    float bias = clamp(0.0001 * tan(acos(cosTheta)), 0.0, 0.0002);

    // Sample shadow map and determine if fragment is lit or not
    float illum = texture(textureShadow, shadowCoord.xy / shadowCoord.w).z < (shadowCoord.z / shadowCoord.w) - bias ? 0.3 : 1.0;

    // Specular calculation
    vec3 e = normalize(dSceneVertex);
    vec3 r = reflect(l, n);
    float cosAlpha = clamp(dot(e, r), 0.0, 1.0);

    color = colorize * 0.1 + // ambient
            colorize * vec4(vec3(illum * 0.9 * cosTheta), 1.0);// + // direct light
            //colorize * vec4(vec3(0.1 * pow(cosAlpha, 8.0)), 1.0); // specular light
}
