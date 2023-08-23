#version 330 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D texture_diffuse1;

uniform vec4 colorTint = vec4(1.0);

void main(){             
    vec4 texColor = texture(texture_diffuse1, vec2(texCoord.x,1.0-texCoord.y));
    fragColor = colorTint*texColor;
}