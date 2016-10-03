#pragma once
#ifndef _DEVICE_H
#define _DEVICE_H
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <algorithm>

#include <GL/glew.h>
#include <GL/glut.h>

#include "CONFIG.h"
#include "Gmath.h"
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

struct ScanLineData {		//������processScanLine������
	int current_y;
	float n_dot_l_0;		//n_dot_l -> normal dot light
	float n_dot_l_1;
	float n_dot_l_2;
	float n_dot_l_3;
};

class Device
{
	uint32 width;
	uint32 height;
	uint32 fbuffsize;

	Transform transform;

	float* framebuff;					//ÿ�����ص�rgba
	float* depthbuff;					//ÿ�����ص���Ȼ���
	std::mutex* lockbuff;				//���ÿ�����ص���

	std::vector<std::thread> threads;	//���߳�ά������

public:
	Device();
	~Device();
	Device(uint32& _w, uint32& _h);

	void clear(float r, float g, float b, float a);


	void putPixel(const int& x, const int& y, const float& zdepth, const Color& clr);
	void drawPoint(const Point& point, const Color& clr = Color(1.0f, 1.0f, 1.0f, 1.0f));
	void drawLine(const Point& pt1, const Point& pt2);
	void drawBLine(const Point& pt0, const Point& pt1, const Color& clr = Color(1.0f, 1.0f, 1.0f, 1.0f));									//drawBresenhamLine
	void drawTriangle(Vertex& pt0, Vertex& pt1, Vertex& pt2, const Color& clr = Color(1.0f, 1.0f, 1.0f, 1.0f));				//��������
	void drawFace(std::shared_ptr<Mesh>& mesh, const uint32& start_index, const uint32& end_index);
	

	float computeNDotL(const Vec4& center_point, const Vec4& vn_face, const Vec4& light_pos);
	void processScanLine(const ScanLineData& y, const Vertex& pa, const Vertex& pb, const Vertex& pc, const Vertex& pd, const Color& clr = Color(1.0f, 1.0f, 1.0f, 1.0f));
	auto project(const Vertex& point) -> decltype(point);

	void drawCoordinate();

	void render(std::vector<std::shared_ptr<Mesh>>& g_mesh, const Camera& camera);

};

#endif