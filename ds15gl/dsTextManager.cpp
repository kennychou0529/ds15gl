#include "dsTextManager.h"
#include <utility>
#include <vector>
#include <stdexcept>

// 此函数返回一个不小于输入值的最小的 2 的次方
inline int findNextPowerOf2(int num) {
    int result = 1;

    while (result < num) {
        result <<= 1;
    }

    return result;
}

// 输入一个宽字符，为其创建显示列表
void dsTextManager::makeList(wchar_t ch) {
    // 如果显示列表已经存在，直接返回。
    if (lists.find(ch) != lists.end()) {
        return;
    }

    // 我们首先要做的就是让 FreeType 把字符画成一幅位图。这需要如下几步操作：
    // The first thing we do is get FreeType to render our character
    // into a bitmap.  This actually requires a couple of FreeType commands:

    // 载入字符的轮廓。
    // Load the Glyph for our character.
    if (FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT)) {
        throw std::runtime_error("FT_Load_Glyph failed");
    }

    // 将 face 中的轮廓载入到我们自己创建的一个轮廓对象。
    // Move the face's glyph into a Glyph object.
    FT_Glyph glyph;

    if (FT_Get_Glyph(face->glyph, &glyph)) {
        throw std::runtime_error("FT_Get_Glyph failed");
    }

    // 将轮廓转化为位图。
    // Convert the glyph to a bitmap.
    FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

    // 这个引用可以使我们更容易地访问位图。
    // This reference will make accessing the bitmap easier
    FT_Bitmap& bitmap = bitmap_glyph->bitmap;

    // 使用我们的辅助函数来获取纹理的大小
    // Use our helper function to get the widths of
    // the bitmap data that we will need in order to create
    // our texture.
    int texture_width = findNextPowerOf2(bitmap.width);
    int texture_height = findNextPowerOf2(bitmap.rows);

    // 为纹理数据分配内存空间。
    // Allocate memory for the texture data.
    GLubyte* expanded_data = new GLubyte[2 * texture_width * texture_height];

    // 现在我们填入扩展后的位图。
    // 注意，我们使用了双通道位图（一个 luminocity 值、一个 alpha 值），
    // 但我们把两个值都设为原 FreeType 位图中的那个值。
    // 我们使用 ? : 运算符来使扩展后的位图中那些超出 FreeType 位图范围
    // 的那些区域都为 0。
    // Here we fill in the data for the expanded bitmap.
    // Notice that we are using two channel bitmap (one for
    // luminocity and one for alpha), but we assign
    // both luminocity and alpha to the value that we
    // find in the FreeType bitmap.
    // We use the ?: operator so that value which we use
    // will be 0 if we are in the padding zone, and whatever
    // is the the Freetype bitmap otherwise.
    for (int j = 0; j < texture_height; j++) {
        for (int i = 0; i < texture_width; i++) {
            expanded_data[2 * (i + j * texture_width)] = 255;
            expanded_data[2 * (i + j * texture_width) + 1]
                = (i >= bitmap.width || j >= bitmap.rows) ?
                  0 : bitmap.buffer[i + bitmap.width * j];
            // NeHe 教程中的 Bug
        }
    }

    // 申请一个新纹理编号，并存入 map。
    GLuint new_texture;
    glGenTextures(1, &new_texture);
    textures.insert(std::make_pair(ch, new_texture));

    // 现在我们设置一下纹理参数。
    // Now we just setup some texture paramaters.
    glBindTexture(GL_TEXTURE_2D, new_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // 现在我们正式创建纹理。注意我们使用了 GL_LUMINANCE_ALPHA 来表明
    // 我们用的是双通道的位图数据。
    // Here we actually create the texture itself, notice
    // that we are using GL_LUMINANCE_ALPHA to indicate that
    // we are using 2 channel data.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height,
                 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

    // 纹理已经创建，我们就不需要扩展的位图数据了。
    // With the texture created, we don't need to expanded data anymore
    delete[] expanded_data;

    // 申请新的显示列表编号。
    GLuint new_list = glGenLists(1);
    lists.insert(std::make_pair(ch, new_list));

    // 接下来我们就可以创建显示列表了。
    // So now we can create the display list

    glNewList(new_list, GL_COMPILE);
    {
        glBindTexture(GL_TEXTURE_2D, new_texture);

        // 首先我们向右移动一点，这样字符可以和上一个有正确的距离。
        // first we need to move over a little so that
        // the character has the right amount of space
        // between it and the one before it.
        glTranslatef((GLfloat)bitmap_glyph->left, 0, 0);

        // 然后我们再向下移动一点，因为有些字符（这只作用于 g 或 y 这种字符）。
        // Now we move down a little in the case that the
        // bitmap extends past the bottom of the line
        // (this is only true for characters like 'g' or 'y'.
        glPushMatrix();
        glTranslatef(0, (GLfloat)bitmap_glyph->top - bitmap.rows, 0);

        // 接下来我们就要处理纹理中存在空白区域这个问题了。
        // 我们先算出纹理中真正画有字符的区域所占的比例，
        // 然后在画出的方块中只贴上这片区域。
        // Now we need to account for the fact that many of
        // our textures are filled with empty padding space.
        // We figure what portion of the texture is used by
        // the actual character and store that information in
        // the x and y variables, then when we draw the
        // quad, we will only reference the parts of the texture
        // that we contain the character itself.
        GLfloat x = (GLfloat)bitmap.width / (GLfloat)texture_width;
        GLfloat y = (GLfloat)bitmap.rows / (GLfloat)texture_height;

        // 现在我们来画带有纹理的方块。
        // 我们对纹理坐标的规定异乎寻常，这是因为 FreeType 画出的文字
        // 的方向与我们所需的不同。
        // Here we draw the texturemaped quads.
        // The bitmap that we got from FreeType was not
        // oriented quite like we would like it to be,
        // so we need to link the texture to the quad
        // so that the result will be properly aligned.
        glBegin(GL_QUADS);
        {
            glTexCoord2d(0, 0);
            glVertex2f(0, (GLfloat)bitmap.rows);
            glTexCoord2d(0, y);
            glVertex2f(0, 0);
            glTexCoord2d(x, y);
            glVertex2f((GLfloat)bitmap.width, 0);
            glTexCoord2d(x, 0);
            glVertex2f((GLfloat)bitmap.width, (GLfloat)bitmap.rows);
        }
        glEnd();
        glPopMatrix();
        glTranslatef((GLfloat)(face->glyph->advance.x >> 6), 0, 0);
    }
    glEndList();
    FT_Done_Glyph(glyph); // 我加的，不知道会不会出错。
}

