#pragma once

#include <windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <stdio.h>

#define CLASS_NAME "OpenGL"

class CGLWindow{
private:
	HDC		hDC;
	HGLRC	hRC;
	bool	fullscreen;
	int		width, height;
public:
	HWND	hWnd;
	CGLWindow();
	~CGLWindow();

	bool Initialize(int width, int height,int bpp, bool fullscreen);
	void Terminate();
	void Resize(int width, int height);

	void Swap(){ SwapBuffers(hDC); }

	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

