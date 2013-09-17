#include "dsMap.h"
#include "dsTexture.h"

#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <cmath>

const GLfloat pi = 3.1415926f;

GLfloat DSMap::grid_size = 10.0f;

// 从数组初始化一张地图
void DSMap::init(size_t _x_max, size_t _y_max, TileType* _data) {
    x_max = _x_max;
    y_max = _y_max;

    // 载入所有静态物体
    object_manager.loadAllObjects();

    // 载入平原贴图
    texture_ID_plain = dsLoadTextureBMP2D("data/images/grass.bmp");

    // 载入所有山地贴图: begin
    size_t A, B, C, D, temp;
    std::ostringstream os;
    for (size_t i = 0; i < 16; ++i) {
        os.str("");
        os << "data/images/hill_";
        A = i & 1;
        temp = i >> 1; // temp = DCB
        B = temp & 1;
        temp >>= 1;    // temp = DC
        C = temp & 1;
        D = temp >> 1;
        os << D << C << B << A << ".bmp";
        std::cout << os.str();
        texture_ID_hill[i] = dsLoadTextureBMP2D(os.str());
    }
    // 载入所有山地贴图: end

    // 建立地图数据
    if (data != nullptr) {
        delete[] data;
    }
    data = new TileType[x_max * y_max];
    for (size_t i = 0; i < x_max * y_max; ++i) {
        data[i] = hill;
    }
    data[1] = plain;
    data[17] = plain;

    // 创建所有显示列表
    loadDisplayLists();

    return;
    std::memcpy(data, _data, sizeof(TileType) * x_max * y_max);
}

DSMap::~DSMap() {
    delete[] data;
}

void DSMap::getSize(int* pwidth, int* pheight) const {
    if (pwidth != nullptr) {
        *pwidth = x_max;
    }
    if (pheight != nullptr) {
        *pheight = y_max;
    }
}

TileType DSMap::getTile(size_t x, size_t y) const {
    // 数组越界
    if (x < 0 || x >= x_max || y < 0 || y >= y_max) {
        return invalid;
    }

    return data[y * x_max + x];
}

// 绘制地图网格
void DSMap::renderGrids(bool selectMode) {
    GLfloat x = - grid_size * x_max / 2;
    GLfloat y;
    if (selectMode) {
        glPushName(0);
    }
    for (size_t x_index = 0; x_index < x_max; ++x_index) {
        y = - grid_size * y_max / 2;
        for (size_t y_index = 0; y_index < y_max; ++y_index) {
            if (selectMode) {
                glLoadName(y_index * x_max + x_index);
            }
            glColor4ub(255, 255, 255, 150);
            glBegin(GL_POLYGON);
            {
                glVertex3f(x,             y,             0.1f);
                glVertex3f(x + grid_size, y,             0.1f);
                glVertex3f(x + grid_size, y + grid_size, 0.1f);
                glVertex3f(x,             y + grid_size, 0.1f);
            }
            glEnd();
            y += grid_size;
        }
        x += grid_size;
    }
    if (selectMode) {
        glPopName();
    }

}

// 绘制地图，外部调用的唯一绘制函数
void DSMap::render(bool selectMode) {
    renderHugeGround();
    renderTiles();
    if (selectMode) {
        renderGrids(true);
    } else {
        glCallList(display_list_grids);
    }
}

void DSMap::renderTile(size_t x_index, size_t y_index) {
    GLfloat x, y;
    size_t hill_type = 0;
    getCoords(x_index, y_index, &x, &y);

    switch (data[y_index * x_max + x_index]) {
    case hill:
        glTranslatef(x, y, 0.0f);

        if (x_index == 0 || data[y_index * x_max + (x_index - 1)] == hill) {
            hill_type |= 1u;
        }
        if (y_index == 0 || data[(y_index - 1) * x_max + x_index] == hill) {
            hill_type |= 2u;
        }
        if (x_index == x_max - 1 || data[y_index * x_max + (x_index + 1)] == hill) {
            hill_type |= 4u;
        }
        if (y_index == y_max - 1 || data[(y_index + 1) * x_max + x_index] == hill) {
            hill_type |= 8u;
        }

        glCallList(display_lists_hills[hill_type]);
        glTranslatef(-x, -y, 0.0f);
        break;
    case barrier:
        glTranslatef(x, y, 0.0f);
        glCallList(display_lists[plain]);
        glCallList(display_lists[barrier]);
        glTranslatef(-x, -y, 0.0f);
        break;
    case trap:
        glTranslatef(x, y, 0.0f);
        glCallList(display_lists[plain]);
        glCallList(display_lists[trap]);
        glTranslatef(-x, -y, 0.0f);
        break;
    case temple:
        glTranslatef(x, y, 0.0f);
        glCallList(display_lists[plain]);
        glCallList(display_lists[temple]);
        glTranslatef(-x, -y, 0.0f);
        break;
    case plain:
        glTranslatef(x, y, 0.0f);
        glCallList(display_lists[plain]);
        glTranslatef(-x, -y, 0.0f);
        break;
    case facility:
        glTranslatef(x, y, 0.0f);
        glCallList(display_lists[plain]);
        glCallList(display_lists[facility]);
        glTranslatef(-x, -y, 0.0f);
        break;
    case cannon:
        glTranslatef(x, y, 0.0f);
        glCallList(display_lists[plain]);
        glCallList(display_lists[cannon]);
        glTranslatef(-x, -y, 0.0f);
        break;
    case forest:
        glTranslatef(x, y, 0.0f);
        glCallList(display_lists[plain]);
        glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
        glCallList(display_lists[forest]);
        glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);
        glCallList(display_lists[plain]);
        glTranslatef(-x, -y, 0.0f);
        break;
    default:
        break;
    }
}

