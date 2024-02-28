#version 460 core

uniform vec4 u_color;
uniform sampler2D u_texture;

out vec4 color;
in vec2 textureCoord;

void main(){
    vec4 c=u_color;
    color=texture(u_texture,textureCoord);
}