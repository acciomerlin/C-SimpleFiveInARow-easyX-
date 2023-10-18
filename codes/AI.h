#pragma once
#include "Chess.h"

class AI
{
public:
	void init(Chess *chess); //AI初始化，传入正在下的棋盘
	void go();

private:
	Chess* chess;
	vector<vector<int>> valueMap;  

private:
	void caculateValue();
	ChessPos posDecide();
};

