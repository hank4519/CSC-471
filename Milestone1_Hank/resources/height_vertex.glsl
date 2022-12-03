#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 vertTex;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
out vec3 vertex_pos;
out vec2 vertex_tex;
out vec3 normal;
uniform sampler2D tex;
uniform sampler2D tex2;
uniform sampler2D tex3;

uniform vec3 camoff;
uniform float wave;
uniform float texoff;

void main()
{

    vec2 texcoords=vertTex;
    vec4 tpos =  vec4(vertPos, 1.0);
    tpos.y += 2.95;
    gl_Position = P * V * M * tpos;
    vertex_tex = vertTex;
    normal = vec3(0, 1, 0);
    
    float t=1./300.;
    texcoords -= vec2(camoff.x,camoff.z)*t;
    texcoords += texoff;
    float height = texture(tex3, texcoords).r;
    height *= wave;
    tpos.y += height;
    vertex_pos = tpos.xyz;
    
}
