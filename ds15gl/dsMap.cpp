#include "dsMap.h"
#include <iostream>

// 从文件初始化一张地图
DSMap::DSMap(const char* fileName) : grid_size(10.0f) {
	// 如果没有传入文件名，则产生一张 100 * 100 的地图，且所有元素都是0
	if (fileName[0] == '\0') {
		width = 10;
		height = 10;
		data = new int*[height];
		
		for (size_t i = 0; i < height; i++) {
			data[i] = new int[width];
			for (size_t j = 0; j < width; j++) {
				data[i][j] = 0;
			}
		}
	}
}


DSMap::~DSMap() {
	for (size_t i = 0; i < height; i++) {
		delete[] data[i];
	}
	delete[] data;
}

void DSMap::renderGrid() {
    //glPushAttrib(GL_LIGHTING | GL_TEXTURE_2D | GL_POLYGON_MODE); // 测试不良
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glLineWidth(1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPushMatrix();
    GLfloat x = - grid_size * width / 2;
    GLfloat y;
    for (size_t x_index = 0; x_index < width; ++x_index) {
        y = - grid_size * height / 2;
        for (size_t y_index = 0; y_index < height; ++y_index) {
            glBegin(GL_POLYGON);
            {
                glVertex3f(x,             y,             0.0f);
                glVertex3f(x + grid_size, y,             0.0f);
                glVertex3f(x + grid_size, y + grid_size, 0.0f);
                glVertex3f(x,             y + grid_size, 0.0f);
            }
            glEnd();
            y += grid_size;
        }
        x += grid_size;
    }
    glPopMatrix();
    glPopAttrib();
}

void DSMap::getCoords(size_t x_index, size_t y_index, GLfloat* px, GLfloat* py) const {
    *px = grid_size * (- (GLfloat)width / 2 + x_index + 0.5f);
    *py = grid_size * (- (GLfloat)height / 2 + y_index + 0.5f);
}

