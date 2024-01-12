#include "lab_m1/tema2/tema2.h"
#include <iostream>

using namespace transform3D;

// GENERATE
void Tema2::GenerateEnemies() {
	for (int i = 0; i < enemiesCount; i++) {
		glm::vec3 position;
		// MAKE SURE ENEMIES DON'T SPAWN NEXT TO PLAYER
		do {
			position = glm::vec3(RandomFloat(-maxDistanceSpawnEnemy, maxDistanceSpawnEnemy), 0.0f, RandomFloat(-maxDistanceSpawnEnemy, maxDistanceSpawnEnemy));
		} while (glm::distance(position, player.getPosition()) <= minDistanceSpawnEnemy);

		float rotationBody = RandomFloat(0.0f, M_PI);
		float rotationTurret = RandomFloat(0.0f, M_PI);

		enemies.push_back(Tank(position, rotationBody, rotationTurret));
	}
}

void Tema2::GenerateBuildings() {
	// CHOOSE RANDOM NUMBER OF BUILDINGS
	int n = RandomInt(minBuildings, maxBuildings);

	for (int i = 0; i < n; i++) {
		float x, z; // BUILDING POSITION
		bool isPlacedWrong;

		// CHOOSE RANDOM DIMENSIONS
		float length = RandomFloat(minBuildingDimension, maxBuildingDimension);
		float width = RandomFloat(minBuildingDimension, maxBuildingDimension);
		float height = RandomFloat(minBuildingDimension, maxBuildingDimension);

		do {
			x = RandomFloat(-maxDistanceSpawnBuilding, maxDistanceSpawnBuilding);
			isPlacedWrong = false;

			// CHECK IF IT'S IN PLAYER SPAWN ZONE
			if ((x + width / 2.0f >= -minDistanceSpawnBuilding) && (x - width / 2.0f <= minDistanceSpawnBuilding)) {
				isPlacedWrong = true;
				continue;
			}

			// CHECK COLLISION WITH TANKS
			if ((x + width / 2.0f >= player.getPosition().x - tankRadius) && (x - width / 2.0f <= player.getPosition().x + tankRadius)) {
				isPlacedWrong = true;
				continue;
			}

			for (auto it_enemies = enemies.begin(); it_enemies != enemies.end(); ++it_enemies) {
				if ((x + width / 2.0f >= (*it_enemies).getPosition().x - tankRadius) && (x - width / 2.0f <= (*it_enemies).getPosition().x + tankRadius)) {
					isPlacedWrong = true;
					break;
				}
			}

		} while (isPlacedWrong);

		do {
			z = RandomFloat(-maxDistanceSpawnBuilding, maxDistanceSpawnBuilding);
			isPlacedWrong = false;

			// CHECK IF IT'S IN PLAYER SPAWN ZONE
			if ((z + length / 2.0f >= -minDistanceSpawnBuilding) && (z - length / 2.0f <= minDistanceSpawnBuilding)) {
				isPlacedWrong = true;
				continue;
			}

			// CHECK COLLISION WITH TANKS
			if ((z + length / 2.0f >= player.getPosition().z - tankRadius) && (z - length / 2.0f <= player.getPosition().z + tankRadius)) {
				isPlacedWrong = true;
				continue;
			}

			for (auto it_enemies = enemies.begin(); it_enemies != enemies.end(); ++it_enemies) {
				if ((z + length / 2.0f >= (*it_enemies).getPosition().z - tankRadius) && (z - length / 2.0f <= (*it_enemies).getPosition().z + tankRadius)) {
					isPlacedWrong = true;
					break;
				}
			}

		} while (isPlacedWrong);

		buildings.push_back(Building(glm::vec3(x, 0.0f, z), length, width, height));
	}
}

// RENDER
void Tema2::RenderGround() {
	glm::mat4 modelMatrix = transform3D::Translate(center);
	modelMatrix *= transform3D::Scale(groundScale);
	RenderSimpleMesh(meshes["plane50"], shaders[temaShader], modelMatrix, ground);
}

void Tema2::RenderBuildings() {
	for (auto it = buildings.begin(); it != buildings.end(); ++it) {
		glm::mat4 modelMatrix = transform3D::Translate((*it).getPosition());
		modelMatrix *= transform3D::Scale((*it).getWidth(), (*it).getHeight(), (*it).getLength());
		RenderSimpleMesh(meshes["box"], shaders[temaShader], modelMatrix, grey);
	}
}

