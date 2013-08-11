#ifndef _DSMODEL_H
#define _DSMODEL_H

#include <GL/glut.h>

typedef struct {
    float v[3];
} Vertex3f;

// 纹理坐标
typedef struct {
    short u;
    short v;
} TexCoord2s;

// 纹理坐标
typedef struct {
    float u;
    float v;
} TexCoord2f;

// 什么东西？
typedef struct {
    unsigned char v[3];
    unsigned char normal_index;
} FramePoint;

// 帧信息
typedef struct {
    float scale[3];
    float translate[3];
    char name[16];
    FramePoint fp[1];
} Frame;

// 每个三角形中 点坐标 和 纹理坐标 的编号
typedef struct {
    unsigned short mesh_index[3];
    unsigned short tex_coord_index[3];
} Mesh;

typedef struct {
    int identifier;         // 一个表明自己是 MD2 的神奇数字
    int version;            // 版本，貌似是8
    int skin_width;         // 纹理宽度
    int skin_height;        // 纹理高度
    int frame_size;         // 每个帧占用多少 bytes
    int num_skins;          // 纹理数量？
    int num_vertices;       // 点的数量
    int num_tex_coords;     // 纹理坐标数量
    int num_triangles;      // 三角形数量
    int num_GLcommands;     // ？
    int num_frames;         // 帧的数量
    int offset_skins;       // 皮肤内容位置
    int offset_tex_coord;   // 纹理坐标位置
    int offset_triangles;   // 三角形位置
    int offset_frames;      // 帧数据位置
    int offset_GLcommands;  // OpenGL 命令位置
    int offset_eof;         // 文件结尾位置
} MD2Header;

class MD2Model {
private:
    int num_frames;         // 帧的数量
    int num_vertices;       // 点的数量
    int num_triangles;      // 三角形数量
    int num_tex_coords;     // 纹理坐标数量
    int frame_size;         // 每个帧占用多少 bytes
    int current_frame;      // 当前帧编号
    int next_frame;         // 下一帧编号
    float interpol;         // 什么玩意？
    Mesh* triangle_list;
    TexCoord2f* tex_coord_list;
    Vertex3f* vertex_list;
    GLuint texture_ID;
public:
    MD2Model();
    ~MD2Model();

    int load(char* model_file, char* skin_file);

    int loadModel(char* model_file);

    int loadSkin(char* skin_file);

    int renderFrame(int frame_index);
};

#endif