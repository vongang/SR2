#include "Mesh.h"

Mesh::Mesh()
{
	vertices = nullptr;
	verticesCount = 0;
}

Mesh::Mesh(std::string _name, int _verticesCount) {
	this->name = _name;
	this->verticesCount = _verticesCount;
	this->vertices = new Vec4[verticesCount];
}

Mesh::~Mesh()
{
	delete[] vertices;
	vertices = nullptr;
}
