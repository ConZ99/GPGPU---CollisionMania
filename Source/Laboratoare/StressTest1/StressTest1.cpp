#include "StressTest1.h"

#include <vector>
#include <iostream>

#include <cstdlib>
#include <Core/Engine.h>
#include "Obiecte.h"
#include "Transformari.h"
#include <Laboratoare/Laborator2/Item.cpp>

using namespace std;

StressTest1::StressTest1()
{
}

StressTest1::~StressTest1()
{

}

void StressTest1::Init()
{
	{
		Shader* shader = new Shader("ShaderTema");
		shader->AddShader("Source/Laboratoare/StressTest1/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/StressTest1/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{
		//initializam mesh box
		Mesh* Box = Obiecte::Box("box", glm::vec3(.8, .2, .8));
		AddMeshToList(Box);

		//initializam mesh cub
		Mesh* Cub = Obiecte::Cube("cub", glm::vec3(.1, 0, .3));
		AddMeshToList(Cub);

		//initializam mesh obstacol DOAR PENTRU TESTARE
		Mesh* obstacol = Obiecte::Cube("obstacol", glm::vec3(.6, 1, .6));
		AddMeshToList(obstacol);

		//initializam mesh sfera
		Mesh* Sfera = Obiecte::Sphere("sfera", glm::vec3(1, 1, 1));
		Sfera->SetDrawMode(GL_TRIANGLE_FAN);
		AddMeshToList(Sfera);

		//initializam mesh cilindru
		Mesh* Cilindru = Obiecte::Cylinder("cilindru", glm::vec3(1, 0, 1));
		Cilindru->SetDrawMode(GL_TRIANGLES);
		AddMeshToList(Cilindru);
	}
	{
		//box initializat si fixat in (0, 0, 0)
		box = &Item(0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), meshes["box"]);
	}

	for (int i = 0; i < 100; i++)
	{
		AddSfera(GetCoords(), ApplyRandomForce());
	}
	for (int i = 0; i < 100; i++)
	{
		AddCub(GetCoords(), ApplyRandomForce());
	}
	for (int i = 0; i < 100; i++)
	{
		AddCilindru(GetCoords(), ApplyRandomForce());
	}
}

void StressTest1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void StressTest1::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//box rendering
	RenderMesh(meshes["box"], shaders["ShaderTema"], glm::vec3(0, 0, 0), glm::vec3(boxSize));

	renderCuburi(deltaTimeSeconds);
	renderSfere(deltaTimeSeconds);
	renderCilindri(deltaTimeSeconds);

	checkCollision();

	glDisable(GL_CULL_FACE);
}

void StressTest1::FrameEnd()
{
	DrawCoordinatSystem();
}

bool StressTest1::coordsAreNotValid(glm::vec3 pos)
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

glm::vec3 StressTest1::GetCoords()
{
	glm::vec3 coords;
	coords.x = (rand() % ((int)boxSize * 2 - 6)) - boxSize + 3;
	coords.y = (rand() % ((int)boxSize * 2 - 6)) - boxSize + 3;
	coords.z = (rand() % ((int)boxSize * 2 - 6)) - boxSize + 3;

	while (coordsAreNotValid(coords)) {
		coords.x = (rand() % ((int)boxSize * 2 - 6)) - boxSize + 3;
		coords.y = (rand() % ((int)boxSize * 2 - 6)) - boxSize + 3;
		coords.z = (rand() % ((int)boxSize * 2 - 6)) - boxSize + 3;
	}

	return coords;
}

glm::vec3 StressTest1::ApplyRandomForce()
{
	glm::vec3 force;
	force.x = (rand() % 2 - 1) * 5;
	force.y = (rand() % 2 - 1) * 7;
	force.z = rand() % 2;

	return force;
}

void StressTest1::AddCub(glm::vec3 pos, glm::vec3 force)
{
	Item* cub = new Item(objId, force, pos, meshes["cub"]);
	cuburi.push_back(cub);
	objId++;
}

void StressTest1::AddSfera(glm::vec3 pos, glm::vec3 force)
{
	Item* sfera = new Item(objId, force, pos, meshes["sfera"]);
	sfere.push_back(sfera);
	objId++;
}

void StressTest1::AddCilindru(glm::vec3 pos, glm::vec3 force)
{
	Item* cilindru = new Item(objId, force, pos, meshes["cilindru"]);
	cilindri.push_back(cilindru);
	objId++;
}

