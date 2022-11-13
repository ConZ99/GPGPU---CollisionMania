#include "StressTest2.h"

#include <vector>
#include <iostream>

#include <cstdlib>
#include <Core/Engine.h>
#include "Obiecte.h"
#include "Transformari.h"
#include "Item.cpp"

using namespace std;

StressTest2::StressTest2()
{
}

StressTest2::~StressTest2()
{

}

void StressTest2::Init()
{
	{
		Shader* shader = new Shader("ShaderTema");
		shader->AddShader("Source/Laboratoare/StressTest2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/StressTest2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{
		//initializam mesh box
		Mesh* Box = Obiecte::Box("box", glm::vec3(.8, .2, .8));
		AddMeshToList(Box);

		//initializam mesh cub
		Mesh* Cub = Obiecte::Cube("cub", glm::vec3(.6, 0, .6));
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

	for (int i = 0; i < 250; i++)
	{
		AddSfera(GetCoords(), ApplyRandomForce());
	}
	for (int i = 0; i < 500; i++)
	{
		AddCub(GetCoords(), ApplyRandomForce());
	}
	for (int i = 0; i < 1000; i++)
	{
		AddCilindru(GetCoords(), ApplyRandomForce());
	}
}

void StressTest2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void StressTest2::Update(float deltaTimeSeconds)
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

	checkCollision(deltaTimeSeconds);

	glDisable(GL_CULL_FACE);
}

void StressTest2::FrameEnd()
{
	DrawCoordinatSystem();
}

bool StressTest2::coordsAreNotValid(glm::vec3 pos)
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

glm::vec3 StressTest2::GetCoords()
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

glm::vec3 StressTest2::ApplyRandomForce()
{
	glm::vec3 force;
	force.x = (rand() % 2 - 1) * 5;
	force.y = (rand() % 2 - 1) * 7;
	force.z = rand() % 2;

	return force;
}

void StressTest2::AddCub(glm::vec3 pos, glm::vec3 force)
{
	Item* cub = new Item(objId, force, pos, meshes["cub"]);
	cuburi.push_back(cub);
	objId++;
	//cout << "Spawn de cub la: x = " << pos.x << " y = " << pos.y << " z = " << pos.z << endl;
}

void StressTest2::AddSfera(glm::vec3 pos, glm::vec3 force)
{
	Item* sfera = new Item(objId, force, pos, meshes["sfera"]);
	sfere.push_back(sfera);
	objId++;
	//cout << "Spawn de sfera la: x = " << pos.x << " y = " << pos.y << " z = " << pos.z << endl;
}

void StressTest2::AddCilindru(glm::vec3 pos, glm::vec3 force)
{
	Item* cilindru = new Item(objId, force, pos, meshes["cilindru"]);
	cilindri.push_back(cilindru);
	objId++;
	//cout << "Spawn de cilindru la: x = " << pos.x << " y = " << pos.y << " z = " << pos.z << endl;
}

void StressTest2::OnInputUpdate(float deltaTime, int mods)
{

}

void StressTest2::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_P)
	{
		AddCub(GetCoords(), ApplyRandomForce());
	}
	if (key == GLFW_KEY_L)
	{
		AddSfera(GetCoords(), ApplyRandomForce());
	}
	if (key == GLFW_KEY_M)
	{
		AddCilindru(GetCoords(), ApplyRandomForce());
	}
	if (key == GLFW_KEY_R)
	{
		for (Item* it : cuburi)
		{
			it->acceleratieGravitationala = glm::vec3(0, -4, 0);
			it->pozitiaCurenta = GetCoords();
			it->fortaAplicataCurent = ApplyRandomForce();
		}
		for (Item* it : sfere)
		{
			it->acceleratieGravitationala = glm::vec3(0, -4, 0);
			it->pozitiaCurenta = GetCoords();
			it->fortaAplicataCurent = ApplyRandomForce();
		}
		for (Item* it : cilindri)
		{
			it->acceleratieGravitationala = glm::vec3(0, -4, 0);
			it->pozitiaCurenta = GetCoords();
			it->fortaAplicataCurent = ApplyRandomForce();
		}
	}
}

void StressTest2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void StressTest2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void StressTest2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void StressTest2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void StressTest2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void StressTest2::OnWindowResize(int width, int height)
{
}

