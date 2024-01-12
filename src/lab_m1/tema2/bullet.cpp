#include "bullet.h"

Bullet::Bullet(bool shot_by_player, glm::vec3 position, glm::vec3 direction, float time_to_live) : shot_by_player(shot_by_player), position(position), direction(direction), time_to_live(time_to_live) { }

// SETTERS
void Bullet::setPosition(glm::vec3 position) {
	this->position = position;
}

void Bullet::moveForward(float speed) {
	setPosition(position + speed * direction);
}

void Bullet::setTimeToLive(float time_to_live) {
	this->time_to_live = time_to_live;
}

void Bullet::decreaseTimeToLive(float deltaTime) {
	setTimeToLive(time_to_live - deltaTime);
}

// GETTERS
glm::vec3 Bullet::getPosition() {
	return position;
}

glm::vec2 Bullet::getXZ() {
	return glm::vec2(position.x, position.z);
}

float Bullet::getTimeToLive() {
	return time_to_live;
}

bool Bullet::getShotByPlayer() {
	return shot_by_player;
}
