#include "tank.h"
#include "utils/math_utils.h"

// CONSTRUCTORS
Tank::Tank(glm::vec3 position, float rotationTurret, float rotationBody, float timeLastBullet, int hp, int action, float actionTime, float actionDuration) : 
	position(position), rotationTurret(rotationTurret), rotationBody(rotationBody), 
	timeLastBullet(timeLastBullet), hp(hp), action(action), 
	actionTime(actionTime), actionDuration(actionDuration) {}

// SETTERS
void Tank::setPosition(glm::vec3 position) {
	this->position = position;
}

void Tank::setXZ(glm::vec2 position) {
	this->position = glm::vec3(position.x, this->position.y, position.y);
}

void Tank::setRotationTurret(float rotationTurret) {
	if (rotationTurret >= 0.0f) {
		this->rotationTurret = (rotationTurret < (2 * M_PI) ? (rotationTurret) : (rotationTurret - (2 * M_PI)));
	}
	else {
		this->rotationTurret = rotationTurret;
	}
}

void Tank::setRotationBody(float rotationBody) {
	if (rotationBody >= 0.0f) {
		this->rotationBody = (rotationBody < (2 * M_PI) ? (rotationBody) : (rotationBody - (2 * M_PI)));
	}
	else {
		this->rotationBody = rotationBody;
	}
}

void Tank::setTimeLastBullet(float time) {
	if (time >= 0.0f)
		timeLastBullet = time;
}

void Tank::setHP(int hp) {
	if (hp >= 0 && hp <= 3)
		this->hp = hp;
}

void Tank::setAction(int action) {
	if (action >= 0 && action <= 4)
		this->action = action;
}

void Tank::setActionTime(float actionTime) {
	if (actionTime >= 0.0f)
		this->actionTime = actionTime;
}

void Tank::setActionDuration(float actionDuration) {
	if (actionDuration >= 0.0f)
		this->actionDuration = actionDuration;
}

// GETTERS
glm::vec2 Tank::getXZ() {
	return glm::vec2(position.x, position.z);
}

glm::vec3 Tank::getPosition() {
	return position;
}

float Tank::getRotationTurret() {
	return rotationTurret;
}

float Tank::getRotationBody() {
	return rotationBody;
}

glm::vec3 Tank::getTurretDirection() {
	return glm::vec3(sin(rotationTurret), 0.0f, -cos(rotationTurret));
}

float Tank::getTimeLastBullet() {
	return timeLastBullet;
}

int Tank::getHP() {
	return hp;
}

int Tank::getAction() {
	return action;
}

float Tank::getActionTime() {
	return actionTime;
}

float Tank::getActionDuration() {
	return actionDuration;
}

// MOVE
void Tank::moveForward(float distance) {
	setPosition(glm::vec3(position.x + distance * sin(rotationBody), 0.0f, position.z - distance * cos(rotationBody)));
}