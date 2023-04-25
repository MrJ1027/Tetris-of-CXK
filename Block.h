#pragma once
#include<graphics.h>
#include<vector>
using namespace std;
struct Point //�ĸ�����ÿ�������λ��
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
	void draw(int leftmargin, int topmargin);//���Ʒ��鵽��Ļ��
	static IMAGE** getimags();//���ص��Ǿ�̬��Ա����������������Ը÷���Ҳ�Ǿ�̬��
	Block& operator=(const Block& other);//���������
	bool Blockinmap(const vector<vector<int>>& map);//�жϷ���λ���Ƿ�Ϸ�
	void Solidblocks(vector<vector<int>>& map);//�̻�����
	int Getblocktype();


private:
	
	int blocktype;//�������ͣ���״��
	Point Smallblocks[4];
	IMAGE* img;

private:
	//static���Σ�ֻ��һ��ռ䣬ÿ��������
	static int size;
	static IMAGE* imgs[7];
};

