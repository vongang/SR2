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
	// 计算插值：t为[0,1]之间的数值
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

	Vec4& format();					//x/w, y/w, z/w, 将w调整为1

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

	void set_identity();	//设置单位矩阵
	void set_zero();		//矩阵归零

	void set_translate(const float& x, const float& y, const float& z);						//平移变换
	void set_scale(const float& x, const float& y, const float& z);							//缩放变换
	void set_rotate(const float& x, const float& y, const float& z, const float& theta);	//旋转变换
	void set_screen_project(const float& width, const float& height);						//将CVV投影到屏幕

	/**
	*设置摄像机
	*
	*eye	摄像机位置
	*at 摄像机看的位置
	*up		向上的向量
	*/
	void set_lookat(const Vec4& eye, const Vec4& at, const Vec4& up);
	
	/**
	*设置透视投影矩阵
	*===============================================
	*fovy	观察时y轴方向的角度(弧度), 就是观察范围夹角
	*aspect 纵横比，在视空间宽度除以高度
	*zn		近裁剪面位置z值
	*zf		远裁剪面位置z值
	*/
	void set_perspective(const float& fovy, const float& aspect, const float& zn, const float& zf);

};

#endif