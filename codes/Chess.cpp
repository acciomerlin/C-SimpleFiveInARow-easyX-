#include "Chess.h"
#include <math.h>
#include <mmsystem.h>
#include <conio.h>
#pragma comment(lib, "winmm.lib")

void Chess::putimagePNG(int x, int y, IMAGE* picture)
{
    // 变量初始化
    DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
    DWORD* draw = GetImageBuffer();
    DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
    int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
    int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
    int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
    int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
    int dstX = 0;    //在显存里像素的角标

    // 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
    for (int iy = 0; iy < picture_height; iy++)
    {
        for (int ix = 0; ix < picture_width; ix++)
        {
            int srcX = ix + iy * picture_width; //在显存里像素的角标
            int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
            int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
            int sg = ((src[srcX] & 0xff00) >> 8);   //G
            int sb = src[srcX] & 0xff;              //B
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
            {
                dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
                    | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
                    | (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
            }
        }
    }
}

Chess::Chess(int gradeSize, int marginX, int marginY, float chessSize)
{
    this->gradeSize = gradeSize;
    this->margin_x = marginX;
    this->margin_y = marginY;
    this->chessSize = chessSize;
    playerFlag = CHESS_BLACK;

    for (int i = 0; i < gradeSize; i++) {
        vector<int> row;
        for (int j = 0; j < gradeSize; j++) {
            row.push_back(0);
        }
        chessMap.push_back(row);
    }
}

void Chess::init()
{
    //每次重开一局要打回Flag
    playerFlag = CHESS_BLACK;

    //显示棋盘图片
    loadimage(0, "res/chessBoard.png", 1106, 682, true);

    //加载黑白棋图片到chessBlack/WhiteImg
    loadimage(&chessBlackImg, "res/black.png", chessSize, chessSize, true);
    loadimage(&chessWhiteImg, "res/white.png", chessSize, chessSize, true);

    //每轮清空棋盘
    for (int i = 0; i < gradeSize; i++) {
        for (int j = 0; j < gradeSize; j++) {
            chessMap[i][j] = 0;
        }
    }
}

bool Chess::clickBoard(int x, int y, ChessPos* pos)
{   
    int row = (y - margin_y) / chessSize;          //左上角节点位于棋盘的行列值
    int col = (x - margin_x) / chessSize;
    int leftTopPosY = margin_y + chessSize * row;  //左上角节点坐标值
    int leftTopPosX = margin_x + chessSize * col;
    int offset = chessSize * 0.3;

    int len;
    bool ret = false;

    do {
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        //左上角判断
        if (len < offset) {
            pos->row = row;
            pos->col = col;
            if (chessMap[pos->row][pos->col] == 0) {  //还要确认此位置没有棋子
                ret = true;
            }
            break;
        }

        //右上角判断
        int x2 = leftTopPosX + chessSize;
        int y2 = leftTopPosY;
        len = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
        if (len < offset) {
            pos->row = row;
            pos->col = col + 1;
            if (chessMap[pos->row][pos->col] == 0) {  //还要确认此位置没有棋子
                ret = true;
            }
            break;
        }

        //左下角判断
        x2 = leftTopPosX;
        y2 = leftTopPosY + chessSize;
        len = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
        if (len < offset) {
            pos->row = row + 1;
            pos->col = col;
            if (chessMap[pos->row][pos->col] == 0) {  //还要确认此位置没有棋子
                ret = true;
            }
            break;
        }

        //右下角判断
        x2 = leftTopPosX + chessSize;
        y2 = leftTopPosY + chessSize;
        len = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
        if (len < offset) {
            pos->row = row + 1;
            pos->col = col + 1;
            if (chessMap[pos->row][pos->col] == 0) {  //还要确认此位置没有棋子，没占位才返回true
                ret = true;
            }
            break;
        }

    } while (0);
    //有效点击ret返回true，否则返回false
    return ret;
}

void Chess::chessDown(ChessPos* pos, chess_kind_t kind)
{
    //落子音效
    mciSendString("play res/waterClick.wav", 0, 0, 0);

    int x = margin_x + chessSize * (pos->col - 0.5);
    int y = margin_y + chessSize * (pos->row - 0.5);

    if (kind == CHESS_WHITE) {
        putimagePNG(x, y, &chessWhiteImg);
    }
    else {
        putimagePNG(x, y, &chessBlackImg);
    }
    //更新棋盘数据
    updateGameMap(pos);
}

int Chess::getGradeSize()
{
    return gradeSize;
}

int Chess::getChessData(ChessPos* pos)
{
    return chessMap[pos->row][pos->col];
}

int Chess::getChessData(int row, int col)
{
    return chessMap[row][col];
}

bool Chess::checkOver(int* r)
{
    if (checkWin()) {
        //mciSendString("",0,0,0)  //想加倒计时
        Sleep(700);
        mciSendString("play res/coin.wav", 0, 0, 0);
        Sleep(5100);
        if (playerFlag == false) {  //到白棋下棋了，说明黑棋赢了

            //showBlackWinImage
            loadimage(0, "res/bWin.png", 1106, 682, true);

            //recordResult
            *r = CHESS_BLACK;
        }
        else {  //到黑棋下棋了，说明白棋赢了
            
            //showWhiteWinImage
            loadimage(0, "res/wWin.png", 1106, 682, true);

            //recordResult
            *r = CHESS_WHITE;
        }
        return true; //结束返回真
    }
    return false; //没结束返回假
}

void Chess::updateGameMap(ChessPos* pos)
{
    lastPos = *pos;
    chessMap[pos->row][pos->col] = playerFlag ? CHESS_BLACK : CHESS_WHITE;
    playerFlag = !playerFlag; //黑白方交换行棋
}

bool Chess::checkWin()
{
    //最近落子点行列信息
    int row = lastPos.row;
    int col = lastPos.col;

    for (int i = 0; i < 5; i++) {
        
        //水平判断
        if (col - i >= 0 && col - i +4 < gradeSize &&
            chessMap[row][col - i] == chessMap[row][col - i + 1] &&
            chessMap[row][col - i] == chessMap[row][col - i + 2] &&
            chessMap[row][col - i] == chessMap[row][col - i + 3] &&
            chessMap[row][col - i] == chessMap[row][col - i + 4]) {
            return true;
        }

        //垂直判断
        if (row - i >= 0 && row - i + 4 < gradeSize &&
            chessMap[row - i][col] == chessMap[row - i + 1][col] &&
            chessMap[row - i][col] == chessMap[row - i + 2][col] &&
            chessMap[row - i][col] == chessMap[row - i + 3][col] &&
            chessMap[row - i][col] == chessMap[row - i + 4][col]) {
            return true;
        }

        //'/'方向判断
        if (row + i -4 >= 0 && row + i < gradeSize &&
            col - i >= 0 && col - i + 4 < gradeSize &&
            chessMap[row + i][col - i] == chessMap[row + i - 1][col - i + 1] &&
            chessMap[row + i][col - i] == chessMap[row + i - 2][col - i + 2] &&
            chessMap[row + i][col - i] == chessMap[row + i - 3][col - i + 3] &&
            chessMap[row + i][col - i] == chessMap[row + i - 4][col - i + 4]) {
            return true;
        }

        //'\'方向判断
        if (row - i >= 0 && row - i + 4 < gradeSize &&
            col - i >= 0 && col - i + 4 < gradeSize &&
            chessMap[row - i][col - i] == chessMap[row - i + 1][col - i + 1] &&
            chessMap[row - i][col - i] == chessMap[row - i + 2][col - i + 2] &&
            chessMap[row - i][col - i] == chessMap[row - i + 3][col - i + 3] &&
            chessMap[row - i][col - i] == chessMap[row - i + 4][col - i + 4]) {
            return true;
        }
    }

    return false;
}
