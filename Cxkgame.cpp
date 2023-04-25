#include "Cxkgame.h"
#include "Block.h"
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include <iostream>
#include<fstream>
#include <Windows.h>
#include <mmsystem.h>
#include<graphics.h>
#pragma comment(lib,"winmm.lib")

#define MAXLEVEL 15//没有该死的分号
const int SPEED_NORMAL[MAXLEVEL] = { 500,400,300,200,100,80,70,70,70,70,70,70,70,70,70};//设置降落速度
const int SPEED_QUICK = 30;

Cxkgame::Cxkgame(int rows, int cols, int left, int top, int blocksize)
{
	this->rows = rows;
	this->cols = cols;
	this->leftmargin = left;
	this->topmargin = top;
	this->blocksize = blocksize;
	for (int i = 0; i < rows; i++)
	{
		vector<int>Maprow;
		for (int j = 0; j < cols; j++)
		{
			Maprow.push_back(0);
		}
		map.push_back(Maprow);
	}
}

void Cxkgame::init()
{
	PlaySound("cxkmusic.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);//背景音乐

	delays = SPEED_NORMAL[0];
	srand(time(NULL));//设置随机数种子

	initgraph(800, 605);//初始化背景
	//加载背景图片以及结束图片
	loadimage(&imgBg, "cxkbg(1).png");

	//char map[20][10]
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			map[i][j] = 0;//初始化整个地图，都设为0
		}
	}
	score = 0;//初始化分数为0
	lineCount = 0;//消行数
	level = 1;//难度
	gameover = false;//刚开始游戏不结束

	//读文件里的数据
	ifstream Fin;
	Fin.open("recordscore.txt", ios::in);
	if (!Fin.is_open())
	{
		cout << "error" << endl;
		highestScore = 0;
	}
	else
	{
		Fin >> highestScore;
	}
	Fin.close();
}

void Cxkgame::game()
{
	bool is_in=beginMap();//判断是否开始
	if ( is_in== false) return;

	init();//初始化工作
	//创建下落方块以及右侧的预告方块
	Curblock = new Block;
	Nextblock = Curblock;
	Curblock = new Block;
	int timer = 0;//计时器
	while (true)
	{
		Playermove();//玩家输入
		timer += Getdelays();
		//渲染（时间间隔）
		if (timer > delays)
		{
			timer = 0;//计时器置空
			Drop();
			upload = true;
		}
		if (upload)//便于随时进行渲染(只需将upload置成TRUE
		{
			upload = false;
			Uploadwindow();
			Clearline();//消行

		}
		if (gameover)
		{
			saveScore();//保存分数
			/// <summary>
			///此句是有被执行的，但是上面那个不知道为何没有显示图片（设置断点调试证明每一句都走过了
			/// 后记：解决了，我把类里胜利和失败的图片变量提前就可以显示了，我也不知道为什么，竟然和放的位置有关系
			/// </summary>
			displayOver();
			system("pause");
			init();
		}
	}
}

void Cxkgame::Uploadwindow()
{
	putimage(0, 0, &imgBg);//输出背景图
	IMAGE** imgs = Block::getimags();
	BeginBatchDraw();
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (map[i][j] == 0)
				continue;
			int x = j * blocksize + leftmargin;
			int y = i * blocksize + topmargin;
			putimage(x, y, imgs[map[i][j] - 1]);
		}
	}
	Curblock->draw(leftmargin, topmargin);
	Nextblock->draw(510, topmargin);
	Drawscore();//绘制边栏数据
	EndBatchDraw();
}

int Cxkgame::Getdelays()
{
	static unsigned long long lastTime = 0;
	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0)
	{
		lastTime = currentTime;
		return 0;
	}
	else
	{
		int ret = currentTime - lastTime;
		lastTime = currentTime;
		return ret;
	}
}

void Cxkgame::Drop()
{
	Bakblock = *Curblock;
	Curblock->drop();
	if (!Curblock->Blockinmap(map))
	{
		Bakblock.Solidblocks(map);
		delete Curblock;
		Curblock = Nextblock;
		Nextblock = new Block;
		checkOver();//降落后检查一下这个方块是否合法位置，若不合法，则gameover为TRUE
	}
	delays = SPEED_NORMAL[level-1];
}

