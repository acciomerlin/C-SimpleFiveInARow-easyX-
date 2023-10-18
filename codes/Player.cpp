#include "Player.h"

void Player::init(Chess* chess)
{
	this->chess = chess;
}

void Player::go(int *forMenu)
{
	MOUSEMSG msg;  //鼠标信息结构体
	ChessPos pos;

	while (1) {
		//获取鼠标点击信息，等到你点
		msg = GetMouseMsg(); //获取鼠标点击信息，返回类型是MOUSEMSG

		//通过chess对象确认点击位置正确,正确才能出循环落子，同时存取有效pos信息
		int chessBoardMargin = 680;
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x <= chessBoardMargin) {
			if (chess->clickBoard(msg.x, msg.y, &pos)) {
				break;
			}
		}

		//返回目录/重新开始/退出按钮
		if (msg.uMsg == WM_LBUTTONDOWN) {
			if (msg.x >= 857 && msg.x <= 1051 && msg.y >= 299 && msg.y <= 386) {
				// 重新开始
				mciSendString("play res/electricClick.wav", 0, 0, 0);
				chess->init();
				continue;
			}
			else if (msg.x >= 857 && msg.x <= 1051 && msg.y >= 423 && msg.y <= 509) {
				// 返回目录
				mciSendString("play res/electricClick.wav", 0, 0, 0);
				*forMenu = 1;
				return;
			}
			else if (msg.x >= 857 && msg.x <= 1051 && msg.y >= 545 && msg.y <= 631) {
				// 退出游戏
				mciSendString("play res/electricClick.wav", 0, 0, 0);
				exit(0);
			}
		}

	}

	//printf("%d, %d\n", pos.row, pos.col);  //调试时看坐标点
	//落子
	if (chess->playerFlag == true) {
		chess->chessDown(&pos, CHESS_BLACK);
	}
	else if (chess->playerFlag == false) {
		chess->chessDown(&pos, CHESS_WHITE);
	}
}
