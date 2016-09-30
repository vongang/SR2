#pragma once
#include "gmath.h"
#include <vector>
#include <string>
#include "Object.h"

class Mesh : public Object
{
public:
	std::string name;
	Vec4* vertices;
	uint32 verticesCount;

	Mesh();
	Mesh(std::string _name, int verticesCount);
	~Mesh();
};