void StressTest1::OnInputUpdate(float deltaTime, int mods)
{

}

void StressTest1::OnKeyPress(int key, int mods)
{
}

void StressTest1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void StressTest1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void StressTest1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void StressTest1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void StressTest1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void StressTest1::OnWindowResize(int width, int height)
{
}

void StressTest1::renderCuburi(float deltaTimeSeconds) {
	for (Item* it : cuburi) {
		//verifica coliziuni
		//schimba pozitia curenta bazat pe forte
		//daca are o forta aplicata deja, aia scade in delta time seconds si se aplica incet incet gravitatia
		it->pozitiaCurenta += (it->acceleratieGravitationala + it->fortaAplicataCurent) * deltaTimeSeconds;

		if (it->fortaAplicataCurent.x > 0)
			it->fortaAplicataCurent.x += frecareaCuAerul.x * deltaTimeSeconds + it->frecarea.x * deltaTimeSeconds;
		else if (it->fortaAplicataCurent.x < 0)
			it->fortaAplicataCurent.x += -frecareaCuAerul.x * deltaTimeSeconds - it->frecarea.x * deltaTimeSeconds;

		if (it->fortaAplicataCurent.y > 0)
			it->fortaAplicataCurent.y -= (-it->acceleratieGravitationala.y + frecareaCuAerul.y) * deltaTimeSeconds;
		else it->fortaAplicataCurent.y -= frecareaCuAerul.y * deltaTimeSeconds;

		if (it->fortaAplicataCurent.z > 0)
			it->fortaAplicataCurent.z += frecareaCuAerul.z * deltaTimeSeconds + it->frecarea.z * deltaTimeSeconds;
		else if (it->fortaAplicataCurent.z < 0)
			it->fortaAplicataCurent.z += -frecareaCuAerul.z * deltaTimeSeconds - it->frecarea.z * deltaTimeSeconds;

		isCollided(it);

		RenderMesh(it->mesh, shaders["ShaderTema"], it->pozitiaCurenta);
	}
}

void StressTest1::renderSfere(float deltaTimeSeconds)
{
	for (Item* it : sfere) {
		//verifica coliziuni
		//schimba pozitia curenta bazat pe forte
		//daca are o forta aplicata deja, aia scade in delta time seconds si se aplica incet incet gravitatia
		it->pozitiaCurenta += (it->acceleratieGravitationala + it->fortaAplicataCurent) * deltaTimeSeconds;

		if (it->fortaAplicataCurent.x > 0)
			it->fortaAplicataCurent.x += frecareaCuAerul.x * deltaTimeSeconds + it->frecarea.x * deltaTimeSeconds;
		else if (it->fortaAplicataCurent.x < 0)
			it->fortaAplicataCurent.x += -frecareaCuAerul.x * deltaTimeSeconds - it->frecarea.x * deltaTimeSeconds;

		if (it->fortaAplicataCurent.y > 0)
			it->fortaAplicataCurent.y -= (-it->acceleratieGravitationala.y + frecareaCuAerul.y) * deltaTimeSeconds;
		else it->fortaAplicataCurent.y -= frecareaCuAerul.y * deltaTimeSeconds;

		if (it->fortaAplicataCurent.z > 0)
			it->fortaAplicataCurent.z += frecareaCuAerul.z * deltaTimeSeconds + it->frecarea.z * deltaTimeSeconds;
		else if (it->fortaAplicataCurent.z < 0)
			it->fortaAplicataCurent.z += -frecareaCuAerul.z * deltaTimeSeconds - it->frecarea.z * deltaTimeSeconds;

		isCollided(it);

		RenderMesh(it->mesh, shaders["ShaderTema"], it->pozitiaCurenta);
	}
}

