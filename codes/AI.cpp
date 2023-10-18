#include "AI.h"

void AI::init(Chess* chess)
{
	this->chess = chess;   //��ȡ�������µ�����

	int size = chess->getGradeSize();  //��ʼ��valueMap
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
	Sleep(1000);  //��װ˼����ģ�����˶�������
	chess->chessDown(&pos, CHESS_WHITE);
}

void AI::caculateValue()
{
	int playercNum = 0; //���������������
	int aicNum = 0;		//ai������������
	int emptyNum = 0;	//ĳ�����Ͽհ�λ����


	//ÿ����һ��valueMapҪ����
	for (int i = 0; i < valueMap.size(); i++) {
		for (int j = 0; j < valueMap[i].size(); j++) {
			valueMap[i][j] = 0;
		}
	}

	//��ʼ����valueMap
	int size = chess->getGradeSize();
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {

			//��ÿ������й�ֵ����������������
			if (chess->getChessData(row, col)) continue;

			//�˸������ж�ʵ�֣�y(-1:up;0:present;1:down);x(-1:left;0:present;1:right)
			for (int y = -1; y <= 0; y++) {
				for (int x = -1; x <= 1; x++) {
					if (y == 0 && x == 0) continue;  //���������
					if (y == 0 && x != 1) continue;

					//ÿ�������жϺ��������ͬ������5��Ӯ���Ի������˫�����������㣬��λ��Ҳ����
					playercNum = 0;
					aicNum = 0;
					emptyNum = 0;

					//��������ڴ�λ�����ӻṹ��ʲô����
					for (int i = 1; i <= 4; i++) {
						int curRow = row + y * i;
						int curCol = col + x * i;

						if (curRow >= 0 && curRow < size &&  //�������壬������������
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 1) {
							playercNum++;
						}
						else if (curRow >= 0 && curRow < size &&  //������λ�ã�emptyNum���ӣ��ϵ�
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {  //�������壬�ϵ�
							break;
						}
					}

					//�����ж�
					for (int i = 1; i <= 4; i++) {
						int curRow = row - y * i;
						int curCol = col - x * i;

						if (curRow >= 0 && curRow < size &&  //�������壬������������
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 1) {
							playercNum++;
						}
						else if (curRow >= 0 && curRow < size &&  //������λ�ã�emptyNum���ӣ��ϵ�
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {  //�������壬�ϵ�
							break;
						}
					}

					//�Դ˵㽫�γɵ����θ���
					if (playercNum == 1) {   //��2
						valueMap[row][col] += 10;
					}
					else if (playercNum == 2) {
						if (emptyNum == 1) {  //��3
							valueMap[row][col] += 30;
						}
						else if (emptyNum == 2) {  //��3
							valueMap[row][col] += 40;
						}
					}
					else if (playercNum == 3) {
						if (emptyNum == 1) {  //��4
							valueMap[row][col] += 60;
						}
						else if (emptyNum == 2) {  //��4
							valueMap[row][col] += 200;
						}
					}
					else if (playercNum >= 4) {    //��5
						valueMap[row][col] += 20000;
					}

					//�������(ai)�ڴ�λ�����ӻṹ��ʲô����

					emptyNum = 0; //��λ������

					for (int i = 1; i <= 4; i++) {
						int curRow = row + y * i;
						int curCol = col + x * i;

						if (curRow >= 0 && curRow < size &&  //�������壬������������
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == -1) {
							aicNum++;
						}
						else if (curRow >= 0 && curRow < size &&  //������λ�ã�emptyNum���ӣ��ϵ�
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {  //�������壬�ϵ�
							break;
						}
					}

					//�����ж�
					for (int i = 1; i <= 4; i++) {
						int curRow = row - y * i;
						int curCol = col - x * i;

						if (curRow >= 0 && curRow < size &&  //�������壬������������
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == -1) {
							aicNum++;
						}
						else if (curRow >= 0 && curRow < size &&  //������λ�ã�emptyNum���ӣ��ϵ�
							curCol >= 0 && curCol < size &&
							chess->getChessData(curRow, curCol) == 0) {
							emptyNum++;
							break;
						}
						else {  //�������壬�ϵ�
							break;
						}
					}

					//�Դ˵㽫�γɵ����θ���
					if (aicNum == 0) {
						valueMap[row][col] += 5;
					}
					else if (aicNum == 1) {   //��2
						valueMap[row][col] += 10;
					}
					else if (aicNum == 2) {
						if (emptyNum == 1) {  //��3
							valueMap[row][col] += 25;
						}
						else if (emptyNum == 2) {  //��3
							valueMap[row][col] += 50;
						}
					}
					else if (aicNum == 3) {
						if (emptyNum == 1) {  //��4
							valueMap[row][col] += 55;
						}
						else if (emptyNum == 2) {  //��4
							valueMap[row][col] += 10000;
						}
					}
					else if (aicNum >= 4) {    //��5
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
