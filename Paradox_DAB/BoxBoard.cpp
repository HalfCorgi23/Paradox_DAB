#include "stdafx.h"
#include "Define.h"
#include <iostream>
#include <stdio.h>
#include "time.h"
#include "windows.h"
#include "BoxBoard.h"

using namespace std;

//BoxType
BoxType::BoxType()
{
	Type = 0;//Ĭ������
	BelongingChainNum = 0;//Ĭ��Ϊ�ؿյ�0�ſռ�
}

//ChainInfo
ChainInfo::ChainInfo()
{
	Type = NotDefine;				//����ΪΪ����
	ChainBoxNum = 0;				//�����������ڿ�ʼʱ��Ϊ0
	ConditionOfPreCircle = false;	//�Ƿ�Ԥ�������Ⱦ�����
}

//BoxBoard
BoxBoard::BoxBoard(Board NewB)
{
	//���ƻ�������
	for (int i = 0; i < LEN; i++)
	{
		for (int j = 0; j < LEN; j++)
		{
			board[i][j] = NewB.board[i][j];
		}
	}
	Step = NewB.Step;
	//�����������
	//DefineBoxesType();
}
BoxBoard::BoxBoard(int Array[LEN][LEN], int step)
{
	//���ƻ�������
	for (int i = 0; i < LEN; i++)
	{
		for (int j = 0; j < LEN; j++)
		{
			board[i][j] = Array[i][j];
		}
	}
	Step = step;
	//�����������
	//DefineBoxesType();
}
void BoxBoard::DefineBoxesType()
{
	for (int y = 1; y <= BOXLEN; y++)//������
	{
		for (int x = 1; x <= BOXLEN; x++)//������
		{
			int bx = (x * 2) - 1;//Board�е�����
			int by = (y * 2) - 1;//Board�е�����
			Boxes[x][y].BoxLoc.Set(bx, by);//��������
			//����Owner
			if (board[bx][by] == BOX)//�����û��ռ��
				Boxes[x][y].BoxOwner = 0;//δ��ռ��
			else
				Boxes[x][y].BoxOwner = board[bx][by] / 2;//���Զ�֮�����ֵ��ͬ����ұ��
			//����Type
			int bl = GetBoxLiberties(bx, by);//�õ����ӵ����ɶ�
			if (bl == 4)
				Boxes[x][y].Type = FREEBOX;	//�����ɸ�
			else
				Boxes[x][y].Type = bl;	//��Ȼ�͸����ɱߵ���������ͬ�ġ�
		}
	}
}
int BoxBoard::GetBoxType(int bx, int by)
{
	if (bx >= 1 && by >= 1 && bx <= BOXLEN&& by <= BOXLEN)//�������Ҫ��
		return Boxes[bx][by].Type;//���ظ�������
	return FREEBOX;//���೬����Χ�ľ��������ɸ�
}
void BoxBoard::ShowBoxType()
{
	for (int j = 1; j <= BOXLEN; j++)
	{
		for (int i = 1; i <= BOXLEN; i++)
		{
			if (Boxes[i][j].Type == FULLBOX)
				cprintf("��", 8);
			else if (Boxes[i][j].Type == DEADBOX)
				cprintf("��", 2);
			else if (Boxes[i][j].Type == CHAINBOX)
				cprintf("��", 14);
			else if (Boxes[i][j].Type == FREEBOX)
				cprintf("��", 5);
		}
		cout << endl;
	}
	cprintf("\n���ɸ�", 5);
	cprintf("\n����", 14);
	cprintf("\n����", 2);
	cprintf("\n�ѱ�ռ��\n", 8);
}
int BoxBoard::GetFirstEmptyChainNum()
{
	for (int i = 1; i < BOXNUM; i++)//������0�ſռ�
	{
		if (Chains[i].Type == NotDefine)
			return i;
	}
	cout << "���ռ䲻�㣡" << endl;
	system("pause");
	return 0;
}

