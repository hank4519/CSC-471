#version 330 core
out vec3 color;
in vec3 pos;

void main()
{
    float l = length(pos);
    if ( l < 0.3 ){
        // make a decent sun-like yellow 
        color = vec3(1, 1, 0.4 + l);
        
    } else{
        color = vec3(1, l * 1.6, l);
    }
}
