#include "Device.h"


Device::Device()
{
	framebuff = nullptr;
	depthbuff = nullptr;
	lockbuff = nullptr;
}

Device::~Device()
{
	delete[] framebuff;
	delete[] depthbuff;
	delete[] lockbuff;
	depthbuff = nullptr;
	framebuff = nullptr;
	lockbuff = nullptr;
}

Device::Device(uint32& _w, uint32& _h){
	width = _w;
	height = _h;
	fbuffsize = _w *_h * 4;
	framebuff = new float[fbuffsize];
	depthbuff = new float[_w * _h];
	lockbuff = new std::mutex[_w * _h];
}

void Device::clear(float r, float g, float b, float a){
	for (uint32 indx = 0; indx < fbuffsize; indx += 4) {
		framebuff[indx] = r;
		framebuff[indx + 1] = g;
		framebuff[indx + 2] = b;
		framebuff[indx + 3] = a;
		depthbuff[indx / 4] = 1e9;
	}
}

void Device::putPixel(const int& x, const int& y, const float& zdepth, const Color& clr) {
	auto indx = (x + y * this->width) * 4;
	auto indx_depth = (x + y * this->width);
	{
#ifdef MULTI_PROCESS
		std::unique_lock<std::mutex> lg(lockbuff[indx_depth]);				//对当前像素加锁
#endif
		//(lockbuff[indx_depth]).lock();
		//if (Math::dbcmp(depthbuff[indx_depth] - zdepth) <= 0)	return;		//stupid, 自己控制锁还加return，return以后锁根本就没释放
		if (Math::dbcmp(depthbuff[indx_depth] - zdepth) >= 0) {		//深度测试
			depthbuff[indx_depth] = zdepth;

			framebuff[indx] = clr.Red;
			framebuff[indx + 1] = clr.Green;
			framebuff[indx + 2] = clr.Blue;
			framebuff[indx + 3] = clr.Alpha;
		}
		//(lockbuff[indx_depth]).unlock();
		//lock_guard在这里被自动释放
	}

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

void Device::processScanLine(const ScanLineData& data, const Vertex& pa, const Vertex& pb, const Vertex& pc, const Vertex& pd, const Color& clr) {
	auto coefficient1 = (pa.coordinate.y != pb.coordinate.y) ? (data.current_y - pa.coordinate.y) / (pb.coordinate.y - pa.coordinate.y) : 1;
	auto coefficient2 = (pc.coordinate.y != pd.coordinate.y) ? (data.current_y - pc.coordinate.y) / (pd.coordinate.y - pc.coordinate.y) : 1;

	auto sx = Math::interp(pa.coordinate.x, pb.coordinate.x, coefficient1);
	auto ex = Math::interp(pc.coordinate.x, pd.coordinate.x, coefficient2);
	


	//开始的z值与结束的z值
	auto sz = Math::interp(pa.coordinate.z, pb.coordinate.z, coefficient1);
	auto ez = Math::interp(pc.coordinate.z, pd.coordinate.z, coefficient2);
	
	//高氏着色 插值

	auto snl = Math::interp(data.n_dot_l_0, data.n_dot_l_1, coefficient1);
	auto enl = Math::interp(data.n_dot_l_2, data.n_dot_l_3, coefficient2);
		

	if (sx > ex) {
		std::swap(sx, ex);
		std::swap(sz, ez);
		std::swap(snl, enl);
	}
	
	float t, z, n_dot_l;

	for (int x = int(sx); x <= int(ex); x++) {
		t = float(x - sx) / (ex - sx);
		z = Math::interp(sz, ez, t);
		n_dot_l = Math::interp(snl, enl, t);
		drawPoint(Vec4(x, data.current_y, z), clr * n_dot_l);
	}
}

float Device::computeNDotL(const Vec4& center_point, const Vec4& vn_face, const Vec4& light_pos) {
	Vec4 light_dir = light_pos - center_point;
	Vec4 normal = vn_face;
	light_dir.normalize();
	normal.normalize();
	float res = normal.dot(light_dir);
	return (res < 0 ? -res : res);
}

bool Device::backCullingCheck(const Vec4& vnormal, const Vec4& vlight) {
	Vec4 vn_face = vnormal;
	Vec4 vn_light = vlight;
	vn_face.normalize();
	vn_light.normalize();
	return Math::dbcmp(vn_face.dot(vn_light)) >= 0;
}

void Device::drawTriangle(Vertex& v0, Vertex& v1, Vertex& v2, const Color& clr) {
	if (v0.coordinate.y > v1.coordinate.y)	std::swap(v0, v1);
	if (v1.coordinate.y > v2.coordinate.y)	std::swap(v1, v2);
	if (v0.coordinate.y > v1.coordinate.y) 	std::swap(v0, v1);
	
	Vec4 p0 = v0.coordinate;
	Vec4 p1 = v1.coordinate;
	Vec4 p2 = v2.coordinate;

	Vec4 vn_face = (v0.normal + v1.normal + v2.normal) / 3;
	Vec4 center_point = (v0.world_coordiante + v1.world_coordiante + v2.world_coordiante) / 3;

	Vec4 light_pos(0.0f, 10.0f, 10.0f);

	if (!backCullingCheck(vn_face, Vec4(0.0f, 0.0f, 10.0f) - center_point))	return;

	float n_dot_l_0 = computeNDotL(v0.world_coordiante, v0.normal, light_pos);
	float n_dot_l_1 = computeNDotL(v1.world_coordiante, v1.normal, light_pos);
	float n_dot_l_2 = computeNDotL(v2.world_coordiante, v2.normal, light_pos);


	ScanLineData sld;

	for (int y = int(v0.coordinate.y); y <= int(v2.coordinate.y); ++y) {
		sld.current_y = y;
		if (y <= int(v1.coordinate.y)) {
			sld.n_dot_l_0 = n_dot_l_0;
			sld.n_dot_l_1 = n_dot_l_2;
			sld.n_dot_l_2 = n_dot_l_0;
			sld.n_dot_l_3 = n_dot_l_1;
			processScanLine(sld, v0, v2, v0, v1, clr);
		}
		else {
			sld.n_dot_l_0 = n_dot_l_0;
			sld.n_dot_l_1 = n_dot_l_2;
			sld.n_dot_l_2 = n_dot_l_1;
			sld.n_dot_l_3 = n_dot_l_2;
			processScanLine(sld, v0, v2, v1, v2, clr);
		}
	}
}

auto Device::project(const Vertex& point) -> decltype(point) {
	Vertex res;
	Mat4 world_trans_matrix = transform.world_matrix;

	res.coordinate = (point.coordinate * transform.transform_matrix).format();
	res.world_coordiante = (point.coordinate * world_trans_matrix);

	world_trans_matrix.matrix_inv_traspose();
	res.normal = (point.normal * world_trans_matrix);
	
	return std::move(res);
}

void Device::drawCoordinate() {
	//Vec4 p0(0.0f, 0.0f, 0.0f);
	//Vec4 p1(3.0f, 0.0f, 0.0f);
	//Vec4 p2(0.0f, 3.0f, 0.0f);
	//Vec4 p3(0.0f, 0.0f, 3.0f);

	//auto px0 = project(p0, transform.transform_matrix);
	//auto px1 = project(p1, transform.transform_matrix);
	//auto px2 = project(p2, transform.transform_matrix);
	//auto px3 = project(p3, transform.transform_matrix);

	//drawTriangle(px0, px0, px1, Color(1.0f, 0.0f, 0.0f));
	//drawTriangle(px0, px0, px2, Color(.0f, 1.0f, 0.0f));
	//drawTriangle(px0, px0, px3, Color(.0f, 0.0f, 1.0f));


	/*drawBLine(px0, px1, Color(1.0f, 0.0f, 0.0f));
	drawBLine(px0, px2, Color(0.0f, 1.0f, 0.0f));
	drawBLine(px0, px3, Color(0.0f, 0.0f, 1.0f));*/
}

void Device::drawFace(std::shared_ptr<Mesh>& mesh, const uint32& start_index, const uint32& end_index){
	if (start_index >= mesh->face_count)	return;

	Color clr1(1.0f, 1.0f, 1.0f, 1.0f);
	//Color clr2(128.0 / 255, 128.0 / 255, 128.0 / 255, 1.0f);
	bool color_flag = false;
	uint32 end = std::min(end_index, mesh->face_count);

	for (uint32 idx = start_index; idx < end; ++idx) {

		Vertex va; 
		Vertex vb; 
		Vertex vc; 
		
		va.coordinate = mesh->vertices[mesh->faces[idx].A];
		vb.coordinate = mesh->vertices[mesh->faces[idx].B];
		vc.coordinate = mesh->vertices[mesh->faces[idx].C];

		va.normal = mesh->normals[mesh->faces[idx].n0];
		vb.normal = mesh->normals[mesh->faces[idx].n1];
		vc.normal = mesh->normals[mesh->faces[idx].n2];

		auto pixel_a = project(va);
		auto pixel_b = project(vb);
		auto pixel_c = project(vc);

		//drawTriangle(pixel_a, pixel_b, pixel_c, color_flag ? clr1 : clr2);
		drawTriangle(pixel_a, pixel_b, pixel_c, clr1);
		color_flag = !color_flag;
	}
}
	
void Device::render(std::vector<std::shared_ptr<Mesh>>& g_mesh, const Camera& camera) {
	transform.view_matrix.set_lookat(camera.position, camera.at, camera.up);
	transform.projection_matrix.set_perspective(0.78f, float(width) / height, 0.01f, 1.0f);
	transform.screen_project_matrix.set_screen_project(width, height);
	transform.world_matrix.set_identity();
	
	transform.update();

	//drawCoordinate();

	
	bool flag = true;
	for (auto mesh : g_mesh) {
		
		transform.world_matrix = mesh->translate_matrix * mesh->rotation_matrix;

		transform.update();
		int seg = (mesh->face_count + THREAD_COUNT) / THREAD_COUNT;
#ifdef MULTI_PROCESS
		threads.clear();
		
		for (int i = 0; i < THREAD_COUNT; ++i) {
			threads.push_back(std::thread(std::bind(&Device::drawFace, this, mesh, i*seg, (i + 1)*seg))); //********类的成员函数传入std::thread要用std::bind********
		}
		std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
#else
		for (int i = 0; i < THREAD_COUNT; ++i) {
			drawFace(mesh, i*seg, (i + 1)*seg);	
		}
#endif
	}  
	glDrawPixels(width, height, GL_RGBA, GL_FLOAT, framebuff);
}