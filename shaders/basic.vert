#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;

void main() {
    // Note, we calculate lighting in world space which is more intuitive. However,
    // most would calculate it in view space since we always know the viewer is at the origin.
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // In general, we want to calculate the normal matrix on the CPU and send it to shaders before drawing
    // The normal matrix avoids scales and translations that would change the normal vector, while still 
    // moving to world space for the fragment shader. This is especially important for non-uniform scales.
    Normal = mat3(transpose(inverse(model))) * aNormal; // since we need to calc the normal matrix
    FragPos = vec3(model * vec4(aPos, 1.0));
}