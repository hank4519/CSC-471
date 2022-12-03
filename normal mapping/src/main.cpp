/* Lab 6 base code - transforms using local matrix functions
	to be written by students -
	based on lab 5 by CPE 471 Cal Poly Z. Wood + S. Sueda
	& Ian Dunn, Christian Eckhardt
*/
#include <iostream>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "GLSL.h"
#include "Program.h"
#include "Shape.h"
#include "MatrixStack.h"
#include "WindowManager.h"
#include "camera.h"
// used for helper in perspective
#include "glm/glm.hpp"
// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
using namespace glm;






class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	// Our shader program
	std::shared_ptr<Program> prog;

	// Shape to be used (from obj file)
	shared_ptr<Shape> shape;
	
	//camera
	camera mycam;

	//texture for sim
	GLuint Texture;
	GLuint Texture2;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;

	// Data necessary to give our triangle to OpenGL
	GLuint VertexBufferID;

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
	}

	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		double posX, posY;

		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &posX, &posY);
			cout << "Pos X " << posX <<  " Pos Y " << posY << endl;
		}
	}

	void resizeCallback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void init(const std::string& resourceDirectory)
	{


		GLSL::checkVersion();

		
		// Set background color.
		glClearColor(0.12f, 0.34f, 0.56f, 1.0f);

		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);

		//culling:
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		//transparency
		glEnable(GL_BLEND);
		//next function defines how to mix the background color with the transparent pixel in the foreground. 
		//This is the standard:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

		// Initialize the GLSL program.
		prog = make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(resourceDirectory + "/simple_vert.glsl", resourceDirectory + "/simple_frag.glsl");
		if (! prog->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		prog->init();
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");
		prog->addUniform("campos");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");
		prog->addAttribute("vertTex");
		prog->addAttribute("vertTan");
		prog->addAttribute("vertBinorm");


	}

	void initGeom(const std::string& resourceDirectory)
	{
		// Initialize mesh.
		shape = make_shared<Shape>();
		shape->loadMesh(resourceDirectory + "/sphere.obj");
		shape->resize(); 
		shape->calc_SxT();
		shape->init();

		
		
		int width, height, channels;
		char filepath[1000];

		//texture 1
		string str = resourceDirectory + "/earth.jpg";
		strcpy(filepath, str.c_str());		
		unsigned char* data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &Texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		//texture 2
		str = resourceDirectory + "/earth_normalmap.jpg";
		strcpy(filepath, str.c_str());
		data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &Texture2);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Texture2);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		//[TWOTEXTURES]
		//set the 2 textures to the correct samplers in the fragment shader:
		GLuint Tex1Location = glGetUniformLocation(prog->pid, "tex");//tex, tex2... sampler in the fragment shader
		GLuint Tex2Location = glGetUniformLocation(prog->pid, "tex2");
		// Then bind the uniform samplers to texture units:
		glUseProgram(prog->pid);
		glUniform1i(Tex1Location, 0);
		glUniform1i(Tex2Location, 1);
	}

	void render()
	{
		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		float aspect = width / (float)height;
		glViewport(0, 0, width, height);

		auto P = std::make_shared<MatrixStack>();
		P->pushMatrix();	
		P->perspective(70., width, height, 0.1, 100.0f);
		glm::mat4 M,V;		
		static float angle = 0;
		angle += 0.001;

		M = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -3));		
		glm::mat4 Ry = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0));
		float pih = 3.1415926 / 2.0;
		glm::mat4 Rx = glm::rotate(glm::mat4(1.f), pih, glm::vec3(1, 0, 0));

		M = M * Ry * Rx;

		V = mycam.process();

		
		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Draw mesh using GLSL
		prog->bind();		
		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));	
		glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, &V[0][0]);
		glUniform3fv(prog->getUniform("campos"), 1,  &mycam.pos.x);
		shape->draw(prog);
			
		prog->unbind();
	}
};
//*********************************************************************************************************
int main(int argc, char **argv)
{
	// Where the resources are loaded from
	std::string resourceDir = "../../resources";

	if (argc >= 2)
	{
		resourceDir = argv[1];
	}


	Application *application = new Application();

	// Your main will always include a similar set up to establish your window
	// and GL context, etc.

	WindowManager *windowManager = new WindowManager();
	windowManager->init(640, 480);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state

	application->init(resourceDir);
	application->initGeom(resourceDir);

	// Loop until the user closes the window.
	while (! glfwWindowShouldClose(windowManager->getHandle()))
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



void GetTangent(
	glm::vec3 *posA, glm::vec3 *posB, glm::vec3 *posC,
	glm::vec2 *texA, glm::vec2 *texB, glm::vec2 *texC,
	glm::vec3 *nnA, glm::vec3 *nnB, glm::vec3 *nnC,
	glm::vec3 *tanA, glm::vec3 *tanB, glm::vec3 *tanC);

//calculate tangents and binormals
void computeTangentSpace(Shape *shape)
{
	GLfloat* tangents = new GLfloat[shape->posBuf.size()]();
	GLfloat* binormals = new GLfloat[shape->posBuf.size()]();

	std::vector<glm::vec3 > tangent;
	std::vector<glm::vec3 > binormal;
	int im = 0;

	for (unsigned int i = 0; i < shape->eleBuf.size(); i = i + 3) {

		if (shape->eleBuf.at(i + 0) > im)			im = shape->eleBuf.at(i + 0);
		if (shape->eleBuf.at(i + 1) > im)			im = shape->eleBuf.at(i + 1);
		if (shape->eleBuf.at(i + 2) > im)			im = shape->eleBuf.at(i + 2);
		


		glm::vec3 vertex0 = glm::vec3(shape->posBuf.at(shape->eleBuf.at(i)*3),		shape->posBuf.at(shape->eleBuf.at(i) * 3 + 1), shape->posBuf.at(shape->eleBuf.at(i) * 3 + 2));
		glm::vec3 vertex1 = glm::vec3(shape->posBuf.at(shape->eleBuf.at(i + 1) * 3),	shape->posBuf.at(shape->eleBuf.at(i + 1) * 3 + 1), shape->posBuf.at(shape->eleBuf.at(i + 1) * 3 + 2));
		glm::vec3 vertex2 = glm::vec3(shape->posBuf.at(shape->eleBuf.at(i + 2) * 3),	shape->posBuf.at(shape->eleBuf.at(i + 2) * 3 + 1), shape->posBuf.at(shape->eleBuf.at(i + 2) * 3 + 2));

		glm::vec3 normal0 = glm::vec3(shape->norBuf.at(shape->eleBuf.at(i) * 3),		shape->norBuf.at(shape->eleBuf.at(i) * 3 + 1),		shape->norBuf.at(shape->eleBuf.at(i) * 3 + 2));
		glm::vec3 normal1 = glm::vec3(shape->norBuf.at(shape->eleBuf.at(i + 1) * 3),	shape->norBuf.at(shape->eleBuf.at(i + 1) * 3 + 1),	shape->norBuf.at(shape->eleBuf.at(i + 1) * 3 + 2));
		glm::vec3 normal2 = glm::vec3(shape->norBuf.at(shape->eleBuf.at(i + 2) * 3),	shape->norBuf.at(shape->eleBuf.at(i + 2) * 3 + 1),	shape->norBuf.at(shape->eleBuf.at(i + 2) * 3 + 2));

		glm::vec2 tex0 = glm::vec2(shape->texBuf.at(shape->eleBuf.at(i) * 2),			shape->texBuf.at(shape->eleBuf.at(i) * 2 + 1));
		glm::vec2 tex1 = glm::vec2(shape->texBuf.at(shape->eleBuf.at(i + 1) * 2),		shape->texBuf.at(shape->eleBuf.at(i + 1) * 2 + 1));
		glm::vec2 tex2 = glm::vec2(shape->texBuf.at(shape->eleBuf.at(i + 2) * 2),		shape->texBuf.at(shape->eleBuf.at(i + 2) * 2 + 1));

		glm::vec3 tan0, tan1, tan2; // tangents
		glm::vec3 bin0, bin1, bin2; // binormal

	
		GetTangent(&vertex0, &vertex1, &vertex2, &tex0, &tex1, &tex2, &normal0, &normal1, &normal2, &tan0, &tan1, &tan2);
		
		
		bin0 = glm::normalize(glm::cross(tan0, normal0));
		bin1 = glm::normalize(glm::cross(tan1, normal1));
		bin2 = glm::normalize(glm::cross(tan2, normal2));

		// write into array - for each vertex of the face the same value
		tangents[shape->eleBuf.at(i) * 3] = tan0.x;
		tangents[shape->eleBuf.at(i) * 3 + 1] = tan0.y;
		tangents[shape->eleBuf.at(i) * 3 + 2] = tan0.z;

		tangents[shape->eleBuf.at(i + 1) * 3] = tan1.x;
		tangents[shape->eleBuf.at(i + 1) * 3 + 1] = tan1.y;
		tangents[shape->eleBuf.at(i + 1) * 3 + 2] = tan1.z;

		tangents[shape->eleBuf.at(i + 2) * 3] = tan2.x;
		tangents[shape->eleBuf.at(i + 2) * 3 + 1] = tan2.y;
		tangents[shape->eleBuf.at(i + 2) * 3 + 1] = tan2.z;

		binormals[shape->eleBuf.at(i) * 3] = bin0.x;
		binormals[shape->eleBuf.at(i) * 3 + 1] = bin0.y;
		binormals[shape->eleBuf.at(i) * 3 + 2] = bin0.z;

		binormals[shape->eleBuf.at(i + 1) * 3] = bin1.x;
		binormals[shape->eleBuf.at(i + 1) * 3 + 1] = bin1.y;
		binormals[shape->eleBuf.at(i + 1) * 3 + 2] = bin1.z;

		binormals[shape->eleBuf.at(i + 2) * 3] = bin2.x;
		binormals[shape->eleBuf.at(i + 2) * 3 + 1] = bin2.y;
		binormals[shape->eleBuf.at(i + 2) * 3 + 1] = bin2.z;
	}
	// Copy the tangent and binormal to meshData
	for (unsigned int i = 0; i < shape->posBuf.size(); i++) {
		shape->tanBuf.push_back(tangents[i]);
		shape->binormBuf.push_back(binormals[i]);
	}
}


void GetTangent(
	glm::vec3 *posA, glm::vec3 *posB, glm::vec3 *posC,
	glm::vec2 *texA, glm::vec2 *texB, glm::vec2 *texC,
	glm::vec3 *nnA, glm::vec3 *nnB, glm::vec3 *nnC,
	glm::vec3 *tanA, glm::vec3 *tanB, glm::vec3 *tanC)
{

	if (!posA || !posB || !posC) return;
	if (!texA || !texB || !texC) return;
	if (!nnA || !nnB || !nnC) return;

	glm::vec3 v1 = *posA;
	glm::vec3 v2 = *posB;
	glm::vec3 v3 = *posC;

	glm::vec2 w1 = *texA;
	glm::vec2 w2 = *texB;
	glm::vec2 w3 = *texC;





	float x1 = v2.x - v1.x;
	float x2 = v3.x - v1.x;
	float y1 = v2.y - v1.y;
	float y2 = v3.y - v1.y;
	float z1 = v2.z - v1.z;
	float z2 = v3.z - v1.z;

	float s1 = w2.x - w1.x;
	float s2 = w3.x - w1.x;
	float t1 = w2.y - w1.y;
	float t2 = w3.y - w1.y;
	float inf = (s1 * t2 - s2 * t1);
	if (inf == 0)	inf = 0.0001;
	float r = 1.0F / inf;
	glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
		(t2 * z1 - t1 * z2) * r);
	glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
		(s1 * z2 - s2 * z1) * r);


	glm::vec3 erg;

	erg = sdir - (*nnA) * glm::dot(*nnA, sdir);
	erg =glm::normalize(erg);

	*tanA = erg;
	if (tanB)
	{
		erg = sdir - (*nnB) * glm::dot(*nnB, sdir);
		erg = glm::normalize(erg);
		*tanB = erg;
	}
	if (tanC)
	{
		erg = sdir - (*nnC) * glm::dot(*nnC, sdir);
		erg = glm::normalize(erg);
		*tanC = erg;
	}

}
