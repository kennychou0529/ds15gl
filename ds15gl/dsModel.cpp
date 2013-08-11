#include "dsModel.h"
#include "dsTexture.h"
#include <fstream>

void CalculateNormal(float* p1, float* p2, float* p3) {
    float a[3], b[3], result[3];
    float length;

    a[0] = p1[0] - p2[0];
    a[1] = p1[1] - p2[1];
    a[2] = p1[2] - p2[2];

    b[0] = p1[0] - p3[0];
    b[1] = p1[1] - p3[1];
    b[2] = p1[2] - p3[2];

    result[0] = a[1] * b[2] - b[1] * a[2];
    result[1] = b[0] * a[2] - a[0] * b[2];
    result[2] = a[0] * b[1] - b[0] * a[1];

    // calculate the length of the normal
    length = (float)sqrt(result[0] * result[0] + result[1] * result[1] + result[2] * result[2]);

    // normalize and specify the normal
    glNormal3f(result[0] / length, result[1] / length, result[2] / length);
}

MD2Model::MD2Model():
    num_frames(0),
    num_triangles(0),
    num_vertices(0),
    num_tex_coords(0),
    frame_size(0),
    current_frame(0),
    next_frame(1),
    interpol(0),
    triangle_list(nullptr),
    tex_coord_list(nullptr),
    vertex_list(nullptr) {}

MD2Model::~MD2Model() {}

int MD2Model::load(char* model_file, char* skin_file) {
    std::ifstream is;
    is.open(model_file, is.in | is.binary);
    if (!is) {
        return -1;
    }

    // 确认文件长度
    is.seekg(0, is.end);
    size_t file_length = is.tellg();
    is.seekg(0, is.beg);

    // 将整个文件全部载入 buffer，然后文件就可以关闭了
    char* file_buffer = new char[file_length + 1];
    is.read(file_buffer, file_length);
    is.close();

    // 接下来从 buffer 中获取 MD2 文件头
    MD2Header* model_header = (MD2Header*)file_buffer;

    // 获取文件头中我们比较感兴趣的内容
    num_frames = model_header->num_frames;
    num_vertices = model_header->num_vertices;
    frame_size = model_header->frame_size;
    num_tex_coords = model_header->num_tex_coords;
    num_triangles = model_header->num_triangles;

    // 载入点数据
    // 文件头中提示，每帧都有 num_vertices 个顶点
    // 所以我们需要分配 num_vertices * num_frames 个点
    vertex_list = new Vertex3f[num_vertices * num_frames];
    Vertex3f* vertex_ptr;
    MD2Frame* frame_ptr;
    for (int frame_index = 0; frame_index < num_frames; ++frame_index) {
        frame_ptr = (MD2Frame*)&file_buffer[model_header->offset_frames + frame_size * frame_index];
        vertex_ptr = (Vertex3f*)&vertex_list[num_vertices * frame_index];

        for (int vertex_index = 0; vertex_index < num_vertices; ++ vertex_index) {
            vertex_ptr[vertex_index].v[0] = frame_ptr->vertex_list[vertex_index].v[0] * frame_ptr->scale[0] + frame_ptr->translate[0];
            vertex_ptr[vertex_index].v[1] = frame_ptr->vertex_list[vertex_index].v[1] * frame_ptr->scale[1] + frame_ptr->translate[1];
            vertex_ptr[vertex_index].v[2] = frame_ptr->vertex_list[vertex_index].v[2] * frame_ptr->scale[2] + frame_ptr->translate[2];
        }
    }

    // 载入纹理文件，获取纹理编号 texture_ID
    // 这里使用了 dsTexture 中的函数
    unsigned int texture_height, texture_width;
    texture_ID = dsLoadTextureBMP2D(skin_file, &texture_height, &texture_width); // modify it!

    // 载入纹理坐标
    tex_coord_list = new TexCoord2f[num_tex_coords];
    MD2TexCoord2s* tex_coord_2s_ptr = (MD2TexCoord2s*)(&file_buffer[model_header->offset_tex_coord]);
    for (int tex_coord_index = 0; tex_coord_index < num_tex_coords; ++tex_coord_index) {
        tex_coord_list[tex_coord_index].u = (float)tex_coord_2s_ptr[tex_coord_index].u / (float)texture_width;
        tex_coord_list[tex_coord_index].v = (float)tex_coord_2s_ptr[tex_coord_index].v / (float)texture_height;
    }

    // 载入三角形
    triangle_list = new Mesh[num_triangles];
    Mesh* triangle_ptr = (Mesh*)&file_buffer[model_header->offset_triangles];
    for (int triangle_index = 0; triangle_index < num_triangles; ++ triangle_index) {
        triangle_list[triangle_index].mesh_index[0] = triangle_ptr[triangle_index].mesh_index[0];
        triangle_list[triangle_index].mesh_index[1] = triangle_ptr[triangle_index].mesh_index[1];
        triangle_list[triangle_index].mesh_index[2] = triangle_ptr[triangle_index].mesh_index[2];

        triangle_list[triangle_index].tex_coord_index[0] = triangle_ptr[triangle_index].tex_coord_index[0];
        triangle_list[triangle_index].tex_coord_index[1] = triangle_ptr[triangle_index].tex_coord_index[1];
        triangle_list[triangle_index].tex_coord_index[2] = triangle_ptr[triangle_index].tex_coord_index[2];
    }

    delete[] file_buffer;
    

    current_frame = 0;
    next_frame = 1;
    interpol = 0.0;

    return 0;
}

int MD2Model::renderFrame(int frame_index) {
    Vertex3f* vertex_list = &this->vertex_list[num_vertices * frame_index];

    glBindTexture(GL_TEXTURE_2D, texture_ID);

    glBegin(GL_TRIANGLES);
    {
        for (int triangle_index = 0; triangle_index < num_triangles; ++triangle_index) {
            CalculateNormal(vertex_list[triangle_list[triangle_index].mesh_index[0]].v,
                            vertex_list[triangle_list[triangle_index].mesh_index[1]].v,
                            vertex_list[triangle_list[triangle_index].mesh_index[2]].v);

            for (int point_index = 0; point_index < 3; ++point_index) {
                glTexCoord2f(tex_coord_list[triangle_list[triangle_index].tex_coord_index[point_index]].u,
                             tex_coord_list[triangle_list[triangle_index].tex_coord_index[point_index]].v);
                
                glVertex3fv(vertex_list[triangle_list[triangle_index].mesh_index[point_index]].v);
            }
        }
    }
    glEnd();
    return 0;
}

