/*
* 开发日志：
* 1. 创建项目，导入素材
* 2. 设计框架（类们）：在项目右键创建Player, Chess, AI, ChessGame类->设计各类对外接口(方法）->方法中发现要新的数据成员->添加数据成员
*/
#include <iostream>
#include "ChessGame.h"


int main(void) {
	Player player;
	Chess chess(15, 33, 33, 44.2);
	AI ai;
	ChessGame game(&player, &ai, &chess);

	//游戏窗口创建
	initgraph(1106, 682);

	//游戏本体
	game.startup();

	return 0;
}