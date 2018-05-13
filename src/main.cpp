#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "camera.hpp"
#include "objectModel.hpp"
#include "terrainModel.hpp"
#include "skyboxModel.hpp"
#include "light.hpp"
#include "textController.hpp"


// static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
KeyInput getKeyInput(GLFWwindow *window);
void updateWithInput(KeyInput keys, GLFWwindow *window);
std::string currentSeasonToString(int currentSeason, float seasonLerp);
float lerp(float v0, float v1, float t);
std::string currentTimeToString(int currentTime, float currentTimeLerp);


// camera
Camera camera(glm::vec3(0.0f, 10.0f, 30.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


/*-----------CURRENTLY DEBUGGING-----------*/

TextController text;

/*-----------CURRENTLY DEBUGGING-----------*/

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool lightToggle = false;
bool drawContour = false;

bool lerpSeasons = true;
float currentSeasonLerp = 0.0f;
int currentSeason = 0;

bool lerpTime = true;
int currentTime = 0;
float currentTimeLerp = 0.0f;

int currentScreenHeight = SCR_HEIGHT;
int currentScreenWidth	= SCR_WIDTH;

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

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "framework", nullptr, nullptr); // glfwGetPrimaryMonitor()
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

	glewExperimental = GL_TRUE;
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

	//Model model("assets/models/old\ man/muro.obj");
	objectModel model("assets/models/model/ask21mi.obj");
	terrainModel terrain("assets/heightmap/height100.png");
	terrain.scale(0.2f);
	skyboxModel skybox("assets/skybox/skybox", ".jpg");

	Shader shader("shaders/testvertex.vert", "shaders/testfragment.frag");
	Shader terrainShader("shaders/terrainVertex.vert", "shaders/terrainFragment.frag");
	Shader skyboxShader("shaders/skyboxVertex.vert", "shaders/skyboxFragment.frag");
	Shader textShader("shaders/freetypeVertex.vert", "shaders/freetypeFragment.frag");
	terrainShader.setOnlyMaterials(true);
	float lastFrame = 0;

	Light light;
	light.init();

	text.init();
	//light.initSpotLight();

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


																	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

	//model.translate(glm::vec3(0.25f*504, 30.0f, 0.25f*1004));
	//model.translate(glm::vec3(0.03f * 504, 30.0f, 0.50f * 1004));
	model.translate(glm::vec3(-0.03f*504, 30.0f, 0.50f * 1004));
	model.scale(2.0f);
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		if (lerpSeasons) {
			currentSeasonLerp += (deltaTime / seasonTimeScaleInSeconds);

			if (currentSeasonLerp >= 1.0f) {
				currentSeasonLerp = 0.0f;
				if (currentSeason < 3)		currentSeason++;
				else if (currentSeason == 3)currentSeason = 0;
			}
		}

		if (lerpTime) {
			currentTimeLerp += (deltaTime / dayLightTimeScaleInSeconds);

			if (currentTimeLerp >= 1.0f) {
				currentTimeLerp = 0.0f;
				if (currentTime < 3)		currentTime++;
				else if (currentTime == 3)	currentTime = 0;
			}
		}

		light.lerpLight(currentTime, currentTimeLerp);
		

		// render
		// ------

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);


		glm::mat4 view = camera.GetViewMatrix();
		shader.use();
		light.sendLightToShader(shader, camera);

		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		model.rotate(25 * deltaTime, glm::vec3(0, 1, 0));
		model.translate(glm::vec3(-15 * deltaTime, 0, 0));
		model.Draw(shader);

		terrainShader.use();
		light.sendLightToShader(terrainShader, camera);
	
		terrainShader.setVec3("viewPos", camera.Position);
		terrainShader.setMat4("projection", projection);
		terrainShader.setMat4("view", view);
		terrainShader.setInt("currentSeasonId", currentSeason);
		terrainShader.setFloat("seasonLerpPos", currentSeasonLerp);
		terrainShader.setBool("contourLines", drawContour);
		terrain.Draw(terrainShader);

		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skybox.Draw(skyboxShader, view, projection);

		float screenHeight = currentScreenHeight - currentScreenHeight / 15.0f;
		/*
		------- Render Text -------
		(Shader, textToDisplay, startX, startY, scale, vec3 color)
		*/

		std::string seasonAsText = currentSeasonToString(currentSeason, currentSeasonLerp);
		text.RenderText(textShader, seasonAsText,
		25.0f, screenHeight, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		text.RenderText(textShader, seasonAsText,
		20.0f, screenHeight, 1.05f, glm::vec3(0.1f, 0.1f, 0.1f));

		std::string timeAsText = currentTimeToString(currentTime, currentTimeLerp);
		text.RenderText(textShader, timeAsText,
		currentScreenWidth - currentScreenWidth/10.0f, screenHeight, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		text.RenderText(textShader, timeAsText,
		currentScreenWidth - currentScreenWidth / 10.0f - 5.0f, screenHeight, 1.05f, glm::vec3(0.1f, 0.1f, 0.1f));
		
		KeyInput keyInput;
		glfwSwapBuffers(window);
		glfwPollEvents();

		keyInput = getKeyInput(window);
		updateWithInput(keyInput, window);
		
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;

}

void error_callback(int error, const char* description)
{
	std::cout << "Error " << error << ": " << description;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	text.setProjection(width, height);
	currentScreenHeight = height;
	currentScreenWidth	= width;
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

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		drawContour = !drawContour;
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		lerpSeasons = !lerpSeasons;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		currentSeasonLerp = 0.0f;
		currentSeason = 3;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		currentSeasonLerp = 0.0f;
		currentSeason = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		currentSeasonLerp = 0.0f;
		currentSeason = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		currentSeasonLerp = 0.0f;
		currentSeason = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
		currentTimeLerp = 0.0f;
		currentTime = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
		currentTimeLerp = 0.0f;
		currentTime = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
		currentTimeLerp = 0.0f;
		currentTime = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
		currentTimeLerp = 0.0f;
		currentTime = 3;
	}
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		lerpTime = !lerpTime;
	}
}

