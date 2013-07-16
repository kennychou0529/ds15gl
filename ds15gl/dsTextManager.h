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

	// ��ʼ�����ֹ������������Ǳ�������һ����
	void init(const char* font_file_name, unsigned int height);

	void clean();

	/**************************************************************************
	 * <Function>--------------------------------------------------------------
	 * dsTextManager::print
	 *
	 * <Description>-----------------------------------------------------------
	 * ��ʾһ���ַ�����֧�ֶ��У����� x, y ָ�� [�ַ����ĵ�һ�е����½�] ��λ�ã�
	 * (x, y) ��һ���������꣬����ʾ [�õ������[��ǰ�ӿڵ����½�]] ��λ�á�
	 * ���磬�� (x, y) == (0, 0) ���ַ����ĵ�һ�н����ӿ����½���ʾ��
	 * ���������������˵��x, y �ƺ����Թ涨Ϊ������
	 * ��ʹ�øú���ǰ��Ӧ������ ModelView �� glLoadIdentity(); ��������ܵĴ���
	 * 
	 * <Example>---------------------------------------------------------------
	 * ...
	 * // �������ֹ���������
	 * dsTextManager mytext;
	 * ...
	 * // ��ʼ����΢���ź����壬��С 16��
	 * mytext.init("C:/Windows/Fonts/msyh.ttc", 16); 
	 * ...
	 * // �ڵ�ǰ�ӿ����½����� 200 ���أ����� 300 ���أ���ʾ���֡�
	 * glPushMatrix();
	 * glLoadIdentity();
	 * mytext.print(200, 300, L"Hello!\n���ã�");
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