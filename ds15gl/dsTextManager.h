#ifndef _DSTEXTMANAGER_H
#define _DSTEXTMANAGER_H

// FreeType2 Headers
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

// STL Headers
#include <vector>
#include <string>
#include <map>
#include <utility>

// GLUT Header
#include <GL/glut.h>

// 一个 dsTextManager 只能用一种字体，一种大小
class dsTextManager {
public:
	~dsTextManager() { clean(); }
	void init(const char* font_file_name, unsigned int height);
	void clean();

	// 显示一个字符串，支持多行，参数 x, y 分别表示 [字符串的第一行的左下角] 的位置
	// (x, y) 是一个像素坐标，它表示 [该点相对于[当前视口的左下角]] 的位置
	// 例如，若 (x, y) == (0, 0) 则字符串的第一行紧贴视口左下角显示
	// 从这个意义上来说，x, y 似乎可以规定为整数
	// 在使用该函数前，应该先在 ModelView 中 glLoadIdentity();
	void print(GLfloat x, GLfloat y, const std::wstring& str);

private:
	FT_Library library;
	FT_Face face;
	float font_height;
	void makeList(wchar_t ch);

	std::map<wchar_t, GLuint> textures;
	std::map<wchar_t, GLuint> lists;
};

#endif