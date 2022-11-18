#version 330

struct Light {
    bool isActive;
    vec3 position;
    vec3 spotDir;
    float cutOff; // cosine of angle
};

in vec3 normal;
in vec4 eye;
in vec3 color;
in vec4 lightspace_pos[4];
in vec3 frag_pos;

uniform int num_lights;
// x = diffuse, y = ambient, z = specular, w = shininess
uniform vec4 material;  
uniform Light lights[4];
uniform sampler2DArray shadowMap;

uniform mat4 uViewMatrix;

out vec4 frag_colour;

float ShadowCalculation(int light_idx) {
    if(light_idx >= num_lights) {
        return 0.0;
    }
    vec3 lightDir = normalize(vec3(lights[light_idx].position - frag_pos));
    vec4 fragPosLightSpace = lightspace_pos[light_idx];
    float bias = max(0.005 * (1.0 - dot(normal, lightDir+(eye.xyz/eye.w))), 0.002);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, vec3(projCoords.xy, light_idx)).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
    for(int x = -3; x <= 3; ++x)
    {
        for(int y = -3; y <= 3; ++y)
        {
            float pcfDepth = texture(shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, light_idx)).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 49.0;
    return shadow;
}

vec4 LightCalculation(int light_idx) {
    vec4 diffuse = vec4(vec3(material.x), 1);
    vec4 specular = vec4(vec3(material.z), 1);
    float shininess = material.w;
    vec4 spec = vec4(0);
    float intensity = 0;

    vec3 lightDir = normalize(lights[light_idx].position - frag_pos);
    vec3 lightSpotDir = normalize(lights[light_idx].spotDir);
    
    if(dot(lightDir, lightSpotDir) < lights[light_idx].cutOff) {
        intensity = 0.0;
    }
    else {
        vec3 n = normalize(normal);
        float dotProd = dot(n, lightDir);
        intensity = max(dotProd, 0); 
        if(intensity > 0.0) {
            vec3 e = normalize(vec3(eye));
            vec3 h = normalize(lightDir + e);
            float intSpec = max(dot(h, n), 0.0);
            spec = specular * pow(intSpec, shininess);
        }
    }
    return (intensity * diffuse + spec);
}

void main () 
{
    vec4 lighting = vec4(0);
    float shadow = 0;
    vec4 final = vec4(0);
    for(int i = 0; i < num_lights; i++) {
        if(lights[i].isActive) {
            lighting = LightCalculation(i);
            shadow = ShadowCalculation(i);
        }
        final += vec4(vec3(material.y), 1) + (1.0 - shadow) * lighting;
    }
    frag_colour = vec4(color, 1);
    frag_colour = final / num_lights * frag_colour; 
}
