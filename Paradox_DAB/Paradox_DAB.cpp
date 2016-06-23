#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include "time.h"
#include "math.h"
#include "windows.h"

#include "Define.h"
#include "Paradox.h"
#include "BoxBoard.h"
#include "MCTS.h"

using namespace std;

void Model_AI_Game(Board &CB)
{
	int model;
	char red_name[20];
	char blue_name[20];
	void(*func_red)(Board&, int, bool);
	void(*func_blue)(Board&, int, bool);
	clock_t t_start, t_finish;		//���ü�ʱ���ı���
	double t_totaltime;				//���ü�ʱ���ı���
	srand((unsigned)time(NULL));	//����һ�����������


	//ѡ����ĵ��㷨
	for (;;)
	{
		cprintf("Ϊ�췽ѡ���㷨��\n�� 1.GameTurnMove_Rave ��\n�� 2.GameTurnMove_RaveSolverDouble ��\n�� 3.UCT�㷨 ��\n�� 4.UCT�㷨_2 ��\n�� 5.GameTurnMove_T ��\n�� 6.GameTurnMove ��\n\nѡ��Ϊ��", 12);
		cin >> model;
		if (model >= 1 && model <= 6)//���ǵø�������
		{
			if (model == 1)
			{
				func_red = GameTurnMove_Rave;
				strcpy_s(red_name, "GameTurnMove_Rave");
			}
			else if(model ==2)
			{
				func_red = RaveSolverDouble;
				strcpy_s(red_name, "RaveSolverDouble");
			}
			else if (model == 3)
			{
				func_red = UCTMove;
				strcpy_s(red_name, "UCT�㷨");
			}
			else if (model == 4)
			{
				func_red = UCTMove_2;
				strcpy_s(red_name, "UCT_2�㷨");
			}
			else if (model == 5)
			{
				func_red = GameTurnMove_T;
				strcpy_s(red_name, "GameTurnMove_T�㷨");
			}
			else if (model == 6)
			{
				func_red = GameTurnMove;
				strcpy_s(red_name, "GameTurnMove");
			}
			break;
		}
		else
		{
			cprintf("�������", 8);
		}
	}
	for (;;)
	{
		cprintf("Ϊ����ѡ���㷨��\n�� 1.GameTurnMove_Rave ��\n�� 2.RaveSolverDouble ��\n�� 3.UCT�㷨 ��\n�� 4.UCT�㷨_2 ��\n�� 5.GameTurnMove_T ��\n�� 6.GameTurnMove ��\n\nѡ��Ϊ��", 9);
		cin >> model;
		if (model >= 1 && model <= 6)//���ǵø�������
		{
			if (model == 1)
			{
				func_blue = GameTurnMove_Rave;
				strcpy_s(blue_name, "GameTurnMove_Rave");
			}
			else if(model ==2)
			{
				func_blue = RaveSolverDouble;
				strcpy_s(blue_name, "RaveSolverDouble");
			}
			else if (model == 3)
			{
				func_blue = UCTMove;
				strcpy_s(blue_name, "UCT�㷨");
			}
			else if (model == 4)
			{
				func_blue = UCTMove_2;
				strcpy_s(blue_name, "UCT_2�㷨");
			}
			else if (model == 5)
			{
				func_blue = GameTurnMove_T;
				strcpy_s(blue_name, "GameTurnMove_T�㷨");
			}
			else if (model == 6)
			{
				func_blue = GameTurnMove;
				strcpy_s(blue_name, "GameTurnMove");
			}
			break;
		}
		else
		{
			cprintf("�������", 8);
		}
	}

	//��ʼ����
	int red_count = 0, blue_count = 0;
	for (int n = 1; n<200; n++)
	{
		//system("pause");
		Board NewBoard = CB;
		NewBoard.PrintBoard();
		int player = 1 - (2 * (n % 2));//����Ϊ˫����������
		cout << "��" << n << "����ʼ" << endl;
		printf("%s����", player == RED ? "�췽" : "����");
		for (;;)
		{
			int w = 0, tz = 0;
			if (player == RED)
				cprintf("�췽 ", 12);
			else
				cprintf("���� ", 9);
			cout << "���ڿ��ǵ�" << NewBoard.Step + 1 << "��" << endl;
			cout << "����Ϊ �췽������[" << NewBoard.GetPlayerBoxes(RED) << "]  ����������[" << NewBoard.GetPlayerBoxes(BLUE) << "]" << endl;
			cout << "��[" << n << "]����";
			cprintf(red_name, 12);
			cout << "ʤ[" << red_count << "]�֣�";
			cprintf(blue_name, 9);
			cout << "ʤ[" << blue_count << "]��" << endl;

			//w = GetBoardWinner(NewBoard, -player);
			w = NewBoard.Winner();

			if (w == RED)
			{
				red_count++;
				break;
			}
			if (w == BLUE)
			{
				blue_count++;
				break;
			}
			t_start = clock();
			if (player == RED)
			{
				
				func_red(NewBoard, player, true);
			}
			else
			{
				
				func_blue(NewBoard, player, true);
			}
			t_finish = clock();
			t_totaltime = (double)(t_finish - t_start) / CLOCKS_PER_SEC;
			cout << "\n����ʱ��Ϊ" << t_totaltime << "�룡" << endl;
			player = -player;//����л�
			NewBoard.PrintBoard();//��ʾ����
			w = NewBoard.Winner();
			if (w == RED)
			{
				red_count++;
				break;
			}
			if (w == BLUE)
			{
				blue_count++;
				break;
			}
		}
	}
	cout << "ȫ���������\n\n\n�췽ʤ��" << red_count << "    ����ʤ��" << blue_count << endl;
}
void Model_Rnd_Test(Board &CB)
{
	srand((unsigned)time(NULL));//����һ�����������

	int player = RED;
	Board NewBoard = CB;
	NewBoard.PrintBoard();
	for (;;)
	{
		int w = 0, tz = 0;
		bool Cap = false;
		if (NewBoard.GetFreeEdgeNum()>0)
			RndFilterTurn(NewBoard, player, false, FILTER_RANGE);
		else
			RndTurn(NewBoard, player,false);
		//
		if (player == RED)
			cprintf("\n�췽�Ѿ�����\n", 12);
		else
			cprintf("\n�����Ѿ�����\n", 9);
		player = -player;//����л�
		NewBoard.PrintBoard();//��ʾ����
		
		system("pause");
		w = NewBoard.Winner();
		if (w == RED)
		{
			break;
		}
		if (w == BLUE)
		{
			break;
		}
	}
	cout << "\n\nRnd Test Finish\n\n" << endl;
	exit(0);
}
void Model_Board_Winner_Test()
{
	while (1)
	{
		cout << "New Test Start"<<endl;
		Board Test;
		Test.LoadBoard();
		int r = GetBoardWinner(Test, BLUE);
		int b = GetBoardWinner(Test, RED);
		if (r||b)
		{ 
			cout << "������Ϊ��ʱ��������ʤ����Ϊ";
			if (r == RED)
				cprintf("�췽", 12);
			else
				cprintf("����", 9);

			cout <<endl<< "������Ϊ��ʱ��������ʤ����Ϊ";
			if (b == RED)
				cprintf("�췽", 12);
			else
				cprintf("����", 9);
		}
		cout << endl;
		system("pause");
	}
}
void Model_Define_Board(Board &CB)
{
	CB.LoadBoard();
	cout << "Define Finish!" << endl;
	system("pause");
	system("CLS");
	
}
void Model_Define_Chain()
{
	while (1)
	{
		cout << "New Test Start" << endl;
		Board Test;
		Test.LoadBoard();
		GameTurnMove(Test, RED, true);
		system("pause");
		system("CLS");
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	Board CB;						//�½�һ����Ϸ����ʾ����CB
	clock_t start, finish;			//���ü�ʱ���ı���
	double totaltime;				//���ü�ʱ���ı���
	srand((unsigned)time(NULL));	//����һ�����������

	cout << "Board size from 488 to " << sizeof(Board) << endl;
	cout << "Node size from 1232 to " << sizeof(UCTNode) << endl;

	//��ʼ������Ϣ
	cprintf("��Paradox Dots And Boxes��\n\n", 14);
	cprintf("		Version:2.2.1. \n\n", 11);
	cprintf("��  0 ��Ĭ�ϲ��� \n��  1 ���㷨����ģʽ\n��  2 ���������ģʽ\n��  3 ������ʤ����\n��  4 ������\n���������Զ��岼��\n\n",10);
	cprintf("�治��dabble���ҷǳ�����",8);
	//ģʽѡ��
	int model;
	cin>>model;
	if(model==0)
		cprintf("Ĭ��ģʽ����",8);
	else if (model == 1)
		Model_AI_Game(CB);
	else if (model == 2)
		Model_Rnd_Test(CB);
	else if (model == 3)
		Model_Board_Winner_Test();
	else if (model == 4)
		Model_Define_Chain();
	else
		Model_Define_Board(CB);


	for (;;)
	{
		//�ȼ���Ƿ�ʤ��(��Ϊ��Ϸ�п��ܴ�һ���Ѿ�ʤ���ľ��濪ʼ)
		int W = CB.Winner();
		if (W == RED)
		{
			cout << "�췽ʤ������Ϸ�����ˡ�" << endl;
			CB.PrintBoard();
			system("pause");
			break;
		}
		if (W == BLUE)
		{
			cout << "����ʤ������Ϸ�����ˡ�" << endl;
			CB.PrintBoard();
			system("pause");
			break;
		}

		//Ȼ��ʼִ��˫���߲�
		CB.PrintBoard();
		cout << "\n��0���˹�ռ��  ��1��AIռ��" << endl;
		int com;
		for (;;)
		{
			int inputnumber;
			cin >> inputnumber;
			if (inputnumber == 0 || inputnumber == 1)
			{
				com = inputnumber;
				break;
			}
			cprintf("���������������������", 8);
		}
		if (com == 0)
		{
			system("cls");
			CB.PrintBoard();
			cout << "\n���������������ռ�췽����\"2,1,1\"" << endl;
			for (;;)
			{
				int locx, locy, owner;
				scanf_s("%d,%d,%d", &locx, &locy, &owner);
				if ((owner == 1 || owner == 2) && locx >= 0 && locx <= (LEN - 1) && locy >= 0 && locy <= (LEN - 1))//���������������뷶Χ
				{
					if (owner == 2)
						owner = -1;
					if ((IfOddNumber(locx) && IfEvenNumber(locy)) || (IfEvenNumber(locx) && IfOddNumber(locy)))//�����xyΪһ��������һ��ż��
					{
						bool HH = CB.Move(locx, locy, owner);
						if (HH)
							cout << owner << "ռ����һ�����ӣ�" << endl;
						break;
					}
					else
					{
						cout << "XY��Ӧ����һ��������һ��ż����" << endl;
					}
				}
				cprintf("���������������������\n", 8);
			}
		}
		else if (com == 1)
		{
			cout << "���������ӷ�(1����2)" << endl;
			for (;;)
			{
				int owner;
				cin >> owner;
				if (owner == RED || owner == 2)
				{
					if (owner == 2)
						owner = BLUE;
					//���Ե��ƶ�����
					start = clock();
					GameTurnMove(CB, owner, true);
					finish = clock();
					totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
					cout << "\n����ʱ��Ϊ" << totaltime << "�룡" << endl;
					break;
				}
				cprintf("���������������������", 8);
			}
		}
	}
	return 0;
}
