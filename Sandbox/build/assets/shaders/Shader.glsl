#type vertex
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

#type fragment
#version 460 core

uniform vec4 u_color;
uniform sampler2D u_texture;

out vec4 color;
in vec2 textureCoord;

void main(){
    vec4 c=u_color;
    color=texture(u_texture,textureCoord);
}