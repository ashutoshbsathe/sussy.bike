#version 330
in vec3 pos;
out vec4 frag_colour;
void main()
{             
    // gl_FragDepth = gl_FragCoord.z;
     frag_colour = vec4(1.0, 1.0, 1.0, 1.0);
    // frag_colour = vec4(pos, 1.0);
}
