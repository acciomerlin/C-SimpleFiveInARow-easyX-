#include "ChessGame.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <shlobj.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")


void putimagePNG(int x, int y, IMAGE* picture) //xΪ����ͼƬ��X���꣬yΪY����
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

ChessGame::ChessGame(Player* player, AI* ai, Chess* chess)
{
	this->player = player;
	this->ai = ai;
	this->chess = chess;

	player->init(chess);
	ai->init(chess);
}

//��Ϸ��ʼ����
void ChessGame::startup()
{
	//���ű�������
	mciSendString("play res/start.wav", 0, 0, 0);

    //��Ϸ��ʼ����ͼ��
    loadimage(0, "res/menu.png", 1106, 682, true);  //̫���ˣ��ȱ���70%;����û����putImage�Ǹо���΢���С�ӳٸв�ϲ��
	loadimage(&soundOn, "res/sound.png", 56, 60, true);
	loadimage(&soundOff, "res/soundOff.png", 61, 63, true);

	//������Чͼ��
	putimage(15, 610, &soundOn);


    //��ʼ����Ŀ¼
    int choice = menu();

	//����
	if (choice != CHESS_EXIT && choice != CHESS_RECORD) {
		tips();
	}

    // �����û�ѡ��������Ϸ
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

	//����ѭ��
	while (1) {

		//Ŀ¼ѡ�񡢽����¼����ָ���ʼ��
		int* forMenu = new int;
		*forMenu = 0;
		int* thisRecord = new int;
		*thisRecord = 0;

		//��������
		player->go(forMenu);

		//�������Ŀ¼
		if (*forMenu == 1) {
			//backToMenu
			delete forMenu;
			startup();
		}

		if (chess->checkOver(thisRecord)) {

			//����PVP�����¼
			if (*thisRecord == CHESS_BLACK || *thisRecord == CHESS_WHITE) {
				recordsPVP.push_back(*thisRecord);
				delete thisRecord; //���ָ��
			}

			//��������Ŀ¼
			MOUSEMSG ms;
			int choice = 0;
			while (choice == 0) {
				ms = GetMouseMsg();
				//����Ŀ¼/���¿�ʼ/�˳���ť
				if (ms.uMsg == WM_LBUTTONDOWN) {
					mciSendString("play res/electricClick.wav", 0, 0, 0);
					if (ms.x >= 895 && ms.x <= 1065 && ms.y >= 328 && ms.y <= 405) {
						// ���¿�ʼ
						choice = CHESS_RESTART;
					}
					else if (ms.x >= 895 && ms.x <= 1065 && ms.y >= 437 && ms.y <= 514) {
						// ����Ŀ¼
						choice = CHESS_MENU;
					}
					else if (ms.x >= 895 && ms.x <= 1065 && ms.y >= 546 && ms.y <= 622) {
						// �˳���Ϸ
						choice = CHESS_EXIT;
					}
				}
			}

			//����ѡ����
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

	//����ѭ��
	while (1) {

		//Ŀ¼ѡ�񡢽����¼����ָ���ʼ��
		int* forMenu = new int;
		*forMenu = 0;
		int* thisRecord = new int;
		*thisRecord = 0;

		//�������
		player->go(forMenu);

		//�������Ŀ¼
		if (*forMenu == 1) {
			//backToMenu
			delete forMenu;
			startup();
		}

		if (chess->checkOver(thisRecord)) {

			//����PVE�����¼
			if (*thisRecord == CHESS_BLACK || *thisRecord == CHESS_WHITE) {
				recordsPVE.push_back(*thisRecord);
				delete thisRecord; //���ָ��
			}

			//��������Ŀ¼
			MOUSEMSG ms;
			int choice = 0;
			while (choice == 0) {
				ms = GetMouseMsg();
				//����Ŀ¼/���¿�ʼ/�˳���ť
				if (ms.uMsg == WM_LBUTTONDOWN) {
					mciSendString("play res/electricClick.wav", 0, 0, 0);
					if (ms.x >= 895 && ms.x <= 1065 && ms.y >= 328 && ms.y <= 405) {
						// ���¿�ʼ
						choice = CHESS_RESTART;
					}
					else if (ms.x >= 895 && ms.x <= 1065 && ms.y >= 437 && ms.y <= 514) {
						// ����Ŀ¼
						choice = CHESS_MENU;
					}
					else if (ms.x >= 895 && ms.x <= 1065 && ms.y >= 546 && ms.y <= 622) {
						// �˳���Ϸ
						choice = CHESS_EXIT;
					}
				}
			}

			//����ѡ����
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

		//AI��
		ai->go();
		if (chess->checkOver(thisRecord)) {

			//����PVP�����¼
			if (*thisRecord == CHESS_BLACK || *thisRecord == CHESS_WHITE) {
				recordsPVE.push_back(*thisRecord);
				delete thisRecord; //���ָ��
			}

			//��������Ŀ¼
			MOUSEMSG ms;
			int choice = 0;
			while (choice == 0) {
				ms = GetMouseMsg();
				//����Ŀ¼/���¿�ʼ/�˳���ť
				if (ms.uMsg == WM_LBUTTONDOWN) {
					mciSendString("play res/electricClick.wav", 0, 0, 0);
					if (ms.x >= 895 && ms.x <= 1065 && ms.y >= 328 && ms.y <= 405) {
						// ���¿�ʼ
						choice = CHESS_RESTART;
					}
					else if (ms.x >= 895 && ms.x <= 1065 && ms.y >= 437 && ms.y <= 514) {
						// ����Ŀ¼
						choice = CHESS_MENU;
					}
					else if (ms.x >= 895 && ms.x <= 1065 && ms.y >= 546 && ms.y <= 622) {
						// �˳���Ϸ
						choice = CHESS_EXIT;
					}
				}
			}

			//����ѡ����
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
					mciSendString("play res/electricClick.wav", 0, 0, 0);  //��ť�����Ч
					mciSendString("stop res/start.wav", 0, 0, 0);      //ֹͣ��ʼ��������
                    choice = CHESS_PVP; // ˫��ģʽ
                }
                else if (m.x >= 896 && m.x <= 1067 && m.y >= 435 && m.y <= 512) {
					mciSendString("play res/electricClick.wav", 0, 0, 0);
					mciSendString("stop res/start.wav", 0, 0, 0);      //ֹͣ��ʼ��������
                    choice = CHESS_PVE; // �˻�ģʽ
                }
                else if (m.x >= 896 && m.x <= 1067 && m.y >= 542 && m.y <= 620) {
					mciSendString("play res/electricClick.wav", 0, 0, 0);
					mciSendString("stop res/start.wav", 0, 0, 0);      //ֹͣ��ʼ��������
                    choice = CHESS_EXIT; // �˳�
                }
				else if (m.x >= 1024 && m.x <= 1102 && m.y >= 6 && m.y <= 102) {
					mciSendString("play res/electricClick.wav", 0, 0, 0);
					mciSendString("stop res/start.wav", 0, 0, 0);      //ֹͣ��ʼ��������
					choice = CHESS_RECORD; // ���������¼
				}
            }

			//��Ч�����رհ�ť
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
	//��Ϸ��¼������ʾ
	recordShow();

	//��ť��Ϣ����
	int choice = 0;
	while (choice == 0) {
		if (MouseHit()) {
			MOUSEMSG m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN) {
				if (m.x >= 896 && m.x <= 1068 && m.y >= 332 && m.y <= 410) {
					mciSendString("play res/electricClick.wav", 0, 0, 0);  //��ť�����Ч
					choice = CHESS_CLEAR; // ���record
				}
				else if (m.x >= 896 && m.x <= 1068 && m.y >= 436 && m.y <= 516) {
					mciSendString("play res/electricClick.wav", 0, 0, 0);
					choice = CHESS_DOWNLOAD; // ������Ϸ��¼
				}
				else if (m.x >= 896 && m.x <= 1068 && m.y >= 542 && m.y <= 622) {
					mciSendString("play res/electricClick.wav", 0, 0, 0);
					choice = CHESS_MENU; // �ص�Ŀ¼
				}
			}
		}
	}

	//��ť����ʵ��
	if (choice == CHESS_CLEAR) {
		//clear����vector+ˢ�½���
		vector<int> emptyVec1 = { 0 };
		vector<int> emptyVec2= { 0 };
		swap(recordsPVP, emptyVec1);
		swap(recordsPVE, emptyVec2);
		recordPlace();
	}
	else if (choice == CHESS_DOWNLOAD) {
		//��¼����
		recordDownload();
		//�ص���¼Ŀ¼
		recordPlace();
	}
	else if (choice == CHESS_MENU) {
		//�ص�Ŀ¼����Ϊû�п��ֻ��˲����������Ӱ��
		startup();
	}
}

void ChessGame::recordShow()
{
	IMAGE blackChess;
	IMAGE whiteChess;
	//�е�record����
	loadimage(0, "res/record.png", 1106, 682, true);
	loadimage(&blackChess, "res/black.png", 49, 49, true);
	loadimage(&whiteChess, "res/white.png", 49, 49, true);

	//���ذ񵥻���
	int cPVP = recordsPVP.size() - 1; //��Ϊ��ʼ�������ӣ�vector<int> recordsPVP{ 0 };����PVPcountsҪ��1
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
	// ����ʹ�û�����WINDOWSѡ��Ŀ¼SHBrowseForFolder�����û�����ѡ�񱣴�·��

	//��ʼ��COM���ʼ��Component Object Model(COM) �⣬
	//�����ڲ���Windowsϵͳ�����һ��������ͨ��ʹ��COMǰҪ��ʼ����
	CoInitialize(NULL);

	//BROWSEINFO��һ�����ڶ����ļ�������Ի������Ժ���Ϊ�Ľṹ��
	//ͨ�������뺯��SHBrowseForFolder()һ��ʹ�ã��Դ���һ�������û�ѡ���ļ��еĶԻ���
	//lpszTitle�ַ�������ʾ�������
	//LPITEMIDLIST ��һ��ָ����Ŀ��ʶ�б�Item ID List����ָ��
	//���ڱ�ʶ�û����ļ���ѡ��Ի�������ѡ���ļ��У���ʼĬ�Ͽ�
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = "��ѡ���ļ���";
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  //����bi�����ô��ļ��Ի���

	if (pidl != NULL) {  //ѡ���ļ��в�ȷ����
		char filePath[MAX_PATH];
		if (SHGetPathFromIDList(pidl, filePath)) {
			strcat_s(filePath, "\\records.txt");  //��ָ��·������

			// ����ļ��Ƿ��Ѵ���
			if (PathFileExists(filePath)) {
				int result = MessageBox(NULL, "�ļ��Ѵ��ڣ��Ƿ񸲸ǣ�", "����ȷ��", MB_YESNO | MB_ICONQUESTION);
				if (result == IDNO) {
					return;  //ѡ�񲻸��ǣ�����
				}
			}

			//std::cout << "�û�ѡ����ļ���·��: " << filePath << std::endl; //������

			// ʹ�� ofstream ������д���ı��ļ�
			ofstream outFile(filePath);
			outFile << "*************THAT'S ALL RECORDS******************" << endl;
			outFile << "====================PVP==========================" << endl;
			int size = recordsPVP.size();
			if (size <= 1) {
				outFile << "NO RECORDS CURRENTLY" << endl;
			}
			else {
				for (int i = 1; i <= size - 1; i++) {  //vector��ʼ����{0}����Ҫȥ��һλ
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
				for (int i = 1; i <= size - 1; i++) {  //vector��ʼ����{0}����Ҫȥ��һλ
					string winner = (recordsPVE[i] == 1) ? "BLACK(player)" : "WHITE(ai)";
					outFile << "R[" << i << "]" << ": " << winner << " wins!" << endl;
				}
			}
			outFile << "*************THAT'S ALL RECORDS******************" << endl;

			outFile.close();

			MessageBox(NULL, "�ļ�����ɹ���", "����ɹ�", MB_OK | MB_ICONINFORMATION);
		}

		// �ͷ��ڴ�
		CoTaskMemFree(pidl);
	}

	// �ͷ�COM��
	CoUninitialize();
}



