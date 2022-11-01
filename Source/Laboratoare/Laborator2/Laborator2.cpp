#include "Laborator2.h"

#include <vector>
#include <iostream>

#include <cstdlib>
#include <Core/Engine.h>
#include "Obiecte.h"
#include "Transformari.h"
#include "../../../Visual Studio/Item.cpp"

using namespace std;

Laborator2::Laborator2()
{
}

Laborator2::~Laborator2()
{

}
struct meshuri
{
	
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
	{
		//initializam mesh box
		Mesh* Box = Obiecte::Box("box", glm::vec3(.8, .2, .8));
		AddMeshToList(Box);

		//initializam mesh box
		Mesh* Cub = Obiecte::Cube("cub", glm::vec3(.6, 0, .6));
		AddMeshToList(Cub);

		//initializam mesh box
		Mesh* Sfera = Obiecte::Sphere("sfera", glm::vec3(1, 1, 1));
		Sfera->SetDrawMode(GL_TRIANGLE_FAN);
		AddMeshToList(Sfera);

		//initializam mesh box
		Mesh* Cilindru = Obiecte::Cylinder("cilindru", glm::vec3(1, 0, 1));
		Cilindru->SetDrawMode(GL_TRIANGLES);
		AddMeshToList(Cilindru);
	}
	{
		//box initializat si fixat in (0, 0, 0)
		box = &Item(0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), meshes["box"]);
	}
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
	RenderMesh(meshes["box"], shaders["ShaderTema"], glm::vec3(0, 0, 0), glm::vec3(boxSize));

	// render an object using colors from vertex
	glm::mat4 modelMatrix = glm::mat4(1);
	y += -1 * deltaTimeSeconds;
	glm::vec3 t = glm::vec3(0, -1, 0);
	modelMatrix *= Transformari::Translate(x, y, z);
	RenderMesh(meshes["cub"], shaders["ShaderTema"], glm::vec3(0, 0, 0));
	RenderMesh(meshes["sfera"], shaders["ShaderTema"], glm::vec3(-5, 0, 0));
	RenderMesh(meshes["cilindru"], shaders["ShaderTema"], glm::vec3(5, 0, 0));

	cout << cuburi.size() << endl;
	for (Item* it : cuburi) {
		//verifica coliziuni
		//schimba pozitia curenta bazat pe forte
		//daca are o forta aplicata deja, aia scade in delta time seconds si se aplica incet incet gravitatia
		it->pozitiaCurenta += acceleratieGravitationala * deltaTimeSeconds;
		RenderMesh(it->mesh, shaders["ShaderTema"], it->pozitiaCurenta);
	}

	glDisable(GL_CULL_FACE);
}

void Laborator2::FrameEnd()
{
	DrawCoordinatSystem();
}

bool Laborator2::coordsAreNotValid(glm::vec3 pos)
{
	for (Item* it : cuburi) {
		if (pos.x > it->pozitiaCurenta.x - 1 && pos.x < it->pozitiaCurenta.x + 1)
			if (pos.y > it->pozitiaCurenta.y - 1 && pos.y < it->pozitiaCurenta.y + 1)
				if (pos.z > it->pozitiaCurenta.z - 1 && pos.z < it->pozitiaCurenta.z + 1)
					return true;
	}
	for (Item* it : sfere) {
		if (pos.x > it->pozitiaCurenta.x - 1 && pos.x < it->pozitiaCurenta.x + 1)
			if (pos.y > it->pozitiaCurenta.y - 1 && pos.y < it->pozitiaCurenta.y + 1)
				if (pos.z > it->pozitiaCurenta.z - 1 && pos.z < it->pozitiaCurenta.z + 1)
					return true;
	}
	for (Item* it : cilindri) {
		if (pos.x > it->pozitiaCurenta.x - 1 && pos.x < it->pozitiaCurenta.x + 1)
			if (pos.y > it->pozitiaCurenta.y - 1 && pos.y < it->pozitiaCurenta.y + 1)
				if (pos.z > it->pozitiaCurenta.z - 1 && pos.z < it->pozitiaCurenta.z + 1)
					return true;
	}
	return false;
}

glm::vec3 Laborator2::GetCoords()
{
	glm::vec3 coords;
	//NU FACE RANDOM BINE!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	srand(time(0)); //srand are nev de un seed pt randomizare de numere GASESTE UN SEED MAI NOU
	coords.x = (rand() % ((int)boxSize * 2 - 2)) - boxSize + 1;
	srand(time(0));
	coords.y = (rand() % ((int)boxSize * 2 - 2)) - boxSize + 1;
	srand(time(0));
	coords.z = (rand() % ((int)boxSize * 2 - 2)) - boxSize + 1;

	while (coordsAreNotValid(coords)) {
		srand(time(0));
		coords.x = (rand() % ((int)boxSize * 2 - 2)) - boxSize + 1;
		srand(time(0));
		coords.y = (rand() % ((int)boxSize * 2 - 2)) - boxSize + 1;
		srand(time(0));
		coords.z = (rand() % ((int)boxSize * 2 - 2)) - boxSize + 1;
	}

	return coords;
}

void Laborator2::AddCub(glm::vec3 pos)
{
	Item* cub = new Item(999, glm::vec3(0, 0, 0), pos, meshes["cub"]);
	cuburi.push_back(cub);
	cout << "Spawn de cub la: x = " << pos.x << " y = " << pos.y << " z = " << pos.z << endl;
}

void Laborator2::AddSfera(glm::vec3 pos)
{
	//adauga buton separat, la fel ca la cuburi
}

void Laborator2::AddCilindru(glm::vec3 pos)
{
	//adauga buton separat, la fel ca la cuburi
}

void Laborator2::OnInputUpdate(float deltaTime, int mods)
{

}

void Laborator2::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_SPACE)
	{
		AddCub(GetCoords());
		AddSfera(GetCoords());
		AddCilindru(GetCoords());
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
