#include "dsFrame.h"


DSFrame::DSFrame() {
	setSize(800, 600);
}

void DSFrame::initialize() {
	scene.initialize();
}

DSFrame::~DSFrame() {}


void DSFrame::display() {
	scene.show();
	statusBar.show();
}

void DSFrame::setSize(int width, int height) {
	window_height = height;
	window_width = width;
	statusBar.setSize(width, height);
	scene.setSize(width, height);
}