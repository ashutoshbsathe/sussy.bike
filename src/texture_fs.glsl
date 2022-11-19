#version 330

in vec2 vTex;

uniform sampler2D tex;

out vec4 frag_colour;

void main() {
    frag_colour = texture(tex, vTex);
}
