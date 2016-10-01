#pragma once
#ifndef _CAMERA_H
#define _CAMERA_H
#include "gmath.h"
#include "Object.h"

class Camera : public Object
{
public:
	Vec4 at;
	Vec4 up;
public:
	Camera();
	Camera(Vec4& _eye, Vec4& _at, Vec4& _up  = Vec4(0.0f, 1.0f, 0.0f, 0.0f) );// :eye(_eye), at(_at), up(_up){}
	~Camera() = default;
};
#endif
