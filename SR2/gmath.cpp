#include "gmath.h"


const double Math::EPS = 1e-5;

float Math::CMID(float x, float _min, float _max)  { return (x < _min) ? _min : ((x > _max) ? _max : x); }
float Math::interp(float left, float right, float t) { return left + (right - left) * CMID(t); }
int Math::dbcmp(float x) {
	if (x > EPS)	return 1;
	else if (x < -EPS)	return -1;
	return 0;
}


Vec4 Vec4::interp(const Vec4& rhs, const float& t) const {
	Vec4 temp;
	temp.x = Math::interp(x, rhs.x, t);
	temp.y = Math::interp(y, rhs.y, t);
	temp.z = Math::interp(z, rhs.z, t);
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
	Vec4 temp(x / f, y / f, z / f, w);
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
	if (Math::dbcmp(v_len) == 0)	return;
	x /= v_len;
	y /= v_len;
	z /= v_len;
}

Vec4 Vec4::operator * (const float& f) const {
	Vec4 temp(x * f, y * f, z * f, w);
	return std::move(temp);
}

Vec4 Vec4::operator * (const Mat4& rhs) const {
	Vec4 tmp;
	for (int i = 0; i < 4; ++i) {
		float t = 0.0f;
		for (int j = 0; j < 4; ++j) {
			tmp.v[i] += v[j] * rhs.m[j][i];
		}
	}
	return std::move(tmp);
}