void StressTest1::renderCilindri(float deltaTimeSeconds)
{
	for (Item* it : cilindri) {
		//verifica coliziuni
		//schimba pozitia curenta bazat pe forte
		//daca are o forta aplicata deja, aia scade in delta time seconds si se aplica incet incet gravitatia
		it->pozitiaCurenta += (it->acceleratieGravitationala + it->fortaAplicataCurent) * deltaTimeSeconds;

		if (it->fortaAplicataCurent.x > 0)
			it->fortaAplicataCurent.x += frecareaCuAerul.x * deltaTimeSeconds + it->frecarea.x * deltaTimeSeconds;
		else if (it->fortaAplicataCurent.x < 0)
			it->fortaAplicataCurent.x += -frecareaCuAerul.x * deltaTimeSeconds - it->frecarea.x * deltaTimeSeconds;

		if (it->fortaAplicataCurent.y > 0)
			it->fortaAplicataCurent.y -= (-it->acceleratieGravitationala.y + frecareaCuAerul.y) * deltaTimeSeconds;
		else it->fortaAplicataCurent.y -= frecareaCuAerul.y * deltaTimeSeconds;

		if (it->fortaAplicataCurent.z > 0)
			it->fortaAplicataCurent.z += frecareaCuAerul.z * deltaTimeSeconds + it->frecarea.z * deltaTimeSeconds;
		else if (it->fortaAplicataCurent.z < 0)
			it->fortaAplicataCurent.z += -frecareaCuAerul.z * deltaTimeSeconds - it->frecarea.z * deltaTimeSeconds;

		isCollided(it);

		RenderMesh(it->mesh, shaders["ShaderTema"], it->pozitiaCurenta);
	}
}

void StressTest1::isCollided(Item* it)
{
	float pozitieAproximataX = (int)(it->pozitiaCurenta.x * boxSize + .5);
	pozitieAproximataX = (float)pozitieAproximataX / boxSize;

	float pozitieAproximataY = (int)(it->pozitiaCurenta.y * boxSize + .5);
	pozitieAproximataY = (float)pozitieAproximataY / boxSize;

	float pozitieAproximataZ = (int)(it->pozitiaCurenta.z * boxSize + .5);
	pozitieAproximataZ = (float)pozitieAproximataZ / boxSize;

	if (fabs(pozitieAproximataX) + 0.6 > boxSize)
	{
		it->fortaAplicataCurent = glm::vec3(-it->fortaAplicataCurent.x, it->fortaAplicataCurent.y, it->fortaAplicataCurent.z);
	}

	if (fabs(pozitieAproximataY) + 0.7 > boxSize)
	{
		it->fortaAplicataCurent = glm::vec3(it->fortaAplicataCurent.x, 0, it->fortaAplicataCurent.z);
		it->acceleratieGravitationala = glm::vec3(0, 0, 0);
		it->frecarea = glm::vec3(frecareaCuTerenul.x, 0, frecareaCuTerenul.z);
	}

	if (fabs(pozitieAproximataZ) + 0.6 > boxSize)
	{
		it->fortaAplicataCurent = glm::vec3(it->fortaAplicataCurent.x, it->fortaAplicataCurent.y, -it->fortaAplicataCurent.z);
	}
}

void StressTest1::checkCollision()
{
	//verificam fiecare sfera cu restul itemelor inclusiv alte sfere
	bool done = false;
	int fata = 0;
	for (Item* cub : cuburi)
	{
		for (Item* it : sfere)
		{
			if (whereCollided(cub, it) != -1)
			{
				fata = fataLovita(cub, it);
				done = true;
				continue;
			}
		}
		if (done){
			done = false;
			continue;
		}
		for (Item* it : cuburi)
		{
			if (whereCollided(cub, it) != -1)
			{
				fata = fataLovita(cub, it);
				done = true;
				continue;
			}
		}
		if (done) {
			done = false;
			continue;
		}
		for (Item* it : cilindri)
		{
			if (whereCollided(cub, it) != -1)
			{
				fata = fataLovita(cub, it);
				done = true;
				continue;
			}
		}
		if (done) {
			done = false;
			continue;
		}
	}
	for (Item* sfera : sfere)
	{
		for (Item* it : sfere)
		{
			if (whereCollided(sfera, it) != -1)
			{
				fata = fataLovita(sfera, it);
				done = true;
				continue;
			}
		}
		if (done) {
			done = false;
			continue;
		}
		for (Item* it : cuburi)
		{
			if (whereCollided(sfera, it) != -1)
			{
				fata = fataLovita(sfera, it);
				done = true;
				continue;
			}
		}
		if (done) {
			done = false;
			continue;
		}
		for (Item* it : cilindri)
		{
			if (whereCollided(sfera, it) != -1)
			{
				fata = fataLovita(sfera, it);
				done = true;
				continue;
			}
		}
		if (done) {
			done = false;
			continue;
		}
	}
	for (Item* cilindru : cilindri)
	{
		for (Item* it : sfere)
		{
			if (whereCollided(cilindru, it) != -1)
			{
				fata = fataLovita(cilindru, it);
				done = true;
				continue;
			}
		}
		if (done) {
			done = false;
			continue;
		}
		for (Item* it : cuburi)
		{
			if (whereCollided(cilindru, it) != -1)
			{
				fata = fataLovita(cilindru, it);
				done = true;
				continue;
			}
		}
		if (done) {
			done = false;
			continue;
		}
		for (Item* it : cilindri)
		{
			if (whereCollided(cilindru, it) != -1)
			{
				fata = fataLovita(cilindru, it);
				done = true;
				continue;
			}
		}
		if (done) {
			done = false;
			continue;
		}
	}
}

