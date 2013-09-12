// Written by phisiart

#ifndef _DSMODEL_H
#define _DSMODEL_H

#include <GL/glut.h>
#include <string>

// MD2Model 类不必直接使用，已经封装到 dsSoldier 类里了
// dsSoldier 用法详见 dsSoldier.h

class MD2Model {
public:
    MD2Model();
    MD2Model(MD2Model&& model);
    MD2Model(const MD2Model& model);
    ~MD2Model();
    int load(const std::string& model_file, const std::string& skin_file);
    void clear();
    void renderFrame(size_t frame_index) const;
    void renderSmoothly(
        size_t frame1_index,
        size_t frame2_index,
        GLfloat percentage
    ) const;

private:
    // 以下定义了一些 struct
    // 之所以定义在 class MD2Model 之内是为了避免其它地方的使用

    // 说明: 类型名 的命名规则：凡是 MD2 文件内的规定数据格式
    //       都添加了 MD2 前缀

    // float 点坐标
    typedef struct {
        GLfloat v[3];
    } Vertex3f;

    // MD2 文件中的纹理坐标
    // 以 像素值 储存，而不是 0 ~ 1 之间的数
    // 所以我们今后需要手动用这个数除以 纹理宽高像素值
    typedef struct {
        short u;
        short v;
    } MD2TexCoord2s;

    // 纹理坐标，为 0 ~ 1 之间的数
    typedef struct {
        GLfloat u;
        GLfloat v;
    } TexCoord2f;

    // MD2 文件中的顶点
    // 每个点还附带一个法向量编号，用于在 Quake II 中的法向量列表中搜索
    // 但我们要在确定三角形时手动计算法向量，所以这个法向量编号用不上
    typedef struct {
        unsigned char v[3];
        unsigned char normal_index; // 这个我们用不上
    } MD2Vertex;

    // MD2 文件中的帧，头部有三个变量
    // 其后是 num_vertices 个 MD2Vertex
    // 每个 MD2Vertex 并不是点的真正坐标
    // 真正坐标是 MD2Vertex * scale + translate
    // 所以我们需要手动计算真正坐标，然后存入 Vertex3f 中
    typedef struct {
        float scale[3];
        float translate[3];

        // 看起来每个帧还有个名字
        char name[16];

        // 其实有 num_vertices 个 MD2Vertex，vertices 只是当指针用
        MD2Vertex vertices[1];

    } MD2Frame;

    // 每个三角形中 点坐标 和 纹理坐标 的编号
    typedef struct {
        unsigned short mesh_index[3];
        unsigned short tex_coord_index[3];
    } Mesh;

    typedef struct {
        unsigned int identifier;         // 一个表明自己是 MD2 的神奇数字,
        // 它永远是 0x32504449("IDP2")
        unsigned int version;            // 版本，永远是 8
        unsigned int skin_width;         // 纹理宽度 (像素值), 通常是 256
        unsigned int skin_height;        // 纹理高度 (像素值), 通常是 256
        unsigned int frame_size;         // 每个帧占用多少 bytes
        unsigned int num_skins;          // 纹理数量？
        unsigned int num_vertices;       // 每个帧有多少顶点
        unsigned int num_tex_coords;     // 纹理坐标数量
        unsigned int num_triangles;      // 三角形数量
        unsigned int num_GLcommands;     // GL 命令的数量，我们没用到
        unsigned int num_frames;         // 帧的数量
        unsigned int offset_textures;    // 纹理文件名 (们) 起始位置
        unsigned int offset_tex_coord;   // 纹理坐标起始位置
        unsigned int offset_triangles;   // 三角形起始位置
        unsigned int offset_frames;      // 帧数据起始位置
        unsigned int offset_GLcommands;  // OpenGL 命令起始位置，我们没用到
        unsigned int offset_eof;         // 文件结尾位置
    } MD2Header;

    unsigned int num_frames;         // 帧的数量
    unsigned int num_vertices;       // 点的数量
    unsigned int num_triangles;      // 三角形数量
    unsigned int num_tex_coords;     // 纹理坐标数量
    unsigned int frame_size;         // 每个帧占用多少 bytes
    Mesh* triangles;
    Vertex3f* normal_vecs;
    TexCoord2f* tex_coords;
    Vertex3f* vertices;
    GLuint texture_ID;
};

#endif