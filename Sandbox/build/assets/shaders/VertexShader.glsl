#version 460 core

layout(location = 0) in vec3 a_position;

uniform mat4 u_viewProjectionMatrix;
uniform mat4 u_transformMatrix;

void main(){
    gl_Position = u_viewProjectionMatrix * u_transformMatrix * vec4(a_position,1.0);
}