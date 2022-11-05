#version 330

in vec3 vPosition;
in vec3 vColor;
in vec3 vNormal;

out vec3 normal;
out vec4 eye;
out vec3 color;

uniform mat4 uModelViewProjectMatrix;
uniform mat3 uNormalMatrix;
uniform mat4 uViewMatrix;

void main () 
{
  vec4 position = vec4(vPosition, 1.0);
  gl_Position = uModelViewProjectMatrix * position;
  normal = uNormalMatrix * normalize(vNormal);
  eye = -gl_Position;
  color = vColor;
}
