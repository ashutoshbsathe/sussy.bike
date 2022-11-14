#version 330

in vec2 vTex;

uniform sampler2D sandTrack;

out vec4 frag_colour;

void main() {
    frag_colour = texture(sandTrack, vTex);
}
