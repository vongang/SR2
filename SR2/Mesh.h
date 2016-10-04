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

	uint32 n0;
	uint32 n1;
	uint32 n2;

	Face() {}
	~Face() {}

	Face(uint32 _A, uint32 _B, uint32 _C, uint32 _n0, uint32 _n1, uint32 _n2) : A(_A), B(_B), C(_C), n0(_n0), n1(_n1), n2(_n2) {}
};

class Vertex{
public:
	Vec4 coordinate;		//���ھ���任������
	Vec4 normal;			//��ǰ��ķ�����
	Vec4 world_coordiante;	//��ǰ���ԭʼ��������
};

class Mesh : public Object
{
	
public:
	std::string name;
	Vec4* vertices;			//����λ��
	Vec4* normals;			//����
	Face* faces;			//��
	uint32 vt_count;		//�������
	uint32 face_count;		//�����

	Mesh();
	Mesh(std::string& _name, const uint32& _vt_count);
	Mesh(std::string& _name, const uint32& _vt_count, const uint32& _face_count);
	void set(std::string& _name, const uint32& _vt_count, const uint32& _face_count);
	~Mesh();
};

#endif