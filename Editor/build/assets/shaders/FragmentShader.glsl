#version 460 core

// uniform sampler2D u_texture;
// uniform float u_tilingFactor;
layout(location = 0) out vec4 color;
layout(location = 1) out int entityID;

struct VertexOutput{
    vec4 Color;
    vec2 TextureCoord;
    float TextureIndex;
};

layout(location = 0) in VertexOutput v_vertexInput;
layout(location = 3) in flat int v_entityID;

layout(binding = 0) uniform sampler2D u_textures[32];

void main(){
    vec4 texColor = v_vertexInput.Color;
    
    switch(int(v_vertexInput.TextureIndex)){
        case 0:
            texColor *= texture(u_textures[0], v_vertexInput.TextureCoord);
            break;
        case 1:
            texColor *= texture(u_textures[1], v_vertexInput.TextureCoord);
            break;
        case 2:
            texColor *= texture(u_textures[2], v_vertexInput.TextureCoord);
            break;
        case 3:
            texColor *= texture(u_textures[3], v_vertexInput.TextureCoord);
            break;
        case 4:
            texColor *= texture(u_textures[4], v_vertexInput.TextureCoord);
            break;
        case 5:
            texColor *= texture(u_textures[5], v_vertexInput.TextureCoord);
            break;
        case 6:
            texColor *= texture(u_textures[6], v_vertexInput.TextureCoord);
            break;
        case 7:
            texColor *= texture(u_textures[7], v_vertexInput.TextureCoord);
            break;
        case 8:
            texColor *= texture(u_textures[8], v_vertexInput.TextureCoord);
            break;
        case 9:
            texColor *= texture(u_textures[9], v_vertexInput.TextureCoord);
            break;
        case 10:
            texColor *= texture(u_textures[10], v_vertexInput.TextureCoord);
            break;
        case 11:
            texColor *= texture(u_textures[11], v_vertexInput.TextureCoord);
            break;
        case 12:
            texColor *= texture(u_textures[12], v_vertexInput.TextureCoord);
            break;
        case 13:
            texColor *= texture(u_textures[13], v_vertexInput.TextureCoord);
            break;
        case 14:
            texColor *= texture(u_textures[14], v_vertexInput.TextureCoord);
            break;
        case 15:
            texColor *= texture(u_textures[15], v_vertexInput.TextureCoord);
            break;
        case 16:
            texColor *= texture(u_textures[16], v_vertexInput.TextureCoord);
            break;
        case 17:
            texColor *= texture(u_textures[17], v_vertexInput.TextureCoord);
            break;
        case 18:
            texColor *= texture(u_textures[18], v_vertexInput.TextureCoord);
            break;
        case 19:
            texColor *= texture(u_textures[19], v_vertexInput.TextureCoord);
            break;
        case 20:
            texColor *= texture(u_textures[20], v_vertexInput.TextureCoord);
            break;
        case 21:
            texColor *= texture(u_textures[21], v_vertexInput.TextureCoord);
            break;
        case 22:
            texColor *= texture(u_textures[22], v_vertexInput.TextureCoord);
            break;
        case 23:
            texColor *= texture(u_textures[23], v_vertexInput.TextureCoord);
            break;
        case 24:
            texColor *= texture(u_textures[24], v_vertexInput.TextureCoord);
            break;
        case 25:
            texColor *= texture(u_textures[25], v_vertexInput.TextureCoord);
            break;
        case 26:
            texColor *= texture(u_textures[26], v_vertexInput.TextureCoord);
            break;
        case 27:
            texColor *= texture(u_textures[27], v_vertexInput.TextureCoord);
            break;
        case 28:
            texColor *= texture(u_textures[28], v_vertexInput.TextureCoord);
            break;
        case 29:
            texColor *= texture(u_textures[29], v_vertexInput.TextureCoord);
            break;
        case 30:
            texColor *= texture(u_textures[30], v_vertexInput.TextureCoord);
            break;
        case 31:
            texColor *= texture(u_textures[31], v_vertexInput.TextureCoord);
            break;
    }

    // color = v_color * texture(u_textures[int(v_textureIndex)], v_vertexInput.TextureCoord);
    color = texColor;

    entityID = v_entityID;
}