#version 460 core

// uniform sampler2D u_texture;
// uniform float u_tilingFactor;

out vec4 color;

in vec4 v_color;
in vec2 v_textureCoord;
in float v_textureIndex;

uniform sampler2D u_textures[32];

void main(){
    // color = v_color * texture(u_textures[int(v_textureIndex)], v_textureCoord * u_tilingFactor);
    // color = v_color;
    color = v_color * texture(u_textures[int(v_textureIndex)], v_textureCoord);
}