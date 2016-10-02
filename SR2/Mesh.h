#pragma once
#ifndef _MESH_H
#define _MESH_H
#include "Gmath.h"
#include <vector>
#include <string>
#include "Object.h"

class Face {
public:
	uint32 A;
	uint32 B;
	uint32 C;
	Face() = default;
	~Face() = default;

	Face(uint32 _A, uint32 _B, uint32 _C) :A(_A), B(_B), C(_C) {}
};

class Mesh : public Object
{
	
public:
	std::string name;
	Vec4* vertices;
	Face* faces;
	uint32 vt_count;
	uint32 face_count;

	Mesh();
	Mesh(std::string _name, int vt_count);
	Mesh(std::string _name, int vt_count, int face_count);
	~Mesh();
};

#endif