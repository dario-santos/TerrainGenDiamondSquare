#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;


#include "Mesh/Mesh.hpp"

#include "Generate.hpp"

#include "Shader/GLSLProgram.hpp"


#include "Camera/camera.h"

#include <iostream>

Vertex* CreateDefaultTerrain();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// Terrain vars
int iterations = 5;
int d = 3;
int r = 2;

bool up_pressed   = false;
bool down_pressed = false;

Mesh* terrain = nullptr;

int main() 
{
  // Initialization of the GLFW
  glfwInit();

  glfwWindowHint(GLFW_SAMPLES, 4); //4xMSAA
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Opengl 3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Opengl 3.3
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Removes functionality deprecated opengl functions
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // sets the Opengl profile as core, added in 3.2

  // Creates the window
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Diamond-Square", NULL, NULL);

  // Create window context
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  
  // Initialize GLEW
  glewExperimental = GL_TRUE; // Needed for core profile

  // Initialization of GLEW
  glewInit();

  //glClearColor(45.f/255, 52.f/255, 54.f/255, 0.0f);
  glClearColor(229.f / 255, 229.f / 255, 229.f / 255, 0.0f);

  // tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Enable depth test and blend
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  glm::mat4 projection;
  glm::mat4 view;

  GLSLProgram* shader = new GLSLProgram();
  
  try
  {
    shader->compileShader("Shader/TerrainHeatMap.vert");
    shader->compileShader("Shader/TerrainHeatMap.frag");

    shader->link();
    shader->use();
  }
  catch (GLSLProgramException& e)
  {
    std::cerr << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }

  glfwSwapInterval(1);

  terrain = new Mesh(Generator::Generate(CreateDefaultTerrain(), 5, 3, 6), pow(4, 6), shader);

  // std::cout << terrain->ToString() << std::endl;

  // render scene for each frame
  do
  {
    // per-frame time logic
    // --------------------
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----
    processInput(window);

    // Iterate Objects
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
    view = camera.GetViewMatrix();

    // Render terrain
    terrain->Render(view, projection);
  
    // Swap buffers
    glfwSwapBuffers(window);

    // looking for events
    glfwPollEvents();

  } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  return 0;
}

Vertex* CreateDefaultTerrain()
{
  auto A = new Vertex(0, 0, 0);
  auto B = new Vertex(0, 50, 100);
  auto C = new Vertex(100, 0, 0);
  auto D = new Vertex(100, 5, 100);

  A->right = B;
  A->down = C;
  A->diagonal = D;

  B->left = A;
  B->down = D;

  C->up = A;
  C->right = D;

  D->up = B;
  D->left = C;

  return A;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);


    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !up_pressed)
    {
        terrain = new Mesh(Generator::Generate(CreateDefaultTerrain(), ++iterations, d, r), pow(4, iterations + 1), terrain->shader);
        up_pressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && !down_pressed)
    {
        terrain = new Mesh(Generator::Generate(CreateDefaultTerrain(), --iterations, d, r), pow(4, iterations + 1), terrain->shader);
        down_pressed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
        up_pressed = false;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
        down_pressed = false;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
