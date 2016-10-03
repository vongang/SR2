#include "Mesh.h"

Mesh::Mesh()
{
	vertices = nullptr;
	faces = nullptr;
	normals = nullptr;
	vt_count = 0;
	face_count = 0;
}

Mesh::Mesh(std::string _name, int _vt_count) {
	this->name = _name;
	this->vt_count = _vt_count;
	this->vertices = new Vec4[_vt_count];
	this->normals = new Vec4[_vt_count];
}

Mesh::Mesh(std::string _name, int _vt_count, int _face_count) {
	this->name = _name;
	this->vt_count = _vt_count;
	this->face_count = _face_count;
	this->vertices = new Vec4[_vt_count];
	this->normals = new Vec4[_vt_count];
	this->faces = new Face[_face_count];
}

Mesh::~Mesh()
{
	delete[] vertices;
	delete[] faces;
	delete[] normals;
	vertices = nullptr;
	faces = nullptr;
	normals = nullptr;
}
