/*
ZJ Wood, Dunn, Eckhardt CPE 471 Lab base code
*/

#include <iostream>
#include <glad/glad.h>

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"

#include "WindowManager.h"
#include "Shape.h"
// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;
Shape shape;
Shape cShape;
Shape sShape;


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
		pos = rot = glm::vec3(0, 0, 0);
	}
	glm::mat4 process(double ftime)
	{
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
int arm;
int wave;
int left_wave;
int wave_leg_l;
int wave_leg_r;

class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	// Our shader program
	//std::shared_ptr<Program> prog;
    std::shared_ptr<Program> sphere;
    std::shared_ptr<Program> creature;
    std::shared_ptr<Program> snow;

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
//        if (key == GLFW_KEY_N && action == GLFW_PRESS){
//            arm = -1;
//        }
//        if (key == GLFW_KEY_M && action == GLFW_PRESS){
//            arm = 1;
//        }
        if ((key == GLFW_KEY_Y || key == GLFW_KEY_H) && action == GLFW_PRESS){
            wave = 1;
        }
        if ((key == GLFW_KEY_T || key == GLFW_KEY_G) && action == GLFW_PRESS){
            left_wave = 1;
        }
        if ((key == GLFW_KEY_U || key == GLFW_KEY_J) && action == GLFW_PRESS){
            wave_leg_l = 1;
        }
        if ((key == GLFW_KEY_I || key == GLFW_KEY_K) && action == GLFW_PRESS){
            wave_leg_r = 1;
        }
        if (key == GLFW_KEY_C && action == GLFW_PRESS){
            wave = 0;
            left_wave = 0;
            wave_leg_l = 0;
            wave_leg_r = 0; 
        }
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

			//change this to be the points converted to WORLD
			//THIS IS BROKEN< YOU GET TO FIX IT - yay!
			newPt[0] = 0;
			newPt[1] = 0;

			std::cout << "converted:" << newPt[0] << " " << newPt[1] << std::endl;
			glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
			//update the vertex array with the updated points
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*6, sizeof(float)*2, newPt);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
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
		string resourceDirectory = "../../resources" ;
		// Initialize mesh.
	
//		shape.loadMesh(resourceDirectory + "/t800.obj");
//		shape.resize();
//		shape.init();
//
        shape.loadMesh(resourceDirectory + "/smoothsphere.obj");
        shape.resize();
        shape.init();
        
//        cShape.loadMesh(resourceDirectory + "/FA18.obj");
//        cShape.resize();
//        cShape.init();
        
        sShape.loadMesh(resourceDirectory + "/FA18.obj");
        sShape.resize();
        sShape.init();
        
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
//		glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// Initialize the GLSL program.
		sphere = std::make_shared<Program>();
		sphere->setVerbose(true);
		sphere->setShaderNames(resourceDirectory + "/sphere_vertex.glsl", resourceDirectory + "/sphere_fragment.glsl");
        if (!sphere->init()){
            cout << "error in init" << endl; 
        }
		sphere->addUniform("P");
		sphere->addUniform("V");
		sphere->addUniform("M");
		sphere->addAttribute("vertPos");
        
        creature = std::make_shared<Program>();
        creature-> setVerbose(true);
        creature->setShaderNames(resourceDirectory + "/creature_vertex.glsl", resourceDirectory + "/creature_fragment.glsl");
        if(!creature-> init()){
            std::cerr << "error in creature init" << endl;
        }
        creature->addUniform("P");
        creature->addUniform("V");
        creature->addUniform("M");
        creature->addAttribute("vertPos");
        
        snow = std::make_shared<Program>();
        snow->setVerbose(true);
        snow->setShaderNames(resourceDirectory + "/snow_vertex.glsl", resourceDirectory + "/snow_fragment.glsl");
        if(!snow->init()){
            std::cerr << "error in snow init" << endl;
        }
        snow->addUniform("P");
        snow->addUniform("V");
        snow->addUniform("M");
        snow->addAttribute("vertPos");
        
		
	}
    float getRandX(){
        float randX = rand()%(12-0 + 1) + 0;
        randX = (randX * 0.1) - 0.6;
        return randX;
        
    }
    float getRandY(){
        float randX = rand()%(8-4 + 1) + 4;
        randX = (randX * 0.1);
        return randX;
    }
    float getRandSpeed(){
        float randX = rand()%(8-0 + 1) + 1;
        randX = (randX * 0.2);
        return randX;
    }

	/****DRAW
	This is the most important function in your program - this is where you
	will actually issue the commands to draw any geometry you have set up to
	draw
	********/
	void render()
	{
		double frametime = get_last_elapsed_time();

		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		//float aspect = width/(float)height;
		glViewport(0, 0, width, height);

		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Create the matrix stacks - please leave these alone for now
		
		glm::mat4 V, M, P; //View, Model and Perspective matrix
		V = glm::mat4(1);
		M = glm::mat4(1);
		
		P = glm::perspective((float)(3.14159 / 4.), (float)((float)width/ (float)height), 0.1f, 1000.0f); //so much type casting... GLM metods are quite funny ones

		//animation with the model matrix:
//		static float w = 0.0;
//		w += 1.0 * frametime;//rotation angle
		float trans = 0;// sin(t) * 2;
//		glm::mat4 RotateY = glm::rotate(glm::mat4(1.0f), w, glm::vec3(0.0f, 1.0f, 0.0f));
//		float angle = -3.1415926/2.0;
//
//		glm::mat4 RotateX = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 TransZ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3 + trans));
		glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.8f));

		//M =  TransZ * RotateY * RotateX * S;
        M = TransZ * S; 
		// Draw the box using GLSL.

		V = mycam.process(frametime);
        
