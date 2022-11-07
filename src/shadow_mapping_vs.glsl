#version 330 

in vec3 vPosition;

uniform mat4 uLightSpaceMatrix;
uniform mat4 uModelMatrix;

void main()
{
    gl_Position = uLightSpaceMatrix * vec4(vPosition, 1.0);
}
