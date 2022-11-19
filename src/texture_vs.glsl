#version 330

in vec3 vPosition;
in vec2 vTexture;
in vec3 vNormal;

out vec3 normal;
out vec4 eye;
out vec2 vTex;
out vec4 lightspace_pos[4];
out vec3 frag_pos;

uniform int num_lights;
uniform mat4 uModelMatrix;
uniform mat3 uNormalMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uLightSpaceMatrix[4];

void main() {
    vec4 position = vec4(vPosition, 1.);
    gl_Position = uViewMatrix * uModelMatrix * position;
    normal = uNormalMatrix * normalize(vNormal);
    eye = -gl_Position;
    vTex = vTexture;
    frag_pos = vec3(uModelMatrix * position);
    for(int i = 0; i < num_lights; i++) {
        lightspace_pos[i] = uLightSpaceMatrix[i] * position;
    }
}
