#include "stdafx.h"
#pragma once

#include <unordered_map>

class MGLCamera {
public: 
	MGLCamera();
	virtual ~MGLCamera() {};

	typedef void(MGLCamera::*MGLFloatFunction)(GLfloat);

	glm::mat4 BuildViewMatrix();

	void SetYaw(GLfloat yaw) { m_yaw = yaw; }
	void SetPitch(GLfloat pitch) { m_pitch = pitch; }
	void SetZoom(GLfloat zoom) { m_zoom = zoom; }

	void SetMoveSpeed(GLfloat speed) { m_moveSpeed = speed; }
	void SetPitchSpeed(GLfloat speed) { m_pitchSpeed = speed; }
	void SetYawSpeed(GLfloat speed) { m_yawSpeed = speed; }

	void SetPosition(glm::vec3 position) { m_position = position; }
	void SetWorldUp(glm::vec3 worldUp) { m_worldUp = worldUp; }

	GLfloat GetYaw() const { return m_yaw; }
	GLfloat GetPitch() const { return m_pitch; }
	GLfloat GetZoom() const { return m_zoom; }

	GLfloat GetMoveSpeed() const { return m_moveSpeed; }
	GLfloat GetPitchSpeed() const { return m_pitchSpeed; }
	GLfloat GetYawSpeed() const { return m_yawSpeed; }

	glm::vec3 GetPosition() const { return m_position; }
	glm::vec3 GetWorldUp() const { return m_worldUp; }

	void MoveCamera(MGLenum direction, GLfloat dt);

protected:
	GLfloat m_pitch;
	GLfloat m_yaw;

	GLfloat m_zoom;

	GLfloat m_moveSpeed;
	GLfloat m_pitchSpeed;
	GLfloat m_yawSpeed;

	glm::vec3 m_position;
	glm::vec3 m_up;
	glm::vec3 m_worldUp;
	glm::vec3 m_right;
	glm::vec3 m_front;

	std::unordered_map<MGLenum, MGLFloatFunction> m_movementMap;

	void UpdateCameraVectors();

	void MoveForward(GLfloat dt);
	void MoveBackward(GLfloat dt);
	void MoveLeft(GLfloat dt);
	void MoveRight(GLfloat dt);
	void MoveUp(GLfloat dt);
	void MoveDown(GLfloat dt);
	void MovePitch(GLfloat offset);
	void MoveYaw(GLfloat offset);
	void Zoom(GLfloat amount);
};