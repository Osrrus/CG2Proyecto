/**
 * Include Area
 */ 
#include "define.h"
#include "Shader.h"
#include "./class/camera.h"
#include "./class/helpers.h"
#include "./class/Obj.h"
#include "components/loader.h"

/**
 * Define Area
 */
#define RADIOSITY_TEXTURE_SIZE 1024

using namespace std;

// Window current width
unsigned int windowWidth = 800;
// Window current height
unsigned int windowHeight = 600;
// Window title
const char *windowTitle = "Proyecto";
// Window pointer
GLFWwindow *window;

Camera *camera;
// Shader object
Shader *shader;
// Index (GPU) of the geometry buffer
unsigned int VBO;
// Index (GPU) vertex array object
unsigned int VAO;
// Index (GPU) of the texture

bool pressLeft;
int pressMenu;

std::vector<Obj*> objects;
DrawParameters drawParameters;

//Framebuffer
unsigned int FBO;
unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
unsigned int texture_m[4];
unsigned int depthTexture;
unsigned int renderBuffer;
int lampCounter = 0;
//Create readback buffers for all of the texture data
std::vector<GLfloat> worldspacePositionDataBuffer(RADIOSITY_TEXTURE_SIZE* RADIOSITY_TEXTURE_SIZE * 3);
std::vector<GLfloat> normalVectorDataBuffer(RADIOSITY_TEXTURE_SIZE* RADIOSITY_TEXTURE_SIZE * 3);

std::vector<GLfloat> idDataBuffer(RADIOSITY_TEXTURE_SIZE* RADIOSITY_TEXTURE_SIZE * 3);
std::vector<GLfloat> uvDataBuffer(RADIOSITY_TEXTURE_SIZE* RADIOSITY_TEXTURE_SIZE * 3);
vector<glm::vec3> lightLocations;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void onMouseButton(GLFWwindow* window, int button, int action, int mods);

glm::mat4 projection,model;

void render();

unsigned int gPosition, gNormal, gAlbedoSpec;

bool initFramebuffer() 
{
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// The texture we're going to render to
	glGenTextures(4, texture_m);
	glGenTextures(1, &depthTexture);
	// position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	// normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	// color + specular color buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

	
	//depth texture
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
		NULL);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);


	// Set the list of draw buffers.
	glDrawBuffers(4, attachments); // "1" is the size of DrawBuffers
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		return false;
	}

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;
}

void renderFramebuffer() 
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, windowWidth, windowHeight);

	//Render
	// Use the shader
	shader->use();
	projection = glm::perspective(glm::radians(60.0f), (float)windowWidth / (float)windowHeight, 0.001f, 100.0f);

	shader->setMat4("View", camera->getWorlToViewMatrix());
	shader->setMat4("Projection", projection);

	// Binds the vertex array to be drawn
	for (int i = 0; i < objects.size(); i++)
	{
		shader->setMat4("Model", objects[i]->model);
		shader->setVec3("color", objects[i]->color);
		objects[i]->Draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// reset viewport
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



/**
 * Handles the window resize
 * @param{GLFWwindow} window pointer
 * @param{int} new width of the window
 * @param{int} new height of the window
 * */
void resize(GLFWwindow *window, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    // Sets the OpenGL viewport size and position
    glViewport(0, 0, windowWidth, windowHeight);
}
/**
 * Initialize the glfw library
 * @returns{bool} true if everything goes ok
 * */
bool initWindow()
{
    // Initialize glfw
    glfwInit();
    // Sets the Opegl context to Opengl 3.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creates the window
    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);

    // The window couldn't be created
    if (!window)
    {
        std::cout << "Failed to create the glfw window" << std::endl;
        glfwTerminate(); // Stops the glfw program
        return false;
    }

    // Creates the glfwContext, this has to be made before calling initGlad()
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, onMouseButton);
    // Window resize callback
    glfwSetFramebufferSizeCallback(window, resize);
    return true;
}
/**
 * Initialize the glad library
 * @returns{bool} true if everything goes ok
 * */
