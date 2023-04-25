#pragma once
#include<graphics.h>
#include<vector>
using namespace std;
struct Point //四个方块每个方块的位置
{
	int row;
	int col;
};

class Block
{
public:
	Block();
	void drop();
	void turnleft_right(int offset);
	void rotate();
	void draw(int leftmargin, int topmargin);//绘制方块到屏幕上
	static IMAGE** getimags();//返回的是静态成员数组的数组名，所以该方法也是静态的
	Block& operator=(const Block& other);//运算符重载
	bool Blockinmap(const vector<vector<int>>& map);//判断方块位置是否合法
	void Solidblocks(vector<vector<int>>& map);//固化方块
	int Getblocktype();


private:
	
	int blocktype;//方块类型（形状）
	Point Smallblocks[4];
	IMAGE* img;

private:
	//static修饰，只有一块空间，每个对象共有
	static int size;
	static IMAGE* imgs[7];
};

