#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in int a_entityID;

layout(std140, binding = 0) uniform Camera {
    mat4 ViewProjectionMatrix;
} u_Camera;

struct VertexData{
    vec4 Color;
};

layout(location = 0) out VertexData v_vertexOutput;
layout(location = 1) out flat int v_entityID;

void main(){
    v_vertexOutput.Color = a_color;
    v_entityID = a_entityID;
    gl_Position = u_Camera.ViewProjectionMatrix * vec4(a_position, 1.0);
}