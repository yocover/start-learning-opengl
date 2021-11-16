#version 330 core
out vec4 FragColor;

in vec2 oTexCoord;
in vec3 oNormal;
in vec3 oPosition;

uniform vec3 cameraPos;
uniform samplerCube cubeTexture;
uniform vec3 objectColor;

void main() {
    float ratio = 1.0 / 1.52;
    vec3 I = normalize(oPosition - cameraPos);
    vec3 R = refract(I, normalize(oNormal), ratio);
    FragColor = vec4(texture(cubeTexture, R).rgb + objectColor, 1.0);
}