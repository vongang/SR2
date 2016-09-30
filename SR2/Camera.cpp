#include "Camera.h"


Camera::Camera()
{
	up = Vec4(0.0f, 1.0f, 0.0f, 0.0f);
}

Camera::Camera(Vec4& _eye, Vec4& _at, Vec4& _up) : Object(_eye), at(_at), up(_up) {

}

