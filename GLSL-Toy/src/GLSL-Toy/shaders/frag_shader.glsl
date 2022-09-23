#version 450

layout (location = 0) out vec4 outColor;
uniform float uTime;

void main(){
    outColor = vec4((sin(uTime)+1.)/2., 1.0, 0.0, 1.0);
}