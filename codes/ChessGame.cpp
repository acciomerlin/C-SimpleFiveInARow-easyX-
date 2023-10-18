#include "ChessGame.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <shlobj.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")


void putimagePNG(int x, int y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
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

ChessGame::ChessGame(Player* player, AI* ai, Chess* chess)
{
	this->player = player;
	this->ai = ai;
	this->chess = chess;

	player->init(chess);
	ai->init(chess);
}

//游戏开始界面
void ChessGame::startup()
{
	//播放背景音乐
	mciSendString("play res/start.wav", 0, 0, 0);

    //游戏开始界面图像
    loadimage(0, "res/menu.png", 1106, 682, true);  //太大了，等比缩70%;背景没有用putImage是感觉有微妙的小延迟感不喜欢
	loadimage(&soundOn, "res/sound.png", 56, 60, true);
	loadimage(&soundOff, "res/soundOff.png", 61, 63, true);

	//增加音效图标
	putimage(15, 610, &soundOn);


    //开始界面目录
    int choice = menu();

	//弹窗
	if (choice != CHESS_EXIT && choice != CHESS_RECORD) {
		tips();
	}

    // 根据用户选择启动游戏
    if (choice == CHESS_PVP) {
		playPVP();
    }
    else if (choice == CHESS_PVE) {
        playPVE();
    }
    else if (choice == CHESS_EXIT) {
		exit(0);
    }
	else if (choice == CHESS_RECORD) {
		recordPlace();
	}

}

void ChessGame::playPVP()
{
	chess->init();

	//下棋循环
	while (1) {

		//目录选择、结果记录所需指针初始化
		int* forMenu = new int;
		*forMenu = 0;
		int* thisRecord = new int;
		*thisRecord = 0;

		//黑棋先走
		player->go(forMenu);

		//如果按了目录
		if (*forMenu == 1) {
			//backToMenu
			delete forMenu;
			startup();
		}

		if (chess->checkOver(thisRecord)) {

			//本场PVP结果记录
			if (*thisRecord == CHESS_BLACK || *thisRecord == CHESS_WHITE) {
				recordsPVP.push_back(*thisRecord);
				delete thisRecord; //清除指针
			}

			//结束界面目录
			MOUSEMSG ms;
			int choice = 0;
			while (choice == 0) {
				ms = GetMouseMsg();
				//返回目录/重新开始/退出按钮
				if (ms.uMsg == WM_LBUTTONDOWN) {
					mciSendString("play res/electricClick.wav", 0, 0, 0);
					if (ms.x >= 895 && ms.x <= 1065 && ms.y >= 328 && ms.y <= 405) {
						// 重新开始
						choice = CHESS_RESTART;
					}
					else if (ms.x >= 895 && ms.x <= 1065 && ms.y >= 437 && ms.y <= 514) {
						// 返回目录
						choice = CHESS_MENU;
					}
					else if (ms.x >= 895 && ms.x <= 1065 && ms.y >= 546 && ms.y <= 622) {
						// 退出游戏
						choice = CHESS_EXIT;
					}
				}
			}

			//根据选择处理
			if (choice == CHESS_RESTART) {
				//restart
				chess->init();
				continue;
			}
			else if (choice == CHESS_MENU) {
				//backMenu
				startup();
			}
			else if (choice == CHESS_EXIT) {
				//exit
				exit(0);
			}
		}
	}
}

void ChessGame::playPVE()
{
	chess->init();

	//下棋循环
	while (1) {

		//目录选择、结果记录所需指针初始化
		int* forMenu = new int;
		*forMenu = 0;
		int* thisRecord = new int;
		*thisRecord = 0;

		//玩家先走
		player->go(forMenu);

		//如果按了目录
		if (*forMenu == 1) {
			//backToMenu
			delete forMenu;
			startup();
		}

		if (chess->checkOver(thisRecord)) {

			//本场PVE结果记录
			if (*thisRecord == CHESS_BLACK || *thisRecord == CHESS_WHITE) {
				recordsPVE.push_back(*thisRecord);
				delete thisRecord; //清除指针
			}

			//结束界面目录
			MOUSEMSG ms;
			int choice = 0;
			while (choice == 0) {
				ms = GetMouseMsg();
				//返回目录/重新开始/退出按钮
				if (ms.uMsg == WM_LBUTTONDOWN) {
					mciSendString("play res/electricClick.wav", 0, 0, 0);
					if (ms.x >= 895 && ms.x <= 1065 && ms.y >= 328 && ms.y <= 405) {
						// 重新开始
						choice = CHESS_RESTART;
					}
					else if (ms.x >= 895 && ms.x <= 1065 && ms.y >= 437 && ms.y <= 514) {
						// 返回目录
						choice = CHESS_MENU;
					}
					else if (ms.x >= 895 && ms.x <= 1065 && ms.y >= 546 && ms.y <= 622) {
						// 退出游戏
						choice = CHESS_EXIT;
					}
				}
			}

			//根据选择处理
			if (choice == CHESS_RESTART) {
				//restart
				chess->init();
				continue;
			}
			else if (choice == CHESS_MENU) {
				//backMenu
				startup();
			}
			else if (choice == CHESS_EXIT) {
				//exit
				exit(0);
			}
		}

		//AI走
		ai->go();
		if (chess->checkOver(thisRecord)) {

			//本场PVP结果记录
			if (*thisRecord == CHESS_BLACK || *thisRecord == CHESS_WHITE) {
				recordsPVE.push_back(*thisRecord);
				delete thisRecord; //清除指针
			}

			//结束界面目录
			MOUSEMSG ms;
			int choice = 0;
			while (choice == 0) {
				ms = GetMouseMsg();
				//返回目录/重新开始/退出按钮
				if (ms.uMsg == WM_LBUTTONDOWN) {
					mciSendString("play res/electricClick.wav", 0, 0, 0);
					if (ms.x >= 895 && ms.x <= 1065 && ms.y >= 328 && ms.y <= 405) {
						// 重新开始
						choice = CHESS_RESTART;
					}
					else if (ms.x >= 895 && ms.x <= 1065 && ms.y >= 437 && ms.y <= 514) {
						// 返回目录
						choice = CHESS_MENU;
					}
					else if (ms.x >= 895 && ms.x <= 1065 && ms.y >= 546 && ms.y <= 622) {
						// 退出游戏
						choice = CHESS_EXIT;
					}
				}
			}

			//根据选择处理
			if (choice == CHESS_RESTART) {
				//restart
				chess->init();
				continue;
			}
			else if (choice == CHESS_MENU) {
				//backMenu
				startup();
			}
			else if (choice == CHESS_EXIT) {
				//exit
				exit(0);
			}
		}

	}
}

int ChessGame::menu()
{
    int choice = 0;
	bool soundflag = true;

    while (choice == 0) {
        if (MouseHit()) {
            MOUSEMSG m = GetMouseMsg();
            if (m.uMsg == WM_LBUTTONDOWN) {
                if (m.x >= 896 && m.x <= 1067 && m.y >= 326 && m.y <= 404) {
					mciSendString("play res/electricClick.wav", 0, 0, 0);  //按钮点击音效
					mciSendString("stop res/start.wav", 0, 0, 0);      //停止开始界面音乐
                    choice = CHESS_PVP; // 双人模式
                }
                else if (m.x >= 896 && m.x <= 1067 && m.y >= 435 && m.y <= 512) {
					mciSendString("play res/electricClick.wav", 0, 0, 0);
					mciSendString("stop res/start.wav", 0, 0, 0);      //停止开始界面音乐
                    choice = CHESS_PVE; // 人机模式
                }
                else if (m.x >= 896 && m.x <= 1067 && m.y >= 542 && m.y <= 620) {
					mciSendString("play res/electricClick.wav", 0, 0, 0);
					mciSendString("stop res/start.wav", 0, 0, 0);      //停止开始界面音乐
                    choice = CHESS_EXIT; // 退出
                }
				else if (m.x >= 1024 && m.x <= 1102 && m.y >= 6 && m.y <= 102) {
					mciSendString("play res/electricClick.wav", 0, 0, 0);
					mciSendString("stop res/start.wav", 0, 0, 0);      //停止开始界面音乐
					choice = CHESS_RECORD; // 最近几场记录
				}
            }

			//音效开启关闭按钮
			if (m.x >= 15 && m.x <= 75 && m.y >= 610 && m.y <= 666) {
				if (m.uMsg == WM_LBUTTONDOWN) {
					if (soundflag == true) {
						putimage(15, 610, &soundOff);
						mciSendString("pause res/start.wav", 0, 0, 0);
						soundflag = false;
					}
					else if (soundflag == false) {
						putimage(15, 610, &soundOn);
						mciSendString("resume res/start.wav", 0, 0, 0);
						soundflag = true;
					}
				}
			}
        }
    }
    return choice;
}

void ChessGame::tips()
{
	bool gotit = true;
	loadimage(&tip, "res/tip.png", 548, 331, true);
	putimage(266, 170, &tip);
	while (gotit) {
		if (MouseHit()) {
			MOUSEMSG m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN && m.x >= 456 && m.x <= 618 && m.y >= 430 && m.y <= 474) {
				mciSendString("play res/electricClick.wav", 0, 0, 0);
				gotit = false;
			}
		}
	}
}

