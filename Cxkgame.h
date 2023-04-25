#pragma once
#include "Block.h"
#include<graphics.h>
#include<vector>
using namespace std;
class Cxkgame
{
public:
	Cxkgame(int rows,int cols, int left, int top, int blocksize);
	void init();
	void game();

private:
	void Uploadwindow();//��Ⱦ����
	int Getdelays();//��ȡ�ӳ�
	void Drop();//!!����д�������ǵö��壬����������Ӵ���
	void Playermove();//�������
	void Clearline();//����
	void MoveLeftRight(int offset);//�ƶ�����
	void Rotate();//��ת����
	void Drawscore();//��ʾ��������Ļ
	void saveScore();//�������
	void displayOver();//����
	void checkOver();//����Ƿ����
	void Pause();//��ͣ
	bool beginMap();//��ʼ����


private:
	int delays;
	bool upload;
	int rows;
	int cols;
	int leftmargin;
	int topmargin;
	int blocksize;
	int score;
	int level;
	int lineCount;
	int highestScore;
	bool gameover;
	IMAGE pausepg;//��ͣͼƬ
	IMAGE imgBg;//����ͼƬ
	IMAGE beginPt;//��ʼͼƬ
	IMAGE imgOver;//ʧ�ܺ�ʤ����ʾͼƬ
	IMAGE imgWin;
	Block* Curblock;//��ǰ����
	Block* Nextblock;//Ԥ��ķ���
	Block Bakblock;//���ڱ��ݵķ���
	vector<vector<int>>map;//ʹ��vector��std�������ռ����
};