void StressTest2::renderCuburi(float deltaTimeSeconds) {
	for (Item* it : cuburi) {
		//verifica coliziuni
		//schimba pozitia curenta bazat pe forte
		//daca are o forta aplicata deja, aia scade in delta time seconds si se aplica incet incet gravitatia
		//cout << "Spawn de cub la: x = " << it->fortaAplicataCurent.x << " y = " << it->fortaAplicataCurent.y << " z = " << it->fortaAplicataCurent.z << endl;

		if (it->cadere == 0)
			it->normala.y = it->fortaAplicataCurent.y;
		else if (it->cadere == 1)
		{
			it->normala.y = -it->acceleratieGravitationala.y + it->fortaAplicataCurent.y;
		}
		else if (it->cadere == 2)
		{
			it->normala.y = -it->acceleratieGravitationala.y;
		}

		it->pozitiaCurenta += glm::vec3(it->fortaAplicataCurent.x, it->normala.y + it->acceleratieGravitationala.y, it->fortaAplicataCurent.z) * deltaTimeSeconds;

		if (it->fortaAplicataCurent.x > 0)
			it->fortaAplicataCurent.x += frecareaCuAerul.x * deltaTimeSeconds + it->frecarea.x * deltaTimeSeconds;
		else if (it->fortaAplicataCurent.x < 0)
			it->fortaAplicataCurent.x += -frecareaCuAerul.x * deltaTimeSeconds - it->frecarea.x * deltaTimeSeconds;

		if (it->fortaAplicataCurent.y > 0)
			it->fortaAplicataCurent.y += (it->acceleratieGravitationala.y - frecareaCuAerul.y) * deltaTimeSeconds;
		else if (it->fortaAplicataCurent.y <= 0)
			it->fortaAplicataCurent.y += (it->acceleratieGravitationala.y - frecareaCuAerul.y) * deltaTimeSeconds;

		if (it->fortaAplicataCurent.z > 0)
			it->fortaAplicataCurent.z += frecareaCuAerul.z * deltaTimeSeconds + it->frecarea.z * deltaTimeSeconds;
		else if (it->fortaAplicataCurent.z < 0)
			it->fortaAplicataCurent.z += -frecareaCuAerul.z * deltaTimeSeconds - it->frecarea.z * deltaTimeSeconds;

		isCollided(it);

		if (it->cadere == 0)
			it->normala.y = it->fortaAplicataCurent.y;
		else if (it->cadere == 1)
		{
			it->normala.y = -it->acceleratieGravitationala.y + it->fortaAplicataCurent.y;
		}
		else if (it->cadere == 2)
		{
			it->normala.y = -it->acceleratieGravitationala.y;
		}

		RenderMesh(it->mesh, shaders["ShaderTema"], it->pozitiaCurenta);
	}
}

void StressTest2::renderSfere(float deltaTimeSeconds)
{
	for (Item* it : sfere) {
		//verifica coliziuni
		//schimba pozitia curenta bazat pe forte
		//daca are o forta aplicata deja, aia scade in delta time seconds si se aplica incet incet gravitatia
		if (it->cadere == 0)
			it->normala.y = it->fortaAplicataCurent.y;
		else if (it->cadere == 1)
		{
			it->normala.y = -it->acceleratieGravitationala.y + it->fortaAplicataCurent.y;
		}
		else if (it->cadere == 2)
		{
			it->normala.y = -it->acceleratieGravitationala.y;
		}

		it->pozitiaCurenta += glm::vec3(it->fortaAplicataCurent.x, it->normala.y + it->acceleratieGravitationala.y, it->fortaAplicataCurent.z) * deltaTimeSeconds;

		if (it->fortaAplicataCurent.x > 0)
			it->fortaAplicataCurent.x += frecareaCuAerul.x * deltaTimeSeconds + it->frecarea.x * deltaTimeSeconds;
		else if (it->fortaAplicataCurent.x < 0)
			it->fortaAplicataCurent.x += -frecareaCuAerul.x * deltaTimeSeconds - it->frecarea.x * deltaTimeSeconds;

		if (it->fortaAplicataCurent.y > 0)
			it->fortaAplicataCurent.y += (it->acceleratieGravitationala.y - frecareaCuAerul.y) * deltaTimeSeconds;
		else if (it->fortaAplicataCurent.y <= 0)
			it->fortaAplicataCurent.y += (it->acceleratieGravitationala.y - frecareaCuAerul.y) * deltaTimeSeconds;

		if (it->fortaAplicataCurent.z > 0)
			it->fortaAplicataCurent.z += frecareaCuAerul.z * deltaTimeSeconds + it->frecarea.z * deltaTimeSeconds;
		else if (it->fortaAplicataCurent.z < 0)
			it->fortaAplicataCurent.z += -frecareaCuAerul.z * deltaTimeSeconds - it->frecarea.z * deltaTimeSeconds;

		isCollided(it);

		if (it->cadere == 0)
			it->normala.y = it->fortaAplicataCurent.y;
		else if (it->cadere == 1)
		{
			it->normala.y = -it->acceleratieGravitationala.y + it->fortaAplicataCurent.y;
		}
		else if (it->cadere == 2)
		{
			it->normala.y = -it->acceleratieGravitationala.y;
		}

		RenderMesh(it->mesh, shaders["ShaderTema"], it->pozitiaCurenta);
	}
}

