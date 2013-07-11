#include "dsLight.h"

// �����⣬�ܱ�֤��Դ�Ҳ����ĵط�Ҳ���õ�����������õð�һЩ��������ɰ����İ�ɫ
GLfloat light_ambient[4]  = {0.1f, 0.1f, 0.1f, 1.0f};

// �����
GLfloat light_diffuse[4]  = {0.5f, 250.0f / 255 / 2, 200.0f / 255 / 2, 0.5f};

// ����⣬��������
GLfloat light_specular[4] = {0.5f, 250.0f / 255 / 2, 200.0f / 255 / 2, 2.0f};
// GLfloat specular[4] = {1.0f, 1.0f, 0.0f, 1.0f};
// GLfloat specular[4] = {0.0f, 0.0f, 0.0f, 1.0f};

// ��Դλ��
GLfloat light_position[4] = {5.0f, 0.0f, 5.0f, 1.0f};
// GLfloat position[4] = {5.0f, 0.0f, 5.0f, 1.0f};

void dsSetLight() {
	// ������Դ
	glEnable(GL_LIGHT0);
	// �����������Դ 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	

	

	//// �۹��
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0f);
	//GLfloat spot_direction[] = {- light_position[0], - light_position[1], - light_position[2]};
	//// GLfloat spot_direction[] = {0.0, 0.0, -1};
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);

}

void dsSetMaterial() {
	// ������
	GLfloat ambient[4]  = {1.0f, 1.0f, 1.0f, 10.0f};

	// �����
	GLfloat diffuse[4]  = {1.0f, 1.0f, 1.0f, 10.0f};

	// ����⣬��������
	GLfloat specular[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	
	GLfloat shinness = 5.0;
	
	// �������������
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shinness);

}

void dsShowLightSource() {
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	/*glBegin(GL_POINTS);
	{
		glPointSize(5.0f);
		glVertex3fv(position);
	}
	glEnd();*/
	glPushMatrix();
	glColor3d(1.0, 1.0, 1.0);
	glTranslated(light_position[0], light_position[1], light_position[2]);

	glutSolidSphere(0.5, 20, 20);
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}
