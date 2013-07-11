#include <ft2build.h>
#include FT_FREETYPE_H

#include <wchar.h>
#include <map>
#include <iostream>

#include <GL/glut.h>
using namespace std;

class DSString
{
public:
	DSString();
	~DSString();
	int resize(int size);
	int init();
	void drawString(wchar_t* str, int size);
	const FT_GlyphSlot& getChar(wchar_t Char);
	unsigned long Hash(wchar_t * key);
private:
	FT_Library library; 
	FT_Face face;
	//下面两个看作临时变量
	FT_Error ftError;
	FT_UInt  glyph_index;
	//字体大小
	int size;
	//存放位图的Map
	map<int, FT_GlyphSlot> bitmaps;
	map<unsigned long,GLint> lists;
};

//
//void draw_bitmap( FT_Bitmap*  bitmap,
//	FT_Int      x,
//	FT_Int      y)
//{
//	FT_Int  i, j, p, q;
//	FT_Int  x_max = x + bitmap->width;
//	FT_Int  y_max = y + bitmap->rows;
//
//	for(int i=0;i<HEIGHT;i++){
//		for(int j=0;j<WIDTH;j++){
//			image[i][j]=0;
//		}
//	}
//
//	for ( i = x, p = 0; i < x_max; i++, p++ )
//	{
//		for ( j = y, q = 0; j < y_max; j++, q++ )
//		{
//			if ( i < 0      || j < 0       ||
//				i >= WIDTH || j >= HEIGHT )
//				continue;
//
//			image[j][i] |= bitmap->buffer[q * bitmap->width + p];
//		}
//	}
//}
//void show_image( void )
//{
//	//unsigned char image[HEIGHT][WIDTH];
//	int  i, j;
//	for ( i = 0; i < HEIGHT; i++ )
//	{
//		for ( j = 0; j < WIDTH; j++ ){
//			putchar( image[i][j] == 0 ? '  '
//			: image[i][j] < 128 ? '+'
//			: '*' );
//		}
//		putchar( '\n' );
//	}
//}
//
//void drawString(FT_Face face,wchar_t* str){
//	for(int i=0;i<wcslen(str);i++){
//		glyph_index = FT_Get_Char_Index( face, str[i] ); 
//
//		//cout<<"Index of "<<str[0]<< " is "<<glyph_index<<endl;
//
//		ftError = FT_Load_Glyph( 
//			face, /* face对象的句柄 */ 
//			glyph_index, /* 字形索引 */ 
//			0 ); /* 装载标志，参考下面 */ 
//
//		if(face->glyph->format!=ft_glyph_format_bitmap){
//			fterror = ft_render_glyph( face->glyph, /* 字形槽 */ 
//				ft_render_mode_normal ); /* 渲染模式 */ 
//		}
////
//		FT_Bitmap bitmap=face->glyph->bitmap;
//			
//		draw_bitmap(&bitmap,face->glyph->bitmap_left,HEIGHT-face->glyph->bitmap_top-3);
//		show_image();
//	}
//}




