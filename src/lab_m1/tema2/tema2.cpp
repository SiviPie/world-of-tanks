#include "lab_m1/tema2/tema2.h"

#include <string>
#include <iostream>

using namespace std;

Tema2::Tema2() { }


Tema2::~Tema2() { }


void Tema2::Init() {

	// CAMERA
	{
		cameraRotationAngle = 0.0f;
		cameraDistanceY = 2.5f;
		cameraDistanceZ = 7.0f;
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraOffset = glm::vec3(0.0f, cameraDistanceY, cameraDistanceZ);

		camera = new implemented::Camera();
		camera->setDistanceToTarget(cameraDistanceZ);
		camera->Set(cameraOffset, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
	}

	// COLORS
	{
		ground = glm::vec3(0.2f, 0.4f, 0.2f);
		black = glm::vec3(0.0f, 0.0f, 0.0f);
		grey = glm::vec3(0.5f, 0.5f, 0.5f);
		greyBlue = glm::vec3(0.6f, 0.7f, 0.9f);
		bronze = glm::vec3(0.5f, 0.3f, 0.2f);
		blueLight = glm::vec3(0.0f, 0.7f, 1.0f);
		blueDark = glm::vec3(0.0f, 0.3f, 1.0f);
		orangeLight = glm::vec3(1.0f, 0.7f, 0.0f);
		orangeDark = glm::vec3(1.0f, 0.5f, 0.0f);
	}

	// SCALES
	{
		tankScale = 0.2f;
		groundScale = 10.0f;
		bulletScale = 0.4f;
	}

	// TANKS
	{
		player = Tank();
		enemiesCount = 10;
		tankRadius = 2.0f;
		bulletRadius = 0.25f;
	}

	// BUILDINGS
	{
		minBuildings = 15;
		maxBuildings = 30;
		minBuildingDimension = 5.0f;
		maxBuildingDimension = 20.0f;
	}

	// DISTANCES
	{
		distanceEnemyTargetPlayer = 30.0f;

		minDistanceSpawnEnemy = 30.0f;
		maxDistanceSpawnEnemy = 60.0f;

		minDistanceSpawnBuilding = 5.0f;
		maxDistanceSpawnBuilding = 80.0f;
	}

	// OFFSETS
	{
		tracksOffsetLeft = glm::vec3(-0.7f, -0.6f, -0.05f);
		tracksOffsetRight = glm::vec3(0.7f, -0.6f, -0.05f);
		bodyOffset = glm::vec3(0.0f, 0.75f, 0.0f);
		turretOffset = glm::vec3(0.0f, 1.2f, 0.0f);
		gunOffset = glm::vec3(0.0f, 0.1f, -1.7f);

		bulletOffsetY = glm::vec3(0.0f, turretOffset.y + gunOffset.y, 0.0f);
		bulletOffsetDirection = 3.0f;
		collisionOffset = 0.2f;

		center = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	// SPEED
	{
		tankSpeed = 4.0f;
		bulletSpeed = 20.0f;
		rotationSpeed = 1.0f / 200.0f;
	}

	// TIME 
	{
		timeGameOver = 120.0f;
		timeBetweenBullets = 1.0f;
		minTimeEnemyAction = 2.0f;
		maxTimeEnemyAction = 10.0f;

		isGameOver = false;
		paused = false;
	}

	// SCORE
	score = 0;

	// MESHES
	{
		primitives.push_back("box");
		primitives.push_back("sphere");
		primitives.push_back("plane50");

		for (auto it = primitives.begin(); it != primitives.end(); ++it) {
			Mesh* mesh = new Mesh((*it));
			mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), (*it) + ".obj");
			meshes[mesh->GetMeshID()] = mesh;
		}

		tankParts.push_back("tank_tracks");
		tankParts.push_back("tank_body");
		tankParts.push_back("tank_turret");
		tankParts.push_back("tank_gun");

		for (auto it = tankParts.begin(); it != tankParts.end(); ++it) {
			Mesh* mesh = new Mesh((*it));
			mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tema2"), (*it) + ".obj");
			meshes[mesh->GetMeshID()] = mesh;
		}
	}

	// SHADER
	{
		temaShader = "TemaShader";

		Shader* shader = new Shader(temaShader);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// GENERATE
	GenerateEnemies();
	GenerateBuildings();
}


void Tema2::FrameStart() {
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.5f, 0.6f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// Sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	if (!paused) {
		// HIDE CURSOR AND STICK IT TO WINDOW CENTER
		window->HidePointer();
		window->SetPointerPosition(resolution.x / 2, resolution.y / 2);
	}
	else {
		window->ShowPointer();
	}

}


