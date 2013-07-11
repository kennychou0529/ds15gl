#include "dsFrame.h"

DSFrame::DSFrame() {}

DSFrame::~DSFrame() {}

void DSFrame::initialize() {
	scene.initialize();
}

void DSFrame::display() {
	scene.show();
	statusBar.show();
}