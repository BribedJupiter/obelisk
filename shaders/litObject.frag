#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
    // Ambient
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    // Get difference between the fragment's normal and the direction of the light
    // Use max for cases where dot is negative due to a difference greater than 90 degrees.
    float diff = max(dot(norm, lightDir), 0.0); 
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    // reflect expects the first vector to point from the light source
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.00001), material.shininess); // use 32 as highlight shininess
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}