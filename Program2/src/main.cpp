/*
ZJ Wood CPE 471 Lab 3 base code
Program 2 Dancing Sun
*/

#include <iostream>
#include <string>
#include <glad/glad.h>

#include "GLSL.h"
#include "Program.h"
#include "WindowManager.h"

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Application : public EventCallbacks{

public:

    WindowManager * windowManager = nullptr;

    // Our shader program
    std::shared_ptr<Program> prog;
    std::shared_ptr<Program> sun;

    // Contains vertex information for OpenGL
    GLuint VertexArrayID;
    GLuint sunVertexArrayID;

    // Data necessary to give our shape to OpenGL
    GLuint VertexBufferID;
    GLuint sunVertexBufferID;
    
    // This is the vertices for the sun
    GLfloat sun_vertices[80 * 9];

    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }

    // callback for the mouse when clicked move the triangle when helper functions
    // written
    void mouseCallback(GLFWwindow *window, int button, int action, int mods){
        double posX, posY;
        float newPt[2];
        if (action == GLFW_PRESS){
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
    void resizeCallback(GLFWwindow *window, int in_width, int in_height){
        //get the window size - may be different then pixels for retina
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
    }

    /*Note that any gl calls must always happen after a GL state is initialized */
    void initGeom(){
        //generate the VAO
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        
        
        //generate vertex buffer to hand off to OGL
        glGenBuffers(1, &VertexBufferID);
        //set the current state to focus on our vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);

        static const GLfloat screen[] =
                {
                    -1.0f, 1.0f, 0.0f,
                    -1.0f, -1.0f, 0.0f,
                    1.0f, -1.0f, 0.0f,
                    1.0f, 1.0f, 0.0f,
                    -1.0f, 1.0f, 0.0f,
                    1.0f, -1.0f, 0.0f
                };
        //actually memcopy the data - only do this once
        glBufferData(GL_ARRAY_BUFFER, sizeof(screen), screen, GL_DYNAMIC_DRAW);

        
        glEnableVertexAttribArray(0);
        //key function to get up how many elements to pull out at a time (3)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

         glBindVertexArray(0);

        
        glGenVertexArrays(1, &sunVertexArrayID);
        glBindVertexArray(sunVertexArrayID);

        glGenBuffers(1, &sunVertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, sunVertexBufferID);

        // For generating the points of the sun
        float r = 0.7;
        float angle = 0;
        for (int i = 0; i < 80 * 3; i+= 3){
            sun_vertices[3*i+0] = r * cos(angle);
            sun_vertices[3*i+1] = r * sin(angle);
            sun_vertices[3*i+2] = (i % 2 == 0) ? 0.02: 0.03;
            angle +=  (2.0 * 3.14159) / 80;
            sun_vertices[3*i+3] = r * cos(angle);
            sun_vertices[3*i+4] = r* sin(angle);
            sun_vertices[3*i+5] = (i % 2 == 0) ? 0.03: 0.02;

        }

        //actually memcopy the data - only do this once
        glBufferData(GL_ARRAY_BUFFER, sizeof(sun_vertices), sun_vertices, GL_DYNAMIC_DRAW);

        //we need to set up the vertex array
        glEnableVertexAttribArray(0);
        //key function to get up how many elements to pull out at a time (3)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

        glBindVertexArray(0);
    }

    //General OGL initialization - set OGL state here
    void init(const std::string& resourceDirectory){
        GLSL::checkVersion();

        // Set background color.
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        // Enable z-buffer test.
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA); 

        // Initialize the GLSL program1.
        prog = std::make_shared<Program>();
        prog->setVerbose(true);
        prog->setShaderNames(resourceDirectory + "/shader_vertex.glsl", resourceDirectory + "/shader_fragment.glsl");
        prog->init();
        prog->addUniform("P");
        prog->addUniform("V");
        prog->addUniform("M");
        prog->addAttribute("vertPos");

        // initialize sun's program
        sun = std::make_shared<Program>();
        
        sun->setVerbose(true);
        sun->setShaderNames(resourceDirectory + "/shader_vertext2.glsl", resourceDirectory + "/shader_fragment2.glsl");
        
        sun->init();
        
        sun->addUniform("P");
        sun->addUniform("V");
        sun->addUniform("M");
        
        //for rotating
        sun->addUniform("Sin");
        sun->addUniform("Cos");
        sun->addAttribute("vertPos");
       
        
    }


    /****DRAW
    This is the most important function in your program - this is where you
    will actually issue the commands to draw any geometry you have set up to
    draw
    ********/
    void render(){
        // Get current frame buffer size.
        
        /*** For TESTING ***/
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        int width, height;
        glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
        float aspect = width/(float)height;
        glViewport(0, 0, width, height);

        // Clear framebuffer.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Create the matrix stacks - please leave these alone for now
        
        glm::mat4 V, M, PP;
        V = glm::mat4(1);
        M = glm::mat4(1);
        // Apply orthographic projection.
        PP = glm::ortho(-1 * aspect, 1 * aspect, -1.0f, 1.0f, -2.0f, 100.0f);
        if (width < height){
            PP = glm::ortho(-1.0f, 1.0f, -1.0f / aspect,  1.0f / aspect, -2.0f, 100.0f);
        }
    
        // Draw the background using GLSL.
        prog->bind();
        
        //        float half = height / 2;
        //        int w = width / 2;
        //        double time = glfwGetTime();
        //        float sint = sin(time);
        //        float cost = cos(time);
        
        //send the matrices to the shaders
        glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, &PP[0][0]);
        glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);


        glBindVertexArray(VertexArrayID);

        //actually draw from vertex 0, 2 vertices
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);

        prog->unbind();

        // Draw the sun using GLSL.
        sun->bind();
        
        //send the matrices to the shaders
        glUniformMatrix4fv(sun->getUniform("P"), 1, GL_FALSE, &PP[0][0]);
        glUniformMatrix4fv(sun->getUniform("V"), 1, GL_FALSE, &V[0][0]);
        glUniformMatrix4fv(sun->getUniform("M"), 1, GL_FALSE, &M[0][0]);
        glUniform1f(sun->getUniform("Sin"), sin(glfwGetTime()));
        glUniform1f(sun->getUniform("Cos"), cos(glfwGetTime()));

        glBindVertexArray(sunVertexArrayID);
        
        //
        glDrawArrays(GL_TRIANGLES, 0, 80*3 );

        glBindVertexArray(0);

        sun->unbind();
    }
};
//******************************************************************************************
int main(int argc, char **argv){
    std::string resourceDir = "../../resources"; // Where the resources are loaded from
    // if (argc >= 2)
    // {
    //     resourceDir = argv[1];
    // }

    Application *application = new Application();

    /* your main will always include a similar set up to establish your window
        and GL context, etc. */
    WindowManager * windowManager = new WindowManager();
    windowManager->init(480, 480);
    windowManager->setEventCallbacks(application);
    application->windowManager = windowManager;

    /* This is the code that will likely change program to program as you
        may need to initialize or set up different data and state */
    // Initialize scene.
    application->init(resourceDir);
    application->initGeom();

    // Loop until the user closes the window.
    while(! glfwWindowShouldClose(windowManager->getHandle())){
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
