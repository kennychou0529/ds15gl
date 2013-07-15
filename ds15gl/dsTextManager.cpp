#include "dsTextManager.h"

inline int findNextPowerOf2(int a)
{
	int result = 1;
	while (result < a)
		result <<= 1;
	return result;
}

void dsTextManager::makeList(wchar_t ch) {
	// If the list already exists, then return.
	if (lists.find(ch) != lists.end()) {
		return;
	}

	//The first thing we do is get FreeType to render our character
	//into a bitmap.  This actually requires a couple of FreeType commands:

	//Load the Glyph for our character.
	if (FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT))
		throw std::runtime_error("FT_Load_Glyph failed");

	//Move the face's glyph into a Glyph object.
	FT_Glyph glyph;
	if (FT_Get_Glyph(face->glyph, &glyph))
		throw std::runtime_error("FT_Get_Glyph failed");

	//Convert the glyph to a bitmap.
	FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//This reference will make accessing the bitmap easier
	FT_Bitmap& bitmap = bitmap_glyph->bitmap;

	//Use our helper function to get the widths of
	//the bitmap data that we will need in order to create
	//our texture.
	int texture_width = findNextPowerOf2(bitmap.width);
	int texture_height = findNextPowerOf2(bitmap.rows);

	//Allocate memory for the texture data.
	GLubyte* expanded_data = new GLubyte[2 * texture_width * texture_height];

	//Here we fill in the data for the expanded bitmap.
	//Notice that we are using two channel bitmap (one for
	//luminocity and one for alpha), but we assign
	//both luminocity and alpha to the value that we
	//find in the FreeType bitmap. 
	//We use the ?: operator so that value which we use
	//will be 0 if we are in the padding zone, and whatever
	//is the the Freetype bitmap otherwise.
	for (int j = 0; j < texture_height; j++) {
		for (int i = 0; i < texture_width; i++) {
			expanded_data[2 * (i + j * texture_width)]
			= expanded_data[2 * (i + j * texture_width) + 1]
			= (i >= bitmap.width || j >= bitmap.rows) ?
				0 : bitmap.buffer[i + bitmap.width * j];
		}
	}
	
	GLuint new_texture;
	glGenTextures(1, &new_texture);
	textures.insert(std::make_pair(ch, new_texture));

	//Now we just setup some texture paramaters.
	glBindTexture(GL_TEXTURE_2D, new_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Here we actually create the texture itself, notice
	//that we are using GL_LUMINANCE_ALPHA to indicate that
	//we are using 2 channel data.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height,
		0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

	//With the texture created, we don't need to expanded data anymore
	delete[] expanded_data;

	GLuint new_list = glGenLists(1);
	lists.insert(std::make_pair(ch, new_list));
	//So now we can create the display list

	glNewList(new_list, GL_COMPILE);
	{
		glBindTexture(GL_TEXTURE_2D, new_texture);

		//first we need to move over a little so that
		//the character has the right amount of space
		//between it and the one before it.
		glTranslatef((GLfloat)bitmap_glyph->left, 0, 0);

		//Now we move down a little in the case that the
		//bitmap extends past the bottom of the line 
		//(this is only true for characters like 'g' or 'y'.
		glPushMatrix();
		glTranslatef(0.0f, (GLfloat)bitmap_glyph->top-bitmap.rows, 0);

		//Now we need to account for the fact that many of
		//our textures are filled with empty padding space.
		//We figure what portion of the texture is used by 
		//the actual character and store that information in 
		//the x and y variables, then when we draw the
		//quad, we will only reference the parts of the texture
		//that we contain the character itself.
		GLfloat x = (GLfloat)bitmap.width / (GLfloat)texture_width;
		GLfloat y = (GLfloat)bitmap.rows / (GLfloat)texture_height;

		//Here we draw the texturemaped quads.
		//The bitmap that we got from FreeType was not 
		//oriented quite like we would like it to be,
		//so we need to link the texture to the quad
		//so that the result will be properly aligned.
		glBegin(GL_QUADS);
		{
			glTexCoord2d(0, 0); glVertex2f(0, (GLfloat)bitmap.rows);
			glTexCoord2d(0, y); glVertex2f(0, 0);
			glTexCoord2d(x, y); glVertex2f((GLfloat)bitmap.width, 0);
			glTexCoord2d(x, 0); glVertex2f((GLfloat)bitmap.width, (GLfloat)bitmap.rows);
		}
		glEnd();
		glPopMatrix();
		glTranslatef((GLfloat)(face->glyph->advance.x >> 6), 0, 0);
	}
	glEndList();
}

void dsTextManager::init(const char* font_file_name, unsigned int height) {
	font_height = height;

	//Create and initilize a freetype font library.
	//FT_Library library;
	if (FT_Init_FreeType(&library)) 
		throw std::runtime_error("FT_Init_FreeType failed");

	//The object in which Freetype holds information on a given
	//font is called a "face".
	//FT_Face face;

	//This is where we load in the font information from the file.
	//Of all the places where the code might die, this is the most likely,
	//as FT_New_Face will die if the font file does not exist or is somehow broken.
	if (FT_New_Face(library, font_file_name, 0, &face)) 
		throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");

	// For some twisted reason, Freetype measures font size
	// in terms of 1/64ths of pixels.  Thus, to make a font
	// h pixels high, we need to request a size of h*64.
	// (h << 6 is just a prettier way of writting h*64)
	FT_Set_Char_Size(face, height << 6, height << 6, 96, 96);
}

void dsTextManager::clean() {
	// toto: clear all textures and lists

	//We don't need the face information now that the display
	//lists have been created, so we free the assosiated resources.
	FT_Done_Face(face);

	//Ditto for the library.
	FT_Done_FreeType(library);
}

/// A fairly straight forward function that pushes
/// a projection matrix that will make object world 
/// coordinates identical to window coordinates.
inline void pushScreenCoordinateMatrix() {
	glPushAttrib(GL_TRANSFORM_BIT);
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	// gluOrtho2D(viewport[0], viewport[2], viewport[1], viewport[3]);
	gluOrtho2D(0, viewport[2], 0, viewport[3]);
	glPopAttrib();
}

/// Pops the projection matrix without changing the current
/// MatrixMode.
inline void pop_projection_matrix() {
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

void dsTextManager::print(float x, float y, const std::wstring& str) {
	// We want a coordinate system where things coresponding to window pixels.
	pushScreenCoordinateMatrix();					

	float h = font_height / 0.63f;						//We make the height about 1.5* that of

	// 接下来是分行程序
	std::vector<std::wstring> lines;
	std::wstring::size_type start = 0;
	std::wstring::size_type nfind = 0;
	while ((nfind = str.find(L'\n', start)) != std::wstring::npos) {
		lines.push_back(str.substr(start, nfind - start));
		start = nfind + 1;
	}
	if (start < str.size()) {
		lines.push_back(str.substr(start, str.size() - start));
	}

	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);	
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	float modelview_matrix[16];	
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

	//This is where the text display actually happens.
	//For each line of text we reset the modelview matrix
	//so that the line's text will start in the correct position.
	//Notice that we need to reset the matrix, rather than just translating
	//down by h. This is because when each character is
	//draw it modifies the current matrix so that the next character
	//will be drawn immediatly after it.  
	for (int i = 0; i < lines.size(); i++) {
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(x, y - h * i, 0);
		glMultMatrixf(modelview_matrix);
		for (auto ch = lines[i].cbegin(); ch != lines[i].cend(); ++ch) {
			makeList(*ch);
			glCallList(lists[*ch]);
		}

		glPopMatrix();
	}

	glPopAttrib();		

	pop_projection_matrix();
}
