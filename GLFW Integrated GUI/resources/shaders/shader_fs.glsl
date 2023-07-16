#version 330 core
out vec4 fragColor;

in vec3 color;
in vec2 texCoords;

uniform sampler2D texture_diffuse1;

void main(){             
    vec4 texColor = texture(texture_diffuse1, texCoords);
    fragColor = texColor;
    //FragColor = Color;
}