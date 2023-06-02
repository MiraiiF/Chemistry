#include "../Libraries/Camera.h"

Camera::Camera(glm::vec3 target, glm::vec3 pos) {
		position = pos;
		view = glm::mat4(1.0);
		front = glm::normalize(target - position);
		right = glm::cross(up, front);
		right.y = 0; glm::normalize(right);
		up = glm::cross(front, right);
		view = glm::lookAt(position, position + front, up);
}

Camera::Camera(glm::vec3 pos, GLfloat yaw, GLfloat pitch) {
	position = pos;
	view = glm::mat4(1.0);
	glm::vec3 direction = glm::vec3(sin(glm::radians(yaw)) * cos(glm::radians(pitch)),  sin(glm::radians(pitch)), cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
	front = glm::normalize(direction);
	right = glm::cross(up, front);
	right.y = 0; glm::normalize(right);
	up = glm::cross(front, right);
	view = glm::lookAt(position, position + front, up);
}
