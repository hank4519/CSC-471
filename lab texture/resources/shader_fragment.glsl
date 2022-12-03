#version 330 core
out vec3 color;
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;
uniform vec3 campos;

uniform sampler2D tex;
uniform sampler2D tex2;

void main()
{
    vec3 n = normalize(vertex_normal);
    
    vec3 lp = vec3(20, -20, -100);
    vec3 ld = normalize(vertex_pos - lp);
    
    float diffuse = dot(n, ld);
    
    vec3 earth = texture(tex, vertex_tex).rgb;
    
    vec3 cd = normalize(vertex_pos - campos);
    vec3 h = normalize(cd + ld);
    float spec = dot(n, h);
    spec = clamp(spec, 0, 10);
    spec = pow(spec, 200);
    
    vec3 earthspec = texture(tex2, vertex_tex).rgb;
    
    color = earth * (diffuse * 0.8 + 0.1f) + (spec * earthspec * 0.6);


}

/*
vec3 n = normalize(vertex_normal);
vec3 lp=vec3(10,-20,-100);
vec3 ld = normalize(vertex_pos - lp);
float diffuse = dot(n,ld);

color = texture(tex, vertex_tex).rgb;

color *= diffuse*0.7;

vec3 cd = normalize(vertex_pos - campos);
vec3 h = normalize(cd+ld);
float spec = dot(n,h);
spec = clamp(spec,0,1);
spec = pow(spec,20);
color += vec3(1,1,1)*spec*3;

*/
