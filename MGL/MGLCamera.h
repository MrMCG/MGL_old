#pragma once
#include "stdafx.h"

#include "MGLUtil.h"

#define MGL_CAMERA_FORWARD 0
#define MGL_CAMERA_BACKWARD 1
#define MGL_CAMERA_LEFT 2
#define MGL_CAMERA_RIGHT 3
#define MGL_CAMERA_UP 4
#define MGL_CAMERA_DOWN 5
#define MGL_CAMERA_PITCH 6
#define MGL_CAMERA_YAW 7
#define MGL_CAMERA_ZOOM 8

class MGLCamera {
public: 
	MGLCamera();
	virtual ~MGLCamera() {};

	typedef void(MGLCamera::*MGLFloatFunction)(GLfloat);

	// Created a view matrix based on camera
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

	// Move camera an amount based on update time
	void MoveCamera(MGLenum direction, GLfloat dt);

protected:

	/****** Methods ******/

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

	/****** Data ******/

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
};