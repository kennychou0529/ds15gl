队式 15 展示组
======

显示文字：
* 显示文字的功能现在被包装到了 class dsTextManager 中，该类在 dsTextManager.h 中有声明。
* 每个 dsTextManager 只能使用一种字体，一种大小。
* 在 dsTools.cpp 中定义了一个全局的 dsTextManager dstext，并有 font_file_name 和 font_height。
* 首先要调用 init，这个工作已在 main.cpp 中的 dsInit() 里完成。
* 在 dsScene::show() 中和 DSStatusBar::show() 中都有显示文字的代码。
* 显示文字的代码的使用方式见 dsTextManager.h 中 对 dsTextManager::print() 的说明。

注意：
* 在使用平移、旋转画图时，请使用 glPushMatrix, glPopMatrix 包围，以免影响后续的画图

* 现在的天空盒宽度为 2000.0，地面 z < 0.0，今后地图可以放在 z = 0

说明：
* 场景的拼写应该是 scene 而不是 scence，我已经全部替换掉了
* 我把天空盒的那部分都放到一个类 dsSkybox 里，本来准备了 5 个天空盒，由于网速慢，只上传了一个，其他天空盒请从百度网盘
http://pan.baidu.com/share/link?shareid=3838251435&uk=2033312131&third=2
下载之
* 天空图的纹理编号和显示列表包装了在 dsSkybox 类里
* dsSkybox 有 load() 和 show() 方法，我在 dsScene::initScene 中 调用 load()，并在 dsScene::showScene() 中调用 show()
* 之前之所以我的载入 BMP 函数不起效果，是因为它只支持 24bit BMP 文件，我在新天空盒中的载入 BMP 用了我原来的那个，当然 aux 库的那个保留
* 天空盒宽度为 200.0，高度根据贴图的宽高比自动算出，天空盒的底面的顶点的 x,y 坐标都为 +- 100.0，z = -2.0
* loadT8 被移到 dstexture.cpp 中
