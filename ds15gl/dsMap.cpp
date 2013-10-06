#include "dsMap.h"
#include "dsTexture.h"
#include "dsTools.h"
#include "dsVector.h"
#include "dsEye.h"

#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <cmath>
#include <ctime>

const GLfloat pi = 3.1415926f;

GLfloat DSMap::grid_size = 10.0f;

void DSMap::load() {
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
    
    std::srand(std::time(nullptr));
    
}

// 从数组初始化一张地图
void DSMap::init(size_t _x_max, size_t _y_max, TileType* _data) {
    x_max = _x_max;
    y_max = _y_max;


    // 建立地图数据
    if (data != nullptr) {
        delete[] data;
    }
    data = new TileType[x_max * y_max];
    for (size_t i = 0; i < x_max * y_max; ++i) {
        data[i] = plain;
    }
    data[1] = hill;
    data[17] = hill;

    //     // 创建所有显示列表
    //     loadDisplayLists();
    listLoaded = false;
    map = 0;
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
    if (listLoaded == false)    {
        loadDisplayLists();
        listLoaded = true;
    }
    if (map == 0) {
        dishi();
    }
    glCallList(map);
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
        /*glTranslatef(x, y, 0.0f);

        if (x_index != 0 && data[y_index * x_max + (x_index - 1)] == hill) {
            hill_type |= 1u;
        }
        if (y_index != 0 && data[(y_index - 1) * x_max + x_index] == hill) {
            hill_type |= 2u;
        }
        if (x_index != x_max - 1 && data[y_index * x_max + (x_index + 1)] == hill) {
            hill_type |= 4u;
        }
        if (y_index != y_max - 1 && data[(y_index + 1) * x_max + x_index] == hill) {
            hill_type |= 8u;
        }

        glCallList(display_lists_hills[hill_type]);
        glTranslatef(-x, -y, 0.0f);*/
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
    static GLuint list = 0;
    if (list == 0) {
        list = glGenLists(1);
        glNewList(list, GL_COMPILE);
        int heights[20][20];
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                if ((i <= 10 && i >= 9) && (j >= 9 && j <= 10)) {
                    heights[i][j] = 0;
                } else {
                    heights[i][j] = rand() % 50 - 10;
                }
            }
        }
        glBindTexture(GL_TEXTURE_2D, texture_ID_plain);

        for (int i = 0; i < 19; i++) {
            for (int j = 0; j < 19; j++) {
                if ((i == 9 && j == 9)) {
                    continue;
                }
                glPushMatrix();
                glTranslatef((i - 9.5)*grid_size * x_max, (j - 9.5)*grid_size * y_max, -1);
                glBegin(GL_POLYGON);
                {
                    GLfloat p1[] = {0, 0, heights[i][j]};
                    GLfloat p2[] = {grid_size * x_max, 0, heights[i + 1][j]};
                    GLfloat p3[] = {grid_size * x_max, grid_size * y_max, heights[i + 1][j + 1]};
                    GLfloat res[3];
                    dsNormalVectorOfTriangle3fv(p1, p2, p3, res);

                    glNormal3f(res[0], res[1], res[2]);
                    glTexCoord2f(0, 0);
                    glVertex3f(0, 0, heights[i][j]);
                    glTexCoord2f(1, 0);
                    glVertex3f(grid_size * x_max, 0, heights[i + 1][j]);
                    glTexCoord2f(1, 1);
                    glVertex3f(grid_size * x_max, grid_size * y_max, heights[i + 1][j + 1]);
                    glTexCoord2f(0, 1);
                    glVertex3f(0, grid_size * y_max, heights[i][j + 1]);

                }
                glEnd();
                glPopMatrix();

            }
        }
        /*
                radius = 1000.0f;
                GLfloat theta = 0.0f;
                size_t num_edges = 20;
                GLfloat dtheta = 2 * pi / num_edges;
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
                glEnd();*/
        glEndList();

    }
    glCallList(list);

    //renderArrow(0, 0, 0.0f);
}

