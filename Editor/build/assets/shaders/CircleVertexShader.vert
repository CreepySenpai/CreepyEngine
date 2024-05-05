#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_localPosition;
layout(location = 2) in vec4 a_color;
layout(location = 3) in float a_thickness;
layout(location = 4) in float a_fade;
layout(location = 5) in int a_entityID;

layout(std140, set = 0, binding = 0) uniform Camera {
    mat4 ViewProjectionMatrix;
} u_Camera;

struct VertexData{
    vec2 LocalPosition;
    vec4 Color;
    float Thickness;
    float Fade;
};

layout(location = 0) out VertexData v_vertexOutput;
layout(location = 4) out flat int v_entityID;

void main(){
    v_vertexOutput.LocalPosition = a_localPosition;
    v_vertexOutput.Color = a_color;
    v_vertexOutput.Thickness = a_thickness;
    v_vertexOutput.Fade = a_fade;
    v_entityID = a_entityID;
    gl_Position = u_Camera.ViewProjectionMatrix * vec4(a_position, 1.0);
}