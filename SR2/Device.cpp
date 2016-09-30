#include "Device.h"


Device::Device()
{
	framebuff = nullptr;
}

Device::~Device()
{
	delete[] framebuff;
	framebuff = nullptr;
}

Device::Device(uint32& _w, uint32& _h){
	width = _w;
	height = _h;
	buffsize = _w *_h * 4;
	framebuff = new float[buffsize];
}

void Device::clear(float r, float g, float b, float a){
	for (auto indx = 0; indx < buffsize; indx += 4) {
		framebuff[indx] = r;
		framebuff[indx + 1] = g;
		framebuff[indx + 2] = b;
		framebuff[indx + 3] = a;
	}
}

void Device::putPixel(const int& x, const int& y, const Color& clr) {
	auto indx = (x + y * this->width) * 4;
	framebuff[indx] = clr.Red;
	framebuff[indx + 1] = clr.Green;
	framebuff[indx + 2] = clr.Blue;
	framebuff[indx + 3] = clr.Alpha;
}


void Device::drawPoint(const Point& point) {
	if (point.x >= 0 && point.y >= 0 && point.x < width && point.y < height) {
		putPixel(point.x, point.y, Color(1.0f, 1.0f, 0.0f, 1.0f));
	}
}

void Device::drawLine(const Point& pt0, const Point& pt1) {
	auto dist = (pt1 - pt0).length();
	if (dist < 2)	return;
	auto middle_pt = pt0 + (pt1 - pt0) / 2.0f;
	drawPoint(middle_pt);
	drawLine(pt0, middle_pt);
	drawLine(middle_pt, pt1);
}

auto Device::project(const Point& point, const Mat4& transform_matrix) -> decltype(point) {
	auto pt = point * transform_matrix;
	pt.format();
	pt.x = pt.x * width + float(width) / 2.0f;
	pt.y = -pt.y * height + float(height) / 2.0f;
	return std::move(pt);
}

void Device::render(std::vector<std::shared_ptr<Mesh>>& g_mesh, const Camera& camera) {

	transform.view_matrix.set_lookat(camera.eye, camera.at, camera.up);
	transform.projection_matrix.set_perspective(0.78f, float(width) / height, 0.01f, 1.0f);

	for (auto mesh : g_mesh) {
		
		transform.world_matrix = mesh->rotation_matrix * mesh->translate_matrix;

		transform.update();

		for (auto idx = 0; idx < mesh->verticesCount; ++idx) {
			//for (auto idy = idx + 1; idy < mesh->verticesCount; ++idy) {
				//if (idx == idy)	continue;
				auto pt1 = project(mesh->vertices[idx], transform.transform_matrix);
				auto pt2 = project(mesh->vertices[(idx+1) % mesh->verticesCount], transform.transform_matrix);

				//printf("%f %f %f %f\n", mesh->vertices[idx].x, mesh->vertices[idx].y, mesh->vertices[idx].z, mesh->vertices[idx].w);
				//printf("%f %f %f %f\n", pt.x, pt.y, pt.z, pt.w);

				//drawPoint(pt1);
				drawLine(pt1, pt2);
			//}
		}
	}  
	glDrawPixels(width, height, GL_RGBA, GL_FLOAT, framebuff);
}