#version 330 core
layout (location = 0) in vec3 vertPos;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform float Sin;
uniform float Cos;

out vec3 pos;

void main(){
    
    pos = vertPos;
    if (vertPos.z == 0.02f){
        // Scale it by differenating z values
        pos.x = vertPos.x * 0.35 * (3 + Cos);
        pos.y = vertPos.y * 0.35 * (3 + Cos);
    }else {
        pos.x = vertPos.x * 0.15 * (4.5 + Sin);
        pos.y = vertPos.y * 0.15 * (4.5 + Sin);
    }
    
    gl_Position = P * V * M * vec4(pos, 1.0);
}
