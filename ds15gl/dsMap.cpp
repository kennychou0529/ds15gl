#include "dsMap.h"
#include <iostream>
using namespace std;

//从文件名初始化一张地图
dsMap::dsMap(char* fileName)
{
	//如果没有传入文件名，则产生一张 100 * 100 的地图，且所有元素都是0
	if (strlen(fileName) == 0) {
		width = 100;
		height = 50;
		data = new int*[height];
		for(int i = 0; i < height; i++)
			data[i] = new int[width];
		for(int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				data[i][j] = 0;
			}
		}
	}
	printf_s("%d\n",**data);
}


dsMap::~dsMap()
{
	for(int i = 0; i < height; i++){
		delete[] data[i];
	}
	delete[] data;
}
