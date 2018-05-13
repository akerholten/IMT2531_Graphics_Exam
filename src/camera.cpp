#include "camera.hpp"

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	cameraState = FREEROAM;
	lastPlanePosition = glm::vec3(0.0f);
	waitForPos = 0;
	updateCameraVectors();
}

// Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	cameraState = FREEROAM;
	lastPlanePosition = glm::vec3(0.0f);
	waitForPos = 0;
	updateCameraVectors();
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
	if (cameraState == FOLLOWPLANE) return followPlaneView;
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::followPlane(glm::mat4 planeTransform) {
	glm::mat4 targetTransform;
	glm::quat rotation;
	glm::vec3 planePosition;
	glm::decompose(planeTransform, glm::vec3(), rotation, planePosition, glm::vec3(), glm::vec4());
	glm::vec3 planeMovement = glm::normalize(planePosition - lastPlanePosition);
	/*
	------ FOLLOW PLANE EXPLAINED ------ 
	I was trying to implement quaternion rotations to
	make sure the camera was rotating correctly and positioning correctly 
	behind the plane. I had some issues, so for now the quaternion rotation
	applied to the plane are some-what correct. But for calculating where to
	place the camera I just take an average vector of the plane's last
	movement for the last 10 frames. This gives a pretty decent result, even
	though it is kind of an hack.
	
	To do it properly I wanted to try and calculate the plane's front and up,
	compared to world space. However, as there were som issues, this is how this 
	function works as of now.
	*/

	waitForPos += 1;
	if (waitForPos = 10) {
		lastPlanePosition = planePosition;
		waitForPos = 0;
	}

	//glm::mat4 rotationMatrix = glm::toMat4(rotation);
	glm::vec3 currentUp = glm::normalize(glm::cross(WorldUp, rotation));
	glm::vec3 currentFront = glm::normalize(glm::cross(glm::vec3(-1.0f, 0.0f, 0.0f), rotation));
	Front = currentFront;
	Position = planePosition - (planeMovement * 20.0f) + WorldUp * 5.0f;
	//Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	//Up = glm::normalize(glm::cross(Right, Front));
	followPlaneView = glm::lookAt(Position, planePosition, currentUp);

}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (cameraState == FREEROAM) {
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
	}
	else if (cameraState == RESTRICTEDCAM) {
		glm::vec3 xMove = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 yMove = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 zMove = glm::vec3(0.0f, 0.0f, 1.0f);
		
		if (direction == FORWARD)
			Position += zMove * velocity;
		if (direction == BACKWARD)
			Position -= zMove * velocity;
		if (direction == LEFT)
			Position += xMove * velocity;
		if (direction == RIGHT)
			Position -= xMove * velocity;
		if (direction == UP)
			Position += yMove * velocity;
		if (direction == DOWN)
			Position -= yMove * velocity;
	}
	else if (cameraState == FOLLOWPLANE) {

	}
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}


// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
	if (cameraState != FOLLOWPLANE) {
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
	else {
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
	

}