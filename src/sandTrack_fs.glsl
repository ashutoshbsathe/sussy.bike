#version 330

in vec3 vTex;

uniform samplerCube sandTrack;

out vec4 frag_colour;

void main() {
    frag_colour = texture(sandTrack, vTex);
}
