#include "dsMap.h"
#include <iostream>
#include <cstring>

GLfloat DSMap::grid_size = 10.0f;

// 从数组初始化一张地图
void DSMap::init(size_t _x_max, size_t _y_max, TileType* _data) {
    object_manager.loadAllObjects();
    
    loadDisplayLists();

    x_max = _x_max;
    y_max = _y_max;

    display_list_grids = glGenLists(1);
    glNewList(display_list_grids, GL_COMPILE);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glLineWidth(1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    GLfloat x = - grid_size * x_max / 2;
    GLfloat y;
    for (size_t x_index = 0; x_index < x_max; ++x_index) {
        y = - grid_size * y_max / 2;
        for (size_t y_index = 0; y_index < y_max; ++y_index) {
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
    glPopAttrib();
    glEndList();

    if (data != nullptr) {
        delete[] data;
    }
    data = new TileType[x_max * y_max];
    for (int i = 0; i < x_max * y_max; i++) {
        data[i] = barrier;
    }
    return;
    std::memcpy(data, _data, sizeof(TileType) * x_max * y_max);
}

DSMap::~DSMap() {
    delete[] data;
}

void DSMap::renderGrid() {
    renderTile(0, 0);
    glCallList(display_list_grids);
}

void DSMap::renderTile(size_t x_index, size_t y_index) {

    GLfloat x, y;
    getCoords(x_index, y_index, &x, &y);

    switch (data[x_index * y_max + x_index]) {
    case barrier:
        glTranslatef(x, y, 0.0f);
        glCallList(display_lists[barrier]);
        glTranslatef(-x, -y, 0.0f);
        break;
    case trap:
        glTranslatef(x, y, 0.0f);
        glCallList(display_lists[trap]);
        glTranslatef(-x, -y, 0.0f);
        break;
    case temple:
        glTranslatef(x, y, 0.0f);
        glCallList(display_lists[temple]);
        glTranslatef(-x, -y, 0.0f);
        break;
    default:
        break;
    }

}

void DSMap::getCoords(
    size_t x_index,
    size_t y_index,
    GLfloat* px,
    GLfloat* py
)  const {
    *px = grid_size * (- (GLfloat)x_max / 2 + x_index + 0.5f);
    *py = grid_size * (- (GLfloat)y_max / 2 + y_index + 0.5f);
}

void DSMap::getCoords(
    size_t x_index,
    size_t y_index,
    GLdouble* px,
    GLdouble* py
)  const {
    *px = grid_size * (- (GLdouble)x_max / 2 + x_index + 0.5f);
    *py = grid_size * (- (GLdouble)y_max / 2 + y_index + 0.5f);
}

void DSMap::loadDisplayLists() {
    // Trap: begin
    display_lists[trap] = glGenLists(1);
    glNewList(display_lists[trap], GL_COMPILE);
    glPushMatrix();
    {
        glTranslatef(0.0f, - grid_size * 0.9f / 2, 0.0f);
        object_manager.objects["hek"].render();

        glTranslatef(0.0f, + grid_size * 0.9f, 0.0f);
        object_manager.objects["hek"].render();

        glTranslatef(- grid_size * 0.9f / 2.0f, - grid_size * 0.9 / 2, 0.0f);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        object_manager.objects["hek"].render();

        glTranslatef(0.0f, - grid_size * 0.9f, 0.0f);
        object_manager.objects["hek"].render();
    }
    glPopMatrix();
    glEndList();
    // Trap: end

    // Temple: begin
    display_lists[temple] = glGenLists(1);
    glNewList(display_lists[temple], GL_COMPILE);
    object_manager.objects["temple"].render();
    glEndList();
    // Temple: end

    // Barrier: begin
    display_lists[barrier] = glGenLists(1);
    glNewList(display_lists[barrier], GL_COMPILE);
    glPushMatrix();
    object_manager.objects["hek"].render();
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    object_manager.objects["hek"].render();
    glPopMatrix();
    glEndList();
    // Barrier: end
}
