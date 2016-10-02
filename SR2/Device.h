#pragma once
#ifndef _DEVICE_H
#define _DEVICE_H
#include <vector>
#include <memory>
#include <thread>
#include <algorithm>

#include <GL/glew.h>
#include <GL/glut.h>

#include "Gmath.h"
#include "Mesh.h"
#include "Camera.h"

#define THREAD_COUNT 16

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
	float* depthbuff;
	std::vector<std::thread> threads;

public:
	Device();
	~Device();
	Device(uint32& _w, uint32& _h);

	void clear(float r, float g, float b, float a);
	void putPixel(const int& x, const int& y, const float& zdepth, const Color& clr);
	void drawPoint(const Point& point, const Color& clr = Color(1.0f, 1.0f, 1.0f, 1.0f));
	void drawLine(const Point& pt1, const Point& pt2);
	void drawBLine(const Point& pt0, const Point& pt1, const Color& clr = Color(1.0f, 1.0f, 1.0f, 1.0f));									//drawBresenhamLine
	void drawTriangle(Point& pt0, Point& pt1, Point& pt2, const Color& clr = Color(1.0f, 1.0f, 1.0f, 1.0f));				//»­Èý½ÇÐÎ
	void drawFace(std::shared_ptr<Mesh>& mesh, const uint32& start_index, const uint32& end_index);
	
	void processScanLine(const int& y, const Point& pa, const Point& pb, const Point& pc, const Point& pd, const Color& clr = Color(1.0f, 1.0f, 1.0f, 1.0f));
	auto project(const Point& point, const Mat4& transform_matrix) -> decltype(point);

	void drawCoordinate();

	void render(std::vector<std::shared_ptr<Mesh>>& g_mesh, const Camera& camera);

};

#endif