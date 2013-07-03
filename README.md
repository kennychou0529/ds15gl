队式15展示组
======

哇，这是我们展示组的代码

说明：
1. 场景的拼写应该是 scene 而不是 scence，我已经全部替换掉了
2. 我把天空盒的那部分都放到一个类 dsSkybox 里，本来准备了 5 个天空盒，由于网速慢，只上传了一个，其他天空盒请从百度网盘
http://pan.baidu.com/share/link?shareid=3838251435&uk=2033312131&third=2
下载之
3. 天空图的纹理编号和显示列表包装了在 dsSkybox 类里
4. dsSkybox 有 load() 和 show() 方法，我在 dsScene::initScene 中 调用 load()，并在 dsScene::showScene() 中调用 show()
5. 之前之所以我的载入 BMP 函数不起效果，是因为它只支持 24bit BMP 文件，我在新天空盒中的载入 BMP 用了我原来的那个，当然 aux 库的那个保留
6. 天空盒宽度为 200.0，高度根据贴图的宽高比自动算出，天空盒的底面的顶点的 x,y 坐标都为 +- 100.0，z = -2.0
7. loadT8 被移到 dstexture.cpp 中
