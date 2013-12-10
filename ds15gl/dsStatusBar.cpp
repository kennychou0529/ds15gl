#include <sstream>
#include <iomanip>
#include <iostream>
#include "dsTexture.h"
#include "dsStatusBar.h"
#include "dsTools.h"
#include "dsTextManager.h"
#include "dsFrame.h"
#include "dsEye.h"

extern DSFrame frame;

DSStatusBar::DSStatusBar() {}

DSStatusBar::~DSStatusBar() {}

void DSStatusBar::init() {
    GLuint height, width;
    logo_tex = dsLoadTextureBMP2D("data/images/logo.bmp", &height, &width);
    ratio = static_cast<GLdouble>(height) / width;

    bar_tex = dsLoadTextureBMP2D("data/images/bar.bmp", &bar_height, &width);
}

void DSStatusBar::renderBackground() {
    // 注意：考虑到窗口的大小可能被调整，所以画了上下两个矩形
    // 以填满 Status Bar

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, bar_tex);
    glBegin(GL_POLYGON);
    {
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(
            0.0f,
            static_cast<GLfloat>(window_height - (GLint)bar_height)
        );

        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(
            static_cast<GLfloat>(status_bar_width),
            static_cast<GLfloat>(window_height - (GLint)bar_height)
        );

        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(
            static_cast<GLfloat>(status_bar_width),
            static_cast<GLfloat>(window_height)
        );

        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(
            0.0f,
            static_cast<GLfloat>(window_height)
        );
    }
    glEnd();

    GLint need;
    if (window_height > bar_height) {
        need = window_height - bar_height + 20;
    } else {
        need = 20;
    }
    glBegin(GL_POLYGON);
    {
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(
            static_cast<GLfloat>(status_bar_width),
            0.0f
        );

        glTexCoord2f(
            1.0f,
            static_cast<GLfloat>(need) / bar_height
        );
        glVertex2f(
            static_cast<GLfloat>(status_bar_width),
            static_cast<GLfloat>(need)
        );

        glTexCoord2f(
            0.0f,
            static_cast<GLfloat>(need) / bar_height
        );
        glVertex2f(
            0.0f,
            static_cast<GLfloat>(need)
        );
    }
    glEnd();
}

extern Round_End_Info end_infos[600];
extern int round;

void DSStatusBar::show() {
    // 更改投影方式为 2D 平行投影
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    {
        glLoadIdentity();
        glOrtho(0, status_bar_width, 0, window_height, 0.01, 1000);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glViewport(
            window_width - status_bar_width,
            0,
            status_bar_width,
            window_height
        );
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);

        gluLookAt(0.0, 0.0, 200.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        glColor3d(1.0, 1.0, 1.0);

        renderBackground();

        //fillRectange2D(0, 0, status_bar_width, window_height);

        // 这里画小地图，战场，人物等状态信息
        		

		glColor3f(0,1,1);

        // 让我们在这里写一下眼睛位置
        std::wostringstream os;
        /*os << "(" << std::fixed << std::setprecision(2) << eye[0]
           << ", " << eye[1] << ", " << eye[2] << ")";
        glPushMatrix();
        {
            glLoadIdentity();
            dstext_small.print(8.0, 8.0, os.str());
            dstext_small.print(40.0, 30.0, L"Eye position");
        }
        glPopMatrix();*/


        //让我们写一下被选中人物的信息
        glPushMatrix();
        {
            //std::stringstream o;
            os.str(L"");
            os << frame.actors.selectInfo();
            glLoadIdentity();
            dstext_small.print(12.0, 400.0, os.str());
        }
        glPopMatrix();


		//提示消息
		glPushMatrix();
		{
			
			//std::stringstream o;
			os.str(L"");
			//os << L"帮助\nF1 拉近镜头\nF2 推远镜头\nup,down,left,right\n视角旋转\nw,a,s,d 视线平移\n鼠标点击 选择人物\n鼠标右键 取消选择";
			//os << L"帮助：\nF1 拉近镜头\nF2 推远镜头\n↑ ↓ → ← 视角旋转\nW S A D 视线平移\n";
			/*if (frame.actors.paused) {
			os << L"F4 切换为自动";
			} else {
			os << L"F4 切换为手动";
			}*/
            glLoadIdentity();
			dstext_small.print(15.0f, (GLfloat)window_height - 120, os.str());
		}
		glPopMatrix();
        /*os.str(L"");
        os << frame.actors.list["mage"].winx << L", "
            << frame.actors.list["mage"].winy << L", "
            << frame.actors.list["mage"].winz;
        glPushMatrix();
        {
            glLoadIdentity();
            dstext_small.print(5, 400, os.str().c_str());
        }
        glPopMatrix();*/

        glPushMatrix();
        {
			//glColor3f(0,0,1);
            os.str(L"");
            os << L"FPS: " << frame.getFPS();
            glLoadIdentity();
            glScalef(0.5, 0.5, 0.5);
            dstext.print(15.0f, (GLfloat)window_height - 80, os.str());
        }
        glPopMatrix();

        glPushMatrix();
        {
            os.str(L"");
            os << L"回合 " << frame.actors.getCurrentRound();
            if (frame.actors.round_finished) {
                os << L"\nF3 进入下一回合";
            }
            if (frame.actors.script_playing != 0) {
                os << L"\n" << frame.actors.script_playing << L" 个动作正在播放";
            }
            if (frame.actors.all_finished) {
                os << L"\n已结束";
            }

            glLoadIdentity();
            dstext_small.print(15.0f, 220, os.str());
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
