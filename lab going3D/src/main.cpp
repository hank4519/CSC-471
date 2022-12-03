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

#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"

using namespace glm;
using namespace std;
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


		glGenBuffers(1, &VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);

		GLfloat cube_vertices[] = {
			// front
			-1.0, -4.0,  1.0,
			1.0, -4.0,  1.0,
			1.0,  4.0,  1.0,
			-1.0,  4.0,  1.0,
			// back
			-1.0, -4.0, -1.0,
			1.0, -4.0, -1.0,
			1.0,  4.0, -1.0,
			-1.0,  4.0, -1.0,
		};
		//make it a bit smaller
		for (int i = 0; i < 24; i++)
			cube_vertices[i] *= 0.5;
		//actually memcopy the data - only do this once
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_DYNAMIC_DRAW);

		//we need to set up the vertex array
		glEnableVertexAttribArray(0);
		//key function to get up how many elements to pull out at a time (3)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

		//color
		GLfloat cube_colors[] = {
			// front colors
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0,
			1.0, 1.0, 1.0,
			// back colors
			1.0, 0.0, 0.0,
			0.0, 1.0, 0.0,
			0.0, 0.0, 1.0,
			1.0, 1.0, 1.0,
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
			// top
			1, 5, 6,
			6, 2, 1,
			// back
			7, 6, 5,
			5, 4, 7,
			// bottom
			4, 0, 3,
			3, 7, 4,
			// left
			4, 5, 1,
			1, 0, 4,
			// right
			3, 2, 6,
			6, 7, 3,
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

//    glm::mat4 createIdentityMat(){
//        return mat4(1);
//    }
    float* createIdentityMat(){
        float *Matrix = new float[16];
        Matrix[0] = 1;
        Matrix[5] = 1;
        Matrix[10] = 1;
        Matrix[15] = 1;
        return Matrix;
    }
    
    float* createTranslateMat(float x,float y, float z){
		float *R = createIdentityMat();
        R[12] = x;
        R[13] = y;
        R[14] = z;
        return R;
    }

	float* createRotationMatrixX(float angle_x){
		float* R = createIdentityMat();
		R[5] = cos(angle_x);
		R[6] = -sin(angle_x);
		R[9] = sin(angle_x);
		R[10] = cos(angle_x);
		return R;
	}
	
	float* createRotationMatrixY(float angle_y){
		float* R = createIdentityMat();
		R[0] = cos(angle_y);
		R[2] = sin(angle_y);
		R[8] = -sin(angle_y);
		R[10] = cos(angle_y);
		return R;
	}

	float* createRotationMatrixZ(float angle_y){
		float* R = createIdentityMat();
		R[0] = cos(angle_y);
		R[1] = -sin(angle_y);
		R[4] = sin(angle_y);
		R[5] = cos(angle_y);
		return R;
	}
	
	float* createScaleMat(float x,float y, float z){
		float* R = createIdentityMat();
		R[0] = x;
		R[5] = y;
		R[10] = z;
		return R;
	}
    
    float* multiply(float *a, float *b){
        float * r = new float[16];
        for (int i=0; i<16; i+=4){
            for (int j=0; j<4; j++){
                r[i+j] = b[i]*a[j] + b[i+1]*a[j+4] + b[i+2]*a[j+8] + b[i+3]*a[j+12];
            }
        }
        return r;
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

		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Create the matrices
		
		glm::mat4 V, M, P; //View, Model and Perspective matrix
		V = glm::mat4(1);
		M = glm::mat4(1);
		// Apply orthographic projection....
		P = glm::ortho(-1 * aspect, 1 * aspect, -1.0f, 1.0f, -2.0f, 100.0f);		
		if (width < height)
			P = glm::ortho(-1.0f, 1.0f, -1.0f / aspect,  1.0f / aspect, -2.0f, 100.0f);
		// ...but we overwrite it (optional) with a perspective projection.
		P = glm::perspective((float)(3.14159 / 4.), (float)((float)width/ (float)height), 0.1f, 100.0f); //so much type casting... GLM metods are quite funny ones

		
		// Draw the box using GLSL.
		prog.bind();

		//bind the cube's VAO:
		glBindVertexArray(VertexArrayID);
		
		//send the matrices to the shaders
		glUniformMatrix4fv(prog.getUniform("P"), 1, GL_FALSE, &P[0][0]);
		glUniformMatrix4fv(prog.getUniform("V"), 1, GL_FALSE, &V[0][0]);
        
        //mat4 R = createIdentityMat();
        //R = createTranslateMat(0.5, 1.0, 1.1);
        //cout << glm::to_string(R) <<endl;
        
        
		//set model matrix and draw the cube		
		//M = ...
        
//       static float w = 0;
//       w += 0.05;
//       mat4 R = rotate(mat4(1), w, vec3(0,1,0));
//       mat4 S = scale(mat4(1), vec3(0.3, 0.3, 0.3));
//       mat4 T = glm::translate(glm::mat4(1), glm::vec3(0, 0, -3));
	//    cout << glm::to_string(T) <<endl;
//       M = T * R * S;
        
        static float w = 0;
        w += 0.05;
        
        float *S1 = createScaleMat(0.3, 0.3, 0.3);
        float *T1 = createTranslateMat(0, 0, -3);
        static float *R1 = multiply(T1, S1);
		glUniformMatrix4fv(prog.getUniform("M"), 1, GL_FALSE, &R1[0]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);

        float *S2 = createScaleMat(0.3, 0.3, 0.3);
        float *T2 = createTranslateMat(0.5, 0, -3);
        static float *R2 = multiply(T2, S2);
        glUniformMatrix4fv(prog.getUniform("M"), 1, GL_FALSE, &R2[0]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);

        float *S3 = createScaleMat(0.2, 0.2, 0.2);
        float *T3 = createTranslateMat(0.25, 0, -3);
        float *R = createRotationMatrixZ(130);
        float *tmp = multiply(R, S3);
        static float *R3 = multiply(T3, tmp);
        glUniformMatrix4fv(prog.getUniform("M"), 1, GL_FALSE, &R3[0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
        
        float *S4 = createScaleMat(0.3, 0.3, 0.3);
        float *T4 = createTranslateMat(1.2, 0, -3);
        static float *R4 = multiply(T4, S4);
        glUniformMatrix4fv(prog.getUniform("M"), 1, GL_FALSE, &R4[0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
        
//        for (int x = -3; x < 4; x++ ){
//            for (int y = -3; y < 4; y++ ){
//                for (int z = 0; z < 5; z++) {
//                    T = glm::translate(glm::mat4(1), glm::vec3(x, y, -3-z));
//                    M = T * R * S;
//                    glUniformMatrix4fv(prog.getUniform("M"), 1, GL_FALSE, &M[0][0]);
//                    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
//                }
//            }
//        }
        
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
	windowManager->init(640, 480);
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
