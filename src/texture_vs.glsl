#version 330

in vec2 vTexture;
in vec3 vPosition;
out vec2 vTex;

uniform mat4 uModelViewProjectMatrix;

void main() {
    vTex = vTexture;
    gl_Position = uModelViewProjectMatrix * vec4(vPosition, 1);
}
