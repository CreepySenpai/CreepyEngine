#version 460 core

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_tilingFactor;

out vec4 color;

in vec2 textureCoord;

void main(){
    color = u_color * texture(u_texture, textureCoord * u_tilingFactor);
}