#pragma once

#ifndef	GMATH_H
#define GMATH_H
#include <cmath>
#include <utility>

#define IABS(x)	(x) < 0 ? (-(x)) : (x)

#define CREATE_GET(name)\
	auto get##name()->decltype(name) { \
		return name; \
	}\

#define CREATE_SET(type, name)\
	void set##name(type v) {\
		name = v; \
	}\

typedef unsigned int uint32;
typedef unsigned char uint8;

class Vec4;
class Mat4;

typedef Vec4 Color;
typedef Vec4 Point;


class Math {
private:
	static const double EPS;// = 1e-5;
public:
	static float CMID(float x, float _min = 0.0f, float _max = 1.0f);// { return (x < _min) ? _min : ((x > _max) ? _max : x); }
	// �����ֵ��tΪ[0,1]֮�����ֵ
	static float interp(float left, float right, float t);// { return left + (right - left) * CMID(t); }
	static int dbcmp(float x);
};


class Vec4 {
public:
	union {
		float v[4];
		struct { float x, y, z, w; };
		struct { float Red, Green, Blue, Alpha; };
	};
public:
	Vec4() { x = y = z = 0.0f; w = 1.0f; }
	Vec4(float _x, float _y, float _z = 0.0f, float _w = 1.0f) :x(_x), y(_y), z(_z), w(_w){}
	~Vec4() = default;	
	
	//vector operate function
	float length();						// |V|

	Vec4& operator = (const Vec4& rhs) {
		x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w;
		return *this;
	}

	Vec4 operator + (const Vec4& rhs) const; // x + y
		
	Vec4 operator - (const Vec4& rhs) const;	//x - y

	Vec4 operator * (const Mat4& rhs) const;

	Vec4 operator * (const float& f) const;

	Vec4 operator / (const float& f) const;

	float dot(const Vec4& rhs) const;			//dot product

	Vec4 cross(const Vec4& rhs) const;		//cross product

	Vec4 interp(const Vec4& rhs, const float& t) const;

	void normalize();					//normalize

	Vec4& format();					//x/w, y/w, z/w, ��w����Ϊ1

};


class Mat4 {
public:
	union {
		float m[4][4];
		float mm[16];
		struct {
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};
	};
	friend Vec4;
public:
	
	Mat4(); 
	
	Mat4(Mat4& rhs); 
	
	Mat4& operator = (const Mat4& rhs);

	~Mat4() = default;

	//matrix operate functions
	
	Mat4 operator + (const Mat4& rhs) const;
	
	Mat4 operator - (const Mat4& rhs) const;

	Mat4 operator * (const Mat4& rhs) const;

	Mat4 operator * (const float& f) const;

	bool matrix_inv();

	void set_identity();	//���õ�λ����
	void set_zero();		//�������

	void set_translate(const float& x, const float& y, const float& z);						//ƽ�Ʊ任
	void set_scale(const float& x, const float& y, const float& z);							//���ű任
	void set_rotate(const float& x, const float& y, const float& z, const float& theta);	//��ת�任
	void set_screen_project(const float& width, const float& height);						//��CVVͶӰ����Ļ

	/**
	*���������
	*
	*eye	�����λ��
	*at ���������λ��
	*up		���ϵ�����
	*/
	void set_lookat(const Vec4& eye, const Vec4& at, const Vec4& up);
	
	/**
	*����͸��ͶӰ����
	*===============================================
	*fovy	�۲�ʱy�᷽��ĽǶ�(����), ���ǹ۲췶Χ�н�
	*aspect �ݺ�ȣ����ӿռ��ȳ��Ը߶�
	*zn		���ü���λ��zֵ
	*zf		Զ�ü���λ��zֵ
	*/
	void set_perspective(const float& fovy, const float& aspect, const float& zn, const float& zf);

};

#endif