void DSMap::renderTiles() {
    for (size_t x = 0; x < x_max; ++x)
        for (size_t y = 0; y < y_max; ++y) {
            renderTile(x, y);
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

void DSMap::renderHugeGround(GLfloat radius) {
    radius = 1000.0f;
    GLfloat theta = 0.0f;
    size_t num_edges = 20;
    GLfloat dtheta = 2 * pi/ num_edges;
    GLfloat x, y;
    glBindTexture(GL_TEXTURE_2D, texture_ID_plain);
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    
    for (size_t index = 0; index < num_edges; ++index) {
        x = std::cos(theta);
        y = std::sin(theta);
        glTexCoord2f(x, y);
        glVertex3f(radius * x, radius * y, -0.1f);
        theta += dtheta;
    }
    glEnd();
}

void DSMap::loadDisplayLists() {
    // Hill: begin
    display_lists[hill] = glGenLists(1);
    glNewList(display_lists[hill], GL_COMPILE);
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ID_hill[0]);
    glBegin(GL_QUADS);
    {
        glNormal3f(0.0f, 0.0f, 1.0f);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(- grid_size / 2.0f, - grid_size / 2.0f, 0.0f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(+ grid_size / 2.0f, - grid_size / 2.0f, 0.0f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(+ grid_size / 2.0f, + grid_size / 2.0f, 0.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(- grid_size / 2.0f, + grid_size / 2.0f, 0.0f);
    }
    glEnd();
    glPopAttrib();
    glEndList();
    // Hill: end

    // Plain: begin
    display_lists[plain] = glGenLists(1);
    glNewList(display_lists[plain], GL_COMPILE);
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ID_plain);
    glBegin(GL_QUADS);
    {
        glNormal3f(0.0f, 0.0f, 1.0f);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(- grid_size / 2.0f, - grid_size / 2.0f, 0.0f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(+ grid_size / 2.0f, - grid_size / 2.0f, 0.0f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(+ grid_size / 2.0f, + grid_size / 2.0f, 0.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(- grid_size / 2.0f, + grid_size / 2.0f, 0.0f);
    }
    glEnd();
    glPopAttrib();
    glEndList();
    // Plain: end

    // Forest: begin
    display_lists[forest] = glGenLists(1);
    glNewList(display_lists[forest], GL_COMPILE);
    object_manager.objects["plant2"].render();
    // object_manager.objects["banana"].render();
    glEndList();
    // Forest: end

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

    // Facility: begin
    display_lists[facility] = glGenLists(1);
    glNewList(display_lists[facility], GL_COMPILE);
    object_manager.objects["pallete"].render();
    glEndList();
    // Facility: end

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

    // Cannon: begin
    GLfloat cannon_move = grid_size * 0.7f / 2;
    display_lists[cannon] = glGenLists(1);
    glNewList(display_lists[cannon], GL_COMPILE);
    glTranslatef(0.0f, - cannon_move, 0.0f);
    object_manager.objects["cannon"].render();
    glTranslatef(cannon_move, cannon_move, 0.0f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    object_manager.objects["cannon"].render();
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(- cannon_move, cannon_move, 0.0f);
    glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    object_manager.objects["cannon"].render();
    glRotatef(-180.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(- cannon_move, - cannon_move, 0.0f);
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
    object_manager.objects["cannon"].render();
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(cannon_move, 0.0f, 0.0f);
    glEndList();
    // Cannon: end

    // Hills: begin
    for (size_t index = 0; index < 16; ++index) {
        display_lists_hills[index] = glGenLists(1);
        glNewList(display_lists_hills[index], GL_COMPILE);
        glPushAttrib(GL_ENABLE_BIT);
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_ID_hill[index]);
        glBegin(GL_QUADS);
        {
            glNormal3f(0.0f, 0.0f, 1.0f);

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(- grid_size / 2.0f, - grid_size / 2.0f, 0.0f);

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(+ grid_size / 2.0f, - grid_size / 2.0f, 0.0f);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(+ grid_size / 2.0f, + grid_size / 2.0f, 0.0f);

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(- grid_size / 2.0f, + grid_size / 2.0f, 0.0f);
        }
        glEnd();
        glPopAttrib();
        glEndList();
    }
    // Hills: end

    // Grids: begin
    display_list_grids = glGenLists(1);
    glNewList(display_list_grids, GL_COMPILE);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glLineWidth(1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    renderGrids();
    glPopAttrib();
    glEndList();
    // Grids: end
}