// 初始化文字管理器对象，这是必须做的一步。
void dsTextManager::init(const char* font_file_name, unsigned int height) {
    font_height = (GLfloat)height;

    // 初始化 FreeType Library。
    // Initilize the freetype font library.
    if (FT_Init_FreeType(&library)) {
        throw std::runtime_error("FT_Init_FreeType failed");
    }

    // FreeType 中包含字体信息的对象叫做 face。
    // The object in which Freetype holds information on a given
    // font is called a "face".

    // 我们在这里从文件中载入字体信息。
    // 这一步是最容易报错的地方，因为字体文件可能不存在或无法读取。
    // This is where we load in the font information from the file.
    // Of all the places where the code might die, this is the most likely,
    // as FT_New_Face will die if the font file does not exist or is somehow broken.
    if (FT_New_Face(library, font_file_name, 0, &face)) {
        throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");
    }

    // For some twisted reason, Freetype measures font size
    // in terms of 1/64ths of pixels.  Thus, to make a font
    // h pixels high, we need to request a size of h * 64.
    // (h << 6 is just a prettier way of writting h * 64)
    FT_Set_Char_Size(face, height << 6, height << 6, 96, 96);
}

void dsTextManager::clean() {
    // 清除 map 中的所有 纹理 和 显示列表
    for (auto & texture_pair : textures) {
        glDeleteTextures(1, &(texture_pair.second));
    }

    textures.clear();

    for (auto & list_pair : lists) {
        glDeleteLists(list_pair.second, 1);
    }

    lists.clear();

    // We don't need the face information now that the display
    // lists have been created, so we free the assosiated resources.
    FT_Done_Face(face);

    // Ditto for the library.
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
    // 我历经千辛万苦修改了这行，它是关键的一行
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

void dsTextManager::print(GLfloat x, GLfloat y, const std::wstring& str) {
    // We want a coordinate system where things coresponding to window pixels.
    pushScreenCoordinateMatrix();

    // We make the height about 1.5 * that of font_height
    GLfloat h = font_height * 1.5f; // 0.63f;

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

    // 分行完成

    glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float modelview_matrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

    // This is where the text display actually happens.
    // For each line of text we reset the modelview matrix
    // so that the line's text will start in the correct position.
    // Notice that we need to reset the matrix, rather than just translating
    // down by h. This is because when each character is
    // draw it modifies the current matrix so that the next character
    // will be drawn immediatly after it.
    for (std::vector<std::wstring>::size_type i = 0; i < lines.size(); ++i) {
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(x, y - h * (GLfloat)i, 0);
        glMultMatrixf(modelview_matrix);

        for (auto& ch : lines[i]) {
            makeList(ch);
            glCallList(lists[ch]);
        }

        glPopMatrix();
    }

    glPopAttrib();
    pop_projection_matrix();
}