void StressTest2::renderCilindri(float deltaTimeSeconds)
{
	for (Item* it : cilindri) {
		//verifica coliziuni
		//schimba pozitia curenta bazat pe forte
		//daca are o forta aplicata deja, aia scade in delta time seconds si se aplica incet incet gravitatia
		if (it->cadere == 0)
			it->normala.y = it->fortaAplicataCurent.y;
		else if (it->cadere == 1)
		{
			it->normala.y = -it->acceleratieGravitationala.y + it->fortaAplicataCurent.y;
		}
		else if (it->cadere == 2)
		{
			it->normala.y = -it->acceleratieGravitationala.y;
		}

		it->pozitiaCurenta += glm::vec3(it->fortaAplicataCurent.x, it->normala.y + it->acceleratieGravitationala.y, it->fortaAplicataCurent.z) * deltaTimeSeconds;

		if (it->fortaAplicataCurent.x > 0)
			it->fortaAplicataCurent.x += frecareaCuAerul.x * deltaTimeSeconds + it->frecarea.x * deltaTimeSeconds;
		else if (it->fortaAplicataCurent.x < 0)
			it->fortaAplicataCurent.x += -frecareaCuAerul.x * deltaTimeSeconds - it->frecarea.x * deltaTimeSeconds;

		if (it->fortaAplicataCurent.y > 0)
			it->fortaAplicataCurent.y += (it->acceleratieGravitationala.y - frecareaCuAerul.y) * deltaTimeSeconds;
		else if (it->fortaAplicataCurent.y <= 0)
			it->fortaAplicataCurent.y += (it->acceleratieGravitationala.y - frecareaCuAerul.y) * deltaTimeSeconds;

		if (it->fortaAplicataCurent.z > 0)
			it->fortaAplicataCurent.z += frecareaCuAerul.z * deltaTimeSeconds + it->frecarea.z * deltaTimeSeconds;
		else if (it->fortaAplicataCurent.z < 0)
			it->fortaAplicataCurent.z += -frecareaCuAerul.z * deltaTimeSeconds - it->frecarea.z * deltaTimeSeconds;

		isCollided(it);

		if (it->cadere == 0)
			it->normala.y = it->fortaAplicataCurent.y;
		else if (it->cadere == 1)
		{
			it->normala.y = -it->acceleratieGravitationala.y + it->fortaAplicataCurent.y;
		}
		else if (it->cadere == 2)
		{
			it->normala.y = -it->acceleratieGravitationala.y;
		}

		RenderMesh(it->mesh, shaders["ShaderTema"], it->pozitiaCurenta);
	}
}

//trebuie sa sara putin + sa se opreasca la 0

