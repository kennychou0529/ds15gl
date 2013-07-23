#include "dsTexture.h"
GLuint dsLoadTextureBMP2D(const char* file_name, GLuint* pheight, GLuint* pwidth) {
	const int BMP_Header_Length = 54;

	GLint width, height, total_bytes;
	GLubyte* pixels = nullptr;
	
	// 打开文件
	ifstream is;
	is.open(file_name, is.in | is.binary);
	if (!is) {
		return 0;
	}

	// 读取图像的宽度和高度
	is.seekg(0x0012, is.beg);
	is.read((char*)&width, sizeof(width));

	is.seekg(0x0016, is.beg);
	is.read((char*)&height, sizeof(height));

	is.seekg(BMP_Header_Length, is.beg);

	// 计算每行像素所占字节数，并根据此数据计算总像素字节数
	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}

	// 根据总像素字节数分配内存
	pixels = new GLubyte[total_bytes];
	if (pixels == nullptr) {
		is.close();
		return 0;
	}
	
	// 读取纹理图片数据
	if (is.read((char*)pixels, total_bytes) <= 0) {
		delete[] pixels;
		is.close();
		return 0;
	}
	is.close();

	// 在这里我们不对纹理进行缩放

	// 取得新的纹理编号
	GLuint texture_ID = 0;
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0) {
		delete[] pixels;
		is.close();
		return 0;
	}

	// 绑定新的纹理，载入纹理并设置纹理参数
	
	// 保存原来绑定的纹理编号
	GLuint last_texture_ID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&last_texture_ID);

	// 绑定新的纹理，以便进行操作
	glBindTexture(GL_TEXTURE_2D, texture_ID);

	// 如果大纹理放到小形状，则线性插值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// 如果小纹理放到大形状时，则线性插值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 如果纹理 x 坐标超出范围，则重复，默认值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	// 如果纹理 y 坐标超出范围，则重复，默认值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// 纹理和光照共存
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// 载入纹理
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
				 GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

	// 再把当前纹理绑定回原来的那个，当然新的纹理已经被储存起来了
	glBindTexture(GL_TEXTURE_2D, last_texture_ID);

	delete[] pixels;

	if (pwidth != nullptr) {
		*pwidth = width;
	}
	if (pheight != nullptr) {
		*pheight = height;
	}

	return texture_ID;
}

// 载入一张图片作为纹理，调用 aux 库
//bool LoadT8(char *filename, GLuint &texture)
//{	
//	AUX_RGBImageRec* pImage = nullptr;
//	pImage = auxDIBImageLoad(filename);
//	if (pImage == nullptr)
//		return false;
//	glGenTextures(1, &texture);	
//	glBindTexture    (GL_TEXTURE_2D, texture);
//	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, pImage->sizeX, 
//				  pImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);
//	free(pImage->data);
//	free(pImage);
//	return true;
//}