//������
void BoxBoard::ShowBelongingChain()
{
	cout << "  ";
	for (int i = 0; i <= (LEN - 1); i++)
	{
		printf("%2d", i);
	}
	cout << "\n";
	for (int j = 0; j<LEN; j++)
	{
		printf("%2d", j);
		for (int i = 0; i<LEN; i++)
		{
			if (board[i][j] == BOX || board[i][j] == RED_BOX || board[i][j] == BLUE_BOX)
			{
				if (Boxes[(i + 1) / 2][(j + 1) / 2].BelongingChainNum != 0)
				{
					cprintf(" ", 8);
					cprintNum(14, Boxes[(i + 1) / 2][(j + 1) / 2].BelongingChainNum);
				}
				else
				{
					if (Boxes[(i + 1) / 2][(j + 1) / 2].BoxOwner != 0)
					{
						if (Boxes[(i + 1) / 2][(j + 1) / 2].BoxOwner == RED)
							cprintf("��", 12);
						else
							cprintf("��", 9);
					}
					else
						cprintf("  ", 8);
				}

			}
			else if (board[i][j] == EDGE)
			{
				if (IfOddNumber(j) && IfEvenNumber(i))
				{
					cout << "  ";
					//cprintf("��",15);
				}
				else
				{
					cout << "  ";
					//cprintf("��",15);
				}
			}
			else if (board[i][j] == RED_EDGE)
			{
				if (IfOddNumber(j) && IfEvenNumber(i))
				{
					cprintf("��", 12);
				}
				else
				{
					cprintf("��", 12);
				}
			}
			else if (board[i][j] == BLUE_EDGE)
			{
				if (IfOddNumber(j) && IfEvenNumber(i))
				{
					cprintf("��", 9);
				}
				else
				{
					cprintf("��", 9);
				}
			}
			else if (board[i][j] == EMPTY)
			{
				cprintf("��", 15);
			}
		}
		printf("%2d", j);
		cout << "\n";
	}
	cout << "  ";
	for (int i = 0; i <= (LEN - 1); i++)
	{
		printf("%2d", i);
	}
	cout << "\n";

	for (int i = 1; i < BOXNUM; i++)
	{
		if (Chains[i].Type != NotDefine)
		{
			cout << "Chain [" << i << "] Box [" << Chains[i].ChainBoxNum << "] Type [";
			if (Chains[i].Type == SingleChain)
				cout << "SingleChain]";
			else if (Chains[i].Type == ShortChain)
				cout << "ShortChain]";
			else if (Chains[i].Type == LongChain)
				cout << "LongChain]";
			else if (Chains[i].Type == PreCircle)
				cout << "PreCircle]";
			else if (Chains[i].Type == Circle)
				cout << "Circle]";
			else
			{
				cout << "Wrong";
				system("pause");
			}

			cout << "  PreChain = ";
			if (Chains[i].ConditionOfPreCircle)
				cout << "yes" << endl;
			else
				cout << "no" << endl;
		}

	}
}
void BoxBoard::InheritChain(int InheritorRegNum, int AncesterRegNum)
{
	//cout <<InheritorRegNum<< " inherit " << AncesterRegNum << endl;
	for (int j = 1; j <= BOXLEN; j++)
	{
		for (int i = 1; i <= BOXLEN; i++)
		{
			if (Boxes[i][j].BelongingChainNum == AncesterRegNum)
				Boxes[i][j].BelongingChainNum = InheritorRegNum;//���̳�
		}
	}
	Chains[InheritorRegNum].ChainBoxNum += Chains[AncesterRegNum].ChainBoxNum;//�������ۼ�
	Chains[InheritorRegNum].Type = LongChain;//�ض��ǳ�����

	//���ٱ��̳��ߵ�ע��ռ�
	Chains[AncesterRegNum].ChainBoxNum = 0;
	Chains[AncesterRegNum].Type = NotDefine;
}
void BoxBoard::RegisterChain(LOC FreeBoxLoc, LOC FirstLoc)
{
	//��̬ע��������һ�����ӳ��� ����һ�����ӿ�ʼ��������ע�ᡣһ��������ɸ����

	int x = FreeBoxLoc.x;				//��ʼXֵ
	int y = FreeBoxLoc.y;				//��ʼYֵ
	LOC Loc = FreeBoxLoc;				//�����������
	LOC NewLoc = FirstLoc;				//����������
	int Re = GetFirstEmptyChainNum();	//��ȡһ���հ׵����ռ�
	Chains[Re].StartLoc = Loc;			//���Ϊĳ�����ɸ�
	Chains[Re].ChainBoxNum = 0;			//�������
	Chains[Re].Type = NotDefine;

	//�����¹����У�NewLoc����ʵ������ÿһ�����ӵ����ꡣ������������������ʱ���ж�
	for (int i = 0; i < BOXNUM; i++)
	{
		if (GetBoxType(NewLoc.x, NewLoc.y) != CHAINBOX)//������������ʱ���ж��������͡���ʱ��NewLocΪ���������յ������
		{
			Chains[Re].EndLoc = NewLoc;//���������յ�
			if (NewLoc.x == FreeBoxLoc.x&&NewLoc.y == FreeBoxLoc.y&&Boxes[NewLoc.x][NewLoc.y].BelongingChainNum == EMPTY)
			{
				//���յ���������������ͬ���Ҹø�δ���϶����ͣ����϶�ΪPreCircle
				Chains[Re].Type = PreCircle;
				Boxes[NewLoc.x][NewLoc.y].BelongingChainNum = Re;//�����ø�����Ҳ��Ҫ��ע��
				Chains[Re].ChainBoxNum++;
			}
			else//�����յ㲻һ������Ϊ����������Ŀ�ж�������
			{
				if (Chains[Re].ChainBoxNum == 1)
					Chains[Re].Type = SingleChain;
				else if (Chains[Re].ChainBoxNum == 2)
					Chains[Re].Type = ShortChain;
				else
					Chains[Re].Type = LongChain;
			}
			break;
		}
		else//ÿ�ѵ�һ���µĸ���
		{
			LOC mLoc = NewLoc;
			//������ӵ���Ϣ
			Boxes[NewLoc.x][NewLoc.y].BelongingChainNum = Re;	//�����ҹ�
			Chains[Re].ChainBoxNum++;							//���ĸ�����Ŀ����1
			NewLoc = FindNextBox(NewLoc, Loc);					//Ѱ����һ����
			Loc = mLoc;
		}
	}
}
void BoxBoard::RegisterCircle(LOC StartLoc, LOC NextLoc)
{
	//��̬ע��������һ�����ӳ��� ����һ�����ӿ�ʼ��������ע�ᡣһ��������ɸ����

	int x = StartLoc.x;//��ʼXֵ
	int y = StartLoc.y;//��ʼYֵ
	LOC Loc = StartLoc;//�����������
	LOC NewLoc = NextLoc;//����������
	int Re = GetFirstEmptyChainNum();//��ȡһ���հ׵����ռ�
	Chains[Re].ChainBoxNum = 0;//��Ϊ0
	Chains[Re].Type = NotDefine;
	//�����¹����У�NewLoc����ʵ������ÿһ�����ӵ����ꡣ������������������ʱ���ж�
	for (int i = 0; i < BOXNUM; i++)
	{
		if (Boxes[NewLoc.x][NewLoc.y].Type != 2)//���������ɸ���ȡ��ע�ᡣ
		{
			InheritChain(EMPTY, Re);//0�����̲������Ѿ�ע�����
			break;
		}
		else if (NewLoc.x == StartLoc.x&&NewLoc.y == StartLoc.y)//�ѵ���㣬ȷ��ΪCircle
		{
			Chains[Re].Type = Circle;
			Chains[Re].StartLoc = NewLoc;						//������յ㶼���Ϊ�������
			Chains[Re].EndLoc = NewLoc;							//������յ㶼���Ϊ�������
			Boxes[NewLoc.x][NewLoc.y].BelongingChainNum = Re;	//�����ø�����Ҳ��Ҫ��ע��
			Chains[Re].ChainBoxNum++;							//���ĸ�����Ŀ����1
			break;
		}
		else//ÿ�ѵ�һ���µĸ���
		{
			LOC mLoc = NewLoc;
			//������ӵ���Ϣ
			Boxes[NewLoc.x][NewLoc.y].BelongingChainNum = Re;	//�����ҹ�
			Chains[Re].ChainBoxNum++;							//���ĸ�����Ŀ����1
			NewLoc = FindNextBox(NewLoc, Loc);					//Ѱ����һ����
			Loc = mLoc;
		}
	}
}
void BoxBoard::SearchingFromBox(LOC BoxLoc)
{
	int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
	for (int n = 0; n < 4; n++)
	{
		int bx = BoxLoc.x + Dir[n][0];
		int by = BoxLoc.y + Dir[n][1];
		int lx = (BoxLoc.x * 2) - 1 + Dir[n][0];
		int ly = (BoxLoc.y * 2) - 1 + Dir[n][1];
		if (board[lx][ly] == EDGE&&GetBoxType(bx, by) == CHAINBOX)//�ڱ�Ϊ�գ�Ŀ�����Ϊ����
		{
			if (Boxes[bx][by].BelongingChainNum == EMPTY)//����Ϊδ�����ĸ��ӣ����⻷�ظ��Ӳ�ͬ���������
			{
				LOC NewLoc;
				NewLoc.Set(bx, by);
				RegisterChain(BoxLoc, NewLoc);
			}
		}
	}

}
void BoxBoard::SearchingCircle(LOC BoxLoc)
{
	int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
	for (int n = 0; n < 4; n++)
	{
		int bx = BoxLoc.x + Dir[n][0];
		int by = BoxLoc.y + Dir[n][1];
		int lx = (BoxLoc.x * 2) - 1 + Dir[n][0];
		int ly = (BoxLoc.y * 2) - 1 + Dir[n][1];
		if (board[lx][ly] == EDGE&&GetBoxType(bx, by) == CHAINBOX)//�ڱ�Ϊ�գ�Ŀ�����Ϊ����
		{
			if (Boxes[bx][by].BelongingChainNum == EMPTY)//����Ϊδ�����ĸ��ӣ����⻷�ظ��Ӳ�ͬ���������
			{
				LOC NewLoc;
				NewLoc.Set(bx, by);
				RegisterCircle(BoxLoc, NewLoc);
			}
		}
	}
}
void BoxBoard::ResetChainsInfo()
{
	for (int y = 1; y <= BOXLEN; y++)
	{
		for (int x = 1; x <= BOXLEN; x++)
		{
			Boxes[x][y].BelongingChainNum = 0;
		}
	}
	for (int i = 1; i < BOXNUM; i++)
	{
		Chains[i].ChainBoxNum = 0;
		Chains[i].Type = NotDefine;
		Chains[i].ConditionOfPreCircle = false;
	}
}
void BoxBoard::DefineAllChains(bool ChainPlus)
{
	DefineBoxesType();//���ȶ������и��ӵ�����
	ResetChainsInfo();//������������

	//���ȴ����ɸ�����������г���
	for (int i = 1; i <= BOXLEN; i++)
	{
		for (int j = 1; j <= BOXLEN; j++)
		{
			if (Boxes[i][j].Type == 3)
			{
				LOC k;
				k.Set(i, j);
				SearchingFromBox(k);
			}
		}
	}

	//Ȼ���ڱ�Ե������û�б��Ų����
	for (int i = 1; i <= BOXLEN; i++)
	{
		LOC k;
		k.Set(i, 0);
		SearchingFromBox(k);
	}
	for (int i = 1; i <= BOXLEN; i++)
	{
		LOC k;
		k.Set(0, i);
		SearchingFromBox(k);
	}
	for (int i = 1; i <= BOXLEN; i++)
	{
		LOC k;
		k.Set(i, BOXLEN + 1);
		SearchingFromBox(k);
	}
	for (int i = 1; i <= BOXLEN; i++)
	{
		LOC k;
		k.Set(BOXLEN + 1, i);
		SearchingFromBox(k);
	}

	/*Ȼ���ڱ�Ե������û�б��Ų����
	for (int i = 1; i <= BOXLEN; i++)
	{
	//�����ϱ�
	if (board[(i * 2) - 1][0] == EDGE&&Boxes[i][1].BelongingChainNum == EMPTY&&Boxes[i][1].Type == CHAINBOX)
	{
	LOC BoxLoc,NewLoc;
	BoxLoc.Set(i, 0);
	NewLoc.Set(i, 1);
	RegisterChain(BoxLoc, NewLoc);
	}
	//�����ұ�
	if (board[LEN-1][(i * 2) - 1] == EDGE&&Boxes[BOXLEN][i].BelongingChainNum == EMPTY&&Boxes[BOXLEN][i].Type == CHAINBOX)
	{
	LOC BoxLoc, NewLoc;
	BoxLoc.Set(BOXLEN+1, i);
	NewLoc.Set(BOXLEN, i);
	RegisterChain(BoxLoc, NewLoc);
	}
	//�����±�
	if (board[(i * 2) - 1][LEN - 1] == EDGE&&Boxes[i][BOXLEN].BelongingChainNum == EMPTY&&Boxes[i][BOXLEN].Type == CHAINBOX)
	{
	LOC BoxLoc, NewLoc;
	BoxLoc.Set(i,BOXLEN + 1);
	NewLoc.Set(i,BOXLEN);
	RegisterChain(BoxLoc, NewLoc);
	}
	//�������
	if (board[0][(i * 2) - 1] == EDGE&&Boxes[1][i].BelongingChainNum == EMPTY&&Boxes[1][i].Type == CHAINBOX)
	{
	LOC BoxLoc, NewLoc;
	BoxLoc.Set(0, i);
	NewLoc.Set(1, i);
	RegisterChain(BoxLoc, NewLoc);
	}
	}*/


	//Ȼ��������û�б������������񣬿���Ϊ��
	for (int i = 1; i <= BOXLEN; i++)
	{
		for (int j = 1; j <= BOXLEN; j++)
		{
			if (Boxes[i][j].Type == CHAINBOX&&Boxes[i][j].BelongingChainNum == EMPTY)
			{
				LOC BoxLoc;
				BoxLoc.Set(i, j);
				SearchingCircle(BoxLoc);
			}
		}
	}

	//���ʼChainPlus
	if (ChainPlus)
	{
		//���Ȳ����Ƿ�����β��ͬ����������ΪPreChain
		for (int i = 1; i < BOXNUM; i++)
		{
			if (Chains[i].Type == LongChain)
			{
				for (int j = i + 1; j <= BOXNUM; j++)
				{
					if (Chains[j].Type == LongChain&&LocEqual(Chains[i].StartLoc, Chains[j].StartLoc) && LocEqual(Chains[i].EndLoc, Chains[j].EndLoc))
					{
						if (Boxes[Chains[j].StartLoc.x][Chains[j].StartLoc.y].BelongingChainNum == EMPTY&&Boxes[Chains[j].EndLoc.x][Chains[j].EndLoc.y].BelongingChainNum == EMPTY)
						{
							//��ʱ��i��jͬΪ����������β��ͬ����β�ĸ��Ӷ�δ���϶��������϶�����ΪPreChain
							InheritChain(i, j);//��i�̳�����j
							Boxes[Chains[i].StartLoc.x][Chains[i].StartLoc.y].BelongingChainNum = i;
							Boxes[Chains[i].EndLoc.x][Chains[i].EndLoc.y].BelongingChainNum = i;
							Chains[i].ChainBoxNum = Chains[i].ChainBoxNum + 2;
							Chains[i].StartLoc = Chains[i].EndLoc;
							Chains[i].Type = PreCircle;
						}
					}
				}
			}
		}

		//Ȼ�������PreCircle��PreChain�����
		for (int y = 1; y <= BOXLEN; y++)
		{
			for (int x = 1; x <= BOXLEN; x++)
			{
				if (Boxes[x][y].Type == 3 && Chains[Boxes[x][y].BelongingChainNum].Type == PreCircle)
				{
					int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
					int bx = x * 2 - 1;
					int by = y * 2 - 1;
					//�����Ȳ���һ������
					for (int n = 0; n < 4; n++)
					{
						int ex = bx + Dir[n][0];
						int ey = by + Dir[n][1];
						int nbx = x + Dir[n][0];
						int nby = y + Dir[n][1];
						if (board[ex][ey] == EDGE&&Chains[Boxes[nbx][nby].BelongingChainNum].Type == LongChain)
						{
							Chains[Boxes[nbx][nby].BelongingChainNum].ConditionOfPreCircle = true;
						}
					}
				}
			}
		}
		//�������л�û��������ɸ����������������������ĺϲ���
		for (int y = 1; y < BOXNUM; y++)
		{
			for (int x = 1; x <= BOXNUM; x++)
			{
				int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
				if (Boxes[x][y].Type == FREEBOX&&Boxes[x][y].BelongingChainNum == EMPTY)//�������ɸ�δȷ��
				{
					int ChainRegNum[4];
					int Total = 0;
					for (int n = 0; n < 4; n++)
					{
						int bx = x + Dir[n][0];
						int by = y + Dir[n][1];
						int lx = (x * 2) - 1 + Dir[n][0];
						int ly = (y * 2) - 1 + Dir[n][1];
						if (board[lx][ly] == EDGE&&GetBoxType(bx, by) == CHAINBOX)//�ڱ�Ϊ�գ�Ŀ�����Ϊ����
						{
							ChainRegNum[Total] = Boxes[bx][by].BelongingChainNum;
							Total++;
						}
					}
					//���ҳ����
					int FirstChainNum = 0;
					int FirstChainBoxes = 0;
					for (int i = 0; i < Total; i++)
					{
						if (Chains[ChainRegNum[i]].ChainBoxNum > FirstChainBoxes)
						{
							FirstChainNum = ChainRegNum[i];
							FirstChainBoxes = Chains[ChainRegNum[i]].ChainBoxNum;
						}
					}
					//���ҳ��γ���
					int SecChainNum = 0;
					int SecChainBoxes = 0;
					for (int i = 0; i < Total; i++)
					{
						if (Chains[ChainRegNum[i]].ChainBoxNum > SecChainBoxes&&ChainRegNum[i] != FirstChainNum)
						{
							SecChainNum = ChainRegNum[i];
							SecChainBoxes = Chains[ChainRegNum[i]].ChainBoxNum;
						}
					}
					//�����������Լ������ۼ���һ��
					Boxes[x][y].BelongingChainNum = FirstChainNum;
					Chains[FirstChainNum].ChainBoxNum++;
					InheritChain(FirstChainNum, SecChainNum);
				}
			}
		}
	}
}

