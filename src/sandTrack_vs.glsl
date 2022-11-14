#version 330

in vec3 vPosition;
out vec3 vTex;

uniform mat4 uModelViewProjectMatrix;

void main() {
    vTex = vPosition;
    gl_Position = uModelViewProjectMatrix * vec4(vPosition, 1);
}
