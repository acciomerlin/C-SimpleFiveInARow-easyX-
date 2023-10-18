#pragma once
#include "Chess.h"

class AI
{
public:
	void init(Chess *chess); //AI��ʼ�������������µ�����
	void go();

private:
	Chess* chess;
	vector<vector<int>> valueMap;  

private:
	void caculateValue();
	ChessPos posDecide();
};

