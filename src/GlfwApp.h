#pragma once

struct GlfwApp {
	bool keyL=false;
	int keyS = 1;
	bool keyT=true;

	float scrollY = 1e-2f;

	double xpos=0, ypos=0;
	double time=0;
	int SCR_WIDTH = 1280;
	int SCR_HEIGHT = 800;

	GlfwApp();
	~GlfwApp();
	bool run();

	void processInput();
};

