#version 330 core
out vec4 color;
in vec3 vertex_color;
void main()
{
    color.rgb = vec3(0.0, 0.3, 1.0);
    color.a = 0.8;
}
