#include "dsTools.h"

static const double viewMoveSpeed = 1.0;
static const GLdouble pi = 3.1415926;

// �۾�λ�ã��������� (r, phi, theta) ��ʾ
// ���У�phi ��ʾ�� z ��ļн�
// theta ��ʾ�� xy ƽ���ͶӰ����ת��
GLdouble eye_sphere[3] = {20.0, pi / 4, - pi / 2};

// ȫ��ʹ��
int window_width = 1280;
int window_height = 720;
int status_bar_width = 200;

DSString str;
dsTextManager dstext;

const char* font_file_name = "C:/Windows/Fonts/msyhbd.ttc";

// ����
// GLdouble center_sphere[3] = {20.0, 3 * pi / 4, - pi / 2};

GLdouble up[3] = {0.0, 0.0, 1.0};

// �������ĵ㣬�������ԭ��
GLdouble center[3] = {0.0, 0.0, 0.0};

// ���λ�ã���ֱ���޸ģ�ͨ���������Ӳ���
GLdouble eye[3];

GLdouble axeLength = eye_sphere[0] * 0.5;

void dsSet() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// ��������ת��Ϊֱ������
	dsSphereToOrtho3dv(eye_sphere, center, eye);
	
	// ��������ͷλ��
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
	
}

void dsSpecialKeys(int key, int x, int y)
{
	static GLdouble rotateSpeed = 0.1;
	static GLdouble scaleSpeed = 0.5;
	GLdouble direction[3];
	switch (key) {
	case GLUT_KEY_UP:
		if (eye_sphere[1] < 3.1415926 - rotateSpeed) {
			eye_sphere[1] += rotateSpeed;
		}
		break;
	case GLUT_KEY_DOWN:
		if (eye_sphere[1] > rotateSpeed) {
			eye_sphere[1] -= rotateSpeed;
		}
		break;
	case GLUT_KEY_LEFT:
		eye_sphere[2] += rotateSpeed;
		break;
	case GLUT_KEY_RIGHT:
		eye_sphere[2] -= rotateSpeed;
		break;
	case GLUT_KEY_F1:
		dsDiff3dv(center, eye, direction);
		dsNormalize3dv(direction);
		center[0] += direction[0] * viewMoveSpeed;
		center[1] += direction[1] * viewMoveSpeed;
		center[2] += direction[2] * viewMoveSpeed;
		break;
	case GLUT_KEY_F2:
		dsDiff3dv(center, eye, direction);
		dsNormalize3dv(direction);
		center[0] -= direction[0] * viewMoveSpeed;
		center[1] -= direction[1] * viewMoveSpeed;
		center[2] -= direction[2] * viewMoveSpeed;
	break;
	}
	axeLength = eye_sphere[0] * 0.5;
}



void dsKeys(unsigned char key, int x, int y){
	//printf_s("%f,%f,%f",eye[0],eye[1],eye[2]);
	vector2f dir = vector2f(center[0] - eye[0], center[1] - eye[1]);
	dir.normalLise();
	vector2f left = dir.turnLeft();
	switch (key) {
	case 'a': case 'A':
		center[0] += left.x * viewMoveSpeed;
		center[1] += left.y * viewMoveSpeed;
		break;
	case 'w': case 'W':
		center[0] += dir.x * viewMoveSpeed;
		center[1] += dir.y * viewMoveSpeed;
		break;
	case 'd': case 'D':
		center[0] -= left.x * viewMoveSpeed;
		center[1] -= left.y * viewMoveSpeed;
		break;
	case 's': case 'S':
		center[0] -= dir.x * viewMoveSpeed;
		center[1] -= dir.y * viewMoveSpeed;
		break;
	default:
		break;
	}
	
}

//Ч��������
//void dsPassiveMonitionFunc(int x,int y){
//	extern int width;
//	extern int height;
//	if(x<width/20){
//		center[0]-=viewMoveSpeed;
//	}else if(x>width*19/20){
//		center[0]+=viewMoveSpeed;
//	}
//}


// �����õĺ�������һ��ֱ������ϵ
void dsShowAxes() {
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPerspective(60,1,2,1000);
	glMatrixMode(GL_MODELVIEW);
	
	glViewport(0, 0, 100, 100);
	//glPushMatrix();
	//glLoadIdentity();
	glTranslated(center[0], center[1], center[2]);
	
	//glutSolidCube(1);
	glLineWidth(1.0);
	glEnable(GL_LINE_SMOOTH);
	glBegin(GL_LINES);
	{
		
		glColor3d(1.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(axeLength, 0.0, 0.0);
		
		glColor3d(0.0, 1.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, axeLength, 0.0);

		glColor3d(0.0, 0.0, 1.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, axeLength);
	}
	glEnd();
	//glPopMatrix();
	
	glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}

void fillRectange2D(int x, int y, int width, int length){
	int height = -10;
	glBegin(GL_QUADS);
		glVertex3d(x, y, height);
		glVertex3d(x + width, y, height);
		glVertex3d(x + width, y + length, height);
		glVertex3d(x, y + length, height);
	glEnd();
}

void drawRectange2D(int x, int y, int width, int length){
	int height = -10;
	glBegin(GL_LINE_LOOP);
	{
		glVertex3d(x, y, height);
		glVertex3d(x + width, y, height);
		glVertex3d(x + width, y + length, height);
		glVertex3d(x, y + length, height);
	}
	glEnd();
}