void StressTest2::isCollided(Item* it)
{
	if (it->pozitiaCurenta.x > boxSize - .45)
	{
		it->pozitiaCurenta.x = boxSize - 0.5;
		it->fortaAplicataCurent = glm::vec3(-it->fortaAplicataCurent.x, it->fortaAplicataCurent.y, it->fortaAplicataCurent.z);
	}
	if (it->pozitiaCurenta.x < -boxSize + .45)
	{
		it->pozitiaCurenta.x = -boxSize + 0.5;
		it->fortaAplicataCurent = glm::vec3(-it->fortaAplicataCurent.x, it->fortaAplicataCurent.y, it->fortaAplicataCurent.z);
	}
	if (it->pozitiaCurenta.y < -boxSize + .45)
	{
		it->pozitiaCurenta.y = -boxSize + 0.5;
		it->fortaAplicataCurent = glm::vec3(it->fortaAplicataCurent.x, 0, it->fortaAplicataCurent.z);
		it->frecarea = glm::vec3(frecareaCuTerenul.x, 0, frecareaCuTerenul.z);
	}
	if (it->pozitiaCurenta.z < -boxSize + .45)
	{
		it->pozitiaCurenta.z = -boxSize + 0.5;
		it->fortaAplicataCurent = glm::vec3(it->fortaAplicataCurent.x, it->fortaAplicataCurent.y, -it->fortaAplicataCurent.z);
	}
	if (it->pozitiaCurenta.z > boxSize - .45)
	{
		it->pozitiaCurenta.z = boxSize - 0.5;
		it->fortaAplicataCurent = glm::vec3(it->fortaAplicataCurent.x, it->fortaAplicataCurent.y, -it->fortaAplicataCurent.z);
	}
}

