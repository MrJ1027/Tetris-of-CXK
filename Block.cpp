#include "Block.h"
#include"Cxkgame.h"
#include<stdlib.h>

//staticb变量的初始化
IMAGE* Block::imgs[7] = {NULL,  };
int Block::size = 29;

Block::Block()
{
	if (imgs[0] == NULL)
	{
		IMAGE Temp;
		loadimage(&Temp,"cxkblocksend.png");//加载图片
		SetWorkingImage(&Temp);
		for (int i = 0; i < 7; i++)
		{
			imgs[i] = new IMAGE;
			getimage(imgs[i], i * size, 0, size, size);//切割图片(后四个:x,y还有切出来的长和宽）

		}
		SetWorkingImage();
	}
	int blocks[7][4] = 
	{
	1,3,5,7,
	2,4,5,7,
	3,5,4,6,
	3,5,4,7,
	2,3,5,7,
	3,5,7,6,
	2,3,4,5
	};//七种类型的方块
	blocktype = rand() % 7 + 1;//确定方块类型
	for (int i = 0; i < 4; i++)
	{
		int value = blocks[blocktype-1][i];//每个小位置的数值，根据这个还有下标来求row和col。
		Smallblocks[i].row = value / 2;
		Smallblocks[i].col = value %2;
	}

	img = imgs[blocktype - 1];//把图片换成数组里的，这步第一次忘了
}

void Block::drop()//下降
{
	for (int i = 0; i < 4; i++)
	{
		Smallblocks[i].row++;//每个小方块的行数都加一，也即往下运动一格
	
	}
}

void Block::turnleft_right(int offset)//左右移动
{
	for (int i = 0; i < 4; i++)
	{
		Smallblocks[i].col += offset;
	}
}

void Block::rotate()//旋转
{
	//math 方法
	if (blocktype == 7) return;
	Point p = Smallblocks[1];
	for (int i = 0; i < 4; i++)
	{
		Point tmp = Smallblocks[i];
		Smallblocks[i].col = p.col - tmp.row + p.row;
		Smallblocks[i].row = p.row + tmp.col - p.col;
	}

}

void Block::draw(int leftmargin, int topmargin)//根据坐标绘制
{
	for (int i = 0; i < 4; i++)
	{
		int x = Smallblocks[i].col * size + leftmargin;
		int y = Smallblocks[i].row * size + topmargin;
		putimage(x, y, img);//img = imgs[blocktype - 1]的作用在这里

	}

}

IMAGE** Block::getimags()
{
	return imgs;
}

Block& Block::operator=(const Block& other)
{
	if (this == &other)//自己赋值给自己，直接返回本身
		return *this;
	this->blocktype = other.blocktype;
	for (int i = 0; i < 4; i++)
	{
		this->Smallblocks[i] = other.Smallblocks[i];
	}
}

bool Block::Blockinmap(const vector<vector<int>>& map)//判断方块是不是在地图里面（是否在合法位置）
{
	int rows = map.size();//size函数求vector的长度
	int cols = map[0].size();
	for (int i = 0; i < 4; i++)
	{
		if (Smallblocks[i].col < 0 || Smallblocks[i].col >= cols ||
			Smallblocks[i].row < 0 ||
			Smallblocks[i].row >= rows
			|| map[Smallblocks[i].row][Smallblocks[i].col])
			return false;//返回false说明以及超界
	}
	return true;

}

void Block::Solidblocks(vector<vector<int>>& map)
{
	for (int i = 0; i < 4; i++)
	{
		map[Smallblocks[i].row][Smallblocks[i].col] = blocktype;
	}
}

int Block::Getblocktype()
{
	return blocktype;
}
