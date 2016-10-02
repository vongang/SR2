#include "Device.h"


Device::Device()
{
	framebuff = nullptr;
	depthbuff = nullptr;
}

Device::~Device()
{
	delete[] framebuff;
	delete[] depthbuff;
	depthbuff = nullptr;
	framebuff = nullptr;
}

Device::Device(uint32& _w, uint32& _h){
	width = _w;
	height = _h;
	buffsize = _w *_h * 4;
	framebuff = new float[buffsize];
	depthbuff = new float[_w * _h];
}

void Device::clear(float r, float g, float b, float a){
	for (uint32 indx = 0; indx < buffsize; indx += 4) {
		framebuff[indx] = r;
		framebuff[indx + 1] = g;
		framebuff[indx + 2] = b;
		framebuff[indx + 3] = a;
		depthbuff[indx / 4] = 1.0;
	}
}

void Device::putPixel(const int& x, const int& y, const float& zdepth, const Color& clr) {
	auto indx = (x + y * this->width) * 4;
	auto indx_depth = (x + y * this->width);
	
	if (Math::dbcmp(depthbuff[indx_depth] - zdepth) <= 0) return;
	depthbuff[indx_depth] = zdepth;

	framebuff[indx] = clr.Red;
	framebuff[indx + 1] = clr.Green;
	framebuff[indx + 2] = clr.Blue;
	framebuff[indx + 3] = clr.Alpha;
}


void Device::drawPoint(const Point& point, const Color& clr) {
	if (point.x >= 0 && point.y >= 0 && point.x < width && point.y < height) {
		putPixel(point.x, point.y, point.z, clr);
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

void Device::drawBLine(const Point& pt0, const Point& pt1, const Color& clr) {
	int x0 = pt0.x;
	int y0 = pt0.y;
	int x1 = pt1.x;
	int y1 = pt1.y;
	//printf("(%d, %d), (%d, %d)\n", x0, y0, x1, y1);
	int dx = IABS(x1 - x0);
	int dy = IABS(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while (true) {
		drawPoint(Point(x0, y0), clr);
		//printf("%d %d\n", x0, y0);
		if ((x0 == x1) && (y0 == y1))	break;

		int e2 = 2 * err;
		if (e2 > -dy) { err -= dy; x0 += sx; }
		if (e2 < dx) { err += dx; y0 += sy; }
	}
}


//(x - p0.x) / (p2.x - p0.x) = (y - p0.y) / (p2.y - p0.y);
// p2
// -
// --
// - -
// -  -
// -   - p1
// -  -
// - -
// -
// p0

void Device::processScanLine(const int& y, const Point& pa, const Point& pb, const Point& pc, const Point& pd, const Color& clr) {
	auto coefficient1 = (pa.y != pb.y) ? ((y - pa.y) / (pb.y - pa.y)) : 1;
	auto coefficient2 = (pc.y != pd.y) ? ((y - pc.y) / (pd.y - pc.y)) : 1;

	auto sx = static_cast<int>(Math::interp(pa.x, pb.x, coefficient1));
	auto ex = static_cast<int>(Math::interp(pc.x, pd.x, coefficient2));
	auto delta_x = sx < ex ? 1 : -1;
	for (auto x = sx; x != ex; x += delta_x) {
		drawPoint(Vec4(x, y), clr);
	}
}

void Device::drawTriangle(Point& pt0, Point& pt1, Point& pt2, const Color& clr) {
	if (pt0.y > pt1.y)	std::swap(pt0, pt1);
	if (pt1.y > pt2.y)	std::swap(pt1, pt2);
	if (pt0.y > pt1.y) 	std::swap(pt0, pt1);

	
	float invslope01, invslope02;

	if (Math::dbcmp(pt1.y - pt0.y) > 0) 
		invslope01 = (pt1.x - pt0.x) / (pt1.y - pt0.y);
	else 
		invslope01 = 0;

	if (Math::dbcmp(pt2.y - pt0.y) > 0) 
		invslope02 = (pt2.x - pt0.x) / (pt2.y - pt0.y);
	else 
		invslope02 = 0;

	for (int y = static_cast<int>(pt0.y); y <= static_cast<int>(pt2.y); ++y) {
		if (y <= static_cast<int>(pt1.y)) {
			processScanLine(y, pt0, pt1, pt0, pt2, clr);
		}
		else {
			processScanLine(y, pt1, pt2, pt0, pt2, clr);
		}
	}
}

auto Device::project(const Point& point, const Mat4& transform_matrix) -> decltype(point) {
	auto pt = (point * transform_matrix).format();
	return std::move(pt);
}

void Device::drawCoordinate() {
	Vec4 p0(0.0f, 0.0f, 0.0f);
	Vec4 p1(1.0f, 0.0f, 0.0f);
	Vec4 p2(0.0f, 1.0f, 0.0f);
	Vec4 p3(0.0f, 0.0f, 1.0f);

	auto px0 = (p0 * transform.transform_matrix).format();
	auto px1 = (p1 * transform.transform_matrix).format();
	auto px2 = (p2 * transform.transform_matrix).format();
	auto px3 = (p3 * transform.transform_matrix).format();

	drawBLine(px0, px1, Color(1.0f, 0.0f, 0.0f));
	drawBLine(px0, px2, Color(0.0f, 1.0f, 0.0f));
	drawBLine(px0, px3, Color(0.0f, 0.0f, 1.0f));
}

void Device::render(std::vector<std::shared_ptr<Mesh>>& g_mesh, const Camera& camera) {
	

	transform.view_matrix.set_lookat(camera.position, camera.at, camera.up);
	transform.projection_matrix.set_perspective(0.78f, float(width) / height, 0.01f, 1.0f);
	transform.screen_project_matrix.set_screen_project(width, height);
	transform.world_matrix.set_identity();
	transform.update();

	drawCoordinate();

	Color clr1(1.0f, 1.0f, 1.0f, 0.0f), clr2(128.0 / 255, 128.0 / 255, 128.0 / 255, 0.0f);
	bool flag = true;
	for (auto mesh : g_mesh) {
		
		transform.world_matrix = mesh->translate_matrix * mesh->rotation_matrix;

		transform.update();

		for (uint32 idx = 0; idx < mesh->face_count; ++idx) {
			auto curr_face = mesh->faces[idx];
			auto va = mesh->vertices[curr_face.A];
			auto vb = mesh->vertices[curr_face.B];
			auto vc = mesh->vertices[curr_face.C];

			auto pixel_a = project(va, transform.transform_matrix);
			auto pixel_b = project(vb, transform.transform_matrix);
			auto pixel_c = project(vc, transform.transform_matrix);

			/*drawBLine(pixel_a, pixel_b);
			drawBLine(pixel_b, pixel_c);
			drawBLine(pixel_c, pixel_a);*/
			drawTriangle(pixel_a, pixel_b, pixel_c, flag ? clr1 : clr2);
			flag = !flag;
		}
	}  
	glDrawPixels(width, height, GL_RGBA, GL_FLOAT, framebuff);
}