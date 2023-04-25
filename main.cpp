#include <iostream>
#include "Block.h"
#include "Cxkgame.h"
using namespace std;

int main()
{
	Cxkgame cxk(20,10,220,25,29);
	cxk.game();
	return 0;
}

#pragma region 一些问题
//1.音乐播放不出来//准备用SDL试试
//2.结束动画加载不出来(一闪而过）//已解决
//3.打包出去的文件无法存储最高分//已解决，要用管理员身份打开

#pragma endregion
