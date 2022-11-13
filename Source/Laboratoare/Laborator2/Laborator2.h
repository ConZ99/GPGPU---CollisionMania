#pragma once

#include <Component/SimpleScene.h>
#include <Core/Engine.h>
#include "Item.cpp"

class Laborator2 : public SimpleScene
{
	public:
		Laborator2();
		~Laborator2();

		void Init() override;
		float x= 0.0f, z = 0.0f, y = 0.0f;
		int objId = 5;

		float boxSize = 14.0f;
		glm::vec3 frecareaCuAerul = glm::vec3(-.1, -.1, -.1);
		glm::vec3 frecareaCuTerenul = glm::vec3(-2, -1, -2);

		//box initializat si fixat in (0, 0, 0)
		Item* box;

		//initializare lista de CUBURI
		list<Item*> cuburi;

		//initializare lista de SFERE
		list<Item*> sfere;

		//initializare lista de CILINDRI
		list<Item*> cilindri;

		glm::vec3 GetCoords();
		void AddCub(glm::vec3 pos, glm::vec3 force);
		void AddSfera(glm::vec3 pos, glm::vec3 force);
		void AddCilindru(glm::vec3 pos, glm::vec3 force);
		bool coordsAreNotValid(glm::vec3 pos);
		glm::vec3 ApplyRandomForce();
		void renderCuburi(float deltaTimeSeconds);
		void renderSfere(float deltaTimeSeconds);
		void renderCilindri(float deltaTimeSeconds);
		void isCollided(Item* it);
		void checkCollision(float deltaTimeSeconds);
		int whereCollided(Item* object, Item* obstacle);
		int fataLovita(Item* object, Item* obstacle, float deltaTimeSeconds);

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
};
