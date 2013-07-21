#include "dsFrame.h"

void DSFrame::initialize() {
	scene.initialize();
}

void DSFrame::display() {
	scene.show();
	statusBar.show();
}
