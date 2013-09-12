#ifndef _DSMAP_H
#define _DSMAP_H

#include <GL/glut.h>
#include "dsObject.h"

enum TileType {
    plain,   // 平原
    hill,    // 山地
    forest,  // 森林
    barrier, // 屏障
    cannon,  // 炮
    temple,  // 神庙
    trap,    // 陷阱
    facility // 机关
};

class DSMap {
public:
    // data的大小不应小于 x_max * y_max
    void init(size_t x_max = 10, size_t y_max = 10, TileType* data = nullptr);
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

    // 绘制一个格子
    void renderTile(size_t x_index, size_t y_index);

    // 输入格子坐标，给出绘图实际坐标
    void getCoords(
        size_t x_index,
        size_t y_index,
        GLfloat* px,
        GLfloat* py
    ) const;

    void getCoords(
        size_t x_index,
        size_t y_index,
        GLdouble* px,
        GLdouble* py
    ) const;

private:
    DSObjectManager object_manager;

    size_t x_max;
    size_t y_max;
    static GLfloat grid_size; // 地图每块的大小
    TileType* data;
    GLuint display_lists[8];
};

#endif