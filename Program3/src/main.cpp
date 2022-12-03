/*
ZJ Wood CPE 471 Lab 3 base code
*/

#include <iostream>
#include <glad/glad.h>

#include "GLSL.h"
#include "Program.h"
#include "WindowManager.h"

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;
double get_last_elapsed_time()
{
	static double lasttime = glfwGetTime();
	double actualtime =glfwGetTime();
	double difference = actualtime- lasttime;
	lasttime = actualtime;
	return difference;
}
class camera
{
public:
	glm::vec3 pos, rot;
	int w, a, s, d;
	camera()
	{
		w = a = s = d = 0;
		pos = rot = glm::vec3(0, 0, -1);
	}
	glm::mat4 process()
	{
		double ftime = get_last_elapsed_time();
		float speed = 0;
		if (w == 1)
		{
			speed = 1*ftime;
		}
		else if (s == 1)
		{
			speed = -1*ftime;
		}
		float yangle=0;
		if (a == 1)
			yangle = -1*ftime;
		else if(d==1)
			yangle = 1*ftime;
		rot.y += yangle;
		glm::mat4 R = glm::rotate(glm::mat4(1), rot.y, glm::vec3(0, 1, 0));
		glm::vec4 dir = glm::vec4(0, 0, speed,1);
		dir = dir*R;
		pos += glm::vec3(dir.x, dir.y, dir.z);
		glm::mat4 T = glm::translate(glm::mat4(1), pos);
		return R*T;
	}
};

camera mycam;
//int arm;
//int wave;

int topMove;
int bottomMove;

class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	// Our shader program
	Program prog;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;

	// Data necessary to give our box to OpenGL
	GLuint VertexBufferID, VertexColorIDBox, IndexBufferIDBox;

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		
		if (key == GLFW_KEY_W && action == GLFW_PRESS)
		{
			mycam.w = 1;
		}
		if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		{
			mycam.w = 0;
		}
		if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{
			mycam.s = 1;
		}
		if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		{
			mycam.s = 0;
		}
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			mycam.a = 1;
		}
		if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		{
			mycam.a = 0;
		}
		if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			mycam.d = 1;
		}
		if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
			mycam.d = 0;
		}
        
        if (key == GLFW_KEY_Z && action == GLFW_PRESS){
            bottomMove = -1;
        }
        if(key== GLFW_KEY_Z && action == GLFW_RELEASE){
            bottomMove = 0;
        }
        
        if (key == GLFW_KEY_C && action == GLFW_PRESS){
            bottomMove = 1;
        }
        if(key == GLFW_KEY_C && action == GLFW_RELEASE){
            bottomMove = 0;
        }
        
        if (key == GLFW_KEY_J && action == GLFW_PRESS){
            topMove = -1;
        }
        if (key == GLFW_KEY_J && action == GLFW_RELEASE){
            topMove = 0;
        }
        
        if (key == GLFW_KEY_L && action == GLFW_PRESS ){
            topMove = 1;
        }
        if (key == GLFW_KEY_L && action == GLFW_RELEASE ){
            topMove = 0;
        }
        
