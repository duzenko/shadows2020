#pragma once

#include "..\glhelpers\GlProgram.h"


struct GlfwWin {
	bool keyL=false;
	int keyS = 1;
	bool keyT=true;

	float scrollY = 1e-2f;

	struct CursorPos {
		double x = 0, y = 0;
	} cursorPos;
	bool mouseMoved;
	double time=0;
	int SCR_WIDTH = 1280;
	int SCR_HEIGHT = 800;

	GlfwWin();
	~GlfwWin();
	bool messageLoop();

	void processInput();
};
