//#include "dsString.h"
//#include <cmath>
//
//#ifdef WIN32
////宋体
//#define SONGTI "C:/Windows/fonts/msyhbd.ttc"
//#endif // WIN32
//
//#ifdef __unix__
//#define SONGTI "/usr/share/fonts/truetype/wqy/wqy-microhei.ttc"
//#define printf_s printf
//#endif
//
//#define FONT GLUT_BITMAP_HELVETICA_18
//
//const int MAX = (int)std::pow(2, sizeof(unsigned long));
//
//unsigned long DSString::Hash(wchar_t* key) {
//	unsigned long h = 0;
//	while (*key) {
//		h = (h << 4) + *key++;
//		unsigned long g = h & 0xf0000000L;
//		if (g) {
//			h ^= g >> 24;
//		}
//		h &= ~g;
//	}
//	return h % MAX;
//}
//
//DSString::DSString() {
//	size = 16;
//	init();
//}
//
//DSString::~DSString() {
//	for (auto it = bitmaps.begin(); it != bitmaps.end(); ++it){
//		delete[] it->second->bitmap.buffer;
//		delete it->second;
//	}
//	bitmaps.clear();
//
//	for (auto it = lists.begin(); it != lists.end(); ++it) {
//		if (glIsList(it->second)) {
//			glDeleteLists(it->second, 1);
//		}
//	}
//	lists.clear();
//};
//
//int DSString::resize(int size) {
//	ftError = FT_Set_Pixel_Sizes(face, 0, size);
//
//	if (ftError) {
//		printf_s("Set size Error.\n");
//		return 1;
//	}
//	return 0;
//}
//
//void DSString::drawString(wchar_t* str, int size) {
//
//	map<unsigned long, GLint>::const_iterator temp = lists.find(Hash(str));
//
//	//如果画过这句话
//	if (temp != lists.end()) {
//		glCallList(temp->second);
//		return;
//	}
//	GLint newList = glGenLists(1);
//	glNewList(newList, GL_COMPILE_AND_EXECUTE);
//	{
//		size_t len = wcslen(str);
//		if (size != this->size) {
//			if (resize(size))
//				cerr << "resize error" << endl;
//			this->size = size;
//		}
//
//		for (size_t i = 0; i < len; ++i) {
//			if ((str[i] >> 8) == 0) {
//				//英文
//				glutBitmapCharacter(FONT, str[i]);
//				continue;
//			}
//			const FT_GlyphSlot& slot=getChar(str[i]);
//			FT_Bitmap bitmap = slot->bitmap;
//			glBitmap(bitmap.width, bitmap.rows, (GLfloat)slot->bitmap_left, (GLfloat)slot->bitmap_top, bitmap.width * 1.2f, 0, bitmap.buffer);
//		}
//	}
//	glEndList();
//	glCallList(newList);
//	lists[Hash(str)] = newList;
//}
//
//const FT_GlyphSlot DSString::getChar(wchar_t Char) {
//	//map<int, FT_GlyphSlot*>::const_iterator temp=;
//	if (bitmaps.find((Char << 8) + size) == bitmaps.end()){
//		glyph_index = FT_Get_Char_Index(face, Char);
//		ftError = FT_Load_Glyph(face, glyph_index, 0);
//		if (ftError) {
//			cerr << "Load Glyph Error.\n";
//		}
//
//		ftError = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);
//
//		bitmaps[(Char << 8) + size] = copy(face->glyph);
//		return face->glyph;
//	} else {
//		return bitmaps[(Char << 8) + size];
//	}
//}
//
//int DSString::init() {
//	ftError = FT_Init_FreeType(&library);
//	if (ftError) {
//		printf_s("Error when initialize a library.\n");
//		return 1;
//	}
//
//	ftError = FT_New_Face(library, SONGTI, 0, &face);
//	if (ftError == FT_Err_Unknown_File_Format){
//		printf_s("Unsupport Font File.\n");
//		return 1;
//	} else if (ftError) {
//		printf_s("Cannot open the file.\n");
//		return 1;
//	}
//
//	printf_s("Total Glyphs: %d\n", face->num_glyphs);
//
//	ftError = FT_Set_Pixel_Sizes(face, 0, size);
//	if (ftError) {
//		printf_s("Set size Error.\n");
//	}
//
//	FT_Matrix matrix; /* 变换矩阵 */
//	FT_Vector pen;
//
//	matrix.xx = (FT_Fixed)0x10000L;
//	matrix.xy = 0;
//	matrix.yx = 0;
//	matrix.yy = (FT_Fixed)-0x10000L;
//
//	pen.x = 0;
//	pen.y = 0;
//
//	FT_Set_Transform(face, &matrix, &pen);
//	printf_s("DSString init Success.\n");
//
//	return 0;
//}
//
//FT_GlyphSlot DSString::copy(FT_GlyphSlot slot) {
//	//拷贝源字形
//	FT_GlyphSlot newSlot = new FT_GlyphSlotRec(*slot);
//
//	int len = slot->bitmap.pitch * slot->bitmap.rows;
//	newSlot->bitmap.buffer = new unsigned char[len];
//	for (int i = 0 ; i < len; i++) {
//		newSlot->bitmap.buffer[i] = slot->bitmap.buffer[i];
//	}
//	return newSlot;
//}
