#include <iostream>
#include <cstdio>
#include <cstring>
#include <memory>

#include <GL/glew.h>
#include <GL/glut.h>

#include "Device.h"
#include "Mesh.h"
#include "Camera.h"
#include "Util.h"

unsigned int window_width = 800, window_height = 600;
float rotate_theta = 0.0f;

Util util;
Camera camera;
std::shared_ptr<Device> g_device(new Device(window_width, window_height));
std::vector<std::shared_ptr<Mesh>> g_mesh;

/*
void initScene(){
	auto mesh = std::make_shared<Mesh>("Box", 8, 12);

	mesh->vertices[0] = Vec4(-1, 1, 1 );
	mesh->vertices[1] = Vec4(1, 1, 1 );
	mesh->vertices[2] = Vec4(-1, -1, 1 );
	mesh->vertices[3] = Vec4(-1, -1, -1 );
	mesh->vertices[4] = Vec4(-1, 1, -1 );
	mesh->vertices[5] = Vec4(1, 1, -1 );
	mesh->vertices[6] = Vec4(1, -1, 1 );
	mesh->vertices[7] = Vec4(1, -1, -1 );
	
	mesh->faces[0] = Face{ 0, 1, 2 };
	mesh->faces[1] = Face{ 1, 2, 6 };
	mesh->faces[2] = Face{ 0, 4, 2 };
	mesh->faces[3] = Face{ 3, 4, 2 };
	mesh->faces[4] = Face{ 5, 6, 1 };
	mesh->faces[5] = Face{ 7, 6, 5 };

	mesh->faces[6] = Face{ 0, 5, 1 };
	mesh->faces[7] = Face{ 4, 5, 0 };
	mesh->faces[8] = Face{ 4, 7, 5 };
	mesh->faces[9] = Face{ 3, 7, 4 };
	mesh->faces[10] = Face{ 2, 7, 6 };
	mesh->faces[11] = Face{ 3, 7, 2 };

	//mesh->move(Vec4(1, 1, 1));
	g_mesh.push_back(mesh);
	Vec4 eye(0.0f, 0.0f, 1.0f);
	Vec4 at(0.0f, 0.0f, 0.0f);
	camera = Camera(eye, at);
}
*/

void initScene(){
	auto mesh = std::make_shared<Mesh>();
	if (!util.LoadOBJ("dog.obj", mesh.get())){
		_asm{
			int 3;
		}
	}
	mesh->move(Vec4(-0.2f, 0.0f, 0.0f));
	g_mesh.push_back(mesh);
	/*
	for (auto mesh : g_mesh) {
		for (int i = 0; i < mesh->vt_count; ++i) {
			printf("%f %f %f\n", mesh->vertices[i].x, mesh->vertices[i].y, mesh->vertices[i].z);
		}
		for (int i = 0; i < mesh->face_count; ++i) {
			printf("%d %d %d\n", mesh->faces[i].A, mesh->faces[i].B, mesh->faces[i].C);
		}
	}
	*/
	Vec4 eye(0.0f, 0.0f, 400.0f);
	Vec4 at(0.0f, 0.0f, 0.0f);
	camera = Camera(eye, at);
}

void display(){
	
	for (auto mesh : g_mesh){
		mesh->rotation_matrix.set_rotate(0.0f, 1.0f, 0.0f, rotate_theta);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	g_device->clear(0.0f, 0.0f, 0.0f, 1.0f);

	g_device->render(g_mesh, camera);

	rotate_theta += 0.01;

	glutSwapBuffers();
}


int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	initScene();

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("SR2");

	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	//glutMouseFunc(mouse_button);
	//glutMotionFunc(mouse_motion);
	//glutKeyboardFunc(keyboard);
	glutIdleFunc(display);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//glPointSize(2);

	glutMainLoop();
	return 0;
}

/*
struct rgbf { float r; float g; float b; float a; };
//WBL 9 May 2007 Based on
//http://www.codeguru.com/cpp/w-d/dislog/commondialogs/article.php/c1861/
//Common.h
void toRGBf(const float h, const float s, const float v,
	rgbf* rgb)
{
	
	//rgbf* rgb = (rgbf*) out;
	double min, max, delta, hue;

	max = v;
	delta = max * s;
	min = max - delta;

	hue = h;
	if (h > 300 || h <= 60)
	{
		rgb->r = max;
		if (h > 300)
		{
			rgb->g = min;
			hue = (hue - 360.0) / 60.0;
			rgb->b = ((hue * delta - min) * -1);
		}
		else
		{
			rgb->b = min;
			hue = hue / 60.0;
			rgb->g = (hue * delta + min);
		}
	}
	else
	if (h > 60 && h < 180)
	{
		rgb->g = max;
		if (h < 120)
		{
			rgb->b = min;
			hue = (hue / 60.0 - 2.0) * delta;
			rgb->r = min - hue;
		}
		else
		{
			rgb->r = min;
			hue = (hue / 60 - 2.0) * delta;
			rgb->b = (min + hue);
		}
	}
	else
	{
		rgb->b = max;
		if (h < 240)
		{
			rgb->r = min;
			hue = (hue / 60.0 - 4.0) * delta;
			rgb->g = (min - hue);
		}
		else
		{
			rgb->g = min;
			hue = (hue / 60 - 4.0) * delta;
			rgb->r = (min + hue);
		}
	}
	rgb->a = 0.0f;
}


//Convert a wide range of data values into nice colours 
void colour(const float data, float* out) {
	//convert data to angle
	const float a = atan2(data, 1) / (2 * atan2(1, 1)); // -1 .. +1
	const float angle = (1 + a) * 180; //red=0 at -1,+1

	const float saturation = 1;

	const float h = (data<-1 || data>1) ? 1 : fabs(data);

	toRGBf(angle, saturation, h, (rgbf*)out);
}



void display()
{
	//Create some nice colours (3 floats per pixel) from data -10..+10
	float* pixels = new float[size * 4];
	for (int i = 0; i<size; i++) {
		colour(10.0 - ((i*20.0) / size), &pixels[i * 4]);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//http://msdn2.microsoft.com/en-us/library/ms537062.aspx
	//glDrawPixels writes a block of pixels to the framebuffer.

	glDrawPixels(window_width, window_height, GL_RGBA, GL_FLOAT, pixels);

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("SR1");

	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	//glutMouseFunc(mouse_button);
	//glutMotionFunc(mouse_motion);
	//glutKeyboardFunc(keyboard);
	//glutIdleFunc(idle);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//glPointSize(2);

	glutMainLoop();
}*/