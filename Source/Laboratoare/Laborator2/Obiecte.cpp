#include "Obiecte.h"
#include <vector>
#include <string>
#include <iostream>
#include <Core/Engine.h>

Mesh* Obiecte::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices) {
	unsigned int VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

	glBindVertexArray(0);

	CheckOpenGLError();

	Mesh* mesh = new Mesh(name);
	mesh->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	mesh->vertices = vertices;
	mesh->indices = indices;
	return mesh;
}

Mesh* Obiecte::Box(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 1, 1), color),
		VertexFormat(glm::vec3(1, -1,  1), glm::vec3(1, 0, 1), color),
		VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1, 0, 0), color),
		VertexFormat(glm::vec3(1,  1,  1), glm::vec3(0, 1, 0), color),
		VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), color),
		VertexFormat(glm::vec3(1, -1, -1), glm::vec3(0, 1, 1), color),
		VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1, 1, 0), color),
		VertexFormat(glm::vec3(1,  1, -1), glm::vec3(0, 0, 1), color),
	};

	std::vector<unsigned short> indices =
	{
		
		2, 3, 7,		2, 7, 6,
		7, 3, 2,		6, 7, 2,//double
		1, 7, 3,		1, 5, 7,
		3, 7, 1,		7, 5, 1,//double
		6, 7, 4,		7, 5, 4,
		4, 7, 6,		4, 5, 7,//double
		0, 4, 1,		1, 4, 5,
		1, 4, 0,		5, 4, 1,//double
		2, 6, 4,		0, 2, 4,
		4, 6, 2,		4, 2, 0,//double
	};

	return CreateMesh(name.c_str(), vertices, indices);
}

Mesh* Obiecte::Cube(std::string name, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 1, 1), color),
		VertexFormat(glm::vec3(1, -1,  1), glm::vec3(1, 0, 1), color),
		VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1, 0, 0), color),
		VertexFormat(glm::vec3(1,  1,  1), glm::vec3(0, 1, 0), color),
		VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), color),
		VertexFormat(glm::vec3(1, -1, -1), glm::vec3(0, 1, 1), color),
		VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1, 1, 0), color),
		VertexFormat(glm::vec3(1,  1, -1), glm::vec3(0, 0, 1), color),
	};

	std::vector<unsigned short> indices =
	{
		0, 1, 2,		1, 3, 2,
		2, 3, 7,		2, 7, 6,
		1, 7, 3,		1, 5, 7,
		6, 7, 4,		7, 5, 4,
		0, 4, 1,		1, 4, 5,
		2, 6, 4,		0, 2, 4,
	};

	return CreateMesh(name.c_str(), vertices, indices);
}

Mesh* Obiecte::Sphere(std::string name, glm::vec3 color) {
	std::vector<VertexFormat> cerc_vertices{};
	std::vector<unsigned short> cerc_indices{};
	
	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / 1;    // vertex normal
	int index = 1;
	float sectorStep = 2 * 3.142 / 72;
	float stackStep = 3.142 / 36;
	float sectorAngle, stackAngle;
	int k1, k2;
	for (int i = 0; i <= 36; ++i)
	{
		k1 = i * (72 + 1);     // beginning of current stack
		k2 = k1 + 72 + 1;      // beginning of next stack
		stackAngle = 3.142 / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = 1 * cosf(stackAngle);             // r * cos(u)
		z = 1 * sinf(stackAngle);              // r * sin(u)

		
		for (int j = 0; j <= 72; ++j, ++k1, ++k2)
		{
			if (i != 0)
			{
				cerc_indices.push_back(k1);
				cerc_indices.push_back(k2);
				cerc_indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (36 - 1))
			{
				cerc_indices.push_back(k1 + 1);
				cerc_indices.push_back(k2);
				cerc_indices.push_back(k2 + 1);
			}

			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;

			cerc_vertices.push_back(VertexFormat(glm::vec3(x, y, z), glm::vec3(nx, ny, nz), color));
		}
	}
	return CreateMesh(name.c_str(), cerc_vertices, cerc_indices);
}

Mesh* Obiecte::Cylinder(std::string name, glm::vec3 color) {
	
	std::vector<VertexFormat> cerc_vertices{};
	std::vector<unsigned short> cerc_indices{};

	int centru = 0;
	int sides = 360;
	double radius = .5;
	int index = 1;
	int z = 1;

	for (int i = 0; i < 360; i += 1) {
		double heading = i * 3.142 / 180;
		cerc_vertices.push_back(VertexFormat(glm::vec3(cos(heading) * radius, sin(heading) * radius, 0), glm::vec3(1, 1, 1)));
	}
	for (int i = 0; i < 360; i += 1) {
		double heading = i * 3.142 / 180;
		cerc_vertices.push_back(VertexFormat(glm::vec3(cos(heading) * radius, sin(heading) * radius, 1), glm::vec3(1, 1, 1)));
	}

	int k1 = 0;                         // 1st vertex index at base
	int k2 = 360 + 1;           // 1st vertex index at top

	// indices for the side surface
	for (int i = 0; i < 360; ++i, ++k1, ++k2)
	{
		// 2 triangles per sector
		// k1 => k1+1 => k2
		cerc_indices.push_back(k1);
		cerc_indices.push_back(k1 + 1);
		cerc_indices.push_back(k2);

		// k2 => k1+1 => k2+1
		cerc_indices.push_back(k2);
		cerc_indices.push_back(k1 + 1);
		cerc_indices.push_back(k2 + 1);
	}

	// indices for the base surface
	//NOTE: baseCenterIndex and topCenterIndices are pre-computed during vertex generation
	//      please see the previous code snippet
	for (int i = 0, k = 0 + 1; i < 360; ++i, ++k)
	{
		if (i < 360 - 1)
		{
			cerc_indices.push_back(0);
			cerc_indices.push_back(k + 1);
			cerc_indices.push_back(k);
		}
		else // last triangle
		{
			cerc_indices.push_back(0);
			cerc_indices.push_back(1);
			cerc_indices.push_back(k);
		}
	}

	// indices for the top surface
	for (int i = 0, k = 360 + 1; i < 360; ++i, ++k)
	{
		if (i < 360 - 1)
		{
			cerc_indices.push_back(360);
			cerc_indices.push_back(k);
			cerc_indices.push_back(k + 1);
		}
		else // last triangle
		{
			cerc_indices.push_back(360);
			cerc_indices.push_back(k);
			cerc_indices.push_back(360 + 1);
		}
	}


	return CreateMesh(name.c_str(), cerc_vertices, cerc_indices);
}