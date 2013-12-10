#ifndef _DSMAP_H
#define _DSMAP_H

#include <GL/glut.h>
#include "dsObject.h"
#include "display_basic.h"

enum TileType {
    plain,    // 平原
    hill,     // 山地
    forest,   // 森林
    barrier,  // 屏障
    cannon,   // 炮
    temple,   // 神庙
    trap,     // 陷阱
    facility, // 机关
    mirror,   // 镜子
    invalid   // 返回错误
};

class DSMap {
public:
    // data的大小不应小于 x_max * y_max
	void load();

    void init(size_t x_max = 10, size_t y_max = 10, TileType* data = nullptr);

    void init(Game_Info* game_info);

    ~DSMap();

public:

    // 返回地图尺寸
    void getSize(int* pwidth = nullptr, int* pheight = nullptr) const;

    // 返回特定格子的信息
    TileType getTile(size_t x, size_t y) const;

    // 从单一序号中提取出 x, y 坐标
    void getXY(size_t index, size_t* x, size_t* y) const {
        *x = index % x_max;
        *y = index / x_max;
    }

    // 绘制地图，外部调用的唯一绘制函数
    void render(bool selectMode);

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

    void renderArrow(size_t x_index, size_t y_index, GLfloat duration);

private:
    // 求出应该用哪张山地贴图
    size_t getHillType(size_t x_index, size_t y_index);

    // 绘制地图网格
    void renderGrids(bool selectMode = false);

    // 绘制一个格子
    void renderTile(size_t x_index, size_t y_index);

    // 绘制所有格子
    void renderTiles();

    // 绘制一个超大的地面，延伸至很远很远
    void renderHugeGround(GLfloat radius = 5.0f);

    // 所有山地
	GLuint map;

    // 创建地势 和 山地显示列表
	void dishi();

    // 载入所有显示列表
    void loadDisplayLists();

	bool listLoaded;

    // 静态物体管理器
    DSObjectManager object_manager;

    size_t x_max;
    size_t y_max;

    // 地图每块的大小
    static GLfloat grid_size;

    // 地图数据 data[y_index * x_max + x_index]
    TileType* data;

    // 多种地形
    GLuint display_lists[10];

    // 线状网格
    GLuint display_list_grids;

    // 地图的外延
    GLuint display_list_huge_ground;

    // 有了山地地势，这个应该不用了
    GLuint display_lists_hill[16];

    // 平原贴图编号
    GLuint texture_ID_plain;

    // 镜子贴图编号
    GLuint texture_ID_mirror;

    // 山地贴图详解：
    // 一块山地分为四个边
    //             D
    //  +--------------------+
    //  |                    |
    //  |                    |
    //  |                    |
    //  |                    |
    // A|                    |C
    //  |                    |
    //  |                    |
    //  |                    |
    //  +--------------------+
    //             B
    // 记 绿色边（即相邻格子不是山地）为 0，记山地边（即相邻格子还是山地）为 1
    // 则共有 2 ** 4 = 16 种格子
    // 现编号 'DCBA'，即 D << 3 + C << 2 + B << 1 + A
    GLuint texture_ID_hill[16];
};

#endif