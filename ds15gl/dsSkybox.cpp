#include "dsSkybox.h"

void DSSkybox::del() {
	// 如果 显示列表 已存在，则删除之
	if (glIsList(skyBox)) {
		glDeleteLists(skyBox, 1);
	}

	// 清空原来的天空盒 纹理
	for (GLuint i = 0; i < 6; ++i) {
		if (glIsTexture(texture[i]))
			glDeleteTextures(1, texture + i);
	}
}

void DSSkybox::load(GLuint index) {

	// 原来可能已经存在天空盒，需要清空
	del();

	GLuint texture_height, texture_width;

	// 先载入纹理，将纹理编号存入 texture[]
	// 我搜集了几种天空盒
	switch (index) {
	case 0:
		// 精美绝伦花园天空盒
		texture[0] = dsLoadTextureBMP2D("data/images/skybox1/east.bmp", &texture_height, &texture_width);
		texture[1] = dsLoadTextureBMP2D("data/images/skybox1/west.bmp");
		texture[2] = dsLoadTextureBMP2D("data/images/skybox1/south.bmp");
		texture[3] = dsLoadTextureBMP2D("data/images/skybox1/north.bmp");
		texture[4] = dsLoadTextureBMP2D("data/images/skybox1/up.bmp");
		texture[5] = dsLoadTextureBMP2D("data/images/skybox1/down.bmp");
		extern GLdouble center[3];
		center[2] = 95.0;

		break;
	case 1:
		// 锦绣壮丽山川天空盒
		texture[0] = dsLoadTextureBMP2D("data/images/skybox0/lostvalley_east.bmp", &texture_height, &texture_width);
		texture[1] = dsLoadTextureBMP2D("data/images/skybox0/lostvalley_west.bmp");
		texture[2] = dsLoadTextureBMP2D("data/images/skybox0/lostvalley_south.bmp");
		texture[3] = dsLoadTextureBMP2D("data/images/skybox0/lostvalley_north.bmp");
		texture[4] = dsLoadTextureBMP2D("data/images/skybox0/lostvalley_up.bmp");
		texture[5] = dsLoadTextureBMP2D("data/images/skybox0/lostvalley_down.bmp");
		extern GLdouble center[3];
		center[2] = 70.0;

		break;
	case 2:
		// 水天交接天空盒
		texture[0] = dsLoadTextureBMP2D("data/images/skybox2/east.bmp", &texture_height, &texture_width);
		texture[1] = dsLoadTextureBMP2D("data/images/skybox2/west.bmp");
		texture[2] = dsLoadTextureBMP2D("data/images/skybox2/south.bmp");
		texture[3] = dsLoadTextureBMP2D("data/images/skybox2/north.bmp");
		texture[4] = dsLoadTextureBMP2D("data/images/skybox2/up.bmp");
		texture[5] = dsLoadTextureBMP2D("data/images/skybox2/down.bmp");
		extern GLdouble center[3];
		center[2] = 100.0;

		break;
	case 3:
		// 夕阳无限红霞天空盒
		texture[0] = dsLoadTextureBMP2D("data/images/skybox3/east.bmp", &texture_height, &texture_width);
		texture[1] = dsLoadTextureBMP2D("data/images/skybox3/west.bmp");
		texture[2] = dsLoadTextureBMP2D("data/images/skybox3/south.bmp");
		texture[3] = dsLoadTextureBMP2D("data/images/skybox3/north.bmp");
		texture[4] = dsLoadTextureBMP2D("data/images/skybox3/up.bmp");
		texture[5] = dsLoadTextureBMP2D("data/images/skybox3/down.bmp");
		extern GLdouble center[3];
		center[2] = 950.0;

		break;
	case 4:
		// 原来的天空盒
		texture[0] = dsLoadTextureBMP2D("data/images/skybox4/east.bmp", &texture_height, &texture_width);
		texture[1] = dsLoadTextureBMP2D("data/images/skybox4/west.bmp");
		texture[2] = dsLoadTextureBMP2D("data/images/skybox4/south.bmp");
		texture[3] = dsLoadTextureBMP2D("data/images/skybox4/north.bmp");
		texture[4] = dsLoadTextureBMP2D("data/images/skybox4/up.bmp");
		texture[5] = dsLoadTextureBMP2D("data/images/skybox4/down.bmp");
		extern GLdouble center[3];
		center[2] = 40.0;

		break;
	default:
		break;
	}

	// 再创建显示列表
	width = 2000;
	height = width / (GLdouble)texture_width * (GLdouble)texture_height;
	
	GLdouble x = width / 2;

	skyBox = glGenLists(1);

	glNewList(skyBox, GL_COMPILE_AND_EXECUTE);
	{
		glDisable(GL_LIGHTING);

		// 地面
		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glBegin(GL_QUADS);
		{
			glTexCoord2d(1, 0); glVertex3d(- x, - x, - 2);
			glTexCoord2d(1, 1); glVertex3d(x, - x, - 2);
			glTexCoord2d(0, 1); glVertex3d(x, x, - 2);
			glTexCoord2d(0, 0); glVertex3d(- x, x, - 2);
		}
		glEnd();

		// 东面
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glBegin(GL_QUADS);
		{
			glTexCoord2d(0, 0); glVertex3d(x, x, - 2);
			glTexCoord2d(0, 1); glVertex3d(x, x, height - 2);
			glTexCoord2d(1, 1); glVertex3d(x, - x, height - 2);
			glTexCoord2d(1, 0); glVertex3d(x, - x, - 2);
		}
		glEnd();

		// 西面
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glBegin(GL_QUADS);
		{
			glTexCoord2d(0, 0); glVertex3d(- x, - x, -2);
			glTexCoord2d(0, 1); glVertex3d(- x, - x, height - 2);
			glTexCoord2d(1, 1); glVertex3d(- x, x, height - 2);
			glTexCoord2d(1, 0); glVertex3d(- x, x, -2);
		}
		glEnd();

		// 南面
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glBegin(GL_QUADS);
		{
			glTexCoord2d(0, 0); glVertex3d(x, - x, -2);
			glTexCoord2d(0, 1); glVertex3d(x, - x, height - 2);
			glTexCoord2d(1, 1); glVertex3d(- x, - x, height - 2);
			glTexCoord2d(1, 0); glVertex3d(- x, - x, -2);		
		}
		glEnd();

		// 北面
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glBegin(GL_QUADS);
		{
			glTexCoord2d(0, 0); glVertex3d(- x, x, -2);
			glTexCoord2d(0, 1); glVertex3d(- x, x, height - 2);
			glTexCoord2d(1, 1); glVertex3d(x, x, height - 2);
			glTexCoord2d(1, 0); glVertex3d(x, x, -2);		
		}
		glEnd();

		// 顶面
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glBegin(GL_QUADS);
		{
			glTexCoord2d(1, 1); glVertex3d(- x, -x, height - 2);
			glTexCoord2d(1, 0); glVertex3d(x, -x, height - 2);
			glTexCoord2d(0, 0); glVertex3d(x, x, height - 2);
			glTexCoord2d(0, 1); glVertex3d(- x, x, height - 2);
		}
		glEnd();

		glEnable(GL_LIGHTING);

	}
	glEndList();
}

void DSSkybox::show() {
	glCallList(skyBox);
}