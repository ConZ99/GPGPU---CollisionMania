#include "Header.h"
#include "Transformari.h"
#include "Obiecte.h"
//#include "Camera.h"

#include <vector>
#include <string>
#include <iostream>
#include <Core/Engine.h>
using namespace std;

Tema1::Tema1() {}

Tema1::~Tema1() {}

void Tema1::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix) {
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);

	int loc = glGetUniformLocation(shader->program, "Model");

	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	loc = glGetUniformLocation(shader->program, "View");

	glm::mat4 viewMatrix = camera->GetViewMatrix();
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	loc = glGetUniformLocation(shader->program, "Projection");

	//glm::mat4 projectionMatrix = projectionMatrix;
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


	GLfloat timeElapsed = Engine::GetElapsedTime();
	loc = glGetUniformLocation(shader->program, "timeElapsed");
	glUniform1f(loc, timeElapsed);

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema1::RenderCustomMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix) {
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);

	int loc = glGetUniformLocation(shader->program, "Model");

	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	loc = glGetUniformLocation(shader->program, "View");

	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	loc = glGetUniformLocation(shader->program, "Projection");

	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


	GLfloat timeElapsed = Engine::GetElapsedTime();
	loc = glGetUniformLocation(shader->program, "timeElapsed");
	glUniform1f(loc, timeElapsed);

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema1::Init() {
	//begin-camera
	{
		renderCameraTarget = false;
		camera = new Camera::Camera();
		camera->Set(glm::vec3(.0001, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
		//camera->RotateThirdPerson_OX(3);
		fov = 60;
		perspectiveProjection = true;
		projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
	}
	//end-camera

	//begin-meshes
	{
		Mesh* podea = Obiecte::Cube("podea", glm::vec3(.2, .2, .2));
		AddMeshToList(podea);
		Mesh* obstacol = Obiecte::Cube("obstacol", glm::vec3(1, 0, 0));
		AddMeshToList(obstacol);
		//world
	}
	//end-meshes
}

void Tema1::makeFloor()
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= Transformari::Translate(0, 0, -10);
	modelMatrix *= Transformari::Scale(10, .001, 20);
	RenderSimpleMesh(meshes["podea"], shaders["VertexNormal"], modelMatrix);
}

void Tema1::FrameStart() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds) {
	glClearColor(0, 0.75, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	makeFloor();
}

void Tema1::FrameEnd() {
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema1::OnInputUpdate(float deltaTime, int mods) {
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			camera->TranslateForward(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			camera->TranslateRight(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			camera->TranslateForward(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			camera->TranslateRight(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			camera->TranslateUpword(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			camera->TranslateUpword(-cameraSpeed * deltaTime);
		}
	}
}

void Tema1::OnKeyPress(int key, int mods) {
}

void Tema1::OnKeyRelease(int key, int mods) {
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = -0.001f;
		float sensivityOY = -0.001f;

		if (window->GetSpecialKeyState() == 0) {
			renderCameraTarget = false;
			// TODO : rotate the camera in First-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateFirstPerson_OX(deltaY * sensivityOY);
			camera->RotateFirstPerson_OY(deltaX * sensivityOX);
		}

		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
			renderCameraTarget = true;
			// TODO : rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateThirdPerson_OX(deltaY * sensivityOY);
			camera->RotateThirdPerson_OY(deltaX * sensivityOX);
		}

	}
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {
}

void Tema1::OnWindowResize(int width, int height) {
}