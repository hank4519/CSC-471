#version 330 core
out vec3 color;
in vec3 pos;
uniform float H;
uniform float W; 
uniform float Sin;
uniform float Cos; 

void main()
{
	// color = vec3(0.0, 0.0, 1.0);
    
    //vec3 white = vec3(1, 1, 1);
    //b_element = 1.0 - abs(length(pos));
    vec3 blue = vec3(0.0, 0.0, 1.0 - 0.5 * sqrt(pos.x * pos.x + pos.y * pos.y));
    color = blue;
    
    //gradient
    /*
	if ( gl_FragCoord.y > H ){
		color = blue;
	} else{ 
		color = vec3(gl_FragCoord.x / W, gl_FragCoord.y / H, 0);
    }
     */
    
    //circle
    /*
    float l = length(pos);
    if (l < 0.1){
        color = white;
    }else{
        color = color + (white * (0.9 * l));
    }
    
    */
    
    
    //circle
	//if (gl_FragCoord.y > 500){
	//	color = vec3(0.0, 0.0, 1.0);
 	//}
}