void Tema2::RenderTank(Tank& tank, bool isEnemy) {
	// TRACKS
	{
		//TRANSLATE TO BODY CENTER, ROTATE, TRANSLATE TO LEFT TRACKS POSITION
		glm::mat4 modelMatrix = transform3D::Translate(tank.getPosition() + bodyOffset);
		modelMatrix *= transform3D::RotateOY(tank.getRotationBody());
		modelMatrix *= transform3D::Translate(tracksOffsetLeft);
		modelMatrix *= transform3D::Scale(tankScale);
		RenderSimpleMesh(meshes[tankParts[0]], shaders[temaShader], modelMatrix, (isEnemy ? bronze : greyBlue), tank.getHP(), true);
	} // LEFT TRACKS

	{
		//TRANSLATE TO BODY CENTER, ROTATE, TRANSLATE TO RIGHT TRACKS POSITION
		glm::mat4 modelMatrix = transform3D::Translate(tank.getPosition() + bodyOffset);
		modelMatrix *= transform3D::RotateOY(tank.getRotationBody());
		modelMatrix *= transform3D::Translate(tracksOffsetRight);
		modelMatrix *= transform3D::Scale(tankScale);
		RenderSimpleMesh(meshes[tankParts[0]], shaders[temaShader], modelMatrix, (isEnemy ? bronze : greyBlue), tank.getHP(), true);
	} // RIGHT TRACKS

	// BODY
	{
		glm::mat4 modelMatrix = transform3D::Translate(tank.getPosition() + bodyOffset);
		modelMatrix *= transform3D::RotateOY(tank.getRotationBody());
		modelMatrix *= transform3D::Scale(tankScale);
		RenderSimpleMesh(meshes[tankParts[1]], shaders[temaShader], modelMatrix, (isEnemy ? orangeDark : blueDark), tank.getHP(), true);
	}

	// TURRET
	{
		glm::mat4 modelMatrix = transform3D::Translate(tank.getPosition() + turretOffset);
		modelMatrix *= transform3D::RotateOY(tank.getRotationTurret());
		modelMatrix *= transform3D::Scale(tankScale);
		RenderSimpleMesh(meshes[tankParts[2]], shaders[temaShader], modelMatrix, (isEnemy ? orangeLight : blueLight), tank.getHP(), true);
	}

	// GUN
	{
		// TRANSLATE TO TURRET CENTER, ROTATE, TRANSLATE TO GUN POSITION
		glm::mat4 modelMatrix = transform3D::Translate(tank.getPosition() + turretOffset);
		modelMatrix *= transform3D::RotateOY(tank.getRotationTurret());
		modelMatrix *= transform3D::Translate(gunOffset);
		modelMatrix *= transform3D::Scale(tankScale);
		RenderSimpleMesh(meshes[tankParts[3]], shaders[temaShader], modelMatrix, (isEnemy ? bronze : greyBlue), tank.getHP(), true);
	}
}

void Tema2::RenderEnemies() {
	for (auto it = enemies.begin(); it != enemies.end(); ++it) {
		RenderTank((*it), true);
	}
}

void Tema2::RenderBullets() {
	for (auto it = bullets.begin(); it != bullets.end(); ++it) {
		RenderBullet(*it);
	}
}

void Tema2::RenderBullet(Bullet bullet) {
	glm::mat4 modelMatrix = transform3D::Translate(bullet.getPosition());
	modelMatrix *= transform3D::Scale(bulletScale, bulletScale, bulletScale);
	RenderSimpleMesh(meshes["sphere"], shaders[temaShader], modelMatrix, black);
}

// UPDATE
void Tema2::UpdateBullets(float deltaTimeSeconds) {
	float speed = bulletSpeed * deltaTimeSeconds;

	for (auto it = bullets.begin(); it != bullets.end(); ++it) {
		(*it).moveForward(speed);
		(*it).decreaseTimeToLive(deltaTimeSeconds);
	}
}

void Tema2::SetEnemiesAction() {
	float time = glfwGetTime();
	std::srand(std::time(0));

	for (auto it = enemies.begin(); it != enemies.end(); ++it) {
		// CHECK IF PREVIOUS ACTION IS DONE
		if (time - (*it).getActionTime() >= (*it).getActionDuration()) {
			// CHOOSE RANDOM ACTION BETWEEN 0 AND 4
			(*it).setAction(std::rand() % 5);
			// CHOOSE RANDOM ACTION DURATION
			(*it).setActionDuration(RandomFloat(minTimeEnemyAction, maxTimeEnemyAction));
			// UPDATE ACTION TIME
			(*it).setActionTime(time);
		}
	}
}

