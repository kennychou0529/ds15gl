#ifndef _DSTEXTURE_H
#define _DSTEXTURE_H

#include <GL/glut.h>
#include <fstream>

using std::ifstream;

// 载入一个 24bit BMP 图片为纹理，返回纹理编号，如果不成功，返回 0
// 参数中传入了两个无符号整数指针，默认为空指针，若非空，将在其中记录图片的尺寸
GLuint dsLoadTextureBMP2D(const char* file_name, GLuint* pheight = nullptr, GLuint* pwidth = nullptr);

// 载入一张图片作为纹理，调用 aux 库
bool LoadT8(char *filename, GLuint &texture);

#endif