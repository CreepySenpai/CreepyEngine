#version 460 core

layout(location=0)in vec3 a_position;
layout(location=1)in vec2 a_textureCoord;

uniform mat4 u_viewProjectionMatrix;
uniform mat4 u_transformMatrix;

out vec2 textureCoord;

void main(){
    textureCoord=a_textureCoord;
    gl_Position=u_viewProjectionMatrix*u_transformMatrix*vec4(a_position,1.);
}
