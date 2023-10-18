#pragma once
#include "Player.h"
#include "AI.h"
#include "Chess.h"

typedef enum {
	CHESS_PVP = 1,
	CHESS_PVE = 2,
	CHESS_EXIT = 3,
	CHESS_RESTART = 4,
	CHESS_MENU = 5,
	CHESS_RECORD = 6,
	CHESS_CLEAR = 7,
	CHESS_DOWNLOAD = 8
}menu_choice;

class ChessGame
{
public:
	ChessGame(Player* player,AI* ai, Chess* chess);

	//游戏开始界面
	void startup();

	//PVP
	void playPVP();

	//PVE
	void playPVE(); 

	//开始界面目录
	int menu();

	//弹窗
	void tips();

	//游戏记录功能
	void recordPlace();

	//游戏记录页面显示和刷新
	void recordShow();

	//游戏记录下载
	void recordDownload();

//添加数据成员
private:
	Player* player;
	AI* ai;
	Chess* chess;
	vector<int> recordsPVP{ 0 };
	vector<int> recordsPVE{ 0 };
	IMAGE background;
	IMAGE soundOn;
	IMAGE soundOff;
	IMAGE tip;
};

