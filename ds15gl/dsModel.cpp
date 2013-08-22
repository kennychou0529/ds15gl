#include "dsModel.h"
#include "dsTexture.h"
#include <fstream>
#include <cstring>
#include "dsVector.h"
#include <iostream>

void CalculateNormal(const GLfloat* p1, const GLfloat* p2, const GLfloat* p3) {
    // 把教程中的代码换成了我的
    //float a[3], b[3], result[3];
    //float length;

    //a[0] = p1[0] - p2[0];
    //a[1] = p1[1] - p2[1];
    //a[2] = p1[2] - p2[2];

    //b[0] = p1[0] - p3[0];
    //b[1] = p1[1] - p3[1];
    //b[2] = p1[2] - p3[2];

    //result[0] = a[1] * b[2] - b[1] * a[2];
    //result[1] = b[0] * a[2] - a[0] * b[2];
    //result[2] = a[0] * b[1] - b[0] * a[1];

    //// calculate the length of the normal
    //length = (float)sqrt(result[0] * result[0] + result[1] * result[1] + result[2] * result[2]);

    //// normalize and specify the normal
    //glNormal3f(result[0] / length, result[1] / length, result[2] / length);

    GLfloat normal_vector[3];
    dsNormalVectorOfTriangle3fv(p1, p2, p3, normal_vector);
    glNormal3fv(normal_vector);
}

MD2Model::MD2Model():
    num_frames(0),
    num_triangles(0),
    num_vertices(0),
    num_tex_coords(0),
    frame_size(0),
    triangles(nullptr),
    normal_vecs(nullptr),
    tex_coords(nullptr),
    vertices(nullptr) {}

MD2Model::~MD2Model() {}

int MD2Model::load(const char* model_file, const char* skin_file) {
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
    char* file_buffer = new char[file_length];
    is.read(file_buffer, file_length);
    is.close();

    // 接下来从 buffer 中获取 MD2 文件头
    MD2Header* model_header = (MD2Header*)file_buffer;

    std::cout << model_header->offset_eof << std::endl << file_length << std::endl;
    // 获取文件头中我们比较感兴趣的内容
    num_frames = model_header->num_frames;
    num_vertices = model_header->num_vertices;
    frame_size = model_header->frame_size;
    num_tex_coords = model_header->num_tex_coords;
    num_triangles = model_header->num_triangles;

    // 载入点数据
    // 文件头中提示，每帧都有 num_vertices 个顶点
    // 所以我们需要分配 num_vertices * num_frames 个点
    vertices = new Vertex3f[num_vertices * num_frames];
    Vertex3f* vertex_list_ptr;
    MD2Frame* frame_ptr;

    for (size_t frame_index = 0; frame_index < num_frames; ++frame_index) {
        frame_ptr = (MD2Frame*)&file_buffer[model_header->offset_frames + frame_size * frame_index];
        vertex_list_ptr = (Vertex3f*)&vertices[num_vertices * frame_index];

        for (size_t vertex_index = 0; vertex_index < num_vertices; ++ vertex_index) {
            vertex_list_ptr[vertex_index].v[0] = frame_ptr->vertices[vertex_index].v[0] * frame_ptr->scale[0] + frame_ptr->translate[0];
            vertex_list_ptr[vertex_index].v[1] = frame_ptr->vertices[vertex_index].v[1] * frame_ptr->scale[1] + frame_ptr->translate[1];
            vertex_list_ptr[vertex_index].v[2] = frame_ptr->vertices[vertex_index].v[2] * frame_ptr->scale[2] + frame_ptr->translate[2];
        }
    }

    // 载入纹理文件，获取纹理编号 texture_ID
    // 这里使用了 dsTexture 中的函数
    unsigned int texture_height, texture_width;
    texture_ID = dsLoadTextureBMP2D(skin_file, &texture_height, &texture_width);

    // 载入纹理坐标
    tex_coords = new TexCoord2f[num_tex_coords];
    MD2TexCoord2s* tex_coord_2s_ptr = (MD2TexCoord2s*)(&file_buffer[model_header->offset_tex_coord]);

    // MD2 文件中的纹理坐标都是像素值，所以我们要用它除以纹理图片的宽高值
    for (size_t tex_coord_index = 0; tex_coord_index < num_tex_coords; ++tex_coord_index) {
        tex_coords[tex_coord_index].u = (GLfloat)tex_coord_2s_ptr[tex_coord_index].u / (GLfloat)texture_width;
        tex_coords[tex_coord_index].v = (GLfloat)tex_coord_2s_ptr[tex_coord_index].v / (GLfloat)texture_height;
    }

    // 载入三角形
    triangles = new Mesh[num_triangles];
    Mesh* triangle_ptr = (Mesh*)&file_buffer[model_header->offset_triangles];

    // 一个 memcpy 替代了下面的循环 -- phisiart
    std::memcpy(triangles, triangle_ptr, sizeof(Mesh) * num_triangles);
    //for (int triangle_index = 0; triangle_index < num_triangles; ++triangle_index) {
    //    triangles[triangle_index].mesh_index[0] = triangle_ptr[triangle_index].mesh_index[0];
    //    triangles[triangle_index].mesh_index[1] = triangle_ptr[triangle_index].mesh_index[1];
    //    triangles[triangle_index].mesh_index[2] = triangle_ptr[triangle_index].mesh_index[2];

    //    triangles[triangle_index].tex_coord_index[0] = triangle_ptr[triangle_index].tex_coord_index[0];
    //    triangles[triangle_index].tex_coord_index[1] = triangle_ptr[triangle_index].tex_coord_index[1];
    //    triangles[triangle_index].tex_coord_index[2] = triangle_ptr[triangle_index].tex_coord_index[2];
    //}

    // 计算法向量 testing
    normal_vecs = new Vertex3f[num_triangles * num_frames];

    for (size_t frame_index = 0; frame_index < num_frames; ++frame_index) {
        for (size_t triangle_index = 0; triangle_index < num_triangles; ++triangle_index) {
            dsNormalVectorOfTriangle3fv(vertices[frame_index * num_vertices + triangles[triangle_index].mesh_index[0]].v,
                                        vertices[frame_index * num_vertices + triangles[triangle_index].mesh_index[1]].v,
                                        vertices[frame_index * num_vertices + triangles[triangle_index].mesh_index[2]].v,
                                        normal_vecs[frame_index * num_triangles + triangle_index].v);
        }
    }

    delete[] file_buffer;
    return 0;
}

