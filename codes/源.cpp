/*
* ������־��
* 1. ������Ŀ�������ز�
* 2. ��ƿ�ܣ����ǣ�������Ŀ�Ҽ�����Player, Chess, AI, ChessGame��->��Ƹ������ӿ�(������->�����з���Ҫ�µ����ݳ�Ա->������ݳ�Ա
*/
#include <iostream>
#include "ChessGame.h"


int main(void) {
	Player player;
	Chess chess(15, 33, 33, 44.2);
	AI ai;
	ChessGame game(&player, &ai, &chess);

	//��Ϸ���ڴ���
	initgraph(1106, 682);

	//��Ϸ����
	game.startup();

	return 0;
}