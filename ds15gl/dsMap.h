#ifndef _DSMAP_H
#define _DSMAP_H

#include <GL/glut.h>

class DSMap {
public:
    // data的大小不应小于x_max * y_max
    void init(size_t x_max = 10, size_t y_max = 10, char* data = NULL);
    ~DSMap();
public:
    void getSize(int* pwidth = nullptr, int* pheight = nullptr) const {
        if (pwidth != nullptr) {
            *pwidth = x_max;
        }
        if (pheight != nullptr) {
            *pheight = y_max;
        }
    }

    int getTile(size_t x, size_t y) const {
        //数组越界
        if (x < 0 || x >= x_max || y < 0 || y >= y_max) {
            return 0;
        }

        return data[y * y_max + x];
    }

    // 绘制地图网格
    void renderGrid();

    // 输入格子坐标，给出绘图实际坐标
    void getCoords(
        size_t x_index,
        size_t y_index,
        GLfloat* px,
        GLfloat* py
    ) const;

private:
    size_t x_max;
    size_t y_max;
    static GLfloat grid_size; // 地图每块的大小
    char* data;
};

#endif