void MD2Model::clear() {
    delete[] triangles;
    delete[] normal_vecs;
    delete[] tex_coords;
    delete[] vertices;
}

int MD2Model::renderFrame(size_t frame_index) {
    Vertex3f* vertex_base = &vertices[num_vertices * frame_index];
    Vertex3f* normal_vec_base = &normal_vecs[frame_index * num_triangles];
    glBindTexture(GL_TEXTURE_2D, texture_ID);

    glBegin(GL_TRIANGLES);
    {
        for (size_t triangle_index = 0; triangle_index < num_triangles; ++triangle_index) {
            // 使用储存下来的法向量，而不是临时计算
            glNormal3fv(normal_vec_base[triangle_index].v);
            /*CalculateNormal(vertex_base[triangles[triangle_index].mesh_index[0]].v,
                            vertex_base[triangles[triangle_index].mesh_index[1]].v,
                            vertex_base[triangles[triangle_index].mesh_index[2]].v);*/


            for (size_t point_index = 0; point_index < 3; ++point_index) {
                glTexCoord2f(tex_coords[triangles[triangle_index].tex_coord_index[point_index]].u,
                             tex_coords[triangles[triangle_index].tex_coord_index[point_index]].v);

                glVertex3fv(vertex_base[triangles[triangle_index].mesh_index[point_index]].v);
            }
        }
    }
    glEnd();
    return 0;
}

int MD2Model::renderSmoothly(size_t frame1_index, size_t frame2_index, GLfloat percentage) {
    Vertex3f* vertex_base1 = &vertices[num_vertices * frame1_index];
    Vertex3f* vertex_base2 = &vertices[num_vertices * frame2_index];
    glBindTexture(GL_TEXTURE_2D, texture_ID);

    Vertex3f points[3];
    glBegin(GL_TRIANGLES);
    {
        for (size_t triangle_index = 0; triangle_index < num_triangles; ++triangle_index) {
            // 线性计算点坐标，存在 points 中
            for (size_t point_index = 0; point_index < 3; ++point_index) {
                for (size_t axis_index = 0; axis_index < 3; ++axis_index) {
                    points[point_index].v[axis_index] = (1.0f - percentage) * vertex_base1[triangles[triangle_index].mesh_index[point_index]].v[axis_index]
                                                       + percentage * vertex_base2[triangles[triangle_index].mesh_index[point_index]].v[axis_index];
                }
            }

            // 计算法向量
            CalculateNormal(points[0].v, points[1].v, points[2].v);

            // 绘制三角形
            for (size_t point_index = 0; point_index < 3; ++point_index) {
                glTexCoord2f(tex_coords[triangles[triangle_index].tex_coord_index[point_index]].u,
                             tex_coords[triangles[triangle_index].tex_coord_index[point_index]].v);

                glVertex3fv(points[point_index].v);
            }

        }
    }
    glEnd();
    return 0;
}
