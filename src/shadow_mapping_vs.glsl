#version 330 

in vec3 vPosition;
in vec3 vColor;
in vec3 vNormal;

uniform mat4 uLightSpaceMatrix;
uniform mat4 uModelMatrix;

void main()
{
    gl_Position = uLightSpaceMatrix * uModelMatrix * vec4(vPosition, 1.0);
}
