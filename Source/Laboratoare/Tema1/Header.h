#pragma once

#include <Component/SimpleScene.h>
#include "Camera.h"
#include <string>
#include <Core/Engine.h>
#include <vector>

class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();
	void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix);
	void RenderCustomMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix);
	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;


	void makeFloor();



	glm::mat4 modelMatrix;
	std::vector<bool> checkCollision;
	std::vector<bool> collision;
	double scaleFactor, lives, fuelLeft;
	double consX, consY, consZ;
	double angularStepOX, angularStepOY, angularStepOZ;
	double translateX, translateY, translateZ;
	double rotateSea, rotateCloud, moveClouds, rotateConsumable, rotation;
	double planeY;
	double cY, cX;

	float fov;

	bool perspectiveProjection;
	bool endgame;

	int mouseX, mouseY;
	int x, y, z;
	int directionUp;

	float playerPos_x, playerPos_y, playerPos_z;

protected:
	Camera::Camera *camera;
	glm::mat4 projectionMatrix;
	bool renderCameraTarget;
};