void Cxkgame::Playermove()
{
	unsigned char ch = ' ';//0-255;
	bool rotateFlags = false;
	int dx = 0;
	if (_kbhit())
	{
		ch = _getch();//接受输入
			switch (ch)
			{
			case 27:
				Pause();
				break;
			case 32:
				rotateFlags = true;
				break;
			case 80:
				delays = SPEED_QUICK;
				break;
			case 75:
				dx = -1;
				break;
			case 77:
				dx = 1;
				break;
			default:
				break;
			}
	}
	//实现旋转
	if (rotateFlags)
	{
		Rotate();
		upload = true;
	}
	//左右移动
	if (dx != 0)
	{
		MoveLeftRight(dx);
		upload = true;//即时渲染
	}
}

void Cxkgame::Clearline()
{
	int line = 0;
	int k = rows - 1;
	for (int i = rows - 1; i >= 0; i--)
	{
		int count = 0;
		for (int j = 0; j < cols; j++)
		{
			if (map[i][j])
			{
				count++;
			}
			map[k][j] = map[i][j];//存储
		}
		if (count < cols)//不是满行
		{
			k--;
		}
		else//count=cols，满行
		{
			line++;
		}
	}
	if (line > 0)
	{	//计算得分
		int addScore[4] = { 20,50,80,100 };
		score += addScore[line - 1];
		upload = true;
		level = (score + 99) / 100;
		if (level > MAXLEVEL)//通关游戏结束(虽然不可能）
		{
			gameover = true;
		}
		lineCount += line;
	}
}

void Cxkgame::MoveLeftRight(int offset)
{
	Bakblock = *Curblock;
	Curblock->turnleft_right(offset);
	if (!Curblock->Blockinmap(map))
	{
		*Curblock = Bakblock;
	}
}

void Cxkgame::Rotate()
{
	Bakblock = *Curblock;
	Curblock->rotate();
	if (!Curblock->Blockinmap(map))
	{
		*Curblock = Bakblock;
	}
}

void Cxkgame::Drawscore()//绘制右侧的边栏，地图大小改了这些也要改
{
	char scoreText[30];
	sprintf_s(scoreText, sizeof(scoreText), "%d", score);
	setcolor(RGB(239,136,190));
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 50;
	f.lfWidth = 30;
	f.lfQuality = ANTIALIASED_QUALITY;//字体抗锯齿效果
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));
	settextstyle(&f);
	setbkmode(TRANSPARENT);//设置背景透明
	outtextxy(610, 360, scoreText);
	//绘制消行数
	sprintf_s(scoreText, sizeof(scoreText), "%d", lineCount);
	outtextxy(690, 465, scoreText);
	//难度
	sprintf_s(scoreText, sizeof(scoreText), "%d", level);
	outtextxy(690, 415, scoreText);
	//最高分
	sprintf_s(scoreText, sizeof(scoreText), "%d", highestScore);
	outtextxy(690, 515, scoreText);
}
//保存最高分到文件(注意得等游戏完全结束才可以，不然手动退出，不然执行不到这里）
void Cxkgame::saveScore()
{
	if (score > highestScore)
	{
		highestScore = score;
		ofstream Fout;
		Fout.open("recordscore.txt", ios::out);
		Fout << highestScore;
		Fout.close();
	}
}

void Cxkgame::displayOver()
{
	if (level > MAXLEVEL)
	{
		loadimage(&imgWin, "winpg(1).png");
		putimage(220, 150, &imgWin);
	}
	else 
	{
		loadimage(&imgOver, "overpg(1).png");
		putimage(220, 150, &imgOver);
	}

}

void Cxkgame::checkOver()
{
	gameover = (Curblock->Blockinmap(map) == false);
}

//esc暂停功能
void Cxkgame::Pause()
{
	loadimage(&pausepg, "overpg(1).png");
	putimage(220, 150, &pausepg);
	system("pause");
}

bool Cxkgame::beginMap()
{
	initgraph(800, 605);
	loadimage(&beginPt, "beginpg.jpg");
	putimage(0, 0, &beginPt);
	char a = _getch();//还可以试试鼠标操作
	if (a == 32)
	{
		return true;
	}
	else
	{
		return false;
	}
}
