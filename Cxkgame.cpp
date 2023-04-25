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

#define MAXLEVEL 15//û�и����ķֺ�
const int SPEED_NORMAL[MAXLEVEL] = { 500,400,300,200,100,80,70,70,70,70,70,70,70,70,70};//���ý����ٶ�
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
	PlaySound("cxkmusic.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);//��������

	delays = SPEED_NORMAL[0];
	srand(time(NULL));//�������������

	initgraph(800, 605);//��ʼ������
	//���ر���ͼƬ�Լ�����ͼƬ
	loadimage(&imgBg, "cxkbg(1).png");

	//char map[20][10]
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			map[i][j] = 0;//��ʼ��������ͼ������Ϊ0
		}
	}
	score = 0;//��ʼ������Ϊ0
	lineCount = 0;//������
	level = 1;//�Ѷ�
	gameover = false;//�տ�ʼ��Ϸ������

	//���ļ��������
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
	bool is_in=beginMap();//�ж��Ƿ�ʼ
	if ( is_in== false) return;

	init();//��ʼ������
	//�������䷽���Լ��Ҳ��Ԥ�淽��
	Curblock = new Block;
	Nextblock = Curblock;
	Curblock = new Block;
	int timer = 0;//��ʱ��
	while (true)
	{
		Playermove();//�������
		timer += Getdelays();
		//��Ⱦ��ʱ������
		if (timer > delays)
		{
			timer = 0;//��ʱ���ÿ�
			Drop();
			upload = true;
		}
		if (upload)//������ʱ������Ⱦ(ֻ�轫upload�ó�TRUE
		{
			upload = false;
			Uploadwindow();
			Clearline();//����

		}
		if (gameover)
		{
			saveScore();//�������
			/// <summary>
			///�˾����б�ִ�еģ����������Ǹ���֪��Ϊ��û����ʾͼƬ�����öϵ����֤��ÿһ�䶼�߹���
			/// ��ǣ�����ˣ��Ұ�����ʤ����ʧ�ܵ�ͼƬ������ǰ�Ϳ�����ʾ�ˣ���Ҳ��֪��Ϊʲô����Ȼ�ͷŵ�λ���й�ϵ
			/// </summary>
			displayOver();
			system("pause");
			init();
		}
	}
}

void Cxkgame::Uploadwindow()
{
	putimage(0, 0, &imgBg);//�������ͼ
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
	Drawscore();//���Ʊ�������
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
		checkOver();//�������һ����������Ƿ�Ϸ�λ�ã������Ϸ�����gameoverΪTRUE
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
		ch = _getch();//��������
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
	//ʵ����ת
	if (rotateFlags)
	{
		Rotate();
		upload = true;
	}
	//�����ƶ�
	if (dx != 0)
	{
		MoveLeftRight(dx);
		upload = true;//��ʱ��Ⱦ
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
			map[k][j] = map[i][j];//�洢
		}
		if (count < cols)//��������
		{
			k--;
		}
		else//count=cols������
		{
			line++;
		}
	}
	if (line > 0)
	{	//����÷�
		int addScore[4] = { 20,50,80,100 };
		score += addScore[line - 1];
		upload = true;
		level = (score + 99) / 100;
		if (level > MAXLEVEL)//ͨ����Ϸ����(��Ȼ�����ܣ�
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

void Cxkgame::Drawscore()//�����Ҳ�ı�������ͼ��С������ЩҲҪ��
{
	char scoreText[30];
	sprintf_s(scoreText, sizeof(scoreText), "%d", score);
	setcolor(RGB(239,136,190));
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 50;
	f.lfWidth = 30;
	f.lfQuality = ANTIALIASED_QUALITY;//���忹���Ч��
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));
	settextstyle(&f);
	setbkmode(TRANSPARENT);//���ñ���͸��
	outtextxy(610, 360, scoreText);
	//����������
	sprintf_s(scoreText, sizeof(scoreText), "%d", lineCount);
	outtextxy(690, 465, scoreText);
	//�Ѷ�
	sprintf_s(scoreText, sizeof(scoreText), "%d", level);
	outtextxy(690, 415, scoreText);
	//��߷�
	sprintf_s(scoreText, sizeof(scoreText), "%d", highestScore);
	outtextxy(690, 515, scoreText);
}
//������߷ֵ��ļ�(ע��õ���Ϸ��ȫ�����ſ��ԣ���Ȼ�ֶ��˳�����Ȼִ�в������
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

//esc��ͣ����
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
	char a = _getch();//����������������
	if (a == 32)
	{
		return true;
	}
	else
	{
		return false;
	}
}