//�����ж�
void BoxBoard::RegisterDeadChain(LOC FreeBoxLoc, LOC FirstLoc)
{
	//��̬ע��������һ�����ӳ��� ����һ�����ӿ�ʼ��������ע�ᡣһ��������ɸ����

	int x = FreeBoxLoc.x;				//��ʼXֵ
	int y = FreeBoxLoc.y;				//��ʼYֵ
	LOC Loc = FreeBoxLoc;				//�����������
	LOC NewLoc = FirstLoc;				//����������
	int Re = GetFirstEmptyChainNum();	//��ȡһ���հ׵����ռ�
	Chains[Re].StartLoc = Loc;			//���Ϊĳ�����ɸ�
	Chains[Re].ChainBoxNum = 1;			//�������
	Chains[Re].Type = NotDefine;

	//�����¹����У�NewLoc����ʵ������ÿһ�����ӵ����ꡣ������������������ʱ���ж�
	for (int i = 0; i < BOXNUM; i++)
	{
		if (GetBoxType(NewLoc.x, NewLoc.y) != CHAINBOX)//������������ʱ���ж��������͡���ʱ��NewLocΪ���������յ������
		{
			Chains[Re].EndLoc = NewLoc;//���������յ�
			if (GetBoxType(NewLoc.x, NewLoc.y) == DEADBOX)//����ն���һ������Ļ��������һ����ѭ��(DeadCircle)
			{
				Chains[Re].Type = DeadCircle;
				Boxes[NewLoc.x][NewLoc.y].BelongingChainNum = Re;//�����ø�����Ҳ��Ҫ��ע��
				Chains[Re].ChainBoxNum++;
			}
			else//�����յ㲻һ������Ϊ����������Ŀ�ж�������
			{
				Chains[Re].Type = DeadChain;
			}
			break;
		}
		else//ÿ�ѵ�һ���µĸ���
		{
			LOC mLoc = NewLoc;
			//������ӵ���Ϣ
			Boxes[NewLoc.x][NewLoc.y].BelongingChainNum = Re;	//�����ҹ�
			Chains[Re].ChainBoxNum++;							//���ĸ�����Ŀ����1
			NewLoc = FindNextBox(NewLoc, Loc);					//Ѱ����һ����
			Loc = mLoc;
		}
	}
}
void BoxBoard::SearchingDeadChain(LOC BoxLoc)
{
	int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
	for (int n = 0; n < 4; n++)
	{
		int bx = BoxLoc.x + Dir[n][0];
		int by = BoxLoc.y + Dir[n][1];
		int lx = (BoxLoc.x * 2) - 1 + Dir[n][0];
		int ly = (BoxLoc.y * 2) - 1 + Dir[n][1];
		if (board[lx][ly] == EDGE&&GetBoxType(bx, by) == CHAINBOX)//�ڱ�Ϊ�գ�Ŀ�����Ϊ����
		{
			if (Boxes[bx][by].BelongingChainNum == EMPTY)//����Ϊδ�����ĸ��ӣ����⻷�ظ��Ӳ�ͬ���������
			{
				LOC NewLoc;
				NewLoc.Set(bx, by);
				RegisterDeadChain(BoxLoc, NewLoc);
			}
		}
	}
}
void BoxBoard::DefineDeadChain()
{
	DefineBoxesType();//���ȶ������и��ӵ�����
	ResetChainsInfo();//������������
	for (int i = 1; i <= BOXLEN; i++)
	{
		for (int j = 1; j <= BOXLEN; j++)
		{
			if (Boxes[i][j].Type == DEADBOX)
			{
				LOC k;
				k.Set(i, j);
				SearchingDeadChain(k);
			}
		}
	}
}
bool BoxBoard::GetDeadChainExist()
{
	DefineDeadChain();
	for (int i = 0; i < BOXNUM; i++)
	{
		if (Chains[i].Type == DeadChain)
			return true;
	}
	return false;
}
bool BoxBoard::GetDeadCircleExist()
{
	DefineDeadChain();
	for (int i = 0; i < BOXNUM; i++)
	{
		if (Chains[i].Type == DeadCircle&&Chains[i].ChainBoxNum>3)
			return true;
	}
	return false;
}