void DSMap::renderArrow(size_t x_index, size_t y_index, GLfloat duration) {
    GLfloat x, y, z;
    z = 1.0f + std::sin(5.0f * duration);
    getCoords(x_index, y_index, &x, &y);
    glTranslatef(x, y, z);

    static GLfloat height = 5.0f;
    static GLfloat width = 2.0f;

    static GLdouble direction[3];
    dsDiff3dv(center, eye, direction);

    static const GLdouble xline[] = {1.0, 0, 0};
    static const GLdouble yline[] = {0, 1.0, 0};
    GLdouble direction_t[] = {direction[0], direction[1], 0};
    // 计算旋转角度
    GLdouble rr = dsIncludedAngle2dv(yline, direction_t);
    GLdouble n[3];
    dsCross3dv(yline, direction, n);
    if (n[2] < 0) {
        rr = -rr;
    }

    glRotated(rr + 90.0, 0, 0, 1.0);


    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, height);
    glColor3f(1.0f / 2.5f, 0.0f, 0.0f);
    glVertex3f(0.0f, width, height + width);
    glVertex3f(0.0f, -width, height + width);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0.0f, width / 2, height + width);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, width / 2, height + width * 5 / 2);
    glVertex3f(0.0f, -width / 2, height + width * 5 / 2);
    glColor3f(1.0f / 2.5f, 0.0f, 0.0f);
    glVertex3f(0.0f, -width / 2, height + width);
    glEnd();
    //glBegin(GL_QUADS);
    //glVertex3f(0.0f, -1.0f, 0.0f);
    //glVertex3f(0.0f, 1.0f, 0.0f);
    //glVertex3f(0.0f, 1.0f, 1.0f);
    //glVertex3f(0.0f, -1.0f, 1.0f);
    //glEnd();
    glRotated(-rr - 90.0, 0, 0, 1.0);
    glPopAttrib();
    glTranslatef(-x, -y, 0.0f);
}

