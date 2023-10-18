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

	//��Ϸ��ʼ����
	void startup();

	//PVP
	void playPVP();

	//PVE
	void playPVE(); 

	//��ʼ����Ŀ¼
	int menu();

	//����
	void tips();

	//��Ϸ��¼����
	void recordPlace();

	//��Ϸ��¼ҳ����ʾ��ˢ��
	void recordShow();

	//��Ϸ��¼����
	void recordDownload();

//������ݳ�Ա
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

