#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

#ifndef MF_DIST
#define MF_LOG(x) x
#include <chrono>
#else 
#define MF_LOG(x)
#endif

#define WIDTH  1280;
#define HEIGHT 720;

void update()
{
	MF_LOG(auto buf_time = std::chrono::high_resolution_clock::now());

	//stuff

	MF_LOG(std::cout << "update() took " << (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - buf_time).count()) / 1000000.f << "\tms\n");
}

void render()
{
	MF_LOG(auto buf_time = std::chrono::high_resolution_clock::now());

	//stuff

	MF_LOG(std::cout << "render() took " << (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - buf_time).count()) / 1000000.f << "\tms\n");
}

int main(int argc, char** argv)
{
	/*
	while (true)
	{
		update();
		render();
	}
	*/
	return 0;
}