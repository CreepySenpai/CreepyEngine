#version 460 core

// uniform sampler2D u_texture;
// uniform float u_tilingFactor;

out vec4 color;

in vec4 v_color;
in vec2 v_textureCoord;
in float v_textureIndex;

uniform sampler2D u_textures[32];

void main(){
    vec4 texColor = v_color;
    
    switch(int(v_textureIndex)){
        case 0:
            texColor *= texture(u_textures[0], v_textureCoord);
            break;
        case 1:
            texColor *= texture(u_textures[1], v_textureCoord);
            break;
        case 2:
            texColor *= texture(u_textures[2], v_textureCoord);
            break;
        case 3:
            texColor *= texture(u_textures[3], v_textureCoord);
            break;
        case 4:
            texColor *= texture(u_textures[4], v_textureCoord);
            break;
        case 5:
            texColor *= texture(u_textures[5], v_textureCoord);
            break;
        case 6:
            texColor *= texture(u_textures[6], v_textureCoord);
            break;
        case 7:
            texColor *= texture(u_textures[7], v_textureCoord);
            break;
        case 8:
            texColor *= texture(u_textures[8], v_textureCoord);
            break;
        case 9:
            texColor *= texture(u_textures[9], v_textureCoord);
            break;
        case 10:
            texColor *= texture(u_textures[10], v_textureCoord);
            break;
        case 11:
            texColor *= texture(u_textures[11], v_textureCoord);
            break;
        case 12:
            texColor *= texture(u_textures[12], v_textureCoord);
            break;
        case 13:
            texColor *= texture(u_textures[13], v_textureCoord);
            break;
        case 14:
            texColor *= texture(u_textures[14], v_textureCoord);
            break;
        case 15:
            texColor *= texture(u_textures[15], v_textureCoord);
            break;
        case 16:
            texColor *= texture(u_textures[16], v_textureCoord);
            break;
        case 17:
            texColor *= texture(u_textures[17], v_textureCoord);
            break;
        case 18:
            texColor *= texture(u_textures[18], v_textureCoord);
            break;
        case 19:
            texColor *= texture(u_textures[19], v_textureCoord);
            break;
        case 20:
            texColor *= texture(u_textures[20], v_textureCoord);
            break;
        case 21:
            texColor *= texture(u_textures[21], v_textureCoord);
            break;
        case 22:
            texColor *= texture(u_textures[22], v_textureCoord);
            break;
        case 23:
            texColor *= texture(u_textures[23], v_textureCoord);
            break;
        case 24:
            texColor *= texture(u_textures[24], v_textureCoord);
            break;
        case 25:
            texColor *= texture(u_textures[25], v_textureCoord);
            break;
        case 26:
            texColor *= texture(u_textures[26], v_textureCoord);
            break;
        case 27:
            texColor *= texture(u_textures[27], v_textureCoord);
            break;
        case 28:
            texColor *= texture(u_textures[28], v_textureCoord);
            break;
        case 29:
            texColor *= texture(u_textures[29], v_textureCoord);
            break;
        case 30:
            texColor *= texture(u_textures[30], v_textureCoord);
            break;
        case 31:
            texColor *= texture(u_textures[31], v_textureCoord);
            break;
    }

    // color = v_color * texture(u_textures[int(v_textureIndex)], v_textureCoord);
    color = texColor;
}