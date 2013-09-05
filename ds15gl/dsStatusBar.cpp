#include <sstream>
#include <iomanip>

#include "dsStatusBar.h"
#include "dsTools.h"
#include "dsTextManager.h"
#include "dsFrame.h"
#include "dsEye.h"

extern DSFrame frame;

DSStatusBar::DSStatusBar() {}

DSStatusBar::~DSStatusBar() {}

void DSStatusBar::init() {}

void DSStatusBar::show() {
    // 更改投影方式为 2D 平行投影
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    {
        glLoadIdentity();
        glOrtho(0, status_bar_width, 0, window_height, 0.01, 1000);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glViewport(window_width - status_bar_width, 0, status_bar_width, window_height);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);

        gluLookAt(0.0, 0.0, 200.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        glColor3d(1.0, 1.0, 1.0);

        //fillRectange2D(0, 0, status_bar_width, window_height);

        // 这里画小地图，战场，人物等状态信息
        // 例如
        glPushMatrix();
        {
            glTranslated(status_bar_width / 2, status_bar_width / 2, 0.0);
            glColor3f(1.0f, 1.0f, 1.0f);
            glutSolidTeapot(50);
        }
        glPopMatrix();

        // 让我们在这里写一下眼睛位置
        std::wostringstream os;
        os << "(" << std::fixed << std::setprecision(2) << eye[0] << ", " << eye[1] << ", " << eye[2] << ")";
        glPushMatrix();
        {
            glLoadIdentity();
            //glScaled(0.75, 0.75, 0.75);
            dstext_small.print(8.0, 8.0, os.str());
            dstext_small.print(40.0, 30.0, L"Eye position");
        }
        glPopMatrix();

        glPushMatrix();
        {
            glLoadIdentity();
            dstext.print(5, 400, L"操作面板");
        }
        glPopMatrix();

        //std::wstringstream os;
        
        glPushMatrix();
        {
            os.str(L"");
            os << L"FPS " << frame.getFPS();
            glLoadIdentity();
            glScalef(0.5, 0.5, 0.5);
            dstext.print(5, (GLfloat)window_height - 20, os.str());
        }
        glPopMatrix();

        glPushMatrix();
        {
            os.str(L"");
            os << L"Round " << frame.actors.getCurrentRound();
            if (frame.actors.round_finished) {
                os << L"\nF3: next round";
            }
            if (!frame.actors.script_finished) {
                os << L"\nplaying";
            }
            if (frame.actors.all_finished) {
                os << L"\nFinished";
            }

            glLoadIdentity();
            dstext_small.print(5, 220, os.str());
        }
        glPopMatrix();

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);

        // 回到透视投影
        glMatrixMode(GL_PROJECTION);
    }
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