KeyInput getKeyInput(GLFWwindow *window) {
	KeyInput returnInput;
	returnInput.wKey		= glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
	returnInput.sKey		= glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
	returnInput.aKey		= glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
	returnInput.dKey		= glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
	return returnInput;
};


void updateWithInput(KeyInput keys, GLFWwindow *window) {
	if (keys.wKey)		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys.sKey)		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys.aKey)		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys.dKey)		camera.ProcessKeyboard(RIGHT, deltaTime);
};

std::string currentSeasonToString(int currentSeason, float seasonLerp) {
	if (currentSeason == 0) {
		if		(seasonLerp < 0.33f) return "Winter, December";
		else if (seasonLerp < 0.67f) return "Winter, January";
		else if (seasonLerp <= 1.0f) return "Winter, February";
	}
	else if (currentSeason == 1) {
		if		(seasonLerp < 0.33f) return "Spring, March";
		else if (seasonLerp < 0.67f) return "Spring, April";
		else if (seasonLerp <= 1.0f) return "Spring, May";
	}

	else if (currentSeason == 2) {
		if		(seasonLerp < 0.33f) return "Summer, June";
		else if (seasonLerp < 0.67f) return "Summer, July";
		else if (seasonLerp <= 1.0f) return "Summer, August";
	}
	else if (currentSeason == 3) {
		if		(seasonLerp < 0.33f) return "Autumn, September";
		else if (seasonLerp < 0.67f) return "Autumn, October";
		else if (seasonLerp <= 1.0f) return "Autumn, November";
	}
};

std::string currentTimeToString(int currentTime, float currentTimeLerp) {
	std::string returnTime = "";
	int time;

	if		(currentTime == 0) time = lerp(0, 360, currentTimeLerp);
	else if (currentTime == 1) time = lerp(360, 720, currentTimeLerp);
	else if (currentTime == 2) time = lerp(720, 1080, currentTimeLerp);
	else if (currentTime == 3) time = lerp(1080, 1440, currentTimeLerp);


	int hours	= time / 60;
	int minutes = time % 60;

	if (hours < 10) {
		returnTime = "0";
		returnTime.append(std::to_string(hours));
	}
	else {
		returnTime = std::to_string(hours);
	}
	
	returnTime.append(":");

	if(minutes < 10) returnTime.append("0");
	returnTime.append(std::to_string(minutes));

	return returnTime;
};

float lerp(float v0, float v1, float t) {
	return (1 - t) * v0 + t * v1;
};