//ռ����
bool BoxBoard::CaptualAnySingleChain(int LatterPlayer)
{
	DefineAllChains(false);

	for (int i = 1; i < BOXNUM; i++)
	{
		if (Chains[i].Type == SingleChain)//�ҵ�������һ������
		{
			for (int y = 1; y <= BOXLEN; y++)
			{
				for (int x = 1; x <= BOXLEN; x++)
				{
					if (Boxes[x][y].BelongingChainNum == i)
					{

						int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
						int bx = x * 2 - 1;
						int by = y * 2 - 1;
						//�����Ȳ���һ������
						for (int n = 0; n < 4; n++)
						{
							int ex = bx + Dir[n][0];
							int ey = by + Dir[n][1];
							if (board[ex][ey] == EDGE)
							{
								Move(ex, ey, -LatterPlayer);
								break;
							}
						}
						//����ռ���������
						for (int n = 0; n < 4; n++)
						{
							int ex = bx + Dir[n][0];
							int ey = by + Dir[n][1];
							if (board[ex][ey] == EDGE)
							{
								Move(ex, ey, LatterPlayer);
								break;
							}
						}
						//ռ�ݽ�����ˢ������Ϣ��������
						Boxes[x][y].BelongingChainNum = 0;
						InheritChain(EMPTY, i);
						DefineBoxesType();
						return true;
					}
				}
			}
		}
	}
	return false;
}
bool BoxBoard::CaptualAnyShortChain(int LatterPlayer)
{
	DefineAllChains(false);

	for (int i = 1; i < BOXNUM; i++)
	{
		if (Chains[i].Type == ShortChain)//�ҵ�������һ������
		{
			//������ռ��������������м���Ǹ���
			for (int y = 1; y <= BOXLEN; y++)
			{
				for (int x = 1; x <= BOXLEN; x++)
				{
					if (Boxes[x][y].BelongingChainNum == i)
					{

						int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
						int bx = x * 2 - 1;
						int by = y * 2 - 1;

						for (int n = 0; n < 4; n++)
						{
							int ex = bx + Dir[n][0];
							int ey = by + Dir[n][1];
							int nbx = x + Dir[n][0];
							int nby = y + Dir[n][1];
							if (board[ex][ey] == EDGE&&Boxes[nbx][nby].BelongingChainNum == i)//�ҵ��������ӽ�����Ǹ�����
							{
								Move(ex, ey, -LatterPlayer);
								break;
							}
						}

					}
				}
			}

			//����ռ�������������ʣ��ı�
			for (int y = 1; y <= BOXLEN; y++)
			{
				for (int x = 1; x <= BOXLEN; x++)
				{
					if (Boxes[x][y].BelongingChainNum == i)
					{
						int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
						int bx = x * 2 - 1;
						int by = y * 2 - 1;
						for (int n = 0; n < 4; n++)
						{
							int ex = bx + Dir[n][0];
							int ey = by + Dir[n][1];
							if (board[ex][ey] == EDGE)
							{
								Move(ex, ey, LatterPlayer);
								break;
							}
						}
						//ռ�ݽ�����ˢ������Ϣ��������
						Boxes[x][y].BelongingChainNum = 0;
					}
				}
			}
			//����
			InheritChain(EMPTY, i);
			DefineBoxesType();
			return true;

		}
	}
	return false;
}
bool BoxBoard::CaptualShortestChain(int LatterPlayer)
{
	DefineAllChains(false);

	//���ҵ���̵���������/��
	int Least = 0;
	int LeastBoxNum = BOXNUM;//������̵�����25��
	for (int i = 1; i < BOXNUM; i++)
	{
		if (Chains[i].Type == LongChain || Chains[i].Type == Circle)
		{
			if (Chains[i].ChainBoxNum < LeastBoxNum)
			{
				LeastBoxNum = Chains[i].ChainBoxNum;
				Least = i;
			}
		}
	}
	if (Least == 0)
		return false;


	//������ռ��������������֮��ı�
	bool Finish = false;
	for (int y = 1; y <= BOXLEN; y++)
	{
		for (int x = 1; x <= BOXLEN; x++)
		{
			if (Boxes[x][y].BelongingChainNum == Least&&!Finish)
			{

				int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
				int bx = x * 2 - 1;
				int by = y * 2 - 1;

				for (int n = 0; n < 4; n++)
				{
					int ex = bx + Dir[n][0];
					int ey = by + Dir[n][1];
					int nbx = x + Dir[n][0];
					int nby = y + Dir[n][1];
					if (board[ex][ey] == EDGE&&Boxes[nbx][nby].BelongingChainNum == Least)//�ҵ��������ӽ�����Ǹ�����
					{
						Move(ex, ey, -LatterPlayer);
						Finish = true;
						break;
					}
				}
			}
		}
	}

	//����ռ�������������������ӿ���ı�
	for (int y = 1; y <= BOXLEN; y++)
	{
		for (int x = 1; x <= BOXLEN; x++)
		{
			if (Boxes[x][y].BelongingChainNum == Least)
			{
				int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
				int bx = x * 2 - 1;
				int by = y * 2 - 1;
				for (int n = 0; n < 4; n++)
				{
					int ex = bx + Dir[n][0];
					int ey = by + Dir[n][1];
					if (board[ex][ey] == EDGE)
					{
						Move(ex, ey, LatterPlayer);
						break;
					}
				}
				//ռ�ݽ�����ˢ������Ϣ��������
				Boxes[x][y].BelongingChainNum = 0;
			}
		}
	}
	//����
	InheritChain(EMPTY, Least);
	DefineBoxesType();
	return true;
}

