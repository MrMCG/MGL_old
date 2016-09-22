#include "stdafx.h"

#include "MGLCamera.h"

MGLCamera::MGLCamera() {
	MapMovementFunctions();
	UpdateCameraVectors();
}

MGLCamera::~MGLCamera() {
	delete movementMap;
}

glm::mat4 MGLCamera::BuildViewMatrix() {
	UpdateCameraVectors();
	return glm::lookAt(positionVec, positionVec + frontVec, upVec);
}

void MGLCamera::UpdateCameraVectors() {
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVec = glm::normalize(newFront);

	rightVec = glm::normalize(glm::cross(frontVec, worldUpVec));
	upVec = glm::normalize(glm::cross(rightVec, frontVec));
}

void MGLCamera::MoveCamera(MGLenum direction, GLfloat dt) {
	(this->*(*movementMap)[direction])(dt);
}

/***** MGLCamera directions *****/

void MGLCamera::MoveForward(GLfloat dt) {
	GLfloat vel = moveSpeed * dt;
	positionVec += frontVec * vel;
}

void MGLCamera::MoveBackward(GLfloat dt) {
	MoveForward(-dt);
}

void MGLCamera::MoveLeft(GLfloat dt) {
	GLfloat vel = moveSpeed * dt;
	positionVec -= rightVec * vel;
}

void MGLCamera::MoveRight(GLfloat dt) {
	MoveLeft(-dt);
}

void MGLCamera::MoveUp(GLfloat dt) {
	GLfloat vel = moveSpeed * dt;
	positionVec += upVec * vel;
}

void MGLCamera::MoveDown(GLfloat dt) {
	MoveUp(-dt);
}

void MGLCamera::MoveYaw(GLfloat offset) {
	yaw += offset * yawSpeed;
}

void MGLCamera::MovePitch(GLfloat offset) {
	pitch += offset * pitchSpeed;
	pitch = glm::min(pitch, 89.0f);
	pitch = glm::max(pitch, -89.0f);
}

void MGLCamera::MoveZoom(GLfloat amount) {
	if (zoom >= 10.0f && zoom <= 65.0f)
		zoom -= amount;

	if (zoom <= 10.0f)
		zoom = 10.0f;

	if (zoom >= 65.0f)
		zoom = 65.0f;

	std::cout << "ZOOM : " << zoom << std::endl;
}

void MGLCamera::MapMovementFunctions() const {
	movementMap->insert(std::make_pair(Forward, &MGLCamera::MoveForward));
	movementMap->insert(std::make_pair(Backward, &MGLCamera::MoveBackward));
	movementMap->insert(std::make_pair(Left, &MGLCamera::MoveLeft));
	movementMap->insert(std::make_pair(Right, &MGLCamera::MoveRight));
	movementMap->insert(std::make_pair(Up, &MGLCamera::MoveUp));
	movementMap->insert(std::make_pair(Down, &MGLCamera::MoveDown));
	movementMap->insert(std::make_pair(Pitch, &MGLCamera::MovePitch));
	movementMap->insert(std::make_pair(Yaw, &MGLCamera::MoveYaw));
	movementMap->insert(std::make_pair(Zoom, &MGLCamera::MoveZoom));
}