Vec4& Vec4::format() {
	x /= w;
	y /= w;
	z /= w;
	w = 1.0;
	return *this;
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

void Mat4::set_screen_project(const float& width, const float& height) {
	set_identity();
	m00 = -width;
	m30 = width / 2.0f;
	m11 = height;
	m31 = height / 2.0f;
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
	float ct = 1.0f / tan(fovy * 0.5f);
	set_zero();
	m00 = ct / aspect;
	m11 = ct;
	m22 = zf / (zf - zn);
	m32 = zn * zf / (zn - zf);
	m23 = 1.0f;
}

void Mat4::matrix_transpose() {	//转置矩阵
	for (int i = 0; i < 4; ++i) {
		for (int j = i + 1; j < 4; ++j) {
			auto tm = m[i][j];
			m[i][j] = m[j][i];
			m[j][i] = tm;
		}
	}
}

bool Mat4::matrix_inv_traspose() {		//逆转置
	if (!matrix_inv())	return false;
	matrix_transpose();
	return true;
}

bool Mat4::matrix_inv(){	//逆矩阵
	float inv[16], det;
	int i;
	inv[0] = mm[5] * mm[10] * mm[15] -
		mm[5] * mm[11] * mm[14] -
		mm[9] * mm[6] * mm[15] +
		mm[9] * mm[7] * mm[14] +
		mm[13] * mm[6] * mm[11] -
		mm[13] * mm[7] * mm[10];

	inv[4] = -mm[4] * mm[10] * mm[15] +
		mm[4] * mm[11] * mm[14] +
		mm[8] * mm[6] * mm[15] -
		mm[8] * mm[7] * mm[14] -
		mm[12] * mm[6] * mm[11] +
		mm[12] * mm[7] * mm[10];

	inv[8] = mm[4] * mm[9] * mm[15] -
		mm[4] * mm[11] * mm[13] -
		mm[8] * mm[5] * mm[15] +
		mm[8] * mm[7] * mm[13] +
		mm[12] * mm[5] * mm[11] -
		mm[12] * mm[7] * mm[9];

	inv[12] = -mm[4] * mm[9] * mm[14] +
		mm[4] * mm[10] * mm[13] +
		mm[8] * mm[5] * mm[14] -
		mm[8] * mm[6] * mm[13] -
		mm[12] * mm[5] * mm[10] +
		mm[12] * mm[6] * mm[9];

	inv[1] = -mm[1] * mm[10] * mm[15] +
		mm[1] * mm[11] * mm[14] +
		mm[9] * mm[2] * mm[15] -
		mm[9] * mm[3] * mm[14] -
		mm[13] * mm[2] * mm[11] +
		mm[13] * mm[3] * mm[10];

	inv[5] = mm[0] * mm[10] * mm[15] -
		mm[0] * mm[11] * mm[14] -
		mm[8] * mm[2] * mm[15] +
		mm[8] * mm[3] * mm[14] +
		mm[12] * mm[2] * mm[11] -
		mm[12] * mm[3] * mm[10];

	inv[9] = -mm[0] * mm[9] * mm[15] +
		mm[0] * mm[11] * mm[13] +
		mm[8] * mm[1] * mm[15] -
		mm[8] * mm[3] * mm[13] -
		mm[12] * mm[1] * mm[11] +
		mm[12] * mm[3] * mm[9];

	inv[13] = mm[0] * mm[9] * mm[14] -
		mm[0] * mm[10] * mm[13] -
		mm[8] * mm[1] * mm[14] +
		mm[8] * mm[2] * mm[13] +
		mm[12] * mm[1] * mm[10] -
		mm[12] * mm[2] * mm[9];

	inv[2] = mm[1] * mm[6] * mm[15] -
		mm[1] * mm[7] * mm[14] -
		mm[5] * mm[2] * mm[15] +
		mm[5] * mm[3] * mm[14] +
		mm[13] * mm[2] * mm[7] -
		mm[13] * mm[3] * mm[6];

	inv[6] = -mm[0] * mm[6] * mm[15] +
		mm[0] * mm[7] * mm[14] +
		mm[4] * mm[2] * mm[15] -
		mm[4] * mm[3] * mm[14] -
		mm[12] * mm[2] * mm[7] +
		mm[12] * mm[3] * mm[6];

	inv[10] = mm[0] * mm[5] * mm[15] -
		mm[0] * mm[7] * mm[13] -
		mm[4] * mm[1] * mm[15] +
		mm[4] * mm[3] * mm[13] +
		mm[12] * mm[1] * mm[7] -
		mm[12] * mm[3] * mm[5];

	inv[14] = -mm[0] * mm[5] * mm[14] +
		mm[0] * mm[6] * mm[13] +
		mm[4] * mm[1] * mm[14] -
		mm[4] * mm[2] * mm[13] -
		mm[12] * mm[1] * mm[6] +
		mm[12] * mm[2] * mm[5];

	inv[3] = -mm[1] * mm[6] * mm[11] +
		mm[1] * mm[7] * mm[10] +
		mm[5] * mm[2] * mm[11] -
		mm[5] * mm[3] * mm[10] -
		mm[9] * mm[2] * mm[7] +
		mm[9] * mm[3] * mm[6];

	inv[7] = mm[0] * mm[6] * mm[11] -
		mm[0] * mm[7] * mm[10] -
		mm[4] * mm[2] * mm[11] +
		mm[4] * mm[3] * mm[10] +
		mm[8] * mm[2] * mm[7] -
		mm[8] * mm[3] * mm[6];

	inv[11] = -mm[0] * mm[5] * mm[11] +
		mm[0] * mm[7] * mm[9] +
		mm[4] * mm[1] * mm[11] -
		mm[4] * mm[3] * mm[9] -
		mm[8] * mm[1] * mm[7] +
		mm[8] * mm[3] * mm[5];

	inv[15] = mm[0] * mm[5] * mm[10] -
		mm[0] * mm[6] * mm[9] -
		mm[4] * mm[1] * mm[10] +
		mm[4] * mm[2] * mm[9] +
		mm[8] * mm[1] * mm[6] -
		mm[8] * mm[2] * mm[5];

	det = mm[0] * inv[0] + mm[1] * inv[4] + mm[2] * inv[8] + mm[3] * inv[12];

	if (det == 0.0f)
		return false;

	det = 1.0f / det;

	for (i = 0; i < 16; i++)
		this->mm[i] = inv[i] * det;

	return true;
}