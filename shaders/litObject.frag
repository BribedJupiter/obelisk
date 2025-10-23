#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    // Get difference between the fragment's normal and the direction of the light
    // Use max for cases where dot is negative due to a difference greater than 90 degrees.
    float diff = max(dot(norm, lightDir), 0.0); 
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    // reflect expects the first vector to point from the light source
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // use 32 as highlight shininess

    vec3 result = (ambient + diffuse + spec) * objectColor;
    FragColor = vec4(result, 1.0);
}