#pragma once
#include "components/simple_scene.h"
#include "components/transform.h"

#include "vector"
#include "transform3D.h"
#include "camera.h"

#include "tank.h"
#include "building.h"
#include "bullet.h"

class Tema2 : public gfxc::SimpleScene {
public:
	Tema2();
	~Tema2();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1), int HP = 3, bool isTank = false);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	// GENERATE
	void GenerateEnemies();
	void GenerateBuildings();

	// RENDER
	void RenderGround();
	void RenderBuildings();
	void RenderTank(Tank& tank, bool isEnemy);
	void RenderEnemies();
	void RenderBullet(Bullet bullet);
	void RenderBullets();

	// UPDATE
	void UpdateBullets(float deltaTimeSeconds);
	void SetEnemiesAction();
	void MoveEnemies(float deltaTimeSeconds);
	void EnemyTargetPlayer();
	void TankShoot(Tank& tank, bool isPlayer);

	// CHECK
	void CheckGameOver();
	void CheckBulletsTimeToLive();
	void CheckCollisionBulletTank();
	void CheckCollisionBulletBuilding();
	void CheckCollisionTankTank();
	void CheckCollisionTankBuilding();

	// HANDLE
	void HandleCollisionTankTank(Tank& tank1, Tank& tank2);
	void HandleCollisionTankBuilding(Tank& tank, Building& building);

	// GENERATE RANDOM NUMBERS
	int RandomInt(int min, int max);
	float RandomFloat(float min, float max);

	// OTHER
	void PauseGame();

protected:
	// CAMERA
	implemented::Camera* camera;
	glm::mat4 modelMatrix;
	glm::mat4 projectionMatrix;
	glm::vec3 cameraUp;
	glm::vec3 cameraOffset;
	float cameraRotationAngle;
	float cameraDistanceY;
	float cameraDistanceZ;
	float fov;

	// MESHES
	std::vector<std::string> primitives;
	std::vector<std::string> tankParts;

	// SHADER
	std::string temaShader;

	// TANKS
	Tank player;
	std::vector<Tank> enemies;
	std::vector<Bullet> bullets;

	int enemiesCount;
	float tankRadius;
	float bulletRadius;

	// BUILDINGS
	std::vector<Building> buildings;

	int minBuildings;
	int maxBuildings;
	
	float minBuildingDimension;
	float maxBuildingDimension;

	// COLORS
	glm::vec3 ground; // GRASS
	glm::vec3 black; // BULLET
	glm::vec3 grey; // BUILDINGS
	glm::vec3 greyBlue; // PLAYER TRACKS/GUN
	glm::vec3 bronze; // ENEMY TRACKS/GUN
	glm::vec3 blueLight; // PLAYER TURRET
	glm::vec3 blueDark; // PLAYER BODY
	glm::vec3 orangeLight; // ENEMY TURRET
	glm::vec3 orangeDark; // ENEMY BODY

	// SCALES
	float groundScale;
	float tankScale;
	float bulletScale;

	// DISTANCES
	float distanceEnemyTargetPlayer; // ENEMY VISION

	float minDistanceSpawnEnemy;
	float maxDistanceSpawnEnemy;

	float minDistanceSpawnBuilding; // BUILDINGS SHOULD NOT GENERATE IN PLAYER SPAWN ZONE
	float maxDistanceSpawnBuilding;

	// OFFSETS
	glm::vec3 tracksOffsetLeft;
	glm::vec3 tracksOffsetRight;
	glm::vec3 bodyOffset;
	glm::vec3 turretOffset;
	glm::vec3 gunOffset;

	glm::vec3 bulletOffsetY;
	float bulletOffsetDirection;
	float collisionOffset;

	glm::vec3 center;

	// SPEED
	float tankSpeed;
	float bulletSpeed;
	float rotationSpeed;

	// TIME
	float timeGameOver; // GAME LENGTH
	float timeBetweenBullets;
	float minTimeEnemyAction;
	float maxTimeEnemyAction;

	bool isGameOver;
	bool paused;

	// SCORE
	int score;

};
