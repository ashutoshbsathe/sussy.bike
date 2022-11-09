#version 330 

in vec3 vPosition;
in vec3 vColor;

out vec3 color;
uniform mat4 uLightSpaceMatrix;

void main()
{
    gl_Position = uLightSpaceMatrix * vec4(vPosition, 1.0);
    color = vColor;
}
