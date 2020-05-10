#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <thread> //ALSO USE: #include <mutex>

#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"

#include <unistd.h> // usleep(1000);

float g_X = 0.0f;
float g_Y = 0.0f;
float g_Z = 0.0f;

/*
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
	
	// std::cout << key << std::endl;
	
	switch(key){
		case GLFW_KEY_W: g_Y += add; break;
		case GLFW_KEY_A: g_X += add; break;
		case GLFW_KEY_S: g_Y -= add; break;
		case GLFW_KEY_D: g_X -= add; break;
		case GLFW_KEY_SPACE: break;
	}
}
/**/

float move_add = 0.25;

bool g_CLOSE = false;

void thread_w(GLFWwindow* window){
	while(!g_CLOSE){
		usleep(10000);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){ g_Y += move_add; }
	}
}
void thread_a(GLFWwindow* window){
	while(!g_CLOSE){
		usleep(10000);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){ g_X += move_add; }
	}
}
void thread_s(GLFWwindow* window){
	while(!g_CLOSE){
		usleep(10000);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){ g_Y -= move_add; }
	}
}
void thread_d(GLFWwindow* window){
	while(!g_CLOSE){
		usleep(10000);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){ g_X -= move_add; }
	}
}
void thread_q(GLFWwindow* window){
	while(!g_CLOSE){
		usleep(10000);
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){ g_Z += move_add; }
	}
}
void thread_e(GLFWwindow* window){
	while(!g_CLOSE){
		usleep(10000);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){ g_Z -= move_add; }
	}
}

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

int main(int argc, char** argv){

	GLFWwindow* window;
	if (!glfwInit()){ std::cerr << "!glfwInit()" << std::endl; return -1; }					    //initialize the library
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "WINDOW_TITLE", NULL, NULL);		    //create a windowed mode window and its OpenGL context
	if (!window){ std::cerr << "!window" << std::endl; glfwTerminate(); return -1; }		    //check if window is valid
	glfwSetWindowSizeLimits(window, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT);  //set min/max size
	glfwMakeContextCurrent(window);															    //make the window's context current
	glfwSwapInterval(1);                                                                        //sync (fps) with monitor refresh rate (vsync)
	if (glewInit() != GLEW_OK){ std::cerr << "ERROR: glewInit() != GLEW_OK" << std::endl; }	    //check if glew is initialized
	std::cout << glGetString(GL_VERSION) << std::endl;										    //print OpenGL version

	//DONT DRAW THE BACK SIDE OF MESHES
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	// glfwSetKeyCallback(window, keyCallback);
	// glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	
	std::cout << "loading 3D OBJ model..." << std::endl;
	Mesh mesh2("./res/object.obj");
	std::cout << "loaded 3D OBJ model" << std::endl;
	
	Shader shader("./res/basicShader");
	Texture texture("./res/tex.jpg");
	Camera camera(glm::vec3(0, -2, -10), 70.0f, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.01f, 1000.0f);
	Transform transform;

	std::thread t_W(thread_w, window);
	std::thread t_A(thread_a, window);
	std::thread t_S(thread_s, window);
	std::thread t_D(thread_d, window);
	std::thread t_Q(thread_q, window);
	std::thread t_E(thread_e, window);
	t_W.detach();
	t_A.detach();
	t_S.detach();
	t_D.detach();
	t_Q.detach();
	t_E.detach();
	
	float counter = 0.0f;
    while(!glfwWindowShouldClose(window)){
		
		glClearColor(0.0f, 0.5f, 0.1f, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// float sinCounter = sinf(counter);
		// float cosCounter = cosf(counter);
		// transform.GetPos().x = sinCounter;
		// transform.GetPos().z = cosCounter;
		
		transform.GetPos().x = g_X;
		transform.GetPos().y = g_Y;
		transform.GetPos().z = g_Z;
	
		// transform.GetRot().x = sinCounter * 15; // cosf(counter * 10);
		transform.GetRot().y = counter * 8;
		// transform.GetRot().z = counter * 8;
		// transform.SetScale(glm::vec3(counter, counter, counter));

		shader.Bind();
		// texture.Bind(0);
		shader.Update(transform, camera);
		mesh2.Draw();

		//update
        glfwSwapBuffers(window); //swap front and back buffers
		glfwPollEvents();        //poll for and process events

		counter += 0.005f;
    }
	
	g_CLOSE = true;

    return 0;
}