void Tema2::MoveEnemies(float deltaTimeSeconds) {
	float speed = tankSpeed * deltaTimeSeconds;

	for (auto it = enemies.begin(); it != enemies.end(); ++it) {
		// IGNORE IF ENEMY IS DEAD
		if (!(*it).getHP())
			continue;

		switch ((*it).getAction()) {
		case 0:
			// STAY
			break;
		case 1:
			// MOVE FORWARD
			(*it).moveForward(speed);
			break;
		case 2:
			// MOVE BACKWARDS
			(*it).moveForward(-speed);
			break;
		case 3:
			// ROTATE BODY LEFT
			(*it).setRotationBody((*it).getRotationBody() - deltaTimeSeconds);
			break;
		case 4:
			// ROTATE BODY RIGHT
			(*it).setRotationBody((*it).getRotationBody() + deltaTimeSeconds);
			break;
		}
	}
}

void Tema2::EnemyTargetPlayer() {
	for (auto it = enemies.begin(); it != enemies.end(); it++) {
		// IGNORE IF ENEMY IS DEAD
		if (!(*it).getHP())
			continue;

		float distance = glm::distance((*it).getXZ(), player.getXZ());
		if (distance <= distanceEnemyTargetPlayer) {
			// CALCULATE ANGLE BETWEEN ENEMY AND PLAYER
			float dx = (*it).getPosition().x - player.getPosition().x;
			float dz = (*it).getPosition().z - player.getPosition().z;
			float ang = atan2(dz, dx);

			// ROTATE ENEMY TURRET
			(*it).setRotationTurret(ang - M_PI / 2.0f);

			// SHOOT PLAYER
			TankShoot((*it), false);
		}
	}
}

void Tema2::TankShoot(Tank& tank, bool isPlayer) {
	float time = glfwGetTime();

	if (time - tank.getTimeLastBullet() >= timeBetweenBullets) {
		bullets.push_back(Bullet(isPlayer, tank.getPosition() + bulletOffsetY + bulletOffsetDirection * tank.getTurretDirection(), tank.getTurretDirection()));
		tank.setTimeLastBullet(time);
	}
}

// CHECK
void Tema2::CheckGameOver() {
	float time = glfwGetTime();

	if (player.getHP() == 0) {
		std::cout << "Your tank has been destroyed :(" << std::endl;
		isGameOver = true;
	}
	else if (time >= timeGameOver) {
		std::cout << "Time has run out!" << std::endl;
		std::cout << "Game ended after " << roundf(time * 100) / 100 << " seconds!" << std::endl;
		std::cout << "Score: " << score << std::endl;
		isGameOver = true;
	}
}

void Tema2::CheckBulletsTimeToLive() {
	for (auto it = bullets.begin(); it != bullets.end(); ) {
		if ((*it).getTimeToLive() <= 0.0f) {
			it = bullets.erase(it);
		}
		else {
			++it;
		}
	}
}

void Tema2::CheckCollisionBulletTank() {
	for (auto it_bullets = bullets.begin(); it_bullets != bullets.end(); ) {
		if ((*it_bullets).getShotByPlayer()) {
			// BULLET-ENEMY
			for (auto it_enemies = enemies.begin(); it_enemies != enemies.end(); ++it_enemies) {
				float distance = glm::distance((*it_bullets).getPosition(), (*it_enemies).getPosition());

				if (distance < bulletRadius + tankRadius) {
					// DECREASE ENEMY HP AND INCREASE SCORE IF KILL
					if ((*it_enemies).getHP() > 0) {
						(*it_enemies).setHP((*it_enemies).getHP() - 1);
						if ((*it_enemies).getHP() == 0)
							++score;
					}

					// BULLET DISAPPEARS
					it_bullets = bullets.erase(it_bullets);
					goto escape_for;
				}
			}
		}
		else {
			//BULLET-PLAYER
			float distance = glm::distance((*it_bullets).getPosition(), player.getPosition());

			if (distance < bulletRadius + tankRadius) {
				// DECREASE HP AND INCREASE SCORE IF KILL
				if (player.getHP() > 0) {
					player.setHP(player.getHP() - 1);
				}

				// ERASE BULLET
				it_bullets = bullets.erase(it_bullets);
				goto escape_for;
			}
		}

		escape_for:
			if (it_bullets != bullets.end())
				++it_bullets;
	}

}

