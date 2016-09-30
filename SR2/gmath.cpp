#include "gmath.h"

Vec4 Vec4::interp(const Vec4& rhs, const float& t) const {
	Vec4 temp;
	temp.x = BaseMathTools::interp(x, rhs.x, t);
	temp.y = BaseMathTools::interp(y, rhs.y, t);
	temp.z = BaseMathTools::interp(z, rhs.z, t);
	temp.w = 1.0f;
	return std::move(temp);
}

float Vec4::length(){
	return sqrt(x*x + y*y + z*z);
}

Vec4 Vec4::operator + (const Vec4& rhs) const {
	Vec4 temp(x + rhs.x, y + rhs.y, z + rhs.z, 1.0f);
	return std::move(temp);
}

Vec4 Vec4::operator - (const Vec4& rhs) const {
	Vec4 temp(x - rhs.x, y - rhs.y, z - rhs.z, 1.0f);
	return std::move(temp);
}

Vec4 Vec4::operator / (const float& f) const {
	Vec4 temp(x / f, y / f, z / f, 1.0f);
	return std::move(temp);	
}

float Vec4::dot(const Vec4& rhs) const {
	return x * rhs.x + y * rhs.y + z * rhs.z;
}

Vec4 Vec4::cross(const Vec4& rhs) const {
	Vec4 temp;
	temp.x = y * rhs.z - z * rhs.y;
	temp.y = z * rhs.x - x * rhs.z;
	temp.z = x * rhs.y - y * rhs.x;
	temp.w = 1.0f;
	return std::move(temp);
}

void Vec4::normalize() {
	float v_len = length();
	if (BaseMathTools::dbcmp(v_len) == 0)	return;
	x /= v_len;
	y /= v_len;
	z /= v_len;
}

Vec4 Vec4::operator*(const Mat4& rhs) const {
	Vec4 tmp;
	for (int i = 0; i < 4; ++i) {
		float t = 0.0f;
		for (int j = 0; j < 4; ++j) {
			tmp.v[i] += v[j] * rhs.m[j][i];
		}
	}
	return std::move(tmp);
}

void Vec4::format() {
	x /= w;
	y /= w;
	z /= w;
	w = 1.0;
}

/*******************************************************/

Mat4::Mat4() {
	for (uint8 i = 0; i < 4; ++i)
	for (uint8 j = 0; j < 4; ++j)
		m[i][j] = 0.0f;
}

Mat4::Mat4(Mat4& rhs) {
	for (uint8 i = 0; i < 4; ++i)
	for (uint8 j = 0; j < 4; ++j)
		m[i][j] = rhs.m[i][j];
}

Mat4& Mat4::operator = (const Mat4& rhs) {
	for (uint8 i = 0; i < 4; ++i)
	for (uint8 j = 0; j < 4; ++j)
		m[i][j] = rhs.m[i][j];
	return *this;
}

Mat4 Mat4::operator + (const Mat4& rhs) const {
	Mat4 res;
	for (uint8 i = 0; i < 4; ++i) {
		for (uint8 j = 0; j < 4; ++j) {
			res.m[i][j] = m[i][j] - rhs.m[i][j];
		}
	}
	return std::move(res);
}

Mat4 Mat4::operator - (const Mat4& rhs) const {
	Mat4 res;
	for (uint8 i = 0; i < 4; ++i) {
		for (uint8 j = 0; j < 4; ++j) {
			res.m[i][j] = m[i][j] - rhs.m[i][j];
		}
	}
	return std::move(res);
}

Mat4 Mat4::operator * (const Mat4& rhs) const {
	Mat4 res;
	res.set_zero();
	for (uint8 i = 0; i < 4; ++i) {
		for (uint8 j = 0; j < 4; ++j) {
			for (uint8 k = 0; k < 4; ++k) {
				res.m[i][j] += this->m[i][k] * rhs.m[k][j];
			}
		}
	}
	return std::move(res);
}

Mat4 Mat4::operator * (const float& f) const {
	Mat4 res;
	for (uint8 i = 0; i < 4; ++i) {
		for (uint8 j = 0; j < 4; ++j) {
			res.m[i][j] = m[i][j] * f;
		}
	}
	return std::move(res);
}

void Mat4::set_identity(){	//设置单位矩阵
	set_zero();
	m00 = m11 = m22 = m33 = 1.0f;
}
void Mat4::set_zero(){		//矩阵归零
	for (uint8 i = 0; i < 4; ++i) 
	for (uint8 j = 0; j < 4; (m[i][j] = 0.0f), ++j);
}

void Mat4::set_translate(const float& x, const float& y, const float& z){						//平移变换
	set_identity();
	m30 = x;
	m31 = y;
	m32 = z;
}
void Mat4::set_scale(const float& x, const float& y, const float& z){							//缩放变换
	set_identity();
	m00 = x;
	m11 = y;
	m22 = z;
}
void Mat4::set_rotate(const float& x, const float& y, const float& z, const float& theta){	//旋转变换 
	Vec4 axis(x, y, z, 0.0f);
	axis.normalize();
	float u = axis.x;
	float v = axis.y;
	float w = axis.z;

	float costheta = cos(theta);
	float sintheta = sin(theta);

	m[0][0] = costheta + (u * u) * (1 - costheta);
	m[0][1] = u * v * (1 - costheta) + w * sintheta;
	m[0][2] = u * w * (1 - costheta) - v * sintheta;
	m[0][3] = 0;

	m[1][0] = u * v * (1 - costheta) - w * sintheta;
	m[1][1] = costheta + v * v * (1 - costheta);
	m[1][2] = w * v * (1 - costheta) + u * sintheta;
	m[1][3] = 0;

	m[2][0] = u * w * (1 - costheta) + v * sintheta;
	m[2][1] = v * w * (1 - costheta) - u * sintheta;
	m[2][2] = costheta + w * w * (1 - costheta);
	m[2][3] = 0;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;

}

/**
*设置摄像机
*
*eye	摄像机位置
*at		摄像机看的位置
*up		向上的向量
*/
void Mat4::set_lookat(const Vec4& eye, const Vec4& at, const Vec4& up) {
	Vec4 x_axis, y_axis, z_axis;
	z_axis = at - eye;
	z_axis.normalize();

	x_axis = up.cross(z_axis);
	x_axis.normalize();

	y_axis = z_axis.cross(x_axis);
	//y_axis.normalize();

	m00 = x_axis.x;
	m10 = x_axis.y;
	m20 = x_axis.z;
	m30 = -x_axis.dot(eye);

	m01 = y_axis.x;
	m11 = y_axis.y;
	m21 = y_axis.z;
	m31 = -y_axis.dot(eye);

	m02 = z_axis.x;
	m12 = z_axis.y;
	m22 = z_axis.z;
	m32 = -z_axis.dot(eye);

	m03 = m13 = m23 = 0.0f;
	m33 = 1.0f;

}

/**
*设置透视投影矩阵
*===============================================
*fovy	观察时y轴方向的角度(弧度), 就是观察范围夹角
*aspect 纵横比，在视空间宽度除以高度
*zn		近裁剪面位置z值
*zf		远裁剪面位置z值
*/
void Mat4::set_perspective(const float& fovy, const float& aspect, const float& zn, const float& zf){
	float ct = 1.0f / tan(fovy * 0.5);
	set_zero();
	m00 = ct / aspect;
	m11 = ct;
	m22 = zf / (zf - zn);
	m32 = zn * zf / (zn - zf);
	m23 = 1.0f;
}

