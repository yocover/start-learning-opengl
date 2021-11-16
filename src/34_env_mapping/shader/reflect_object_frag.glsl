#version 330 core
out vec4 FragColor;

in vec2 oTexCoord;
in vec3 oNormal;
in vec3 oPosition;

uniform vec3 cameraPos;
uniform samplerCube cubeTexture;
uniform vec3 objectColor;

void main()
{             
    vec3 I = normalize(oPosition - cameraPos);
    vec3 R = reflect(I, normalize(oNormal));
    FragColor = vec4(texture(cubeTexture, R).rgb + objectColor, 1.0);
}