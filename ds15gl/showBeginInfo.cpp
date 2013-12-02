#include "dsActorManager.h"
#include "display_basic.h"
#include "dsTools.h"

extern Game_Info info;
extern Round_End_Info end_info;
extern void pushScreenCoordinateMatrix();
extern void pop_projection_matrix();

void DSActorManager::showBeginInfo(float time, float totalTime) {

    float alpha = 1 - time / totalTime;

    pushScreenCoordinateMatrix();
    glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
    glLoadIdentity();

	//glTranslatef(0,0,-1);
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glBegin(GL_QUADS);
    glVertex2f(100 , 100);
    glVertex2f(100, 668);
    glVertex2f(724, 669);
    glVertex2f(724, 100);
    glEnd();

    
	std::wostringstream os;
	os << info.team_name[0].c_str();
	glColor4f(0.0f, 0.0f, 0.0f, alpha * 1.2);
    dstext.print(150, 150, os.str());

	os.str(L"");
	os << info.team_name[1].c_str();
	glColor4f(0.0f, 0.0f, 0.0f, alpha * 1.2);
	dstext.print(500, 500, os.str());

	os.str(L"");
	os << "VS";
	glColor4f(1.0f, 0.0f, 0.0f, alpha * 1.2);
	dstext.print(330, 330, os.str());

    glPopMatrix();
    glPopAttrib();
    pop_projection_matrix();
}

void DSActorManager::showEndInfo(float time, float totalTime) {

	float alpha =  time / totalTime;

	pushScreenCoordinateMatrix();
	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	glLoadIdentity();

	//glTranslatef(0,0,-1);
	glColor4f(1.0f, 1.0f, 1.0f, alpha);
	glBegin(GL_QUADS);
	glVertex2f(100 , 100);
	glVertex2f(100, 668);
	glVertex2f(724, 669);
	glVertex2f(724, 100);
	glEnd();

	glColor4f(0.0f, 0.0f, 0.0f, alpha * 1.5);
	//dstext.print(x  - width / 2 + 20.0f, y + 13.0f, message);

	std::wostringstream os;
	os << info.team_name[0].c_str()<<"\n\n"<<end_info.score[0]<<"\n";
	glColor4f(0.0f, 0.0f, 0.0f, alpha * 1.2);
	dstext.print(150, 600, os.str());



	os.str(L"");
	os << info.team_name[1].c_str()<<"\n\n"<<end_info.score[1]<<"\n";
	glColor4f(0.0f, 0.0f, 0.0f, alpha * 1.2);
	dstext.print(500, 600, os.str());

		

	glPopMatrix();
	glPopAttrib();
	pop_projection_matrix();
}
