#pragma once
#ifndef _DEVICE_H
#define _DEVICE_H
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GL/glut.h>

#include "gmath.h"
#include "Mesh.h"
#include "Camera.h"

class Transform{
public:
	Mat4 world_matrix;
	Mat4 view_matrix;
	Mat4 projection_matrix;
	Mat4 transform_matrix;
	Mat4 screen_project_matrix;
	void update(){
		transform_matrix = world_matrix * view_matrix * projection_matrix * screen_project_matrix;
	}
};

class Device
{
	uint32 width;
	uint32 height;
	uint32 buffsize;

	Transform transform;

	float* framebuff;

public:
	Device();
	~Device();
	Device(uint32& _w, uint32& _h);

	void clear(float r, float g, float b, float a);
	void putPixel(const int& x, const int& y, const Color& clr);
	void drawPoint(const Point& point);
	void drawLine(const Point& pt1, const Point& pt2);
	void drawBLine(const Point& pt0, const Point& pt1);									//drawBresenhamLine
	void render(std::vector<std::shared_ptr<Mesh>>& g_mesh, const Camera& camera);
	auto project(const Point& point, const Mat4& transform_matrix) -> decltype(point);
};

#endif