void Tema2::CheckCollisionBulletBuilding() {
	for (auto it_bullets = bullets.begin(); it_bullets != bullets.end(); ) {
		for (auto it_buildings = buildings.begin(); it_buildings != buildings.end(); ++it_buildings) {
			bool isColliding = ((*it_bullets).getPosition().x >= (*it_buildings).getPosition().x - (*it_buildings).getWidth() / 2) &&
				((*it_bullets).getPosition().x <= (*it_buildings).getPosition().x + (*it_buildings).getWidth() / 2) &&
				((*it_bullets).getPosition().z >= (*it_buildings).getPosition().z - (*it_buildings).getLength() / 2) &&
				((*it_bullets).getPosition().z <= (*it_buildings).getPosition().z + (*it_buildings).getLength() / 2);

			if (isColliding) {
				it_bullets = bullets.erase(it_bullets);
				goto escape_for;
			}
		}

	escape_for:
		if (it_bullets != bullets.end())
			++it_bullets;
	}
}


void Tema2::CheckCollisionTankTank() {
	for (auto it_enemies = enemies.begin(); it_enemies != enemies.end(); ++it_enemies) {
		// PLAYER - ENEMY
		HandleCollisionTankTank((*it_enemies), player);

		// ENEMY - ENEMY
		for (auto it_enemies2 = std::next(it_enemies); it_enemies2 != enemies.end(); ++it_enemies2) {
			HandleCollisionTankTank((*it_enemies), (*it_enemies2));
		}
	}
}

void Tema2::CheckCollisionTankBuilding() {
	for (auto it_buildings = buildings.begin(); it_buildings != buildings.end(); ++it_buildings) {
		// PLAYER - BUILDING
		HandleCollisionTankBuilding(player, (*it_buildings));

		// ENEMIES - BUILDING
		for (auto it_enemies = enemies.begin(); it_enemies != enemies.end(); ++it_enemies) {
			HandleCollisionTankBuilding((*it_enemies), (*it_buildings));
		}
	}
}

// HANDLE
void Tema2::HandleCollisionTankTank(Tank& tank1, Tank& tank2) {
	float distance = glm::distance(tank1.getXZ(), tank2.getXZ());

	if (distance < 2 * tankRadius) {
		glm::vec2 difference = tank1.getXZ() - tank2.getXZ();
		float overlap = 2 * tankRadius - distance;
		glm::vec2 p = overlap * glm::normalize(difference);
		tank1.setXZ(tank1.getXZ() + collisionOffset * p);
		tank2.setXZ(tank2.getXZ() - collisionOffset * p);
	}
}

void Tema2::HandleCollisionTankBuilding(Tank& tank, Building& building) {

	// DISTANCES REQUIRED TO BE A COLLISION
	float minDistanceX = building.getWidth() / 2.0f + tankRadius;
	float minDistanceZ = building.getLength() / 2.0f + tankRadius;

	// ACTUAL DISTANCES 
	float dx = fabs(tank.getPosition().x - building.getPosition().x);
	float dz = fabs(tank.getPosition().z - building.getPosition().z);

	if (dx <= minDistanceX && dz <= minDistanceZ) {
		float overlapX = minDistanceX - dx;
		float overlapZ = minDistanceZ - dz;

		// MOVE THE TANK AWAY
		if (overlapX < overlapZ) {
			// CHOOSE WHICH WAY TO GO ON X AXIS
			if (tank.getPosition().x < building.getPosition().x)
				overlapX = -overlapX;
			tank.setPosition(tank.getPosition() + glm::vec3(overlapX, 0.0f, 0.0f));
		}
		else {
			// CHOOSE WHICH WAY TO GO ON Z AXIS
			if (tank.getPosition().z < building.getPosition().z)
				overlapZ = -overlapZ;
			tank.setPosition(tank.getPosition() + glm::vec3(0.0f, 0.0f, overlapZ));
		}
	}
}

// GENERATE RANDOM NUMBERS
int Tema2::RandomInt(int min, int max) {
	std::srand(static_cast<unsigned int>(std::time(0)));
	return min + std::rand() % (max - min + 1);
}

float Tema2::RandomFloat(float min, float max) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float r = random * (max - min);
	return min + r;
}

// OTHER
void Tema2::PauseGame() {
	paused = !paused;
}