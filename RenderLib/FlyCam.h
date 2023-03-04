#pragma once
#include "stdafx.h"
#include <cmath>

class CameraCollisionCallback
{
public:
	virtual glm::vec3 operator()(const glm::vec3& rOld, const glm::vec3& rNew, float radius) const = 0;
	virtual ~CameraCollisionCallback() {};
};

class FlyCam
{
	glm::vec3 pos;
	glm::vec3 look;
	float theta, phi;
	glm::vec3 up;
	glm::vec3 direction;
	glm::vec3 side;
	CameraCollisionCallback* collCallback;

	void updatevectors()
	{
		direction = glm::vec3(1, 0, 0);
		side = glm::vec3(0, 0, 1);
		// rotate direction and side about up by theta
		glm::mat3 rotMat = glm::rotate(glm::mat4(), glm::radians(theta), up);
		direction = rotMat * direction;
		side = rotMat * side;
		// now rotate direction about side by phi
		rotMat = glm::rotate(glm::mat4(), glm::radians(phi), side);
		direction = rotMat * direction;
	}
public:
	FlyCam() {}
	FlyCam(const glm::vec3& pos, const glm::vec3 &look) : pos(pos), up(glm::vec3(0,1,0)) 
	{
		glm::vec3 r = glm::normalize(look - pos);
		theta = glm::degrees(atan2(-r.z, r.x));
		phi = glm::degrees(asin(r.y));
		updatevectors();
		this->look = this->pos + direction;
	}

	const glm::vec3& GetPos() const { return pos; }
	const glm::vec3& GetLookAt() const { return look; }
	const glm::vec3& GetUp() const { return up; }

	void Pan(float delta)
	{
		theta += delta;
		if (theta < 0.0f)
			theta += 360.0f;
		if (theta > 360.0f)
			theta -= 360.0f;
		updatevectors();
		look = pos + direction;
	}

	void LookUpDown(float delta)
	{
		phi += delta;
		if (phi < -80.0f)
			phi = -80.0f;
		if (phi > 80.0f)
			phi = 80.0f;
		updatevectors();
		look = pos + direction;
	}

	void Move(float deltaMove)
	{
		// move the camera in the direction it's looking in 
		glm::vec3 moveDir = glm::normalize(glm::vec3(direction.x, direction.y, direction.z));
		glm::vec3 newPos = pos + moveDir * deltaMove;
		if (collCallback != nullptr)
		{
			newPos = (*collCallback)(pos, newPos, 1.0);
		}
		pos = newPos;
		look = pos + direction;
	}

	void Strafe(float deltaMove)
	{
		glm::vec3 moveSide = glm::normalize(glm::vec3(side.x, 0, side.z));
		glm::vec3 newPos = pos + moveSide * deltaMove;
		if (collCallback != nullptr)
		{
			newPos = (*collCallback)(pos, newPos, 1.0);
		}
		pos = newPos;
		look = pos + direction;
	}
};


