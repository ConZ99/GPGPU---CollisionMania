#pragma once

#include <Component/SimpleScene.h>
#include <Core/Engine.h>
using namespace std;
class Item {
	public: 
		int IdObiect = -1;
		glm::vec3 fortaAplicataCurent = glm::vec3(0, 0, 0); //o forta ce ii este aplicata obiectului, poate scade/creste/schimba directia s.a.m.d.
		glm::vec3 pozitiaCurenta = glm::vec3(0, 0, 0);		//pozitia curenta a obiectului
		glm::vec3 frecarea = glm::vec3(0, 0, 0);
		glm::vec3 acceleratieGravitationala = glm::vec3(0, -4, 0);
		Mesh* mesh;											//meshul asignat obiectului

		Item(int _idObiect, glm::vec3 _fortaAplicataCurent, glm::vec3 _pozitiaCurenta, Mesh* _mesh) {
			IdObiect = _idObiect;
			fortaAplicataCurent = _fortaAplicataCurent;
			pozitiaCurenta = _pozitiaCurenta;
			mesh = _mesh;
		}

		int GetIdObiect() {
			return IdObiect;
		}
};