#version 330

in vec3 normal;
in vec4 eye;
in vec3 color;

uniform mat4 uViewMatrix;

out vec4 frag_colour;

void main () 
{
  // Defining Materials
  vec4 diffuse = vec4(1.0, 1.0, 1.0, 1.0); 
  vec4 ambient = vec4(0.75, 0.75, 0.75, 1.0);
  vec4 specular = vec4(0.9, 0.9, 0.9, 1.0);
  float shininess = 0.1;
  vec4 spec = vec4(0.0); 

  // Defining Light 
  vec4 lightPos = vec4(1000, 1000, -1000, 1);
  vec3 lightDir = vec3(uViewMatrix * lightPos);  // Transforms with camera
  lightDir = normalize(vec3(lightDir));  

  // Diffuse
  vec3 n = normalize(normal);
  float dotProd = dot(n, lightDir);
  float intensity = max(dotProd, 0);

  if(intensity > 0.0) {
    vec3 e = normalize(vec3(eye));
    vec3 h = normalize(lightDir + e);
    float intSpec = max(dot(h, n), 0.0);
    spec = specular * pow(intSpec, shininess);
  }

  frag_colour = vec4(color, 1.0);

  frag_colour = max((intensity * diffuse + spec)*frag_colour, ambient*frag_colour);
}
