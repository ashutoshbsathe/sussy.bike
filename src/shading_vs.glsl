#version 330

in vec3 vPosition;
in vec3 vColor;
in vec3 vNormal;

out vec3 normal;
out vec4 eye;
out vec3 color;
out vec4 lightspace_pos;
out vec3 frag_pos;

uniform mat4 uModelViewProjectMatrix;
uniform mat3 uNormalMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uLightSpaceMatrix;

void main () 
{
  vec4 position = vec4(vPosition, 1.0);
  gl_Position = uViewMatrix * uModelViewProjectMatrix * position;
  normal = uNormalMatrix * normalize(vNormal);
  eye = -gl_Position;
  color = vColor;
  lightspace_pos = uLightSpaceMatrix * position;
  frag_pos = vec3(uModelViewProjectMatrix * position);
}
