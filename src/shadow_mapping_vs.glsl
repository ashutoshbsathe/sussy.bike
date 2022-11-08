#version 330 

in vec3 vPosition;

out vec3 pos;
uniform mat4 uLightSpaceMatrix;
uniform mat4 uModelMatrix;

void main()
{
    gl_Position = uLightSpaceMatrix * vec4(vPosition, 1.0);
    pos = vPosition * 0.5 + 0.5;
}
