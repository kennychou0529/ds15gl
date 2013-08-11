#ifndef _DSMODEL_H
#define _DSMODEL_H

#include <GL/glut.h>

typedef struct {
    float v[3];
} Vertex3f;

// ��������
typedef struct {
    short u;
    short v;
} TexCoord2s;

// ��������
typedef struct {
    float u;
    float v;
} TexCoord2f;

// ʲô������
typedef struct {
    unsigned char v[3];
    unsigned char normal_index;
} FramePoint;

// ֡��Ϣ
typedef struct {
    float scale[3];
    float translate[3];
    char name[16];
    FramePoint fp[1];
} Frame;

// ÿ���������� ������ �� �������� �ı��
typedef struct {
    unsigned short mesh_index[3];
    unsigned short tex_coord_index[3];
} Mesh;

typedef struct {
    int identifier;         // һ�������Լ��� MD2 ����������
    int version;            // �汾��ò����8
    int skin_width;         // ������
    int skin_height;        // ����߶�
    int frame_size;         // ÿ��֡ռ�ö��� bytes
    int num_skins;          // ����������
    int num_vertices;       // �������
    int num_tex_coords;     // ������������
    int num_triangles;      // ����������
    int num_GLcommands;     // ��
    int num_frames;         // ֡������
    int offset_skins;       // Ƥ������λ��
    int offset_tex_coord;   // ��������λ��
    int offset_triangles;   // ������λ��
    int offset_frames;      // ֡����λ��
    int offset_GLcommands;  // OpenGL ����λ��
    int offset_eof;         // �ļ���βλ��
} MD2Header;

class MD2Model {
private:
    int num_frames;         // ֡������
    int num_vertices;       // �������
    int num_triangles;      // ����������
    int num_tex_coords;     // ������������
    int frame_size;         // ÿ��֡ռ�ö��� bytes
    int current_frame;      // ��ǰ֡���
    int next_frame;         // ��һ֡���
    float interpol;         // ʲô���⣿
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