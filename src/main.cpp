#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "camera.hpp"
#include "objectModel.hpp"
#include "terrainModel.hpp"
#include "skyboxModel.hpp"
#include "light.hpp"
#include "textController.hpp"
#include "planeModel.hpp"


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

TextController text;


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
bool resetPlanePosition = false;
bool randomizePlanePosition = false;

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

	/*-----------CURRENTLY DEBUGGING-----------*/

	planeModel plane("assets/models/model/ask21mi.obj");

	/*-----------CURRENTLY DEBUGGING-----------*/


	terrainModel terrain("assets/heightmap/height50.png");
	//terrain.scale(0.2f);
	plane.getMidPoint(terrain.calculateMidPoint());
	skyboxModel skybox("assets/skybox/skybox", ".jpg");

	Shader shader("shaders/modelVertex.vert", "shaders/modelFragment.frag");
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
	glm::mat4 projection =	glm::perspective(glm::radians(camera.Zoom), 
							(float)currentScreenWidth/(float)currentScreenHeight, 
							MIN_RENDER_DISTANCE, MAX_RENDER_DISTANCE);

	//model.translate(glm::vec3(0.25f*504, 30.0f, 0.25f*1004));
	//model.translate(glm::vec3(0.03f * 504, 30.0f, 0.50f * 1004));
	plane.resetToOriginalPosition();
	//plane.scale(2.0f);
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

		if (camera.cameraState == FOLLOWPLANE) {
			camera.followPlane(plane.getTransform());
		}

		if (resetPlanePosition) {
			plane.resetToOriginalPosition();
			resetPlanePosition = false;
		}
		if (randomizePlanePosition) {
			plane.setNewPosition();
			randomizePlanePosition = false;
		}

		light.lerpLight(currentTime, currentTimeLerp);
		

		// render
		// ------

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection =	glm::perspective(glm::radians(camera.Zoom), 
								(float)currentScreenWidth / (float)currentScreenHeight,
								MIN_RENDER_DISTANCE, MAX_RENDER_DISTANCE);

		glm::mat4 view = camera.GetViewMatrix();
		shader.use();
		light.sendLightToShader(shader, camera);

		shader.setMat4("projection", projection);
		shader.setMat4("view", view);


		plane.Draw(shader);
		//plane.rotate(25 * deltaTime, glm::vec3(0, 1, 0));
		//plane.translate(glm::vec3(-15 * deltaTime, 0, 0));

		terrainShader.use();
		light.sendLightToShader(terrainShader, camera);
	
		terrainShader.setVec3("viewPos", camera.Position);
		terrainShader.setMat4("projection", projection);
		terrainShader.setMat4("view", view);
		terrainShader.setInt("currentSeasonId", currentSeason);
		terrainShader.setFloat("seasonLerpPos", currentSeasonLerp);
		terrainShader.setBool("contourLines", drawContour);
		terrainShader.setFloat("MAX_HEIGHT", (float)LEVEL_MAX_HEIGHT);
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
		plane.update(deltaTime, keyInput);
		
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
	if (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_PRESS) {
		// Debug and realized GLFW_KEY_MINUS is + on european keyboard?
		if(camera.cameraState == FOLLOWPLANE) 
			camera.cameraState = FREEROAM;
		else if(camera.cameraState == FREEROAM) 
			camera.cameraState = RESTRICTEDCAM;
		else if (camera.cameraState == RESTRICTEDCAM) 
			camera.cameraState = FOLLOWPLANE;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		resetPlanePosition = true;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		randomizePlanePosition = true;
	}
}

KeyInput getKeyInput(GLFWwindow *window) {
	KeyInput returnInput;
	returnInput.wKey		= glfwGetKey(window, GLFW_KEY_W)		== GLFW_PRESS;
	returnInput.sKey		= glfwGetKey(window, GLFW_KEY_S)		== GLFW_PRESS;
	returnInput.aKey		= glfwGetKey(window, GLFW_KEY_A)		== GLFW_PRESS;
	returnInput.dKey		= glfwGetKey(window, GLFW_KEY_D)		== GLFW_PRESS;
	
	returnInput.up			= glfwGetKey(window, GLFW_KEY_UP)		== GLFW_PRESS;
	returnInput.down		= glfwGetKey(window, GLFW_KEY_DOWN)		== GLFW_PRESS;
	returnInput.left		= glfwGetKey(window, GLFW_KEY_LEFT)		== GLFW_PRESS;
	returnInput.right		= glfwGetKey(window, GLFW_KEY_RIGHT)	== GLFW_PRESS;
	
	returnInput.iKey		= glfwGetKey(window, GLFW_KEY_I)		== GLFW_PRESS;
	returnInput.kKey		= glfwGetKey(window, GLFW_KEY_K)		== GLFW_PRESS;
	returnInput.jKey		= glfwGetKey(window, GLFW_KEY_J)		== GLFW_PRESS;
	returnInput.lKey		= glfwGetKey(window, GLFW_KEY_L)		== GLFW_PRESS;
	returnInput.yKey		= glfwGetKey(window, GLFW_KEY_Y)		== GLFW_PRESS;
	returnInput.hKey		= glfwGetKey(window, GLFW_KEY_H)		== GLFW_PRESS;

	returnInput.commaKey	= glfwGetKey(window, GLFW_KEY_COMMA)	== GLFW_PRESS;
	returnInput.dotKey		= glfwGetKey(window, GLFW_KEY_PERIOD)	== GLFW_PRESS;
	return returnInput;
};


void updateWithInput(KeyInput keys, GLFWwindow *window) {
	if (keys.iKey)		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys.kKey)		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys.jKey)		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys.lKey)		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (keys.yKey)		camera.ProcessKeyboard(UP, deltaTime);
	if (keys.hKey)		camera.ProcessKeyboard(DOWN, deltaTime);
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