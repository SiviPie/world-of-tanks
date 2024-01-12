#pragma once

#include "utils/glm_utils.h"

class Tank {
public:
	// CONSTRUCTORS
	Tank(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float rotationTurret = 0.0f, float rotationBody = 0.0f, float timeLastBullet = 0.0f, int hp = 3, int action = 0, float actionTime = 0.0f, float actionDuration = 0.0f);

	// SETTERS
	void setPosition(glm::vec3 position);
	void setXZ(glm::vec2 position);
	void setRotationTurret(float rotation_turret);
	void setRotationBody(float rotation_body);
	void setTimeLastBullet(float time);
	void setHP(int hp);
	void setAction(int action);
	void setActionTime(float actionTime);
	void setActionDuration(float actionDuration);

	// GETTERS
	glm::vec2 getXZ();
	glm::vec3 getPosition();
	glm::vec3 getTurretDirection();
	float getRotationTurret();
	float getRotationBody();
	float getTimeLastBullet();
	int getHP();
	int getAction();
	float getActionTime();
	float getActionDuration();

	// MOVE
	void moveForward(float distance);

private:
	glm::vec3 position;
	float rotationTurret;
	float rotationBody;
	float timeLastBullet;
	int hp;

	// ENEMY MOVEMENT
	int action; // 0 = STAY, 1-2 = MOVE FORWARD/BACKWARD, 3-4 ROTATE LEFT/RIGHT
	float actionTime; // ACTION START TIME
	float actionDuration; // HOW LONG SHOULD ACTION LAST
};