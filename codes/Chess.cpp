#include "Chess.h"
#include <math.h>
#include <mmsystem.h>
#include <conio.h>
#pragma comment(lib, "winmm.lib")

void Chess::putimagePNG(int x, int y, IMAGE* picture)
{
    // ������ʼ��
    DWORD* dst = GetImageBuffer();    // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
    DWORD* draw = GetImageBuffer();
    DWORD* src = GetImageBuffer(picture); //��ȡpicture���Դ�ָ��
    int picture_width = picture->getwidth(); //��ȡpicture�Ŀ�ȣ�EASYX�Դ�
    int picture_height = picture->getheight(); //��ȡpicture�ĸ߶ȣ�EASYX�Դ�
    int graphWidth = getwidth();       //��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
    int graphHeight = getheight();     //��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
    int dstX = 0;    //���Դ������صĽǱ�

    // ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
    for (int iy = 0; iy < picture_height; iy++)
    {
        for (int ix = 0; ix < picture_width; ix++)
        {
            int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
            int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
            int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
            int sg = ((src[srcX] & 0xff00) >> 8);   //G
            int sb = src[srcX] & 0xff;              //B
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
            {
                dstX = (ix + x) + (iy + y) * graphWidth; //���Դ������صĽǱ�
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
                    | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //��p=sa/255 , FP=sg , BP=dg
                    | (sb * sa / 255 + db * (255 - sa) / 255);              //��p=sa/255 , FP=sb , BP=db
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
    //ÿ���ؿ�һ��Ҫ���Flag
    playerFlag = CHESS_BLACK;

    //��ʾ����ͼƬ
    loadimage(0, "res/chessBoard.png", 1106, 682, true);

    //���غڰ���ͼƬ��chessBlack/WhiteImg
    loadimage(&chessBlackImg, "res/black.png", chessSize, chessSize, true);
    loadimage(&chessWhiteImg, "res/white.png", chessSize, chessSize, true);

    //ÿ���������
    for (int i = 0; i < gradeSize; i++) {
        for (int j = 0; j < gradeSize; j++) {
            chessMap[i][j] = 0;
        }
    }
}

bool Chess::clickBoard(int x, int y, ChessPos* pos)
{   
    int row = (y - margin_y) / chessSize;          //���Ͻǽڵ�λ�����̵�����ֵ
    int col = (x - margin_x) / chessSize;
    int leftTopPosY = margin_y + chessSize * row;  //���Ͻǽڵ�����ֵ
    int leftTopPosX = margin_x + chessSize * col;
    int offset = chessSize * 0.3;

    int len;
    bool ret = false;

    do {
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        //���Ͻ��ж�
        if (len < offset) {
            pos->row = row;
            pos->col = col;
            if (chessMap[pos->row][pos->col] == 0) {  //��Ҫȷ�ϴ�λ��û������
                ret = true;
            }
            break;
        }

        //���Ͻ��ж�
        int x2 = leftTopPosX + chessSize;
        int y2 = leftTopPosY;
        len = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
        if (len < offset) {
            pos->row = row;
            pos->col = col + 1;
            if (chessMap[pos->row][pos->col] == 0) {  //��Ҫȷ�ϴ�λ��û������
                ret = true;
            }
            break;
        }

        //���½��ж�
        x2 = leftTopPosX;
        y2 = leftTopPosY + chessSize;
        len = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
        if (len < offset) {
            pos->row = row + 1;
            pos->col = col;
            if (chessMap[pos->row][pos->col] == 0) {  //��Ҫȷ�ϴ�λ��û������
                ret = true;
            }
            break;
        }

        //���½��ж�
        x2 = leftTopPosX + chessSize;
        y2 = leftTopPosY + chessSize;
        len = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
        if (len < offset) {
            pos->row = row + 1;
            pos->col = col + 1;
            if (chessMap[pos->row][pos->col] == 0) {  //��Ҫȷ�ϴ�λ��û�����ӣ�ûռλ�ŷ���true
                ret = true;
            }
            break;
        }

    } while (0);
    //��Ч���ret����true�����򷵻�false
    return ret;
}

void Chess::chessDown(ChessPos* pos, chess_kind_t kind)
{
    //������Ч
    mciSendString("play res/waterClick.wav", 0, 0, 0);

    int x = margin_x + chessSize * (pos->col - 0.5);
    int y = margin_y + chessSize * (pos->row - 0.5);

    if (kind == CHESS_WHITE) {
        putimagePNG(x, y, &chessWhiteImg);
    }
    else {
        putimagePNG(x, y, &chessBlackImg);
    }
    //������������
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
        //mciSendString("",0,0,0)  //��ӵ���ʱ
        Sleep(700);
        mciSendString("play res/coin.wav", 0, 0, 0);
        Sleep(5100);
        if (playerFlag == false) {  //�����������ˣ�˵������Ӯ��

            //showBlackWinImage
            loadimage(0, "res/bWin.png", 1106, 682, true);

            //recordResult
            *r = CHESS_BLACK;
        }
        else {  //�����������ˣ�˵������Ӯ��
            
            //showWhiteWinImage
            loadimage(0, "res/wWin.png", 1106, 682, true);

            //recordResult
            *r = CHESS_WHITE;
        }
        return true; //����������
    }
    return false; //û�������ؼ�
}

void Chess::updateGameMap(ChessPos* pos)
{
    lastPos = *pos;
    chessMap[pos->row][pos->col] = playerFlag ? CHESS_BLACK : CHESS_WHITE;
    playerFlag = !playerFlag; //�ڰ׷���������
}

bool Chess::checkWin()
{
    //������ӵ�������Ϣ
    int row = lastPos.row;
    int col = lastPos.col;

    for (int i = 0; i < 5; i++) {
        
        //ˮƽ�ж�
        if (col - i >= 0 && col - i +4 < gradeSize &&
            chessMap[row][col - i] == chessMap[row][col - i + 1] &&
            chessMap[row][col - i] == chessMap[row][col - i + 2] &&
            chessMap[row][col - i] == chessMap[row][col - i + 3] &&
            chessMap[row][col - i] == chessMap[row][col - i + 4]) {
            return true;
        }

        //��ֱ�ж�
        if (row - i >= 0 && row - i + 4 < gradeSize &&
            chessMap[row - i][col] == chessMap[row - i + 1][col] &&
            chessMap[row - i][col] == chessMap[row - i + 2][col] &&
            chessMap[row - i][col] == chessMap[row - i + 3][col] &&
            chessMap[row - i][col] == chessMap[row - i + 4][col]) {
            return true;
        }

        //'/'�����ж�
        if (row + i -4 >= 0 && row + i < gradeSize &&
            col - i >= 0 && col - i + 4 < gradeSize &&
            chessMap[row + i][col - i] == chessMap[row + i - 1][col - i + 1] &&
            chessMap[row + i][col - i] == chessMap[row + i - 2][col - i + 2] &&
            chessMap[row + i][col - i] == chessMap[row + i - 3][col - i + 3] &&
            chessMap[row + i][col - i] == chessMap[row + i - 4][col - i + 4]) {
            return true;
        }

        //'\'�����ж�
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