void Tema2::Update(float deltaTimeSeconds) {
	// RENDER
	RenderGround();
	RenderBuildings();
	RenderTank(player, false);
	RenderEnemies();
	RenderBullets();

	if (paused || isGameOver) {
		return;
	}

	CheckGameOver();

	// UPDATE
	UpdateBullets(deltaTimeSeconds);
	SetEnemiesAction();
	MoveEnemies(deltaTimeSeconds);
	EnemyTargetPlayer();

	// CHECK
	CheckBulletsTimeToLive();
	CheckCollisionBulletTank();
	CheckCollisionBulletBuilding();
	CheckCollisionTankTank();
	CheckCollisionTankBuilding();
}


void Tema2::FrameEnd() { }


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, int HP, bool isTank) {
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// Render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	glm::vec3 lightPosition = player.getPosition() + glm::vec3(0.0f, 1.0f, 0.0f);

	// Set shader uniforms for light & material properties
	GLint location_light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(location_light_position, 1, glm::value_ptr(lightPosition));

	glm::vec3 eyePosition = player.getPosition() + cameraOffset;

	GLint location_eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(location_eye_position, 1, glm::value_ptr(eyePosition));

	GLint loc = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(loc, 1, glm::value_ptr(color));

	loc = glGetUniformLocation(shader->program, "hp");
	glUniform1i(loc, HP);

	loc = glGetUniformLocation(shader->program, "is_tank");
	glUniform1i(loc, isTank);

	// Send deformation factor to vertex shader
	loc = glGetUniformLocation(shader->program, "deform_factor");
	glUniform1i(loc, 3 - HP);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->m_VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::OnInputUpdate(float deltaTime, int mods) {
	if (paused || isGameOver)
		return;

	float speed = tankSpeed * deltaTime;

	if (window->KeyHold(GLFW_KEY_W)) {
		// MOVE FORWARD
		player.moveForward(speed);

		camera->Set(player.getPosition() + cameraOffset, player.getPosition(), cameraUp);
		camera->RotateThirdPerson_OY(cameraRotationAngle);
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		// MOVE BACKWARDS
		player.moveForward(-speed);

		camera->Set(player.getPosition() + cameraOffset, player.getPosition(), cameraUp);
		camera->RotateThirdPerson_OY(cameraRotationAngle);
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		// ROTATE TANK BODY TO LEFT
		player.setRotationBody(player.getRotationBody() - deltaTime);
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		// ROTATE TANK BODY TO RIGHT
		player.setRotationBody(player.getRotationBody() + deltaTime);
	}
}


void Tema2::OnKeyPress(int key, int mods) {
	if (window->KeyHold(GLFW_KEY_P)) {
		PauseGame();
	}
}


void Tema2::OnKeyRelease(int key, int mods) { }


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
	if (paused)
		return;

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		// ROTATE CAMERA
		cameraRotationAngle -= deltaX * rotationSpeed;
		camera->RotateThirdPerson_OY(-deltaX * rotationSpeed);
	}
	else {
		// ROTATE TURRET
		if (!player.getHP())
			return;
		player.setRotationTurret(player.getRotationTurret() + deltaX * rotationSpeed);
	}
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
	if (!paused && !isGameOver) {
		if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
			TankShoot(player, true);
		}
	}
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) { }


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) { }


void Tema2::OnWindowResize(int width, int height) { }