void StressTest2::checkCollision(float deltaTimeSeconds)
{
	//verificam fiecare sfera cu restul itemelor inclusiv alte sfere
	bool done = false;
	int fata = 0;
	for (Item* cub : cuburi)
	{
		cout << cub->IdObiect << endl;
		for (Item* it : sfere)
		{
			if (whereCollided(cub, it) != -1)
			{
				fata = fataLovita(cub, it, deltaTimeSeconds);
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
			if (whereCollided(cub, it) != -1)
			{
				fata = fataLovita(cub, it, deltaTimeSeconds);
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
				fata = fataLovita(cub, it, deltaTimeSeconds);
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
				fata = fataLovita(sfera, it, deltaTimeSeconds);
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
				fata = fataLovita(sfera, it, deltaTimeSeconds);
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
				fata = fataLovita(sfera, it, deltaTimeSeconds);
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
				fata = fataLovita(cilindru, it, deltaTimeSeconds);
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
				fata = fataLovita(cilindru, it, deltaTimeSeconds);
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
				fata = fataLovita(cilindru, it, deltaTimeSeconds);
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

int StressTest2::fataLovita(Item* object, Item* obstacle, float deltaTimeSeconds)
{
	float planZ = object->pozitiaCurenta.z - obstacle->pozitiaCurenta.z;
	float planX = object->pozitiaCurenta.x - obstacle->pozitiaCurenta.x;
	float planY = object->pozitiaCurenta.y - obstacle->pozitiaCurenta.y;

	glm::vec3 objectForce = object->fortaAplicataCurent;
	glm::vec3 obstacleForce = obstacle->fortaAplicataCurent;

	if (planZ > planX && planZ > planY) {
		cout << "spate" << " obiectul " << object->IdObiect << endl;
		//putem baga aici aplicarea fortelor celor doua obiecte ex:
		object->pozitiaCurenta.z = obstacle->pozitiaCurenta.z + 1;

		if (obstacle->fortaAplicataCurent.z > 0 && round(object->fortaAplicataCurent.z) == 0)
		{
			object->fortaAplicataCurent.z = obstacleForce.z / 2;
			obstacle->fortaAplicataCurent.z = obstacleForce.z / 2;
		}
		else if (round(obstacle->fortaAplicataCurent.z) == 0 && object->fortaAplicataCurent.z < 0)
		{
			object->fortaAplicataCurent.z = objectForce.z / 2;
			obstacle->fortaAplicataCurent.z = objectForce.z / 2;
		}

		return 1; //spate
	}
	if (planZ < planX && planZ < planY) {
		cout << "fata" << " obiectul " << object->IdObiect << endl;
		//putem baga aici aplicarea fortelor celor doua obiecte ex:
		object->pozitiaCurenta.z = obstacle->pozitiaCurenta.z - 1;

		if (obstacle->fortaAplicataCurent.z > 0 && object->fortaAplicataCurent.z > 0)
		{
			object->fortaAplicataCurent.z += -obstacleForce.z / 2;
			obstacle->fortaAplicataCurent.z += objectForce.z / 2;
		}
		else if (obstacle->fortaAplicataCurent.z < 0 && object->fortaAplicataCurent.z < 0)
		{
			object->fortaAplicataCurent.z += obstacleForce.z / 2;
			obstacle->fortaAplicataCurent.z += -objectForce.z / 2;
		}
		else if (obstacle->fortaAplicataCurent.z < 0 && object->fortaAplicataCurent.z > 0)
		{
			object->fortaAplicataCurent.z = -objectForce.z / 2 + obstacleForce.z / 2;
			obstacle->fortaAplicataCurent.z = -obstacleForce.z / 2 + objectForce.z / 2;
		}
		else if (obstacle->fortaAplicataCurent.z < 0 && round(object->fortaAplicataCurent.z) == 0)
		{
			object->fortaAplicataCurent.z = obstacleForce.z / 2;
			obstacle->fortaAplicataCurent.z = obstacleForce.z / 2;
		}
		else if (round(obstacle->fortaAplicataCurent.z) == 0 && object->fortaAplicataCurent.z > 0)
		{
			object->fortaAplicataCurent.z = objectForce.z / 2;
			obstacle->fortaAplicataCurent.z = objectForce.z / 2;
		}
		else if (round(obstacle->fortaAplicataCurent.z + object->fortaAplicataCurent.z) == 0)
		{
			object->fortaAplicataCurent.z = object->fortaAplicataCurent.x / 3;
			obstacle->fortaAplicataCurent.z = -object->fortaAplicataCurent.x / 3;
		}

		return 2; //fata
	}
	if (planX > planZ && planX > planY) {
		cout << "stanga" << " obiectul " << object->IdObiect << endl;
		//putem baga aici aplicarea fortelor celor doua obiecte ex:
		object->pozitiaCurenta.x = obstacle->pozitiaCurenta.x + 1;

		if (obstacle->fortaAplicataCurent.x < 0 && object->fortaAplicataCurent.x < 0)
		{
			object->fortaAplicataCurent.x += -obstacleForce.x / 2;
			obstacle->fortaAplicataCurent.x += objectForce.x / 2;
		}
		else if (obstacle->fortaAplicataCurent.x > 0 && object->fortaAplicataCurent.x > 0)
		{
			object->fortaAplicataCurent.x += obstacleForce.x / 2;
			obstacle->fortaAplicataCurent.x += -objectForce.x / 2;
		}
		else if (obstacle->fortaAplicataCurent.x > 0 && round(object->fortaAplicataCurent.x) == 0)
		{
			object->fortaAplicataCurent.x = obstacleForce.x / 2;
			obstacle->fortaAplicataCurent.x = obstacleForce.x / 3;
		}
		else if (round(obstacle->fortaAplicataCurent.x) == 0 && object->fortaAplicataCurent.x < 0)
		{
			object->fortaAplicataCurent.x = objectForce.x / 3;
			obstacle->fortaAplicataCurent.x = objectForce.x / 2;
		}
		else if (round(obstacle->fortaAplicataCurent.x + object->fortaAplicataCurent.x) == 0)
		{
			object->fortaAplicataCurent.x = 0;
			obstacle->fortaAplicataCurent.x = 0;
		}

		return 3; //stanga
	}
	if (planX < planZ && planX < planY) {
		cout << "dreapta" << " obiectul " << object->IdObiect << endl;
		//putem baga aici aplicarea fortelor celor doua obiecte ex:
		object->pozitiaCurenta.x = obstacle->pozitiaCurenta.x - 1;

		if (obstacle->fortaAplicataCurent.x > 0 && object->fortaAplicataCurent.x > 0)
		{
			object->fortaAplicataCurent.x += -obstacleForce.x / 2;
			obstacle->fortaAplicataCurent.x += objectForce.x / 2;
		}
		else if (obstacle->fortaAplicataCurent.x < 0 && object->fortaAplicataCurent.x < 0)
		{
			object->fortaAplicataCurent.x += obstacleForce.x / 2;
			obstacle->fortaAplicataCurent.x += -objectForce.x / 2;
		}
		else if (obstacle->fortaAplicataCurent.x < 0 && object->fortaAplicataCurent.x > 0)
		{
			object->fortaAplicataCurent.x = -objectForce.x / 2 + obstacleForce.x / 2;
			obstacle->fortaAplicataCurent.x = -obstacleForce.x / 2 + objectForce.x / 2;
		}
		else if (obstacle->fortaAplicataCurent.x < 0 && round(object->fortaAplicataCurent.x) == 0)
		{
			object->fortaAplicataCurent.x = obstacleForce.x / 2;
			obstacle->fortaAplicataCurent.x = obstacleForce.x / 3;
		}
		else if (round(obstacle->fortaAplicataCurent.x) == 0 && object->fortaAplicataCurent.x > 0)
		{
			object->fortaAplicataCurent.x = objectForce.x / 3;
			obstacle->fortaAplicataCurent.x = objectForce.x / 2;
		}
		else if (round(obstacle->fortaAplicataCurent.x + object->fortaAplicataCurent.x) == 0)
		{
			object->fortaAplicataCurent.x = object->fortaAplicataCurent.z / 2;
			obstacle->fortaAplicataCurent.x = object->fortaAplicataCurent.z / 2;
		}

		return 4; //dreapta
	}
	if (planY > planX && planY > planZ) {
		cout << "jos" << " obiectul " << object->IdObiect << endl;
		//putem baga aici aplicarea fortelor celor doua obiecte ex:
		object->pozitiaCurenta.y = obstacle->pozitiaCurenta.y + 1;

		if (obstacle->acceleratieGravitationala.y == 0)
		{
			if (round(object->fortaAplicataCurent.y) / 2 > 0)
			{
				object->fortaAplicataCurent = glm::vec3(0, object->fortaAplicataCurent.y / 2, 0);
				object->cadere = 1;
				object->normala = glm::vec3(0, -object->acceleratieGravitationala.y + object->fortaAplicataCurent.y, 0);
				cout << (int)object->fortaAplicataCurent.y / 2 << endl;
			}
			else if (round(object->fortaAplicataCurent.y) / 2 < 0)
			{
				object->fortaAplicataCurent = glm::vec3(0, -object->fortaAplicataCurent.y / 2, 0);
				object->cadere = 1;
				object->normala = glm::vec3(0, -object->acceleratieGravitationala.y + object->fortaAplicataCurent.y, 0);
				cout << object->fortaAplicataCurent.y << endl;
			}
			else if (round(object->fortaAplicataCurent.y) / 2 == 0)
			{
				object->fortaAplicataCurent = glm::vec3(0, 0, 0);
				object->cadere = 2;
			}
		}

		if (obstacle->acceleratieGravitationala.y != 0)
		{
			if (obstacle->fortaAplicataCurent.y + obstacle->acceleratieGravitationala.y > 0)
			{
				if (int(object->fortaAplicataCurent.y / 4) != 0)
				{
					object->fortaAplicataCurent = glm::vec3(0, -object->fortaAplicataCurent.y / 4, 0);
					object->cadere = 1;
					object->normala = glm::vec3(0, -object->acceleratieGravitationala.y + object->fortaAplicataCurent.y, 0);

				}
				else
				{
					object->fortaAplicataCurent = glm::vec3(0, 0, 0);
					object->cadere = 2;
				}

				obstacle->fortaAplicataCurent.y += object->fortaAplicataCurent.y;
			}

			else if (object->fortaAplicataCurent.y + object->acceleratieGravitationala.y < 0)
			{
				obstacle->fortaAplicataCurent.y += object->fortaAplicataCurent.y / 3;
			}
		}

		return 5; //jos
	}
	if (planY < planX && planY < planZ) {
		cout << "sus" << " obiectul " << object->IdObiect << endl;
		object->pozitiaCurenta.y = obstacle->pozitiaCurenta.y - 1;

		if (object->acceleratieGravitationala.y == 0)
		{
			object->fortaAplicataCurent.y = 0;
		}

		if (object->acceleratieGravitationala.y != 0)
		{
			if (object->fortaAplicataCurent.y + object->acceleratieGravitationala.y < 0)
			{
				object->fortaAplicataCurent += glm::vec3(0, obstacle->fortaAplicataCurent.y / 3, 0);
				//object->cadere = 1;
				//object->normala = glm::vec3(0, -object->acceleratieGravitationala.y + object->fortaAplicataCurent.y, 0);
			}
			else if (object->fortaAplicataCurent.y + object->acceleratieGravitationala.y > 0)
			{
				object->fortaAplicataCurent += glm::vec3(0, obstacle->fortaAplicataCurent.y, 0);
				//object->cadere = 2;
				obstacle->fortaAplicataCurent.y += object->fortaAplicataCurent.y;
			}

		}

		return 6; //sus
	}
}

int StressTest2::whereCollided(Item* object, Item* obstacle)
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