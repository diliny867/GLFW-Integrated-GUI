#version 330 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D texture_diffuse1;

void main(){             
    vec4 texColor = texture(texture_diffuse1, texCoord);
    fragColor = texColor;
}