int StressTest1::fataLovita(Item* object, Item* obstacle)
{
	float planZ = object->pozitiaCurenta.z - obstacle->pozitiaCurenta.z;
	float planX = object->pozitiaCurenta.x - obstacle->pozitiaCurenta.x;
	float planY = object->pozitiaCurenta.y - obstacle->pozitiaCurenta.y;

	if (planZ > planX && planZ > planY) {
		//putem baga aici aplicarea fortelor celor doua obiecte ex:
		object->fortaAplicataCurent += glm::vec3(0, 0, 2);
		obstacle->fortaAplicataCurent += glm::vec3(0, 0, -2);
		return 1; //spate
	}
	if (planZ < planX && planZ < planY) {
		//putem baga aici aplicarea fortelor celor doua obiecte ex:
		object->fortaAplicataCurent += glm::vec3(0, 0, -2);
		obstacle->fortaAplicataCurent += glm::vec3(0, 0, 2);
		return 2; //fata
	}
	if (planX > planZ && planX > planY) {
		//putem baga aici aplicarea fortelor celor doua obiecte ex:
		object->fortaAplicataCurent += glm::vec3(2, 0, 0);
		obstacle->fortaAplicataCurent += glm::vec3(-2, 0, 0);
		return 3; //stanga
	}
	if (planX < planZ && planX < planY) {
		//putem baga aici aplicarea fortelor celor doua obiecte ex:
		object->fortaAplicataCurent += glm::vec3(-2, 0, 0);
		obstacle->fortaAplicataCurent += glm::vec3(2, 0, 0);
		return 4; //dreapta
	}
	if (planY > planX && planY > planZ) {
		//putem baga aici aplicarea fortelor celor doua obiecte ex:
		object->fortaAplicataCurent += glm::vec3(0, 2, 0);
		obstacle->fortaAplicataCurent += glm::vec3(0, 0, 0);
		return 5; //jos
	}
	if (planY < planX && planY < planZ) {
		//putem baga aici aplicarea fortelor celor doua obiecte ex:
		object->fortaAplicataCurent += glm::vec3(0, 0, 0);
		obstacle->fortaAplicataCurent += glm::vec3(0, 2, 0);
		return 6; //sus
	}
}

int StressTest1::whereCollided(Item* object, Item* obstacle)
{
	float planZ = object->pozitiaCurenta.z - obstacle->pozitiaCurenta.z;
	int checkZ = 0; //-1 fata; 1 spate
	float planX = object->pozitiaCurenta.x - obstacle->pozitiaCurenta.x;
	int checkX = 0; //-1 stanga; 1 dreapa
	float planY = object->pozitiaCurenta.y - obstacle->pozitiaCurenta.y;
	int checkY = 0; //-1 sub; 1 deasupra

	//sunt in range pe Z, se pot intersecta daca se sincronizeaza si X si Y
	if (planZ >= 0 && planZ <= 1)
	{
		//object este in fata obstacle
		checkZ = -1;
	}
	else if (planZ >= -1 && planZ <= 0)
	{
		//object este in spatele obstacle
		checkZ = 1;
	}

	if (planX >= 0 && planX <= 1)
	{
		//object este in stanga obstacle
		checkX = -1;
	}
	else if (planX >= -1 && planX <= 0)
	{
		//object este in dreapta obstacle
		checkX = 1;
	}

	if (planY >= 0 && planY <= 1)
	{
		//object este sub obstacle
		checkY = -1;
	}
	else if (planY >= -1 && planY <= 0)
	{
		//object este deasupra obstacle
		checkY = 1;
	}

	if (checkZ != 0 && checkX != 0 && checkY != 0)
	{
		
		//avem coliziune
		return 1;
	}
	else
		return -1;
}