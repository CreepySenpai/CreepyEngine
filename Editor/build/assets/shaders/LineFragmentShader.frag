#version 460 core

layout(location = 0) out vec4 out_Color;
layout(location = 1) out int out_entityID;

struct VertexData{
    vec4 Color;
};

layout(location = 0) in VertexData v_vertexInput;
layout(location = 1) in flat int v_entityID;


void main(){
    out_Color = v_vertexInput.Color;
    out_entityID = v_entityID;
}