//        if (key == GLFW_KEY_N && action == GLFW_PRESS){
//            arm = -1;
//        }
//        if (key == GLFW_KEY_M && action == GLFW_PRESS){
//            arm = 1;
//        }
//        if (key == GLFW_KEY_T && action == GLFW_PRESS){
//            wave = 1;
//        }
//        if (key == GLFW_KEY_C && action == GLFW_PRESS){
//            wave = 0;
//        }
	}

	// callback for the mouse when clicked move the triangle when helper functions
	// written
	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		double posX, posY;
		float newPt[2];
		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &posX, &posY);
			std::cout << "Pos X " << posX <<  " Pos Y " << posY << std::endl;

		}
	}

	//if the window is resized, capture the new size and reset the viewport
	void resizeCallback(GLFWwindow *window, int in_width, int in_height)
	{
		//get the window size - may be different then pixels for retina
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}

	/*Note that any gl calls must always happen after a GL state is initialized */
	void initGeom()
	{
		//generate the VAO
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		//generate vertex buffer to hand off to OGL
		glGenBuffers(1, &VertexBufferID);
		//set the current state to focus on our vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);

		GLfloat cube_vertices[] = {
			// front
			-1.0, -1.0,  1.0,
			1.0, -1.0,  1.0,
			1.0,  1.0,  1.0,
			-1.0,  1.0,  1.0,
			// back
			-1.0, -1.0, -1.0,
			1.0, -1.0, -1.0,
			1.0,  1.0, -1.0,
			-1.0,  1.0, -1.0,
			//tube 8 - 11
			-1.0, -1.0,  1.0,
			1.0, -1.0,  1.0,
			1.0,  1.0,  1.0,
			-1.0,  1.0,  1.0,
			//12 - 15
			-1.0, -1.0, -1.0,
			1.0, -1.0, -1.0,
			1.0,  1.0, -1.0,
			-1.0,  1.0, -1.0

			
		};
		//actually memcopy the data - only do this once
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_DYNAMIC_DRAW);

		//we need to set up the vertex array
		glEnableVertexAttribArray(0);
		//key function to get up how many elements to pull out at a time (3)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

		//color
		GLfloat cube_colors[] = {
			// front colors
			1.0, 0.0, 0.5,
			1.0, 0.0, 0.5,
			1.0, 0.0, 0.5,
			1.0, 0.0, 0.5,
			// back colors
			0.5, 0.5, 0.0,
			0.5, 0.5, 0.0,
			0.5, 0.5, 0.0,
			0.5, 0.5, 0.0,
			// tube colors
			0.0, 1.0, 1.0,
			0.0, 1.0, 1.0,
			0.0, 1.0, 1.0,
			0.0, 1.0, 1.0,
			0.0, 1.0, 1.0,
			0.0, 1.0, 1.0,
			0.0, 1.0, 1.0,
			0.0, 1.0, 1.0,
		};
		glGenBuffers(1, &VertexColorIDBox);
		//set the current state to focus on our vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, VertexColorIDBox);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glGenBuffers(1, &IndexBufferIDBox);
		//set the current state to focus on our vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
		GLushort cube_elements[] = {
		
			// front
			0, 1, 2,
			2, 3, 0,
			// back
			7, 6, 5,
			5, 4, 7,
			//tube 8-11, 12-15
			8,12,13,
			8,13,9,
			9,13,14,
			9,14,10,
			10,14,15,
			10,15,11,
			11,15,12,
			11,12,8
			
		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);



		glBindVertexArray(0);

	}

	//General OGL initialization - set OGL state here
	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);

		// Initialize the GLSL program.
		
		prog.setVerbose(true);
		prog.setShaderNames(resourceDirectory + "/shader_vertex.glsl", resourceDirectory + "/shader_fragment.glsl");
		if (!prog.init())
			{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1); //[SHADERBUG] ? breakpoint here!! if program hits that codeline, check the command line window, it will contain the line and place of the error in the shader file
			}
		prog.addUniform("P");
		prog.addUniform("V");
		prog.addUniform("M");
		prog.addAttribute("vertPos");
		prog.addAttribute("vertColor");
	}


	/****DRAW
	This is the most important function in your program - this is where you
	will actually issue the commands to draw any geometry you have set up to
	draw
	********/
	void render()
	{
		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		float aspect = width/(float)height;
		glViewport(0, 0, width, height);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Create the matrix stacks - please leave these alone for now
		
		glm::mat4 V, M, P; //View, Model and Perspective matrix
		V = glm::mat4(1);
		M = glm::mat4(1);
		P = glm::perspective((float)(3.14159 / 4.), (float)((float)width/ (float)height), 0.1f, 1000.0f); //so much type casting... GLM metods are quite funny ones

		//animation with the model matrix:
		static float w = 0.0;
		w += 0.01;//rotation angle
		static float t = 0;
		t += 0.01;
		float trans = 0;// sin(t) * 2;
		
		glm::mat4 RotateX = glm::rotate(glm::mat4(1.0f), w, glm::vec3(-1.0f, 1.0f, 0.0f));
		glm::mat4 TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3 + trans));
		glm::mat4 TransX = glm::translate(glm::mat4(1.0f), glm::vec3(0.4f, 0.0f, 0.0));
		
        
        mat4 R;
        mat4 Tp;
        glm::mat4 T = glm::mat4(1.0f);
        glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, .1f, 0.1f));
        
        
        
        glm::mat4 head;
        glm::mat4 body;
        glm::mat4 upper_l;
        glm::mat4 lower_l;
        glm::mat4 upper_r;
        glm::mat4 lower_r;
        
