#version 330 core
out vec4 color;
in vec3 vertex_pos;
in vec3 vertex_normal;
in vec2 vertex_tex;
uniform sampler2D tex;
void main()
{
    //color.rgb = vec3(abs(2.88 * vertex_pos.x) ,abs(2.77 * vertex_pos.y), abs(2.66 * vertex_pos.z));
    //color.rgb = vec3(1, 1, 1);
    //color.a = 1;
    vec2 tempTex = vertex_tex;
    tempTex *= 8;
    vec4 tempCol = texture(tex, tempTex+0.48);
    color = tempCol;
    color.a = (1 - length(tempCol)) * -2.7;
    //color.a = 1;
}
