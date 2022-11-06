#include "Laborator2.h"

#include <vector>
#include <iostream>

#include <cstdlib>
#include <Core/Engine.h>
#include "Obiecte.h"
#include "Transformari.h"
#include "Item.cpp"

using namespace std;

Laborator2::Laborator2()
{
}

Laborator2::~Laborator2()
{

}

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
	//fixed objects, just for testing, leave tf alone
	RenderMesh(meshes["cub"], shaders["ShaderTema"], glm::vec3(0, 0, 0));
	RenderMesh(meshes["sfera"], shaders["ShaderTema"], glm::vec3(-5, 0, 0));
	RenderMesh(meshes["cilindru"], shaders["ShaderTema"], glm::vec3(5, 0, 0));

	cout << cuburi.size() << endl;

	renderCuburi(deltaTimeSeconds);
	renderSfere(deltaTimeSeconds);
	renderCilindri(deltaTimeSeconds);

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
	srand(time(0));
	coords.x = (rand() % ((int)boxSize * 2 - 2)) - boxSize + 1;
	coords.y = (rand() % ((int)boxSize * 2 - 2)) - boxSize + 1;
	coords.z = (rand() % ((int)boxSize * 2 - 2)) - boxSize + 1;

	while (coordsAreNotValid(coords)) {
		srand(time(0));
		coords.x = (rand() % ((int)boxSize * 2 - 2)) - boxSize + 1;
		coords.y = (rand() % ((int)boxSize * 2 - 2)) - boxSize + 1;
		coords.z = (rand() % ((int)boxSize * 2 - 2)) - boxSize + 1;
	}

	return coords;
}

glm::vec3 Laborator2::ApplyRandomForce()
{
	glm::vec3 force;
	srand(time(0));
	force.x = rand() % 2;
	force.y = 7;
	force.z = rand() % 2;

	return force;
}

void Laborator2::AddCub(glm::vec3 pos, glm::vec3 force)
{
	Item* cub = new Item(999, force, pos, meshes["cub"]);
	cuburi.push_back(cub);
	cout << "Spawn de cub la: x = " << pos.x << " y = " << pos.y << " z = " << pos.z << endl;
}

void Laborator2::AddSfera(glm::vec3 pos, glm::vec3 force)
{
	Item* sfera = new Item(999, force, pos, meshes["sfera"]);
	sfere.push_back(sfera);
	cout << "Spawn de sfera la: x = " << pos.x << " y = " << pos.y << " z = " << pos.z << endl;
}

void Laborator2::AddCilindru(glm::vec3 pos, glm::vec3 force)
{
	Item* cilindru = new Item(999, force, pos, meshes["cilindru"]);
	cilindri.push_back(cilindru);
	cout << "Spawn de cilindru la: x = " << pos.x << " y = " << pos.y << " z = " << pos.z << endl;
}

void Laborator2::OnInputUpdate(float deltaTime, int mods)
{

}