//        static float arm_rad = 0;
//        if (arm == 1 && arm_rad <= 1){
//            arm_rad += 0.01;
//        }
//        else if(arm == -1 && arm_rad >= -1 ){
//            arm_rad -= 0.01;
//        }
//
//        static float wave_f = 0;
//        if(wave){
//            wave_f += 0.01;
//        }
		// Draw the box using GLSL.
        static float x_b = 0;
        if (bottomMove == -1 && x_b >= -0.4){
            x_b-=0.01;
        }
        if (bottomMove == 1 && x_b <= 0.4){
            x_b += 0.01;
        }
        
        static float x_t = 0;
        if (topMove == -1 && x_t >= -0.39){
            x_t -=0.01;
        }
        if (topMove == 1 && x_t <= 0.39 ){
            x_t += 0.01;
        }
		prog.bind();

		V = mycam.process();
		//send the matrices to the shaders
		glUniformMatrix4fv(prog.getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(prog.getUniform("V"), 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(prog.getUniform("M"), 1, GL_FALSE, &M[0][0]);

		glBindVertexArray(VertexArrayID);
		//actually draw from vertex 0, 3 vertices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
		
//		TransX = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
//		M = TransX  * S;
//		glUniformMatrix4fv(prog.getUniform("M"), 1, GL_FALSE, &M[0][0]);
//		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
        // -0.5 to 0.5
        
        S = scale(mat4(1.0f), vec3(0.5f, 0.5f, 1.7f));
        T = translate(mat4(1.0f), vec3(0, -1, -2));
        R = rotate(mat4(1.0f), 0.6f, vec3(0.0f, 0.0f, 0.0f));
        
        int top_score = 0;
        int bottom_score = 0;
        
        head =  T * S;
        glUniformMatrix4fv(prog.getUniform("M"), 1, GL_FALSE, &head[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
        
        S = scale(mat4(1.0f), vec3(0.09f, 0.015f, 0.0f));
        T = translate(mat4(1.0f), vec3(x_b, -0.5, -0.5));
        mat4 bottomPlayer = T * S;
        glUniformMatrix4fv(prog.getUniform("M"), 1, GL_FALSE, &bottomPlayer[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
        
        S = scale(mat4(1.0f), vec3(0.1f, 0.025f, 0.0f));
        T = translate(mat4(1.0f), vec3(x_t, -0.5, -3.3));
        mat4 topPlayer = T * S;
        glUniformMatrix4fv(prog.getUniform("M"), 1, GL_FALSE, &topPlayer[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
        static int top_point = 0;
        static int bottom_point = 0;
        static float ball_x = 0.0f;
        static float ball_goal;
        static int flag = 0;
        static float ball_z = -2.0f;
        if (top_point < 9 && bottom_point < 9){
            if (ball_z < -0.5 && flag == 0){
                ball_z += 0.02;
            }else if (ball_z >= -0.5 && ball_x <= x_b + 0.1 && ball_x >= x_b - 0.1){
                flag = 1;
                int random = rand()%2;
                ball_goal = random - 0.5;
            }else if (ball_z >= -0.5 && (ball_x > x_b + 0.1 || ball_x < x_b - 0.1)){
                ball_goal = 0;
                ball_z = -2;
                ball_x = 0;
                top_point += 1;
            }
            
            if (flag == 1 && ball_z >= -3.3){
                ball_z -= 0.02;
            }else if(ball_z <= -3.3 && ball_x <= x_t + 0.14 && ball_x >= x_t - 0.14){
                flag = 0;
                int random = rand() % 2;
                ball_goal = random - 0.5;
            }else if (ball_z <= -3.3 && (ball_x > x_t + 0.14 || ball_x < x_t - 0.14)){
                ball_goal = 0;
                ball_z = -2;
                ball_x = 0;
                bottom_point += 1;
            }
            
            if(ball_goal > 0 && ball_x < ball_goal){
                ball_x +=0.01;
            } else if(ball_goal < 0 && ball_x > ball_goal){
                ball_x -= 0.01;
            }
            
            if (ball_goal > 0 && ball_x >= ball_goal){
                ball_goal *= -1;
            } else if( ball_goal <0 && ball_x <= ball_goal){
                ball_goal *= -1;
            }
        }else{
            if(top_point == 9){
                ball_z = -0.5;
            }
            else if( bottom_point == 9 ){
                ball_z = -3.3;
            }
        }
        
        S = scale(mat4(1.0f), vec3(0.015f, 0.025f, 0.025f));
        T = translate(mat4(1.0f), vec3(ball_x, -0.5f, ball_z));
        mat4 ball = T * S;
        glUniformMatrix4fv(prog.getUniform("M"), 1, GL_FALSE, &ball[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
        
//        S = scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.2f));
//        T = translate(mat4(1.0f), vec3(0.0f, 0.0f, -0.2f));
//        body = T * S;
//        glUniformMatrix4fv(prog.getUniform("M"), 1, GL_FALSE, &body[0][0]);
//        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
//
//        S = scale(mat4(1.0f), vec3(0.15f, 0.025f, 0.05f));
//        T = translate(mat4(1.0f), vec3(0.2f, 0.1f, -0.2f));
//        R = rotate(mat4(1.0f), arm_rad, vec3(0.0f, 0.0f, 1.0f));
//        Tp = translate(mat4(1.0f), vec3(0.2f, 0.0f, -0.2f));
//
//        mat4 upper_arm = T * R * Tp;
//
//        upper_l = upper_arm * S;
//        glUniformMatrix4fv(prog.getUniform("M"), 1, GL_FALSE, &upper_l[0][0]);
//        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
//
//        R = rotate(mat4(1.0f), abs(sin(wave_f)), vec3(0.0f, 0.0f, 1.0f));
//        T = translate(mat4(1.0f), vec3(0.12f, 0.0f, 0.0f));
//        Tp = translate(mat4(1.0f), vec3(0.16f, 0.0f, 0.0f));
//
//        lower_l = upper_arm * T * R * Tp * S;
//        glUniformMatrix4fv(prog.getUniform("M"), 1, GL_FALSE, &lower_l[0][0]);
//        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
//
//        T = translate(mat4(1.0f), vec3(-0.21f, 0.1f, -0.22f));
//        R = rotate(mat4(1.0f), -arm_rad, vec3(0.0f, 0.0f,1.0f));
//        Tp = translate(mat4(1.0f), vec3(-0.2, 0, -0.22));
//
//        upper_arm = T * R * Tp;
//        upper_r = upper_arm * S;
//        glUniformMatrix4fv(prog.getUniform("M"), 1, GL_FALSE, &upper_r[0][0]);
//        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
//
//
//        R = rotate(mat4(1.0f), -abs(sin(wave_f)), vec3(0.0f, 0.0f, 1.0f));
//        T = translate(mat4(1.0f), vec3(-0.12f, 0.0f, 0.0f));
//        Tp = translate(mat4(1.0f), vec3(-0.16f, 0.0f, 0.0f));
//
//        lower_r = upper_arm * T * R * Tp * S;
//        glUniformMatrix4fv(prog.getUniform("M"), 1, GL_FALSE, &lower_r[0][0]);
//        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
        
        
        
		glBindVertexArray(0);

		prog.unbind();

	}

};
//******************************************************************************************
int main(int argc, char **argv)
{
	std::string resourceDir = "../../resources"; // Where the resources are loaded from
	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	/* your main will always include a similar set up to establish your window
		and GL context, etc. */
	WindowManager * windowManager = new WindowManager();
	windowManager->init(1920, 1080);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	/* This is the code that will likely change program to program as you
		may need to initialize or set up different data and state */
	// Initialize scene.
	application->init(resourceDir);
	application->initGeom();

	// Loop until the user closes the window.
	while(! glfwWindowShouldClose(windowManager->getHandle()))
	{
		// Render scene.
		application->render();

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
