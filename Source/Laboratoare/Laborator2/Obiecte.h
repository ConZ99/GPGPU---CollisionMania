#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include <vector>

namespace Obiecte {

	Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);

	Mesh* Cube(std::string name, glm::vec3 color);
	Mesh* Box(std::string name, glm::vec3 color);
	Mesh* Sphere(std::string name, glm::vec3 color);
	Mesh* Cylinder(std::string name, glm::vec3 color);
}
