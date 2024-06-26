#version 460 core

#extension GL_EXT_buffer_reference : require

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_textureCoord;
layout(location = 3) in float a_textureIndex;
layout(location = 4) in float a_tilingFactor;
layout(location = 5) in int a_entityID;

layout(std140, set = 0, binding = 0) uniform Camera {
    mat4 ViewProjectionMatrix;
} u_Camera;

struct VertexData{
    vec4 Color;
    vec2 TextureCoord;
    float TilingFactor;
};

layout(location = 0) out VertexData v_vertexOutput;
layout(location = 3) out flat float v_textureIndex;
layout(location = 4) out flat int v_entityID;

void main(){
    v_vertexOutput.Color = a_color;
    v_vertexOutput.TextureCoord = a_textureCoord;
    v_vertexOutput.TilingFactor = a_tilingFactor;

    v_textureIndex = a_textureIndex;
    v_entityID = a_entityID;

    gl_Position = u_Camera.ViewProjectionMatrix * vec4(a_position, 1.0);
}