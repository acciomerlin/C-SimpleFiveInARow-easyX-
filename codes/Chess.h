#pragma once
#include <graphics.h>  //easyx图形库头文件
#include <vector>
using namespace std;

typedef enum {
	CHESS_WHITE = -1,      //白棋
	CHESS_BLACK = 1        //黑棋
}chess_kind_t;

//表示落子位置
struct ChessPos {
	int row;
	int col;

	ChessPos(int r=0,int c=0):row(r),col(c){}
};

class Chess
{
public:
	//为了能加载透明背景图片，不敢动graphics.h就先放着吧......
	void putimagePNG(int x, int y, IMAGE* picture);

	//表示现在该谁落子
	bool playerFlag; //true:黑走；false:白走

	Chess(int gradeSize, int marginX, int marginY, float chessSize);

	//棋盘初始化，加载棋盘图片资源，初始化棋盘相关数据
	void init();

	//判断在某坐标(x,y)的点击是否有效
	//如果有效把点击位置行列保存到pos里
	bool clickBoard(int x, int y, ChessPos* pos);

	//在棋盘指定位置pos落(kind)子
	void chessDown(ChessPos* pos, chess_kind_t kind);

	//获取棋盘大小（13，15，19）线
	int getGradeSize();  

	//获取指定位置是黑棋/白棋/空白
	int getChessData(ChessPos* pos);
	int getChessData(int row, int col);

	//判断棋局是否结束
	bool checkOver(int* r);
private:
	IMAGE chessBlackImg; //黑棋子
	IMAGE chessWhiteImg; //白棋子

	int gradeSize;  //棋盘大小（13，15，19）
	int margin_x;   //棋盘左边界
	int margin_y;	//棋盘上边界
	float chessSize; //棋子大小（也=棋盘小方格的大小）

	//储存当前棋局棋子分布数据
	//chessMap[3][5]表示棋盘第三行第五列的落子情况：0：空；-1：白；1：黑
	vector<vector<int>> chessMap;

	//每次落子完更新棋局棋子分布数据
	void updateGameMap(ChessPos* pos);

	//分胜负了返回true，否则返回false
	bool checkWin();  

	//最近落子点位置
	ChessPos lastPos;
};

