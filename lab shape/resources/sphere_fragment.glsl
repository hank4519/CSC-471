#version 330 core
out vec4 color;
in vec3 vertex_pos;
void main()
{
//color = vec3(1,1,1);
color.rgb = vec3(abs(2.88 * vertex_pos.x) ,abs(2.77 * vertex_pos.y), abs(2.66 * vertex_pos.z));
color.a = 0.1;
}
