#include "Block.h"
#include"Cxkgame.h"
#include<stdlib.h>

//staticb�����ĳ�ʼ��
IMAGE* Block::imgs[7] = {NULL,  };
int Block::size = 29;

Block::Block()
{
	if (imgs[0] == NULL)
	{
		IMAGE Temp;
		loadimage(&Temp,"cxkblocksend.png");//����ͼƬ
		SetWorkingImage(&Temp);
		for (int i = 0; i < 7; i++)
		{
			imgs[i] = new IMAGE;
			getimage(imgs[i], i * size, 0, size, size);//�и�ͼƬ(���ĸ�:x,y�����г����ĳ��Ϳ�

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
	};//�������͵ķ���
	blocktype = rand() % 7 + 1;//ȷ����������
	for (int i = 0; i < 4; i++)
	{
		int value = blocks[blocktype-1][i];//ÿ��Сλ�õ���ֵ��������������±�����row��col��
		Smallblocks[i].row = value / 2;
		Smallblocks[i].col = value %2;
	}

	img = imgs[blocktype - 1];//��ͼƬ����������ģ��ⲽ��һ������
}

void Block::drop()//�½�
{
	for (int i = 0; i < 4; i++)
	{
		Smallblocks[i].row++;//ÿ��С�������������һ��Ҳ�������˶�һ��
	
	}
}

void Block::turnleft_right(int offset)//�����ƶ�
{
	for (int i = 0; i < 4; i++)
	{
		Smallblocks[i].col += offset;
	}
}

void Block::rotate()//��ת
{
	//math ����
	if (blocktype == 7) return;
	Point p = Smallblocks[1];
	for (int i = 0; i < 4; i++)
	{
		Point tmp = Smallblocks[i];
		Smallblocks[i].col = p.col - tmp.row + p.row;
		Smallblocks[i].row = p.row + tmp.col - p.col;
	}

}

void Block::draw(int leftmargin, int topmargin)//�����������
{
	for (int i = 0; i < 4; i++)
	{
		int x = Smallblocks[i].col * size + leftmargin;
		int y = Smallblocks[i].row * size + topmargin;
		putimage(x, y, img);//img = imgs[blocktype - 1]������������

	}

}

IMAGE** Block::getimags()
{
	return imgs;
}

Block& Block::operator=(const Block& other)
{
	if (this == &other)//�Լ���ֵ���Լ���ֱ�ӷ��ر���
		return *this;
	this->blocktype = other.blocktype;
	for (int i = 0; i < 4; i++)
	{
		this->Smallblocks[i] = other.Smallblocks[i];
	}
}

bool Block::Blockinmap(const vector<vector<int>>& map)//�жϷ����ǲ����ڵ�ͼ���棨�Ƿ��ںϷ�λ�ã�
{
	int rows = map.size();//size������vector�ĳ���
	int cols = map[0].size();
	for (int i = 0; i < 4; i++)
	{
		if (Smallblocks[i].col < 0 || Smallblocks[i].col >= cols ||
			Smallblocks[i].row < 0 ||
			Smallblocks[i].row >= rows
			|| map[Smallblocks[i].row][Smallblocks[i].col])
			return false;//����false˵���Լ�����
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
