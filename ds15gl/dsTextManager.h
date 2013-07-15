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
	FT_Library library;
	FT_Face face;
	float font_height;
	std::map<wchar_t, GLuint> textures;
	std::map<wchar_t, GLuint> lists;

	void makeList(wchar_t ch);
	void init(const char* font_file_name, unsigned int height);
	void clean();
	void print(float x, float y, const std::wstring& str);
};

#endif