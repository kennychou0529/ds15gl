#include "dsText.h"


//static bool dsTextGenerated = false;
//static GLuint texts; // 显示列表
//static const GLuint MAX_CHAR = 128;
//
//void drawString(const char* str) {
//	if (!dsTextGenerated) { // 如果是第一次调用，执行初始化
//		// 为每一个ASCII字符产生一个显示列表
//		dsTextGenerated = true;
//
//		// 申请MAX_CHAR个连续的显示列表编号
//		texts = glGenLists(MAX_CHAR);
//
//		// 把每个字符的绘制命令都装到对应的显示列表中
//		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, texts);
//	}
//	// 调用每个字符对应的显示列表，绘制每个字符
//	for(; *str != '\0'; ++str)
//		glCallList(texts + *str);
//}

//直接用glut画，glutBitmapString是存在的，但不知道为什么不能用
//只能画英文字母
void drawString(const char* str){
	for(unsigned int i=0;i< strlen(str);i++){
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,str[i]);
	}
}