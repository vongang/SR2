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
	for (uint32 indx = 0; indx < buffsize; indx += 4) {
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

void Device::drawBLine(const Point& pt0, const Point& pt1) {
	float delta_x = pt1.x - pt0.x;
	float delta_y = pt1.y - pt0.y;
	float error = 0;
	float delta_err = delta_y / delta_x;

	int y = 0;
}

auto Device::project(const Point& point, const Mat4& transform_matrix) -> decltype(point) {
	auto pt = point * transform_matrix;
	pt.format();
	pt.x = pt.x * width + float(width) / 2.0f;
	pt.y = -pt.y * height + float(height) / 2.0f;
	return std::move(pt);
}

void Device::render(std::vector<std::shared_ptr<Mesh>>& g_mesh, const Camera& camera) {

	transform.view_matrix.set_lookat(camera.position, camera.at, camera.up);
	transform.projection_matrix.set_perspective(0.78f, float(width) / height, 0.01f, 1.0f);
	transform.screen_project_matrix.set_screen_project(width, height);

	for (auto mesh : g_mesh) {
		
		transform.world_matrix = mesh->rotation_matrix * mesh->translate_matrix;

		transform.update();

		for (uint32 idx = 0; idx < mesh->face_count; ++idx) {
			auto curr_face = mesh->faces[idx];
			auto va = mesh->vertices[curr_face.A];
			auto vb = mesh->vertices[curr_face.B];
			auto vc = mesh->vertices[curr_face.C];

			auto pixel_a = va * transform.transform_matrix;
			auto pixel_b = vb * transform.transform_matrix;
			auto pixel_c = vc * transform.transform_matrix;
			
			pixel_a.format();
			pixel_b.format();
			pixel_c.format();

			drawLine(pixel_a, pixel_b);
			drawLine(pixel_b, pixel_c);
			drawLine(pixel_c, pixel_a);
		}
		
		/*
		for (uint32 idx = 0; idx < mesh->vt_count; ++idx) {
			//for (auto idy = idx + 1; idy < mesh->vt_count; ++idy) {
				//if (idx == idy)	continue;
				auto pt1 = project(mesh->vertices[idx], transform.transform_matrix);
				auto pt2 = project(mesh->vertices[(idx+1) % mesh->vt_count], transform.transform_matrix);

				//printf("%f %f %f %f\n", mesh->vertices[idx].x, mesh->vertices[idx].y, mesh->vertices[idx].z, mesh->vertices[idx].w);
				//printf("%f %f %f %f\n", pt.x, pt.y, pt.z, pt.w);

				//drawPoint(pt1);
				drawLine(pt1, pt2);
			//}
		}*/
	}  
	glDrawPixels(width, height, GL_RGBA, GL_FLOAT, framebuff);
}