//����
LOC BoxBoard::GetOpenShortestChainLoc()
{
	DefineAllChains(false);

	//���ҵ���̵���������/��
	int Least = 0;
	int LeastBoxNum = BOXNUM;//������̵�����25��
	for (int i = 1; i < BOXNUM; i++)
	{
		if (Chains[i].Type == LongChain || Chains[i].Type == Circle)
		{
			if (Chains[i].ChainBoxNum < LeastBoxNum)
			{
				LeastBoxNum = Chains[i].ChainBoxNum;
				Least = i;
			}
		}
	}


	//������ռ��������������֮��ı�
	for (int y = 1; y <= BOXLEN; y++)
	{
		for (int x = 1; x <= BOXLEN; x++)
		{
			if (Boxes[x][y].BelongingChainNum == Least)
			{

				int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
				int bx = x * 2 - 1;
				int by = y * 2 - 1;

				for (int n = 0; n < 4; n++)
				{
					int ex = bx + Dir[n][0];
					int ey = by + Dir[n][1];
					int nbx = x + Dir[n][0];
					int nby = y + Dir[n][1];
					if (board[ex][ey] == EDGE&&Boxes[nbx][nby].BelongingChainNum == Least)//�ҵ��������ӽ�����Ǹ�����
					{
						LOC k;
						k.Set(ex, ey);
						return k;
					}
				}
			}
		}
	}
	LOC k;
	k.Set(0, 0);
	return k;
}
LOC BoxBoard::GetOpenSuitableChainLoc()
{
	DefineAllChains(false);

	//���ҵ���̵���������/��
	int SuitChainNum = 99;
	for (int i = 1; i < BOXNUM; i++)
	{
		if (Chains[i].Type == Circle)
		{
			SuitChainNum = i;
		}
	}

	if (SuitChainNum == 99)
	{
		for (int i = 1; i < BOXNUM; i++)
		{
			if (Chains[i].Type == LongChain&&Chains[i].ConditionOfPreCircle)
			{
				SuitChainNum = i;
			}
		}
	}

	if (SuitChainNum == 99)
	{
		for (int i = 1; i < BOXNUM; i++)
		{
			if (Chains[i].Type == LongChain)
			{
				SuitChainNum = i;
			}
		}
	}

	//=======
	for (int y = 1; y <= BOXLEN; y++)
	{
		for (int x = 1; x <= BOXLEN; x++)
		{
			if (Boxes[x][y].BelongingChainNum == SuitChainNum)
			{

				int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
				int bx = x * 2 - 1;
				int by = y * 2 - 1;

				for (int n = 0; n < 4; n++)
				{
					int ex = bx + Dir[n][0];
					int ey = by + Dir[n][1];
					int nbx = x + Dir[n][0];
					int nby = y + Dir[n][1];
					if (board[ex][ey] == EDGE&&Boxes[nbx][nby].BelongingChainNum == SuitChainNum)//�ҵ��������ӽ�����Ǹ�����
					{
						LOC k;
						k.Set(ex, ey);
						return k;
					}
				}
			}
		}
	}
	LOC k;
	k.Set(0, 0);
	return k;
}

