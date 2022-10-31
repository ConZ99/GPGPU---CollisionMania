#include "Laborator2.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Obiecte.h"
#include "Transformari.h"

using namespace std;

Laborator2::Laborator2()
{
}

Laborator2::~Laborator2()
{

}
struct meshuri
{
	glm::vec3 posCurenta = glm::vec3(0, 0, 0);
	Mesh* mesh;
};

void Laborator2::Init()
{
	{
		Shader* shader = new Shader("ShaderTema");
		shader->AddShader("Source/Laboratoare/Laborator2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	
	list<meshuri> cuburi;
	Mesh* box = Obiecte::Box("box", glm::vec3(.8, .2, .8));
	AddMeshToList(box);
	Mesh* cub = Obiecte::Cube("cub", glm::vec3(.6, 0, .6));
	AddMeshToList(cub);
	/*meshuri test = {
		glm::vec3(0, 0, 5),
		cub
	};
	cuburi.push_back(test);*/

	Mesh* sfera = Obiecte::Sphere("sfera", glm::vec3(1, 1, 1));
	sfera->SetDrawMode(GL_TRIANGLE_FAN);
	AddMeshToList(sfera);
	Mesh* cilindru = Obiecte::Cylinder("cilindru", glm::vec3(1, 0, 1));
	cilindru->SetDrawMode(GL_TRIANGLES);
	AddMeshToList(cilindru);
}

void Laborator2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator2::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//box rendering
	RenderMesh(meshes["box"], shaders["ShaderTema"], glm::vec3(0, 0, 0), glm::vec3(10.0f));

	// render an object using colors from vertex
	glm::mat4 modelMatrix = glm::mat4(1);
	y += -1 * deltaTimeSeconds;
	glm::vec3(0, -1, 0);
	modelMatrix *= Transformari::Translate(x, y, z);
	RenderMesh(meshes["cub"], shaders["ShaderTema"], modelMatrix);
	RenderMesh(meshes["sfera"], shaders["ShaderTema"], glm::vec3(5, 5, 0), glm::vec3(1.0f));
	RenderMesh(meshes["cilindru"], shaders["ShaderTema"], glm::vec3(5, 0, 0), glm::vec3(2.0f));

	glDisable(GL_CULL_FACE);
}

void Laborator2::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator2::OnInputUpdate(float deltaTime, int mods)
{

}

void Laborator2::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_SPACE)
	{
		for (const auto it : meshes) {
			std::cout << it.first << "at" << std::endl;

		}
	}
}

void Laborator2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator2::OnWindowResize(int width, int height)
{
}
