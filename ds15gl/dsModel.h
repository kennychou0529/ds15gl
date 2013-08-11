#ifndef _DSMODEL_H
#define _DSMODEL_H

#include <GL/glut.h>

// float ������
typedef struct {
    float v[3];
} Vertex3f;

// MD2 �ļ��е���������
// �� ����ֵ ���棬������ 0 ~ 1 ֮�����
// �������ǽ����Ҫ�ֶ������������ ����������ֵ
typedef struct {
    short u;
    short v;
} MD2TexCoord2s;

// �������꣬Ϊ 0 ~ 1 ֮�����
typedef struct {
    float u;
    float v;
} TexCoord2f;

// MD2 �ļ��еĶ���
// ÿ���㻹����һ����������ţ������� Quake II �еķ������б�������
// ������Ҫ��ȷ��������ʱ�ֶ����㷨�����������������������ò���
typedef struct {
    unsigned char v[3];
    unsigned char normal_index; // ��������ò���
} MD2Vertex;

// MD2 �ļ��е�֡
typedef struct {
    float scale[3];
    float translate[3];
    char name[16];          // ������ÿ��֡���и�����
    MD2Vertex vertex_list[1];       // ��ʵ�� num_vertices �� MD2Vertex��vertex_list ֻ�ǵ�ָ����
} MD2Frame;

// ÿ���������� ������ �� �������� �ı��
typedef struct {
    unsigned short mesh_index[3];
    unsigned short tex_coord_index[3];
} Mesh;

typedef struct {
    int identifier;         // һ�������Լ��� MD2 ����������, ����Զ�� 0x32504449("IDP2")
    int version;            // �汾����Զ�� 8
    int skin_width;         // ������ (����ֵ), ͨ���� 256
    int skin_height;        // ����߶� (����ֵ), ͨ���� 256
    int frame_size;         // ÿ��֡ռ�ö��� bytes
    int num_skins;          // ����������
    int num_vertices;       // ÿ��֡�ж��ٶ���
    int num_tex_coords;     // ������������
    int num_triangles;      // ����������
    int num_GLcommands;     // GL ���������������û�õ�
    int num_frames;         // ֡������
    int offset_textures;    // �����ļ��� (��) ��ʼλ��
    int offset_tex_coord;   // ����������ʼλ��
    int offset_triangles;   // ��������ʼλ��
    int offset_frames;      // ֡������ʼλ��
    int offset_GLcommands;  // OpenGL ������ʼλ�ã�����û�õ�
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