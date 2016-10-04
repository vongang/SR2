#pragma once
#ifndef _UTIL_H
#define _UTIL_H

#include <vector>
#include <algorithm>

#include "Mesh.h"
#include "gmath.h"

class Util
{
public:
	Util() = default;
	bool LoadOBJ(const char* path, Mesh* mesh);
	~Util() = default;
};
#endif