//�жϺ���
bool BoxBoard::RationalState(LOC BoxNum)
{
	if (BoxNum.x >= BoxNum.y)
		return true;
	return false;
}
LOC BoxBoard::GetRationalStateBoxNum()
{
	//��֪��ǰ�����Ƿ���һ�����Ծ��棬������Ҫ�жϵ�ǰ�������Ŀ��
	DefineAllChains(true);

	//�����������������Ŀ
	int LCNum = 0;
	int LCBox = 0;
	int CNum = 0;
	int CBox = 0;
	int PCNum = 0;
	int PCBox = 0;
	bool OnlyPreChain = true;//�Ƿ����Ԥ������Ҳ����Ԥ�������Ⱦ��������ǵĻ������һ���ض���Ԥ����
	for (int i = 1; i <= BOXNUM; i++)
	{
		if (Chains[i].Type != NotDefine)
		{
			if (Chains[i].Type == LongChain)
			{
				if (Chains[i].ConditionOfPreCircle == false)
					OnlyPreChain = false;
				LCNum++;
				LCBox += Chains[i].ChainBoxNum;
			}
			else if (Chains[i].Type == Circle)
			{
				CNum++;
				CBox += Chains[i].ChainBoxNum;
			}
			else if (Chains[i].Type == PreCircle)
			{
				PCNum++;
				PCBox += Chains[i].ChainBoxNum;
			}
		}
	}

	//��ʼ����������Ŀ
	int Total = LCBox + PCBox + CBox;//�ܸ�����
	int Sacrifice = 0;
	if (OnlyPreChain)
	{
		if (LCNum == 0)//�����״���²����ڳ��������������ǻ�
		{
			Sacrifice = (CNum - 1) * 4;
		}
		else//���ڳ�����������Ԥ���������һ���ض�ΪԤ����
		{
			Sacrifice = (PCNum * 4) + (CNum * 4) + (LCNum * 2) - 4;
		}
	}
	else
	{
		//�г�����ʱ�����һ���ض��ǳ���
		Sacrifice = (PCNum * 4) + (CNum * 4) + (LCNum * 2) - 2;
	}
	//ShowBelongingChain();
	//cout << "Total [" << Total << "]  Sacrifice [" << Sacrifice << "]" << endl;
	LOC num;
	num.Set(Total - Sacrifice, Sacrifice);
	return num;
}
int BoxBoard::GetBoardWinner(int LatterPlayer)
{
	if (GetFilterMoveNum() > 0)
		cout << "Wrong";
	bool debug = false;
	int player = LatterPlayer;
	DefineBoxesType();
	//ShowBelongingChain();
	//system("pause");
	/*
	for (;;)//���̲����еĵ���
	{
		if (CaptualAnySingleChain(player) == false)
			break;
		else
		{
			player = -player;
			//ShowBelongingChain();
			//system("pause");
		}
	}
	if (debug)
	{
		PrintBoard();
		cout << "Single Chain Finish , The Latter Player This Time is " << player << endl;
	}

	for (;;)//���̲����еĶ���
	{
		if (CaptualAnyShortChain(player) == false)
			break;
		else
		{
			player = -player;
			//ShowBelongingChain();
			//system("pause");
		}
	}
	if (debug)
	{
		PrintBoard();
		cout << "Short Chain Finish , The Latter Player This Time is " << player << endl;
	}
	*/
	LOC BoxNum;
	for (;;)//������������̲�����
	{
		DefineAllChains(true);//�ȶ���Ϊ��ȫ״̬�ж�һ��
		BoxNum = GetRationalStateBoxNum();//Ȼ�����ж�һ���������
		if (RationalState(BoxNum))
			break;
		else
		{
			if (!CaptualShortestChain(player))//����Բ���ȥ��Ҳ�˳�
				break;
			else
				player = -player;
		}
	}
	//ShowBelongingChain();
	//system("pause");
	if (Winner() == 0)//Ҳ���ǻ�ûʤ��
	{
		int r, b;
		if (player == RED)
		{
			r = BoxNum.x + GetPlayerBoxes(RED);
			b = BoxNum.y + GetPlayerBoxes(BLUE);
		}
		else
		{
			r = BoxNum.y + GetPlayerBoxes(RED);
			b = BoxNum.x + GetPlayerBoxes(BLUE);
		}
		//cout << "Game Finish!\n Red [" << r << "] Blue [" << b << "]" << endl;
		if (r > b)
			return RED;
		else
			return BLUE;
	}
	else
		return Winner();
}