void ChessGame::recordPlace()
{
	//游戏记录界面显示
	recordShow();

	//按钮消息接收
	int choice = 0;
	while (choice == 0) {
		if (MouseHit()) {
			MOUSEMSG m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN) {
				if (m.x >= 896 && m.x <= 1068 && m.y >= 332 && m.y <= 410) {
					mciSendString("play res/electricClick.wav", 0, 0, 0);  //按钮点击音效
					choice = CHESS_CLEAR; // 清空record
				}
				else if (m.x >= 896 && m.x <= 1068 && m.y >= 436 && m.y <= 516) {
					mciSendString("play res/electricClick.wav", 0, 0, 0);
					choice = CHESS_DOWNLOAD; // 下载游戏记录
				}
				else if (m.x >= 896 && m.x <= 1068 && m.y >= 542 && m.y <= 622) {
					mciSendString("play res/electricClick.wav", 0, 0, 0);
					choice = CHESS_MENU; // 回到目录
				}
			}
		}
	}

	//按钮功能实现
	if (choice == CHESS_CLEAR) {
		//clear两个vector+刷新界面
		vector<int> emptyVec1 = { 0 };
		vector<int> emptyVec2= { 0 };
		swap(recordsPVP, emptyVec1);
		swap(recordsPVE, emptyVec2);
		recordPlace();
	}
	else if (choice == CHESS_DOWNLOAD) {
		//记录下载
		recordDownload();
		//回到记录目录
		recordPlace();
	}
	else if (choice == CHESS_MENU) {
		//回到目录，因为没有开局回退不会对棋局造成影响
		startup();
	}
}

