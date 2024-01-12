#pragma once
#include "utils/glm_utils.h"

class Bullet {
public:
	// CONSTRUCTORS
	Bullet(bool shot_by_player, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f), float time_to_live = 1.5f);

	// SETTERS
	void setPosition(glm::vec3 position);
	void moveForward(float speed);
	void setTimeToLive(float time_to_live);
	void decreaseTimeToLive(float deltaTime);

	// GETTERS
	glm::vec3 getPosition();
	glm::vec2 getXZ();
	float getTimeToLive();
	bool getShotByPlayer();

private:
	bool shot_by_player;
	float time_to_live; // if <= 0, bullet disappears
	glm::vec3 position;
	glm::vec3 direction;

};