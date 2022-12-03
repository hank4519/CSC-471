#version 330 core
out vec4 color;
in vec3 vertex_pos;
in vec2 vertex_tex;
in vec3 normal; 


uniform sampler2D tex;
uniform sampler2D tex2;
uniform sampler2D tex3;

uniform vec3 camoff;
uniform vec3 campos;

uniform vec3 lightpos;
uniform float texoff;

void main()
{

vec3 normalcolor1 = texture(tex, 2 * (vertex_tex * 100. + vec2(texoff, texoff * 0.4))).rgb;
vec3 texnormal1 = normalize(normalcolor1 - vec3(0.5, 0.5, 0.5));
texnormal1 = vec3(texnormal1.x, texnormal1.z, texnormal1.y);

vec3 normalcolor2 = texture(tex2, 2 * (vertex_tex * 100. + vec2(-texoff* 0.2, -texoff * 0.9))).rgb;
vec3 texnormal2 = normalize(normalcolor2 - vec3(0.5, 0.5, 0.5));
texnormal2 = vec3(texnormal2.x, texnormal2.z, texnormal2.y);


vec3 n = normalize(texnormal1 + texnormal2);


vec3 lightp = vec3(0, 50, 0);
vec3 ld = normalize(lightpos - vertex_pos);
vec3 cd = normalize(-campos - vertex_pos);
vec3 h = normalize(ld + cd);
float spec = dot(n, h);
spec = pow(spec, 10);
spec = clamp(spec, 0 , 1);

normalcolor1 = normalcolor1 * vec3(0.1, 0.1, 0.1);
normalcolor2 = normalcolor2 * vec3(0.1, 0.1, 0.1);

vec2 texcoords=vertex_tex;
float t=1./100.;
texcoords -= vec2(camoff.x,camoff.z)*t;

vec3 heightcolor = texture(tex3, texcoords).rgb;

color.rgb = vec3(spec) * heightcolor + normalcolor1 + normalcolor2 + vec3(0, 0, 0.8);
color.a = 1;

}



