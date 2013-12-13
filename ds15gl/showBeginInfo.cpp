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
    //glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
    glLoadIdentity();
    glColor4d(1.0, 1.0, 1.0, alpha);
	
	glDisable(GL_LIGHTING);
    if (beginBmp) {
        glEnable(GL_TEXTURE_2D);
		//glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, beginBmp);
    } else {
        glColor4f(0.7f, 0.7f, 0.7f, alpha);
    }
	
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0);
    glVertex2f(100 , 100);
    glTexCoord2d(0.0, 1.0);
    glVertex2f(100, 668);
    glTexCoord2d(1.0, 1.0);
    glVertex2f(724, 669);
    glTexCoord2d(1.0, 0.0);
    glVertex2f(724, 100);
    glEnd();
    //glDisable(GL_TEXTURE_2D);
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
    //glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
    glLoadIdentity();


    glDisable(GL_LIGHTING);
    if (endBmp) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, endBmp);
    } else {
        glColor4f(0.7f, 0.7f, 0.7f, alpha);
    }
    glBegin(GL_QUADS);
    glVertex2f(100 , 100);
    glVertex2f(100, 668);
    glVertex2f(724, 669);
    glVertex2f(724, 100);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glColor4f(0.0f, 0.0f, 0.0f, alpha * 1.5);
    //dstext.print(x  - width / 2 + 20.0f, y + 13.0f, message);

    std::wostringstream os;
    if (end_info.score[0] > end_info.score[1]) {
        os << info.team_name[0].c_str() << "\n\n" << end_info.score[0] << "\n\n";

        glColor4f(1.0f, 0.0f, 0.0f, alpha * 1.2);
        dstext.print(250, 600, os.str());

        os.str(L"");
        os << info.team_name[1].c_str() << "\n\n" << end_info.score[1] << "\n";

		glColor4f(0.0f, 0.0f, 0.0f, alpha * 1.2);
		dstext.print(250, 400, os.str());

    } else if(end_info.score[0] < end_info.score[1]){
		os << info.team_name[1].c_str() << "\n\n" << end_info.score[1] << "\n\n";

		glColor4f(1.0f, 0.0f, 0.0f, alpha * 1.2);
		dstext.print(250, 600, os.str());

		os.str(L"");
		os << info.team_name[0].c_str() << "\n\n" << end_info.score[0] << "\n";

		glColor4f(0.0f, 0.0f, 0.0f, alpha * 1.2);
		dstext.print(250, 400, os.str());

    }else{
		os << info.team_name[1].c_str() << "\n\n" << end_info.score[1] << "\n\n";

		os << info.team_name[0].c_str() << "\n\n" << end_info.score[0] << "\n";

		glColor4f(0.0f, 0.0f, 0.0f, alpha * 1.2);
		dstext.print(250, 600, os.str());
	}

    glPopMatrix();
    glPopAttrib();
    pop_projection_matrix();
}
