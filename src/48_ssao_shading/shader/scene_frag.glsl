#version 330 core
out vec4 FragColor;

struct Light {
  vec3 position;
  vec3 color;

  float linear;
  float quadratic;
};
uniform Light light;

uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D ssaoColor;

in VS_OUT {
  vec3 FragPos;
  vec3 Normal;
  vec2 TexCoords;
} fs_in;

uniform vec3 viewPos;

void main() {

  vec3 FragPos = texture(gPositionDepth, fs_in.TexCoords).rgb;
  vec3 Normal = texture(gNormal, fs_in.TexCoords).rgb;
  vec3 Diffuse = texture(gAlbedoSpec, fs_in.TexCoords).rgb;
  float AmbientOcclusion = texture(ssaoColor, fs_in.TexCoords).r;

  // Then calculate lighting as usual
  vec3 ambient = vec3(0.3 * AmbientOcclusion);
  vec3 lighting = ambient;
  vec3 viewDir = normalize(-FragPos); // Viewpos is (0.0.0)

  // Diffuse
  vec3 lightDir = normalize(light.position - FragPos);
  vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * light.color;

  // Specular
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(Normal, halfwayDir), 0.0), 8.0);
  vec3 specular = light.color * spec;

  // Attenuation
  float distance = length(light.position - FragPos);
  float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * distance * distance);
  diffuse *= attenuation;
  specular *= attenuation;
  lighting += diffuse + specular;

  FragColor = vec4(lighting, 1.0);
}
