#version 330 core
layout(location = 0) in vec3 vertPos;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
out vec3 pos;
uniform float Sin;
uniform float Cos;

void main()
{
	pos = vec3(vertPos.x + Sin * 0.5, vertPos.y + Cos * 0.5, vertPos.z);
	gl_Position = P * V * M * vec4(vertPos, 1.0);
}
