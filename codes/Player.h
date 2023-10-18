#pragma once
#include "Chess.h"

class Player
{
public:
	void init(Chess* chess);
	void go(int *forMenu);

private:
	Chess *chess;
};