bool initGlad()
{
    // Initialize glad
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // If something went wrong during the glad initialization
    if (!status)
    {
        std::cout << status << std::endl;
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}
/**
 * Initialize the opengl context
 * */
void initGL()
{
    // Enables the z-buffer test
    glEnable(GL_DEPTH_TEST);
    // Sets the ViewPort
    glViewport(0, 0, windowWidth, windowHeight);
    // Sets the clear color
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}
/**
 * Builds all the geometry buffers and
 * loads them up into the GPU
 * (Builds a simple triangle)
 * */
void buildGeometry()
{
   objects.push_back(loadObj("assets/Box.obj"));
   objects.push_back(loadObj("assets/BigBox.obj"));
   objects.push_back(loadObj("assets/MidBox.obj"));
}

/**
 * Initialize everything
 * @returns{bool} true if everything goes ok
 * */
bool init()
{
    // Initialize the window, and the glad components
    if (!initWindow() || !initGlad())
        return false;

    // Initialize the opengl context
    initGL();

    // Loads the shader
    shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");

    camera = new Camera();
    // Loads all the geometry into the GPU
    buildGeometry();
    // Loads the texture into the GPU

	//init frambe buffer
	initFramebuffer();

    return true;
}
/**
 * Process the keyboard input
 * There are ways of implementing this function through callbacks provide by
 * the GLFW API, check the GLFW documentation to find more
 * @param{GLFWwindow} window pointer
 * */
void processKeyboardInput(GLFWwindow *window)
{
    // Checks if the escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        // Tells glfw to close the window as soon as possible
        glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

		camera->moveForward();

	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

		camera->moveBackward();

	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

		camera->strafeLeft();

	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

		camera->strafeRight();

	}
    // Checks if the r key is pressed
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        // Reloads the shader
        delete shader;
        shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    }
}
/**
 * Render Function
 * */
void render()
{
    // Clears the color and depth buffers from the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderFramebuffer();

    /** Draws code goes here **/
    // Use the shader
    shader->use();
    projection = glm::perspective(glm::radians(60.0f), (float)windowWidth / (float)windowHeight,0.001f,100.0f);

    shader->setMat4("View",camera->getWorlToViewMatrix());
    shader->setMat4("Projection",projection);

    // Binds the vertex array to be drawn
    for (int i = 0; i < objects.size(); i++)
    {
        shader->setMat4("Model",objects[i]->model);
        shader->setVec3("color",objects[i]->color);   
        objects[i]->Draw();
    }
    
    // Swap the buffer
    glfwSwapBuffers(window);
}
/**
 * App main loop
 * */
void update()
{
    // Loop until something tells the window, that it has to be closed
    while (!glfwWindowShouldClose(window))
    {
        // Checks for keyboard inputs
        processKeyboardInput(window);

        // Renders everything
        render();

        // Check and call events
        glfwPollEvents();
    }
}
/**
 * App starting point
 * @param{int} number of arguments
 * @param{char const *[]} running arguments
 * @returns{int} app exit code
 * */
int main(int argc, char const *argv[])
{
    // Initialize all the app components
    if (!init())
    {
        // Something went wrong
        std::cin.ignore();
        return -1;
    }

    std::cout << "=====================================================" << std::endl
              << "        Press Escape to close the program            " << std::endl
              << "=====================================================" << std::endl;

    // Starts the app main loop
    update();

    // Deletes the vertex array from the GPU
    glDeleteVertexArrays(1, &VAO);
    // Deletes the vertex object from the GPU
    glDeleteBuffers(1, &VBO);
	glDeleteFramebuffers(1,&FBO);
	glDeleteTextures(1, &depthTexture);
	glDeleteTextures(1, &renderBuffer);
	glDeleteTextures(4, texture_m);

    // Destroy the shader
    delete shader;

    // Stops the glfw program
    glfwTerminate();

    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (pressLeft) {
		camera->mouseUpdate(glm::vec2(xpos, ypos));
	}

	glDeleteRenderbuffers(1, &renderBuffer);	TwMouseMotion(static_cast<int>(xpos), static_cast<int>(ypos));
}

void onMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	auto a = action == GLFW_PRESS ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;
	auto b = TW_MOUSE_LEFT;

	if (a) {

		pressLeft = true;

	}
	else {
		pressLeft = false;
	}

}