void ChessGame::recordShow()
{
	IMAGE blackChess;
	IMAGE whiteChess;
	//切到record界面
	loadimage(0, "res/record.png", 1106, 682, true);
	loadimage(&blackChess, "res/black.png", 49, 49, true);
	loadimage(&whiteChess, "res/white.png", 49, 49, true);

	//加载榜单画面
	int cPVP = recordsPVP.size() - 1; //因为初始化这样子：vector<int> recordsPVP{ 0 };所以PVPcounts要减1
	int cPVE = recordsPVE.size() - 1;
	if (recordsPVP[cPVP] != 0) {
		int i = 0;
		while (cPVP > i && i < 5) {
			IMAGE winChess = (recordsPVP[cPVP - i] == 1) ? blackChess : whiteChess;
			putimagePNG(147, 199 + 78.8 * i, &winChess);
			i++;
		}
	}
	if (recordsPVE[cPVE] != 0) {
		int i = 0;
		while (cPVE > i && i < 5) {
			IMAGE winChess = (recordsPVE[cPVE - i] == 1) ? blackChess : whiteChess;
			putimagePNG(391, 199 + 78.8 * i, &winChess);
			i++;
		}
	}
}

void ChessGame::recordDownload()
{
	// 这里使用基础的WINDOWS选择目录SHBrowseForFolder来让用户可以选择保存路径

	//初始化COM库初始化Component Object Model(COM) 库，
	//是用于操作Windows系统组件的一个东西，通常使用COM前要初始化。
	CoInitialize(NULL);

	//BROWSEINFO是一个用于定义文件夹浏览对话框属性和行为的结构体
	//通常用于与函数SHBrowseForFolder()一起使用，以创建一个允许用户选择文件夹的对话框。
	//lpszTitle字符串，表示框体标题
	//LPITEMIDLIST 是一个指向项目标识列表（Item ID List）的指针
	//用于标识用户在文件夹选择对话框中所选的文件夹，初始默认空
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = "请选择文件夹";
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  //根据bi的设置打开文件对话框

	if (pidl != NULL) {  //选好文件夹并确定了
		char filePath[MAX_PATH];
		if (SHGetPathFromIDList(pidl, filePath)) {
			strcat_s(filePath, "\\records.txt");  //在指定路径下载

			// 检查文件是否已存在
			if (PathFileExists(filePath)) {
				int result = MessageBox(NULL, "文件已存在，是否覆盖？", "覆盖确认", MB_YESNO | MB_ICONQUESTION);
				if (result == IDNO) {
					return;  //选择不覆盖，返回
				}
			}

			//std::cout << "用户选择的文件夹路径: " << filePath << std::endl; //调试用

			// 使用 ofstream 创建并写入文本文件
			ofstream outFile(filePath);
			outFile << "*************THAT'S ALL RECORDS******************" << endl;
			outFile << "====================PVP==========================" << endl;
			int size = recordsPVP.size();
			if (size <= 1) {
				outFile << "NO RECORDS CURRENTLY" << endl;
			}
			else {
				for (int i = 1; i <= size - 1; i++) {  //vector初始化用{0}所以要去掉一位
					string winner = (recordsPVP[i] == 1) ? "BLACK" : "WHITE";
					outFile << "R[" << i << "]" << ": " << winner << " wins!" << endl;
				}
			}
			outFile << "====================PVE==========================" << endl;
			size = recordsPVE.size();
			if (size <= 1) {
				outFile << "NO RECORDS CURRENTLY" << endl;
			}
			else {
				for (int i = 1; i <= size - 1; i++) {  //vector初始化用{0}所以要去掉一位
					string winner = (recordsPVE[i] == 1) ? "BLACK(player)" : "WHITE(ai)";
					outFile << "R[" << i << "]" << ": " << winner << " wins!" << endl;
				}
			}
			outFile << "*************THAT'S ALL RECORDS******************" << endl;

			outFile.close();

			MessageBox(NULL, "文件保存成功！", "保存成功", MB_OK | MB_ICONINFORMATION);
		}

		// 释放内存
		CoTaskMemFree(pidl);
	}

	// 释放COM库
	CoUninitialize();
}



