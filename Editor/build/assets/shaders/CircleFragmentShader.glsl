#version 460 core

layout(location = 0) out vec4 out_Color;
layout(location = 1) out int out_entityID;

struct VertexData{
    vec2 LocalPosition;
    vec4 Color;
    float Thickness;
    float Fade;
};

layout(location = 0) in VertexData v_vertexInput;
layout(location = 4) in flat int v_entityID;

void main(){

    float distance = 1.0 - length(v_vertexInput.LocalPosition);
    
    float circleAlpha = smoothstep(0.0, v_vertexInput.Fade, distance);

    circleAlpha *= smoothstep(v_vertexInput.Thickness + v_vertexInput.Fade, v_vertexInput.Thickness, distance);

    if(circleAlpha == 0.0){
        discard;
    }
    
    // out_Color = vec4(color, 1.0);
    out_Color = v_vertexInput.Color;
    out_Color.a *= circleAlpha;

    
    out_entityID = v_entityID;
    
}