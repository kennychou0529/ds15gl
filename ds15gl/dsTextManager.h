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

// һ�� dsTextManager ֻ����һ�����壬һ�ִ�С
class dsTextManager {
public:
	~dsTextManager() { clean(); }
	void init(const char* font_file_name, unsigned int height);
	void clean();

	// ��ʾһ���ַ�����֧�ֶ��У����� x, y �ֱ��ʾ [�ַ����ĵ�һ�е����½�] ��λ��
	// (x, y) ��һ���������꣬����ʾ [�õ������[��ǰ�ӿڵ����½�]] ��λ��
	// ���磬�� (x, y) == (0, 0) ���ַ����ĵ�һ�н����ӿ����½���ʾ
	// �������������˵��x, y �ƺ����Թ涨Ϊ����
	// ��ʹ�øú���ǰ��Ӧ������ ModelView �� glLoadIdentity();
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