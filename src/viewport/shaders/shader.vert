#version 450

layout(binding=0) uniform TransformMatrices{
    mat4 model;
    mat4 view;
    mat4 proj;
}tm;

layout(location=0) in vec2 inPosition;
layout(location=1) in vec3 inColor;

layout(location=0) out vec3 fragColor;
void main() 
{
    gl_Position = tm.proj * tm.view * tm.model * vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;
}