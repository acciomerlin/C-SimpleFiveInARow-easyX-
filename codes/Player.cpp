#include "Player.h"

void Player::init(Chess* chess)
{
	this->chess = chess;
}

void Player::go(int *forMenu)
{
	MOUSEMSG msg;  //�����Ϣ�ṹ��
	ChessPos pos;

	while (1) {
		//��ȡ�������Ϣ���ȵ����
		msg = GetMouseMsg(); //��ȡ�������Ϣ������������MOUSEMSG

		//ͨ��chess����ȷ�ϵ��λ����ȷ,��ȷ���ܳ�ѭ�����ӣ�ͬʱ��ȡ��Чpos��Ϣ
		int chessBoardMargin = 680;
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x <= chessBoardMargin) {
			if (chess->clickBoard(msg.x, msg.y, &pos)) {
				break;
			}
		}

		//����Ŀ¼/���¿�ʼ/�˳���ť
		if (msg.uMsg == WM_LBUTTONDOWN) {
			if (msg.x >= 857 && msg.x <= 1051 && msg.y >= 299 && msg.y <= 386) {
				// ���¿�ʼ
				mciSendString("play res/electricClick.wav", 0, 0, 0);
				chess->init();
				continue;
			}
			else if (msg.x >= 857 && msg.x <= 1051 && msg.y >= 423 && msg.y <= 509) {
				// ����Ŀ¼
				mciSendString("play res/electricClick.wav", 0, 0, 0);
				*forMenu = 1;
				return;
			}
			else if (msg.x >= 857 && msg.x <= 1051 && msg.y >= 545 && msg.y <= 631) {
				// �˳���Ϸ
				mciSendString("play res/electricClick.wav", 0, 0, 0);
				exit(0);
			}
		}

	}

	//printf("%d, %d\n", pos.row, pos.col);  //����ʱ�������
	//����
	if (chess->playerFlag == true) {
		chess->chessDown(&pos, CHESS_BLACK);
	}
	else if (chess->playerFlag == false) {
		chess->chessDown(&pos, CHESS_WHITE);
	}
}