//        creature->bind();
//        glUniformMatrix4fv(creature->getUniform("P"), 1, GL_FALSE, &P[0][0]);
//        glUniformMatrix4fv(creature->getUniform("V"), 1, GL_FALSE, &V[0][0]);
//        glUniformMatrix4fv(creature->getUniform("M"), 1, GL_FALSE, &M[0][0]);
//        cShape.draw(creature);
//        creature->unbind();
        
        glm::mat4 head;
        glm::mat4 body;
        glm::mat4 upper_l;
        glm::mat4 lower_l;
        glm::mat4 upper_r;
        glm::mat4 lower_r;
        
        static float arm_rad = 0;
        if (arm == 1 && arm_rad <= 1){
            arm_rad += 0.01;
        }
        else if(arm == -1 && arm_rad >= -1 ){
            arm_rad -= 0.01;
        }
        
        static float leg_rad = 1.5;
        
        static float wave_f = 0;
        if(wave){
            wave_f += 0.01;
        }
        static float left_wave_f = 0;
        if(left_wave){
            left_wave_f += 0.01;
        }
        static float left_leg_wave_f = 0;
        if (wave_leg_l){
            left_leg_wave_f += 0.01;
        }
        
        static float right_leg_wave_f = 0;
        if (wave_leg_r){
            right_leg_wave_f += 0.01;
        }
        mat4 R;
        mat4 Tp;
        glm::mat4 T = glm::mat4(1.0f);
        S = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
        creature->bind();
        glUniformMatrix4fv(creature->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(creature->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(creature->getUniform("M"), 1, GL_FALSE, &M[0][0]);

        glBindVertexArray(VertexArrayID);
        //actually draw from vertex 0, 3 vertices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox);
        
        S = scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));
        T = translate(mat4(1.0f), vec3(0, 0.3, -3));
        R = rotate(mat4(1.0f), 0.6f, vec3(0.0f, 0.2f, 0.0f));

        head =  T * R * S;
        glUniformMatrix4fv(creature->getUniform("M"), 1, GL_FALSE, &head[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
        
        S = scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.2f));
        T = translate(mat4(1.0f), vec3(0.0f, 0.0f, -3.0f));
        body = T * S;
        glUniformMatrix4fv(creature->getUniform("M"), 1, GL_FALSE, &body[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);

        S = scale(mat4(1.0f), vec3(0.15f, 0.025f, 0.05f));
        T = translate(mat4(1.0f), vec3(0.15f, 0.1f, -3.0f));
        R = rotate(mat4(1.0f), arm_rad, vec3(0.0f, 0.0f, 1.0f));
        Tp = translate(mat4(1.0f), vec3(0.2f, 0.0f, -0.2f));

        mat4 upper_arm = T * R * Tp;

        upper_l = upper_arm * S;
        glUniformMatrix4fv(creature->getUniform("M"), 1, GL_FALSE, &upper_l[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);

        R = rotate(mat4(1.0f), abs(sin(wave_f)), vec3(0.0f, 0.0f, 1.0f));
        T = translate(mat4(1.0f), vec3(0.12f, 0.0f, 0.0f));
        Tp = translate(mat4(1.0f), vec3(0.16f, 0.0f, 0.0f));

        lower_l = upper_arm * T * R * Tp * S;
        glUniformMatrix4fv(creature->getUniform("M"), 1, GL_FALSE, &lower_l[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);

        T = translate(mat4(1.0f), vec3(-0.15f, 0.1f, -3.0f));
        R = rotate(mat4(1.0f), -arm_rad, vec3(0.0f, 0.0f,1.0f));
        Tp = translate(mat4(1.0f), vec3(-0.2, 0, -0.2f));

        // =====================
        upper_arm = T * R * Tp;
        upper_r = upper_arm * S;
        glUniformMatrix4fv(creature->getUniform("M"), 1, GL_FALSE, &upper_r[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);

        R = rotate(mat4(1.0f), -abs(sin(left_wave_f)), vec3(0.0f, 0.0f, 1.0f));
        T = translate(mat4(1.0f), vec3(-0.12f, 0.0f, -0.0f));
        Tp = translate(mat4(1.0f), vec3(-0.16f, 0.0f, 0.0f));

        lower_r = upper_arm * T * R * Tp * S;
        glUniformMatrix4fv(creature->getUniform("M"), 1, GL_FALSE, &lower_r[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
        
        // left legs
        S = scale(mat4(1.0f), vec3(0.27f, 0.025f, 0.03f));
        T = translate(mat4(1.0f), vec3(-0.15, -0.25f, -3.0f));
        R = rotate(mat4(1.0f), leg_rad, vec3(0, -abs(sin(left_leg_wave_f)), 1.0f));
        Tp = translate(mat4(1.0f), vec3(-0.2, 0, 0.0f));
        
        mat4 lower_leg = T * R * Tp;
        mat4 lower_leg_l;
        lower_leg_l = lower_leg * S;
        glUniformMatrix4fv(creature->getUniform("M"), 1, GL_FALSE, &lower_leg_l[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
        
        //right leg
        S = scale(mat4(1.0f), vec3(0.32f, 0.025f, 0.03f));
        T = translate(mat4(1.0f), vec3(0.15f, -0.2f, -3.0f));
        R = rotate(mat4(1.0f), leg_rad, vec3(0, abs(sin(right_leg_wave_f)), 1.0f));
        Tp = translate(mat4(1.0f), vec3(-0.2, 0, 0.0f));
        lower_leg = T * R * Tp;
        mat4 lower_leg_r;
        lower_leg_r = lower_leg * S;
        glUniformMatrix4fv(creature->getUniform("M"), 1, GL_FALSE, &lower_leg_r[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
        
        creature->unbind();
        
        
        
//        //send the matrices to the shaders
        sphere->bind();
        glUniformMatrix4fv(sphere->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(sphere->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(sphere->getUniform("M"), 1, GL_FALSE, &M[0][0]);

        shape.draw(sphere);
        sphere->unbind();

        
     
        snow->bind();
        S = glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.8f));
        glUniformMatrix4fv(snow->getUniform("P"), 1, GL_FALSE, &P[0][0]);
        glUniformMatrix4fv(snow->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(snow->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        
        static int done = 1;
        static float r_x[5];
        static float r_y[5];
        static float r_s[5];

        M =  TransZ * S;
        if (done) {
            for (int i = 0; i < 5 ; i++ ){
                r_x[i] = getRandX();
                r_y[i] = getRandY();
                r_s[i] = getRandSpeed();
            }
        }
        done = 0;
        for (int i = 0; i < 5; i++ ) {
            if(r_y[i] < -0.79){
                r_y[i] = getRandY();
                r_x[i] = getRandX();
                r_s[i] = getRandSpeed();
            }
            mat4 T = translate(mat4(1.0f), vec3(r_x[i], r_y[i], 0.3));
            S = scale(mat4(1.0f), vec3(0.03f, 0.03f, 0.03f));
            mat4 flake = M * T * S;
            glUniformMatrix4fv(snow->getUniform("M"), 1,GL_FALSE, &flake[0][0]);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
            sShape.draw(snow);
            r_y[i] -= r_s[i] * frametime;
        }
    
        snow->unbind();
        
        
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
