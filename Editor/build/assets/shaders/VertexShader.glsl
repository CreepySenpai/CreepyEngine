#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_textureCoord;
layout(location = 3) in float a_textureIndex;

uniform mat4 u_viewProjectionMatrix;
// uniform mat4 u_transformMatrix;

out vec4 v_color;
out vec2 v_textureCoord;
out float v_textureIndex;

void main(){
    v_color = a_color;
    v_textureCoord = a_textureCoord;
    v_textureIndex = a_textureIndex;
    // gl_Position = u_viewProjectionMatrix * u_transformMatrix * vec4(a_position,1.0);
    gl_Position = u_viewProjectionMatrix * vec4(a_position, 1.0);
}