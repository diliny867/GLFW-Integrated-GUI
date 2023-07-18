#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
//layout (location = 2) in mat4 aModel;

out vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;
uniform float layer;

void main(){
    texCoord = aTexCoord;
    gl_Position = projection * model * vec4(aPos, layer, 1.0);
}