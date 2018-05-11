
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "camera.hpp"
#include "model.hpp"

void testLight(Shader& shader) {

	shader.setBool("dirSet", true);
	shader.setVec3("dirLight.direction", glm::vec3(1.0f, -1.0f, -0.3f));
	shader.setVec3("dirLight.ambient", glm::vec3(0.1f, 0.05f, 0.05f));
	shader.setVec3("dirLight.diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
	shader.setVec3("dirLight.specular", glm::vec3(0.2f, 0.2f, 0.2f));




	// LIGHT CLASS STUFF
	/*shader.setBool("light.isDirection", lightToggle);
	shader.setVec3("light.position", camera.Position);
	shader.setVec3("light.direction", camera.Front);
	shader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
	shader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
	shader.setVec3("light.ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
	shader.setVec3("light.specular", 0.5f, 0.5f, 0.5f);

	// light 600 distance

	shader.setFloat("light.constant", 1.0f);
	shader.setFloat("light.linear", 0.007);
	shader.setFloat("light.quadratic", 0.0002);
	shader.setVec3("viewPos", camera.Position);*/
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 30.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool lightToggle = false;

int main() {

	if (!glfwInit())
	{
		// Initialization failed
		return -4;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "framework", glfwGetPrimaryMonitor(), nullptr); // glfwGetPrimaryMonitor()
	if (!window)
	{
		// Window or OpenGL context creation failed
		return -3;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	// bind glfw events to custom functions
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	glewExperimental = GL_FALSE;
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "OpenGL Error: " << error << std::endl;
	}

	if (glewInit() != GLEW_OK) {
		return -2;
		glfwTerminate();
	}
	


	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glfwSwapInterval(1);

	Model model("assets/models/old\ man/muro.obj");
	Model model2("assets/models/nano/nanosuit.obj");
	Model city("assets/models/box.obj");
	Shader shader("shaders/testvertex.vert", "shaders/testfragment.frag");
	
	float lastFrame = 0;

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


																	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	
	city.scale(0.1f);
	city.translate(glm::vec3(-150, -1050, 500));

	float lightX = 0;
	
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// render
		// ------
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
	
		testLight(shader);

		shader.setInt("spotCount", 1);
		shader.setVec3("spotLight[0].position", camera.Position);
		shader.setVec3("spotLight[0].direction", camera.Front);
		shader.setVec3("spotLight[0].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setVec3("spotLight[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		shader.setVec3("spotLight[0].specular", glm::vec3(0.6f, 0.6f, 0.6f));
		shader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(16.0f)));
		shader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(20.0f)));
		shader.setFloat("spotLight[0].constant", 1.0f);
		shader.setFloat("spotLight[0].linear", 0.007);
		shader.setFloat("spotLight[0].quadratic", 0.0002);

		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		//model.translate(glm::vec3(-20 * deltaTime, 0, 0));
		//model.rotate(40 * deltaTime, glm::vec3(0, 1, 0));
		shader.setMat4("model", model.getTransform());
		model.Draw(shader);
		shader.setMat4("model", model2.getTransform());
		model2.Draw(shader);
		shader.setMat4("model", city.getTransform());
		city.Draw(shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;

}

void error_callback(int error, const char* description)
{
	std::cout << "Error " << error << ": " << description;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
		
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

	camera.ProcessMouseMovement(xoffset*deltaTime, yoffset*deltaTime);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}