#pragma once
#include <graphics.h>  //easyxͼ�ο�ͷ�ļ�
#include <vector>
using namespace std;

typedef enum {
	CHESS_WHITE = -1,      //����
	CHESS_BLACK = 1        //����
}chess_kind_t;

//��ʾ����λ��
struct ChessPos {
	int row;
	int col;

	ChessPos(int r=0,int c=0):row(r),col(c){}
};

class Chess
{
public:
	//Ϊ���ܼ���͸������ͼƬ�����Ҷ�graphics.h���ȷ��Ű�......
	void putimagePNG(int x, int y, IMAGE* picture);

	//��ʾ���ڸ�˭����
	bool playerFlag; //true:���ߣ�false:����

	Chess(int gradeSize, int marginX, int marginY, float chessSize);

	//���̳�ʼ������������ͼƬ��Դ����ʼ�������������
	void init();

	//�ж���ĳ����(x,y)�ĵ���Ƿ���Ч
	//�����Ч�ѵ��λ�����б��浽pos��
	bool clickBoard(int x, int y, ChessPos* pos);

	//������ָ��λ��pos��(kind)��
	void chessDown(ChessPos* pos, chess_kind_t kind);

	//��ȡ���̴�С��13��15��19����
	int getGradeSize();  

	//��ȡָ��λ���Ǻ���/����/�հ�
	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);

	//�ж�����Ƿ����
	bool checkOver(int* r);
private:
	IMAGE chessBlackImg; //������
	IMAGE chessWhiteImg; //������

	int gradeSize;  //���̴�С��13��15��19��
	int margin_x;   //������߽�
	int margin_y;	//�����ϱ߽�
	float chessSize; //���Ӵ�С��Ҳ=����С����Ĵ�С��

	//���浱ǰ������ӷֲ�����
	//chessMap[3][5]��ʾ���̵����е����е����������0���գ�-1���ף�1����
	vector<vector<int>> chessMap;

	//ÿ�����������������ӷֲ�����
	void updateGameMap(ChessPos* pos);

	//��ʤ���˷���true�����򷵻�false
	bool checkWin();  

	//������ӵ�λ��
	ChessPos lastPos;
};

