#include "AI.h"

void AI::init(Chess* chess)
{
	this->chess = chess;   //获取现在在下的棋盘

	int size = chess->getGradeSize();  //初始化valueMap
	for (int i = 0; i < size; i++) {
		vector<int> row;
		for (int j = 0; j < size; j++) {
			row.push_back(0);
		}

		valueMap.push_back(row);
	}
}

void AI::go()
{
	ChessPos pos = posDecide();
	Sleep(1000);  //假装思考来模仿真人对弈体验
	chess->chessDown(&pos, CHESS_WHITE);
}

void AI::caculateValue()
{
	int playercNum = 0; //玩家连续棋子数量
	int aicNum = 0;		//ai连续棋子数量
	int emptyNum = 0;	//某方向上空白位个数


	//每更新一次valueMap要清零
	for (int i = 0; i < valueMap.size(); i++) {
		for (int j = 0; j < valueMap[i].size(); j++) {
			valueMap[i][j] = 0;
		}
	}

	//开始计算valueMap
	int size = chess->getGradeSize();
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {

			//对每个点进行估值，若已落子则跳过
			if (chess->getChessData(row, col)) continue;

			//八个方向判断实现：y(-1:up;0:present;1:down);x(-1:left;0:present;1:right)
			for (int y = -1; y <= 0; y++) {
				for (int x = -1; x <= 1; x++) {
					if (y == 0 && x == 0) continue;  //跳过自身点
					if (y == 0 && x != 1) continue;

					//每个方向判断后因规则是同方向连5才赢所以换方向后双方连续数清零，空位置也清零
					playercNum = 0;
					aicNum = 0;
					emptyNum = 0;

					//假如黑棋在此位置落子会构成什么棋形
					for (int i = 1; i <= 4; i++) {
						int curRow = row + y * i;
						int curCol = col + x * i;

						if (curRow >= 0 && curRow < size &&  //遇到黑棋，连续数量增加
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 1) {
							playercNum++;
						}
						else if (curRow >= 0 && curRow < size &&  //遇到空位置，emptyNum增加，断掉
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {  //遇到白棋，断掉
							break;
						}
					}

					//反向判断
					for (int i = 1; i <= 4; i++) {
						int curRow = row - y * i;
						int curCol = col - x * i;

						if (curRow >= 0 && curRow < size &&  //遇到黑棋，连续数量增加
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 1) {
							playercNum++;
						}
						else if (curRow >= 0 && curRow < size &&  //遇到空位置，emptyNum增加，断掉
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {  //遇到白棋，断掉
							break;
						}
					}

					//对此点将形成的棋形赋分
					if (playercNum == 1) {   //连2
						valueMap[row][col] += 10;
					}
					else if (playercNum == 2) {
						if (emptyNum == 1) {  //死3
							valueMap[row][col] += 30;
						}
						else if (emptyNum == 2) {  //活3
							valueMap[row][col] += 40;
						}
					}
					else if (playercNum == 3) {
						if (emptyNum == 1) {  //死4
							valueMap[row][col] += 60;
						}
						else if (emptyNum == 2) {  //活4
							valueMap[row][col] += 200;
						}
					}
					else if (playercNum >= 4) {    //连5
						valueMap[row][col] += 20000;
					}

					//假如白棋(ai)在此位置落子会构成什么棋形

					emptyNum = 0; //空位置清零

					for (int i = 1; i <= 4; i++) {
						int curRow = row + y * i;
						int curCol = col + x * i;

						if (curRow >= 0 && curRow < size &&  //遇到白棋，连续数量增加
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == -1) {
							aicNum++;
						}
						else if (curRow >= 0 && curRow < size &&  //遇到空位置，emptyNum增加，断掉
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {  //遇到黑棋，断掉
							break;
						}
					}

					//反向判断
					for (int i = 1; i <= 4; i++) {
						int curRow = row - y * i;
						int curCol = col - x * i;

						if (curRow >= 0 && curRow < size &&  //遇到白棋，连续数量增加
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == -1) {
							aicNum++;
						}
						else if (curRow >= 0 && curRow < size &&  //遇到空位置，emptyNum增加，断掉
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {  //遇到黑棋，断掉
							break;
						}
					}

					//对此点将形成的棋形赋分
					if (aicNum == 0) {
						valueMap[row][col] += 5;
					}
					else if (aicNum == 1) {   //连2
						valueMap[row][col] += 10;
					}
					else if (aicNum == 2) {
						if (emptyNum == 1) {  //死3
							valueMap[row][col] += 25;
						}
						else if (emptyNum == 2) {  //活3
							valueMap[row][col] += 50;
						}
					}
					else if (aicNum == 3) {
						if (emptyNum == 1) {  //死4
							valueMap[row][col] += 55;
						}
						else if (emptyNum == 2) {  //活4
							valueMap[row][col] += 10000;
						}
					}
					else if (aicNum >= 4) {    //连5
						valueMap[row][col] += 30000;
					}


				}



			}
		}
	}

}

ChessPos AI::posDecide()
{
	caculateValue();

	vector<ChessPos> maxPoints;
	int maxValue = 0;
	int size = chess->getGradeSize();
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			if (valueMap[row][col] > maxValue) {
				maxValue = valueMap[row][col];
				maxPoints.clear();
				maxPoints.push_back(ChessPos(row,col));
			}
			else if (valueMap[row][col] == maxValue) {
				maxPoints.push_back(ChessPos(row, col));
			}
		}
	}

	int index = rand() % maxPoints.size();
	return maxPoints[index];
	
}
