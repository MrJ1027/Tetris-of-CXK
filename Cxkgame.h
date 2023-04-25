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
	void Uploadwindow();//渲染窗口
	int Getdelays();//获取延迟
	void Drop();//!!函数写出声明记得定义，否则出现链接错误
	void Playermove();//玩家输入
	void Clearline();//清理
	void MoveLeftRight(int offset);//移动方块
	void Rotate();//旋转方块
	void Drawscore();//显示分数到屏幕
	void saveScore();//保存分数
	void displayOver();//结束
	void checkOver();//检查是否结束
	void Pause();//暂停
	bool beginMap();//开始界面


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
	IMAGE pausepg;//暂停图片
	IMAGE imgBg;//背景图片
	IMAGE beginPt;//开始图片
	IMAGE imgOver;//失败和胜利显示图片
	IMAGE imgWin;
	Block* Curblock;//当前方块
	Block* Nextblock;//预告的方块
	Block Bakblock;//用于备份的方块
	vector<vector<int>>map;//使用vector加std的命名空间才行
};

