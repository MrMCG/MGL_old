#include "stdafx.h"
#include "MGLCamera.h"

MGLCamera::MGLCamera() {
	m_pitch = 0.0f;
	m_yaw = -90.0f;

	m_position = glm::vec3(0.0f, 0.0f, 10.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	m_right = glm::vec3(0.0f, 1.0f, 0.0f);
	m_front = glm::vec3(0.0f, 0.0f, -1.0f);

	m_moveSpeed = 3.0f;
	m_pitchSpeed = 0.15f;
	m_yawSpeed = 0.15f;

	m_zoom = 45.0f;

	m_movementMap.insert(std::make_pair(MGL_CAMERA_FORWARD, &MGLCamera::MoveForward));
	m_movementMap.insert(std::make_pair(MGL_CAMERA_BACKWARD, &MGLCamera::MoveBackward));
	m_movementMap.insert(std::make_pair(MGL_CAMERA_LEFT, &MGLCamera::MoveLeft));
	m_movementMap.insert(std::make_pair(MGL_CAMERA_RIGHT, &MGLCamera::MoveRight));
	m_movementMap.insert(std::make_pair(MGL_CAMERA_UP, &MGLCamera::MoveUp));
	m_movementMap.insert(std::make_pair(MGL_CAMERA_DOWN, &MGLCamera::MoveDown));
	m_movementMap.insert(std::make_pair(MGL_CAMERA_PITCH, &MGLCamera::MovePitch));
	m_movementMap.insert(std::make_pair(MGL_CAMERA_YAW, &MGLCamera::MoveYaw));
	m_movementMap.insert(std::make_pair(MGL_CAMERA_ZOOM, &MGLCamera::Zoom));

	UpdateCameraVectors();
}

glm::mat4 MGLCamera::BuildViewMatrix() {
	UpdateCameraVectors();
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

void MGLCamera::UpdateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

void MGLCamera::MoveCamera(MGLenum direction, GLfloat dt) {
	(this->*m_movementMap[direction])(dt);
}

void MGLCamera::MoveForward(GLfloat dt) {
	GLfloat vel = m_moveSpeed * dt;
	m_position += m_front * vel;
}

void MGLCamera::MoveBackward(GLfloat dt) {
	GLfloat vel = m_moveSpeed * dt;
	m_position -= m_front * vel;
}

void MGLCamera::MoveLeft(GLfloat dt) {
	GLfloat vel = m_moveSpeed * dt;
	m_position -= m_right * vel;
}

void MGLCamera::MoveRight(GLfloat dt) {
	GLfloat vel = m_moveSpeed * dt;
	m_position += m_right * vel;
}

void MGLCamera::MoveUp(GLfloat dt) {
	GLfloat vel = m_moveSpeed * dt;
	m_position += m_up * vel;
}

void MGLCamera::MoveDown(GLfloat dt) {
	GLfloat vel = m_moveSpeed * dt;
	m_position -= m_up * vel;
}

void MGLCamera::MoveYaw(GLfloat offset) {
	m_yaw += offset * m_yawSpeed;
}

void MGLCamera::MovePitch(GLfloat offset) {
	m_pitch += offset * m_pitchSpeed;
	m_pitch = glm::min(m_pitch, 89.0f);
	m_pitch = glm::max(m_pitch, -89.0f);
}

void MGLCamera::Zoom(GLfloat amount) {
	if (m_zoom >= 44.0f && m_zoom <= 45.0f)
		m_zoom -= amount;

	if (m_zoom <= 44.0f)
		m_zoom = 44.0f;

	if (m_zoom >= 45.0f)
		m_zoom = 45.0f;

	std::cout << "ZOOM : " << m_zoom << std::endl;
}