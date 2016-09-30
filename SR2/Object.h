#pragma once
#include "gmath.h"

class Object
{
public:
	Vec4 position;
	Mat4 translate_matrix;
	Mat4 scale_matrix;
	Mat4 rotation_matrix;

	Object(){ translate_matrix.set_identity(); scale_matrix.set_identity(); rotation_matrix.set_identity(); };
	~Object() = default;
	void move(Vec4& vt);
	void scale(Vec4& vt);
	void rotate(Vec4& vt, float& theta);

};