void DSMap::dishi() {
    size_t fenkuai = 20;

    // 每个地块都有 fenkuai * fenkuai 个点
    GLfloat* heights = new GLfloat[x_max * y_max * fenkuai * fenkuai];

    for (size_t x_index = 0; x_index < x_max; ++x_index)
        for (size_t y_index = 0; y_index < y_max; ++y_index) {
            if (data[y_index * y_max + x_index] == hill) {
                for (size_t cx = 0; cx < fenkuai; cx++) {
                    for (size_t cy = 0; cy < fenkuai; cy++) {
                        heights[x_index * fenkuai + cx + (y_index * fenkuai + cy) * x_max * fenkuai] = 0.01f;
                    }
                }
            } else {
                for (size_t cx = 0; cx < fenkuai; cx++) {
                    for (size_t cy = 0; cy < fenkuai; cy++) {
                        heights[x_index * fenkuai + cx + (y_index * fenkuai + cy) * x_max * fenkuai] = 0;
                    }
                }
            }
        }

    for (int i = 0; i < x_max * y_max; i++) {
        if (data[i] != hill) {
            continue;
        }
        size_t _x, _y;
        getXY(i, &_x, &_y);
        int x = _x;
        int y = _y;
        int _x_max = x_max;
        int _y_max = y_max;

        for (int j = 0; j < 2; j++) {
            int fx = rand() % fenkuai;
            int fy = rand() % fenkuai;
            for (int cx = fx - 10; cx < fx + 10; cx++) {
                for (int cy = fy - 10; cy < fy + 10; cy++) {
                    int index = x * fenkuai + cx + (y * fenkuai + cy) * _x_max * fenkuai;
                    if (index < 0 || index >= _x_max * _y_max * fenkuai * fenkuai) {
                        continue;
                    }
                    if (heights[index] > 0) {
                        float dis = (cx - fx) * (cx - fx) + (cy - fx) * (cy - fx);
                        if (dis > 25) {
                            heights[index] += 2 / sqrt(dis);
                        } else {
                            heights[index] += 1.4 - sqrt(dis) / 5;
                        }
                    }
                }
            }
        }
    }

    map = glGenLists(1);
    glNewList(map, GL_COMPILE);
    
	GLfloat dsize = 1.0f / fenkuai;

    GLfloat ds = grid_size / GLfloat(fenkuai);


    //// i = x_index * fenkuai + cx
    //// j = y_index * fenkuai + cy
    //for (size_t x_index = 0; x_index < x_max; ++x_index) {
    //    for (size_t y_index = 0; y_index < y_max; ++y_index) {
    //        for (size_t cx_index = 0; cx_index < fenkuai; ++cx_index) {
    //            for (size_t cy_index = 0; cy_index < fenkuai; ++cy_index) {
    //                size_t i = x_index * fenkuai + cx_index;
    //                size_t j = y_index * fenkuai + cy_index;

    //                if (heights[i + j * fenkuai * x_max] > 0.1) {
    //                    glBindTexture(GL_TEXTURE_2D, texture_ID_hill[15]);
    //                } else {
    //                    glBindTexture(GL_TEXTURE_2D, texture_ID_plain);
    //                }
    //                //glBindTexture(GL_TEXTURE_2D, texture_ID_plain);
    //                //glPushMatrix();
    //                glTranslatef(
    //                    (GLfloat(i) - (GLfloat)fenkuai * x_max / 2) / GLfloat(fenkuai) * grid_size,
    //                    (GLfloat(j) - (GLfloat)fenkuai * y_max / 2) / GLfloat(fenkuai) * grid_size,
    //                    -1.0f
    //                );
    //                glBegin(GL_POLYGON);
    //                {

    //                    GLfloat p1[] = {0, 0, heights[i + j * fenkuai* x_max]};
    //                    GLfloat p2[] = {ds, 0, heights[i + 1 + j* fenkuai* x_max]};
    //                    GLfloat p3[] = {0, ds, heights[i + (j + 1)*fenkuai* x_max]};
    //                    GLfloat res[3];
    //                    dsNormalVectorOfTriangle3fv(p1, p2, p3, res);

    //                    float cx = (i%fenkuai)/float(fenkuai);
    //                    float cy = (j%fenkuai)/float(fenkuai);

    //                    glNormal3f(res[0], res[1], res[2]);

    //                    glTexCoord2f(cx, cy);
    //                    glVertex3f(0, 0, heights[i + j * fenkuai * x_max]);

    //                    glTexCoord2f(cx+dsize, cy);
    //                    glVertex3f(ds, 0, heights[i + 1 + j * fenkuai * x_max]);

    //                    glTexCoord2f(cx+dsize, cy+dsize);
    //                    glVertex3f(ds, ds, heights[i + 1 + (j + 1) * fenkuai * x_max]);

    //                    glTexCoord2f(cx, cy+dsize);
    //                    glVertex3f(0, ds, heights[i + (j + 1) * fenkuai * x_max]);

    //                }
    //                glEnd();
    //                glTranslatef(
    //                    - (GLfloat(i) - (GLfloat)fenkuai * x_max / 2) / GLfloat(fenkuai) * grid_size,
    //                    - (GLfloat(j) - (GLfloat)fenkuai * y_max / 2) / GLfloat(fenkuai) * grid_size,
    //                    1.0f
    //                    );
    //                //glPopMatrix();
    //                // printf("%d,%d ", i, j);

    //            } // end for (size_t cy = 0; cy < fenkuai; ++cy)
    //        } // end for (size_t cx = 0; cx < fenkuai; ++cx)
    //    } // end for (size_t y_index = 0; y_index < y_max; ++y_index)
    //} // end for (size_t x_index = 0; x_index < x_max; ++x_index)

    // i = x_index * fenkuai + cx
    // j = y_index * fenkuai + cy
    for (size_t i = 0; i < fenkuai * x_max - 1; i++) {
        for (size_t j = 0; j < fenkuai * y_max - 1; j++) {
            size_t x_index = i / fenkuai;
            size_t y_index = j / fenkuai;
            size_t cx = i % fenkuai;
            size_t cy = j % fenkuai;

            if (data[y_index * x_max + x_index] == hill) {
                glBindTexture(GL_TEXTURE_2D, texture_ID_hill[15]);
            } else {
                continue;
                glBindTexture(GL_TEXTURE_2D, texture_ID_plain);
            }
            

			/*if (heights[i + j * fenkuai * x_max] > 0.1) {
				glBindTexture(GL_TEXTURE_2D, texture_ID_hill[15]);
			} else {
				glBindTexture(GL_TEXTURE_2D, texture_ID_plain);
			}*/

			//glBindTexture(GL_TEXTURE_2D, texture_ID_plain);
            //glPushMatrix();
            glTranslatef(
                (i - (GLfloat)fenkuai * x_max / 2) / GLfloat(fenkuai) * grid_size,
                (j - (GLfloat)fenkuai * y_max / 2) / GLfloat(fenkuai) * grid_size,
                -1.0f
            );
            glBegin(GL_POLYGON);
            {

                GLfloat p1[] = {0, 0, heights[i + j * fenkuai * x_max]};
                GLfloat p2[] = {ds, 0, heights[i + 1 + j * fenkuai * x_max]};
                GLfloat p3[] = {0, ds, heights[i + (j + 1) * fenkuai * x_max]};
                GLfloat res[3];
                dsNormalVectorOfTriangle3fv(p1, p2, p3, res);

				float cx = (i % fenkuai) / GLfloat(fenkuai);
				float cy = (j % fenkuai) / GLfloat(fenkuai);

                glNormal3f(res[0], res[1], res[2]);

                glTexCoord2f(cx, cy);
                glVertex3f(0, 0, heights[i + j * fenkuai * x_max]);

                glTexCoord2f(cx + dsize, cy);
                glVertex3f(ds, 0, heights[i + 1 + j * fenkuai * x_max]);

                glTexCoord2f(cx + dsize, cy + dsize);
                glVertex3f(ds, ds, heights[i + 1 + (j + 1) * fenkuai * x_max]);

                glTexCoord2f(cx, cy + dsize);
                glVertex3f(0, ds, heights[i + (j + 1) * fenkuai * x_max]);

            }
            glEnd();
            glTranslatef(
                - (i - (GLfloat)fenkuai * x_max / 2) / GLfloat(fenkuai) * grid_size,
                - (j - (GLfloat)fenkuai * y_max / 2) / GLfloat(fenkuai) * grid_size,
                1.0f
            );
            //glPopMatrix();
            // printf("%d,%d ", i, j);
        }
    }
    glEndList();
    delete[] heights;
}

void DSMap::loadDisplayLists() {
    // Clear
    for (int i = 0; i < 8; i++) {
        if (glIsList(display_lists[i])) {
            glDeleteLists(1, display_lists[i]);
        }
    }

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
