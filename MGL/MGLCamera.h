#pragma once
#include "stdafx.h"

#include "MGLUtil.h"

class MGLCamera {
public: 
	MGLCamera();
	virtual ~MGLCamera();

	typedef void(MGLCamera::*MGLFloatFunction)(GLfloat);

	glm::mat4 BuildViewMatrix();

	void SetYaw(GLfloat yw) { yaw = yw; }
	void SetPitch(GLfloat ptch) { pitch = ptch; }
	void SetZoom(GLfloat zom) { zoom = zom; }

	void SetMoveSpeed(GLfloat speed) { moveSpeed = speed; }
	void SetPitchSpeed(GLfloat speed) { pitchSpeed = speed; }
	void SetYawSpeed(GLfloat speed) { yawSpeed = speed; }

	void SetPosition(glm::vec3 position) { positionVec = position; }
	void SetWorldUp(glm::vec3 worldUp) { worldUpVec = worldUp; }

	GLfloat GetYaw() const { return yaw; }
	GLfloat GetPitch() const { return pitch; }
	GLfloat GetZoom() const { return zoom; }

	GLfloat GetMoveSpeed() const { return moveSpeed; }
	GLfloat GetPitchSpeed() const { return pitchSpeed; }
	GLfloat GetYawSpeed() const { return yawSpeed; }

	glm::vec3 GetPosition() const { return positionVec; }
	glm::vec3 GetWorldUp() const { return worldUpVec; }

	void MoveCamera(MGLenum direction, GLfloat dt);

	enum CameraMovement { Forward, Backward, Left, Right, Up, Down, Pitch, Yaw, Zoom, MaxMovements };

	MGLCamera(const MGLCamera& other) = delete;
	MGLCamera(const MGLCamera&& other) = delete;
	MGLCamera& operator=(const MGLCamera& other) = delete;
	MGLCamera& operator=(const MGLCamera&& other) = delete;

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
	void MoveZoom(GLfloat amount);

	void MapMovementFunctions() const;

	/****** Data ******/

	GLfloat pitch = 0.0f;
	GLfloat yaw = -90.0f;
	GLfloat zoom = 65.0f;

	GLfloat moveSpeed = 3.0f;
	GLfloat pitchSpeed = 0.15f;
	GLfloat yawSpeed = 0.15f;

	glm::vec3 positionVec = glm::vec3(0.0f, 0.0f, 10.0f);
	glm::vec3 upVec = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 worldUpVec = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 rightVec = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 frontVec = glm::vec3(0.0f, 0.0f, -1.0f);

	std::unordered_map<MGLenum, MGLFloatFunction>* movementMap = new std::unordered_map<MGLenum, MGLFloatFunction>(8);

};