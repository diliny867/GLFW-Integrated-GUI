#version 330 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D screen_texture;

void main(){
    vec4 col = texture(screen_texture,texCoord);
    fragColor = col;
}