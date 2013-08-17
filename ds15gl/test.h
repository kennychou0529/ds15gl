#include <ft2build.h>
#include FT_FREETYPE_H

#include <wchar.h>
#include <map>
#include <iostream>

#include <GL/glut.h>
using namespace std;

const int MAX=pow(2,sizeof(unsigned long));
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

unsigned long DSString::Hash(wchar_t * key)
{
	unsigned long h = 0;
	while(*key)
	{
		h = (h << 4) + *key++;
		unsigned long g = h & 0xf0000000L;
		if(g)
			h ^= g >> 24;
		h &= ~g;
	}
	return h % MAX;
}

DSString::DSString()
{
	size=16;
	init();
}

DSString::~DSString()
{
	bitmaps.clear();
};

int DSString::resize(int size){

	ftError = FT_Set_Pixel_Sizes(face, 0,size);

	if (ftError)	{
		printf_s("Set size Error.\n");
		return 1;
	}
	return 0;
}

void DSString::drawString(wchar_t* str,int size){
	
	map<unsigned long, GLint>::const_iterator temp=lists.find(Hash(str));
	
	if (temp!=lists.end())
	{
		glCallList(temp->second);
		return;
	}
	GLint newList=glGenLists(1);
	glNewList(newList,GL_COMPILE_AND_EXECUTE);{
		int len=wcslen(str);
		if(size!=this->size){
			if(resize(size))
				cerr<<"resize error"<<endl;
			this->size=size;
		}
		int x=0;
		for (int i=0;i<len;i++)
		{
			const FT_GlyphSlot& slot=getChar(str[i]);
			FT_Bitmap bitmap = slot->bitmap;
			glBitmap(bitmap.width,bitmap.rows,0,0,bitmap.width * 1.2f,0,bitmap.buffer);
		}
	}
	glEndList();
	glCallList(newList);
	lists[Hash(str)]=newList;
}

const FT_GlyphSlot& DSString::getChar(wchar_t Char){
	//map<int, FT_GlyphSlot*>::const_iterator temp=;
	if(bitmaps.find((Char<<8)+size)==bitmaps.end()){
		glyph_index = FT_Get_Char_Index( face, Char);
		ftError = FT_Load_Glyph( 
				face, /* face对象的句柄 */ 
				glyph_index, /* 字形索引 */ 
				0 ); /* 装载标志，参考下面 */ 
		if(ftError){
			cerr<<"Load Glyph Error.\n";
		}
	
		ftError = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);

		bitmaps[(Char<<8)+size] = face->glyph;
		return face->glyph;
	}else{
		return bitmaps[(Char<<8)+size];
	}
}

int DSString::init(){
	ftError=FT_Init_FreeType(&library);
	if(ftError){
		printf_s("Error when initialize a library.\n");
		return 1;
	}

	ftError = FT_New_Face( library, "STSONG.TTF", 0, &face ); 
	if(ftError==FT_Err_Unknown_File_Format){
		printf_s("Unsupport Font File.\n");
		return 1;
	}else if(ftError)	{
		printf_s("Cannot open the file.\n");
		return 1;
	}

	printf_s("Total Glyphs: %d\n",face->num_glyphs);

	ftError = FT_Set_Pixel_Sizes( 
		face, /* face对象的句柄 */ 0,size);

	if (ftError)	{
		printf_s("Set size Error.\n");
	}

	printf_s("DSString init Success.\n");
	
	return 0;

}
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




//int main(){
//	DSString dsString;
//	dsString.drawString(L"ABCS 王康",32);
//}