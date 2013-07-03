#pragma once

class dsMap {
public:
	dsMap(char* fileName = "");
	~dsMap();
public:
	const void getSize(int& width, int& height) {
		width = this->width;
		height = this->height;
	}

	const int getElement(int x, int y){
		//数组越界
		if (x < 0 || x >= width || y < 0 || y >= height)
			return 0;

		return data[y][x];
	}
public:
	// 地图每块的大小
	static const int ElementSize = 4;
	
private:
	int width;
	int height;
	int** data;
};

