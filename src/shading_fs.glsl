#version 330

in vec3 normal;
in vec4 eye;
in vec3 color;
in vec4 lightspace_pos;
in vec3 frag_pos;

uniform sampler2D shadowMap;

uniform mat4 uViewMatrix;

out vec4 frag_colour;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir)
{
    float bias = max(0.0015 * (1.0 - dot(normal, lightDir+(eye.xyz/eye.w))), 0.001);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -3; x <= 3; ++x)
    {
        for(int y = -3; y <= 3; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 49.0;
    return shadow;
}

void main () 
{
  // Defining Materials
  vec4 diffuse = vec4(1.0, 1.0, 1.0, 1.0); 
  vec4 ambient = vec4(0.5, 0.5, 0.5, 1.0);
  vec4 specular = vec4(0.9, 0.9, 0.9, 1.0);
  float shininess = 0.1;
  vec4 spec = vec4(0.0); 

  // Defining Light 
  vec3 lightPos = vec3(12500, 12500, 12500);
  vec3 lightDir = vec3(lightPos - frag_pos);
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

  //frag_colour = max((intensity * diffuse + spec)*frag_colour, ambient*frag_colour);
  float shadow = ShadowCalculation(lightspace_pos, lightDir);
  frag_colour = (ambient + (1.0 - shadow) * (intensity * diffuse + spec)) * frag_colour;
  
}
