#ifndef _DSTEXTMANAGER_H
#define _DSTEXTMANAGER_H

// FreeType2 Headers
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

// STL Headers

#include <string>
#include <map>


// GLUT Header
#include <GL/glut.h>

// 一个 dsTextManager 只能用一种字体，一种大小
class dsTextManager {
public:
	~dsTextManager() { clean(); }

	// 初始化文字管理器对象，这是必须做的一步。
	void init(const char* font_file_name, unsigned int height);

	void clean();

	/**************************************************************************
	 * <Function>--------------------------------------------------------------
	 * dsTextManager::print
	 *
	 * <Description>-----------------------------------------------------------
	 * 显示一个字符串，支持多行，参数 x, y 指定 [字符串的第一行的左下角] 的位置；
	 * (x, y) 是一个像素坐标，它表示 [该点相对于[当前视口的左下角]] 的位置。
	 * 例如，若 (x, y) == (0, 0) 则字符串的第一行紧贴视口左下角显示。
	 * （从这个意义上来说，x, y 似乎可以规定为整数）
	 * 在使用该函数前，应该先在 ModelView 中 glLoadIdentity(); 来避免可能的错误。
	 * 
	 * <Example>---------------------------------------------------------------
	 * ...
	 * // 创建文字管理器对象。
	 * dsTextManager mytext;
	 * ...
	 * // 初始化：微软雅黑字体，大小 16。
	 * mytext.init("C:/Windows/Fonts/msyh.ttc", 16); 
	 * ...
	 * // 在当前视口左下角以右 200 像素，以上 300 像素，显示文字。
	 * glPushMatrix();
	 * glLoadIdentity();
	 * mytext.print(200, 300, L"Hello!\n您好！");
	 * glPopMatrix();
	 * ...
	 *                                                                       */
	void print(GLfloat x, GLfloat y, const std::wstring& str);

private:
	FT_Library library;
	FT_Face face;
	GLfloat font_height;
	void makeList(wchar_t ch);

	std::map<wchar_t, GLuint> textures;
	std::map<wchar_t, GLuint> lists;
};

#endif