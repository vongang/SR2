#include <iostream>
#include <cstdio>
#include <cstring>
#include <memory>
#include<chrono>

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
std::chrono::time_point<std::chrono::high_resolution_clock> m_time_begin;

void initScene(){
	auto mesh = std::make_shared<Mesh>();
	if (!util.LoadOBJ("bunny.obj", mesh.get())){
		_asm{
			int 3;
		}
	}
	mesh->move(Vec4(0.0f, 0.0f, 0.0f));
	g_mesh.push_back(mesh);

	Vec4 eye(0.0f, 0.0f, 10.0f);
	Vec4 at(0.0f, 0.0f, 0.0f);
	camera = Camera(eye, at);
}

void display(){
	
	m_time_begin = std::chrono::high_resolution_clock::now();

	for (auto mesh : g_mesh){
		mesh->rotation_matrix.set_rotate(0.0f, 1.0f, 0.0f, rotate_theta);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	g_device->clear(0.0f, 0.0f, 0.0f, 1.0f);

	g_device->render(g_mesh, camera);

	rotate_theta += 0.02;

	double time_count = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_time_begin).count();
	time_count = 1.0 / time_count * 1000000.0;
	//printf("FPS %.2f\n", time_count);

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

	glutMainLoop();
	return 0;
}
