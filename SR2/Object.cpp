#include "Object.h"


void Object::move(Vec4& vt) {
	translate_matrix.set_translate(vt.x, vt.y, vt.z);
}

void Object::scale(Vec4& vt) {
	scale_matrix.set_scale(vt.x, vt.y, vt.z);
}

void Object::rotate(Vec4& vt, float& theta) {
	rotation_matrix.set_rotate(vt.x, vt.y, vt.z, theta);
}