void Laborator2::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_SPACE)
	{
		AddCub(GetCoords(), ApplyRandomForce());
		//AddSfera(GetCoords(), ApplyRandomForce());
		//AddCilindru(GetCoords(), ApplyRandomForce());
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

void Laborator2::renderCuburi(float deltaTimeSeconds) {
	for (Item* it : cuburi) {
		//verifica coliziuni
		//schimba pozitia curenta bazat pe forte
		//daca are o forta aplicata deja, aia scade in delta time seconds si se aplica incet incet gravitatia
		//cout << "Spawn de cub la: x = " << it->fortaAplicataCurent.x << " y = " << it->fortaAplicataCurent.y << " z = " << it->fortaAplicataCurent.z << endl;
		it->pozitiaCurenta += (acceleratieGravitationala + it->fortaAplicataCurent) * deltaTimeSeconds;
		if (it->fortaAplicataCurent.x >= 0)
			it->fortaAplicataCurent.x += frecareaCuAerul.x * deltaTimeSeconds;
		else it->fortaAplicataCurent.x -= frecareaCuAerul.x * deltaTimeSeconds;
		if (it->fortaAplicataCurent.y >= 0)
			it->fortaAplicataCurent.y -= (-acceleratieGravitationala.y + frecareaCuAerul.y) * deltaTimeSeconds;
		else it->fortaAplicataCurent.y -= frecareaCuAerul.y * deltaTimeSeconds;
		if (it->fortaAplicataCurent.z >= 0)
			it->fortaAplicataCurent.z += frecareaCuAerul.z * deltaTimeSeconds;
		else it->fortaAplicataCurent.z -= frecareaCuAerul.z * deltaTimeSeconds;

		RenderMesh(it->mesh, shaders["ShaderTema"], it->pozitiaCurenta);
	}
}
void Laborator2::renderSfere(float deltaTimeSeconds)
{
	for (Item* it : sfere) {
		//verifica coliziuni
		//schimba pozitia curenta bazat pe forte
		//daca are o forta aplicata deja, aia scade in delta time seconds si se aplica incet incet gravitatia
		cout << "Spawn de cub la: x = " << it->fortaAplicataCurent.x << " y = " << it->fortaAplicataCurent.y << " z = " << it->fortaAplicataCurent.z << endl;
		it->pozitiaCurenta += (acceleratieGravitationala + it->fortaAplicataCurent) * deltaTimeSeconds;
		if (it->fortaAplicataCurent.x >= 0)
			it->fortaAplicataCurent.x += frecareaCuAerul.x * deltaTimeSeconds;
		else it->fortaAplicataCurent.x -= frecareaCuAerul.x * deltaTimeSeconds;
		if (it->fortaAplicataCurent.y >= 0)
			it->fortaAplicataCurent.y -= (-acceleratieGravitationala.y + frecareaCuAerul.y) * deltaTimeSeconds;
		else it->fortaAplicataCurent.y -= frecareaCuAerul.y * deltaTimeSeconds;
		if (it->fortaAplicataCurent.z >= 0)
			it->fortaAplicataCurent.z += frecareaCuAerul.z * deltaTimeSeconds;
		else it->fortaAplicataCurent.z -= frecareaCuAerul.z * deltaTimeSeconds;

		RenderMesh(it->mesh, shaders["ShaderTema"], it->pozitiaCurenta);
	}
}
void Laborator2::renderCilindri(float deltaTimeSeconds)
{
	for (Item* it : cilindri) {
		//verifica coliziuni
		//schimba pozitia curenta bazat pe forte
		//daca are o forta aplicata deja, aia scade in delta time seconds si se aplica incet incet gravitatia
		cout << "Spawn de cub la: x = " << it->fortaAplicataCurent.x << " y = " << it->fortaAplicataCurent.y << " z = " << it->fortaAplicataCurent.z << endl;
		it->pozitiaCurenta += (acceleratieGravitationala + it->fortaAplicataCurent) * deltaTimeSeconds;
		if (it->fortaAplicataCurent.x >= 0)
			it->fortaAplicataCurent.x += frecareaCuAerul.x * deltaTimeSeconds;
		else it->fortaAplicataCurent.x -= frecareaCuAerul.x * deltaTimeSeconds;
		if (it->fortaAplicataCurent.y >= 0)
			it->fortaAplicataCurent.y -= (-acceleratieGravitationala.y + frecareaCuAerul.y) * deltaTimeSeconds;
		else it->fortaAplicataCurent.y -= frecareaCuAerul.y * deltaTimeSeconds;
		if (it->fortaAplicataCurent.z >= 0)
			it->fortaAplicataCurent.z += frecareaCuAerul.z * deltaTimeSeconds;
		else it->fortaAplicataCurent.z -= frecareaCuAerul.z * deltaTimeSeconds;

		RenderMesh(it->mesh, shaders["ShaderTema"], it->pozitiaCurenta);
	}
}