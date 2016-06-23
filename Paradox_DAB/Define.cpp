#include "stdafx.h"
#include "Define.h"
#include <iostream>
#include <stdio.h>
#include "time.h"
#include "windows.h"

using namespace std;

//������
void LOC::Set(int lx, int ly)
{
	x = lx;
	y = ly;
}
TURN::TURN()
{
	for (int i = 0; i < LEN; i++)
	{
		for (int j = 0; j < LEN; j++)
		{
			Captual[i][j] = false;
			if (IfOddNumber(i) && IfOddNumber(j))//xy��Ϊ������Ϊ����
			{
				NewBoard[i][j] = BOX;
			}
			else if (IfEvenNumber(i) && IfOddNumber(j))//xż����y������Ϊ��
			{
				NewBoard[i][j] = EDGE;
			}
			else if (IfOddNumber(i) && IfEvenNumber(j))//x������yż����Ϊ��
			{
				NewBoard[i][j] = EDGE;
			}
			else//ʣ�µľ���xy��ż����Ϊ�յ㡣
			{
				NewBoard[i][j] = EMPTY;
			}
		}
	}
}
void TURN::SetBoard(int Array[LEN][LEN])
{
	for (int i = 0; i<LEN; i++)
	{
		for (int j = 0; j<LEN; j++)
		{
			NewBoard[i][j] = Array[i][j];
		}
	}
}
void TURN::SetCaptualInfo(int OB[LEN][LEN], int NB[LEN][LEN])
{
	for (int i = 0; i<LEN; i++)
	{
		for (int j = 0; j<LEN; j++)
		{
			if (OB[i][j] == EDGE&&NB[i][j] != EDGE)
				Captual[i][j] = true;
		}
	}
}

//����������
Board::Board()
{
	Step = 0;
	for(int i=0;i<LEN;i++)
	{
		for(int j=0;j<LEN;j++)
		{
			if(IfOddNumber(i)&&IfOddNumber(j))//xy��Ϊ������Ϊ����
			{
				board[i][j] = BOX;
			}
			else if(IfEvenNumber(i)&&IfOddNumber(j))//xż����y������Ϊ��
			{
				board[i][j] = EDGE;
			}
			else if(IfOddNumber(i)&&IfEvenNumber(j))//x������yż����Ϊ��
			{
				board[i][j] = EDGE;
			}
			else//ʣ�µľ���xy��ż����Ϊ�յ㡣
			{
				board[i][j] = EMPTY;
			}
		}
	}
}
Board::Board(int Array[LEN][LEN],int step)
{
	Step = step;
	for(int i=0;i<LEN;i++)
	{
		for(int j=0;j<LEN;j++)
		{
			board[i][j]=Array[i][j];
		}
	}
}
int Board::GetEmptyEdges()
{
	int lines=0;
	for(int i=0;i<LEN;i++)
	{
		for(int j=0;j<LEN;j++)
		{
			if(board[i][j]==EDGE)
				lines++;
		}
	}
	return lines;
}
int Board::GetEmptyBoxes()
{
	int boxes=0;
	for(int i=0;i<LEN;i++)
	{
		for(int j=0;j<LEN;j++)
		{
			if(board[i][j]==BOX)
				boxes++;
		}
	}
	return boxes;
}
void Board::PrintBoard()
{
	//��ӡ���̺���
	cout<<"  ";
	for(int i = 0;i<=(LEN-1);i++)
	{
		printf("%2d",i);
	}
	cout<<"\n";
	for(int j = 0;j<LEN;j++)
	{
		printf("%2d",j);
		for(int i = 0;i<LEN;i++)
		{
			if(board[i][j]==BOX)
			{
				cprintf("  ",8);
			}
			else if(board[i][j]==RED_BOX)
			{
				cprintf("��",12);
			}
			else if(board[i][j]==BLUE_BOX)
			{
				cprintf("��",9);
			}
			else if(board[i][j]==EDGE)
			{
				if(IfOddNumber(j)&&IfEvenNumber(i))
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
			else if(board[i][j]==RED_EDGE)
			{
				if(IfOddNumber(j)&&IfEvenNumber(i))
				{
					cprintf("��",12);
				}
				else
				{
					cprintf("��",12);
				}
			}
			else if(board[i][j]==BLUE_EDGE)
			{
				if(IfOddNumber(j)&&IfEvenNumber(i))
				{
					cprintf("��",9);
				}
				else
				{
					cprintf("��",9);
				}
			}
			else if(board[i][j]==EMPTY)
			{
				cprintf("��", 15);
			}
		}
		printf("%2d",j);
		cout<<"\n";
	}
	cout<<"  ";
	for(int i = 0;i<=(LEN-1);i++)
	{
		printf("%2d",i);
	}
	cout<<"\n";
}
void Board::ShowBoard()
{
	//��ӡ���̺���
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
			if (board[i][j] == BOX)
			{
				cprintf("  ", 8);
			}
			else if (board[i][j] == RED_BOX)
			{
				cprintf("��", 12);
			}
			else if (board[i][j] == BLUE_BOX)
			{
				cprintf("��", 9);
			}
			else if (board[i][j] == EDGE)
			{
				if (IfOddNumber(j) && IfEvenNumber(i))
				{
					cprintf("��", 15);
				}
				else
				{
					cprintf("��", 15);
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
				if (i == 0 && j == 0)
				{
					cprintf("��", 8);
				}
				else if (i == (LEN - 1) && j == 0)
				{
					cprintf("��", 8);
				}
				else if (i == 0 && j == (LEN - 1))
				{
					cprintf("��", 8);
				}
				else if (i == (LEN - 1) && j == (LEN - 1))
				{
					cprintf("��", 8);
				}
				else if (i == 0 && j != (LEN - 1) && j != 0)
				{
					cprintf("��", 8);
				}
				else if (i == (LEN - 1) && j != (LEN - 1) && j != 0)
				{
					cprintf("��", 8);
				}
				else if (j == 0 && i != (LEN - 1) && i != 0)
				{
					cprintf("��", 8);
				}
				else if (j == (LEN - 1) && i != (LEN - 1) && i != 0)
				{
					cprintf("��", 8);
				}
				else
				{
					cprintf("��", 8);
				}
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
}
void Board::ShowLiberties()
{
	for (int j = 1; j <= BOXLEN; j++)
	{
		for (int i = 1; i <= BOXLEN; i++)
		{
			int bl = GetBoxLiberties(i * 2 - 1, j * 2 - 1);
			if (bl == 0)
			{
				cprintNum(8, bl);
				cout << " ";
			}
			else if (bl == 1)
			{
				cprintNum(2,bl);
				cout << " ";
			}
			else if (bl == 2)
			{
				cprintNum(14,bl);
				cout << " ";
			}
			else
			{
				cprintNum(5,bl);
				cout << " ";
			}
		}
		cout << endl;
	}
}
int Board::GetEdgeOwner(int x,int y)
{
	int num = board[x][y];
	if(num!=EDGE&&num!=RED_EDGE&&num!=BLUE_EDGE)
	{
		cout<<"GetEdgeOwnerPlayer����������һ������ĵ�ַ";
		return 0;
	}
	if(num>0)
	{
		return RED;
		//cout<<x<<" "<<y<<" EdgeOwner = RED"<<endl;
	}
	if(num<0)
	{
		return BLUE;
		//cout<<x<<" "<<y<<" EdgeOwner = BLUE"<<endl;
	}
	return EMPTY;
}
int Board::GetBoxOwner(int x,int y)
{
	int num = board[x][y];
	if(num!=BOX&&num!=RED_BOX&&num!=BLUE_BOX)
	{
		cout<<"GetEdgeOwnerPlayer����������һ������ĵ�ַ";
		return 0;
	}
	if(num>0)
	{
		return RED;
		//cout<<x<<" "<<y<<" BoxOwner = RED"<<endl;
	}
	if(num<0)
	{
		return BLUE;
		//cout<<x<<" "<<y<<" BoxOwner = BLUE"<<endl;
	}
	return EMPTY;
}
int Board::GetBoxLiberties(int x,int y)
{
	int num = board[x][y];
	if(num!=BOX&&num!=RED_BOX&&num!=BLUE_BOX)
	{
		cout<<"GetBoxLiberties����������һ������ĵ�ַ";
		return 0;
	}
	int lib = 0;
	if(board[x-1][y]==EDGE)
		lib++;
	if(board[x][y+1]==EDGE)
		lib++;
	if(board[x+1][y]==EDGE)
		lib++;
	if(board[x][y-1]==EDGE)
		lib++;
	return lib;
}
bool Board::GetEdgeLegality(int x,int y)
{
	if(board[x][y]==EDGE)
		return true;
	return false;
}
int Board::GetPlayerBoxes(int player)
{
	int boxes=0;
	if(player==RED)
	{
		for(int i=0;i<LEN;i++)
		{
			for(int j=0;j<LEN;j++)
			{
				if(board[i][j]==RED_BOX)
					boxes++;
			}
		}
	}
	if(player==BLUE)
	{
		for(int i=0;i<LEN;i++)
		{
			for(int j=0;j<LEN;j++)
			{
				if(board[i][j]==BLUE_BOX)
					boxes++;
			}
		}
	}
	return boxes;
}
int Board::GetPlayerEdges(int player)
{
	int lines=0;
	if(player==RED)
	{
		for(int i=0;i<LEN;i++)
		{
			for(int j=0;j<LEN;j++)
			{
				if(board[i][j]==RED_EDGE)
					lines++;
			}
		}
	}
	if(player==BLUE)
	{
		for(int i=0;i<LEN;i++)
		{
			for(int j=0;j<LEN;j++)
			{
				if(board[i][j]==BLUE_EDGE)
					lines++;
			}
		}
	}
	return lines;
}
void Board::SetBoard(int Array[LEN][LEN])
{
	for (int i = 0; i<LEN; i++)
	{
		for (int j = 0; j<LEN; j++)
		{
			board[i][j] = Array[i][j];
		}
	}
}
int Board::Winner()
{
	int RedBoxes = GetPlayerBoxes(RED);
	int BlueBoxes = GetPlayerBoxes(BLUE);
	if((RedBoxes+BlueBoxes)>=BOXNUM)
	{
		if(RedBoxes>BlueBoxes)
			return RED;
		else
			return BLUE;
	}
	return 0;
}
void Board::LoadBoard()
{
	for (int j = 0; j < LEN; j++)
	{
		for (int i = 0; i < LEN; i++)
		{
			if (board[i][j] != 0 && ((IfOddNumber(j) && IfEvenNumber(i)) || (IfEvenNumber(j) && IfOddNumber(i))))
			{
				system("cls");
				PrintBoard();
				cout << "������[" << i << " , " << j << "]�����ߵ�������0��1��2��" << endl;
				int Num;
				cin >> Num;
				if (Num == 1)
					board[i][j] = 1;
				else if (Num == 2)
					board[i][j] = -1;
				else if (Num == 99)
					goto end;
				else
					board[i][j] = 10;
			}
		}
	}
	for (int j = 0; j < LEN; j++)
	{
		for (int i = 0; i < LEN; i++)
		{
			if (board[i][j] != 0 &&IfOddNumber(j) && IfOddNumber(i))
			{
				system("cls");
				PrintBoard();
				cout << "������[" << i << " , " << j << "]������ӵ�������0��1��2��" << endl;
				int Num;
				cin >> Num;
				if (Num == 1)
					board[i][j] = 2;
				else if (Num == 2)
					board[i][j] = -2;
				else if (Num == 99)
					goto end;
				else
					board[i][j] = 20;
			}
		}
	}
	end:
	PrintBoard();
	cout << "Load Board Finish!" << endl;
}

//�����ֵ
bool Board::GetFreeBoxBool(int bx, int by)
{
	//ע����ǣ������X��Y��Boxʵ�ʵ�ַ
	if (1<=bx&&LEN-2>=bx&&1<=by&&LEN-2>=by&&IfOddNumber(bx)&&IfOddNumber(by))//Boxλ�ñ�ű�����ȷ
	{
		if (GetBoxLiberties(bx, by) >= 3)
			return true;
		else
			return false;
	}
	else
	{
		cout << "Wrong Number In <GetFreeBoxBool> Function";
		system("pause");
	}
	return false;
}
int Board::GetFreeEdgeNum()
{
	int EdgeNum = 0;
	//�õ����е����ɱ�
	for (int y = 1; y < LEN - 1; y = y + 2)
	{
		//���ж�ͷ����һ�����������ı��Ƿ����ɱ�
		if (GetFreeBoxBool(1, y) && GetEdgeLegality(0, y))//��һ��Ϊ�����������罻���ı�Ϊ�ձ�
		{
			EdgeNum++;//�����ɱ���Ŀ����1
		}
		//ѭ���ж��м�ļ�������
		for (int x = 1; x < LEN - 3; x = x + 2)//x��
		{
			if (GetFreeBoxBool(x, y) && GetFreeBoxBool(x + 2, y) && GetEdgeLegality(x + 1, y))
			{
				EdgeNum++;//�����ɱ���Ŀ����1
			}
		}
		//�ж�ĩβ�ĸ���
		if (GetFreeBoxBool(LEN - 2, y) && GetEdgeLegality(LEN - 1, y))//���һ��Ϊ����������罻���ı�Ϊ�ձ�
		{
			EdgeNum++;//�����ɱ���Ŀ����1
		}

		//XY�滻���ٽ���һ���ж�

		//���ж�ͷ����һ�����������ı��Ƿ����ɱ�
		if (GetFreeBoxBool(y, 1) && GetEdgeLegality(y, 0))//��һ��Ϊ�����������罻���ı�Ϊ�ձ�
		{
			EdgeNum++;//�����ɱ���Ŀ����1
		}
		//ѭ���ж��м�ļ�������
		for (int x = 1; x < LEN - 3; x = x + 2)//x��
		{
			if (GetFreeBoxBool(y, x) && GetFreeBoxBool(y, x + 2) && GetEdgeLegality(y, x + 1))
			{
				EdgeNum++;//�����ɱ���Ŀ����1
			}
		}
		//�ж�ĩβ�ĸ���
		if (GetFreeBoxBool(y, LEN - 2) && GetEdgeLegality(y, LEN - 1))//���һ��Ϊ����������罻���ı�Ϊ�ձ�
		{
			EdgeNum++;//�����ɱ���Ŀ����1
		}
	}
	return EdgeNum;//�������ɱߵ�����
}
bool Board::GetCTypeBoxBool(int bx, int by)
{
	//ע����ǣ������X��Y��Boxʵ�ʵ�ַ
	if (bx >= 1 && bx <= LEN - 2 && by >= 1 && by <= LEN - 2 && IfOddNumber(bx) && IfOddNumber(by))//Boxλ�ñ�ű�����ȷ
	{
		if (GetBoxLiberties(bx, by) ==DEADBOX)
			return true;
		else
			return false;
	}
	return false;
}
bool Board::GetCTypeBox(int Player,bool Msg)
{
	//������ǰ�ڣ�������������ʱ��ռ������C�͸�
	for (int y = 1; y < LEN - 1; y = y + 2)
	{
		for (int x = 1; x < LEN - 1; x = x + 2)//x��
		{
			if (GetCTypeBoxBool(x,y))
			{
				int lx, ly;
				if (board[x + 1][y] == EDGE)
				{
					Move(x + 1, y, Player);
					lx = x + 1;
					ly = y;
				}
				else if (board[x - 1][y] == EDGE)
				{
					Move(x - 1, y, Player);
					lx = x - 1;
					ly = y;
				}
				else if (board[x][y + 1] == EDGE)
				{
					Move(x, y + 1, Player);
					lx = x;
					ly = y+1;
				}
				else if (board[x][y - 1] == EDGE)
				{
					Move(x, y - 1, Player);
					lx = x;
					ly = y-1;
				}
				if (Msg)
				{
					MoveMsg(lx, ly, Player);
				}
				return true;//ռ��֮��ͷ�����
			}
		}
	}
	return false;//���ؼ�
}
bool Board::GetLongCTypeBoxExist()
{
	for (int by = 1; by < LEN - 1; by = by + 2)
	{
		for (int bx = 1; bx < LEN - 1; bx = bx + 2)
		{
			if (GetBoxLiberties(bx, by) == DEADBOX)//����������ɶ�Ϊ1�ĸ���
			{
				int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
				for (int n = 0; n < 4; n++)
				{
					int ex = bx + Dir[n][0];
					int ey = by + Dir[n][1];
					int nbx = bx + Dir[n][0] + Dir[n][0];
					int nby = by + Dir[n][1] + Dir[n][1];
					if (board[ex][ey] == EDGE&&nbx>=1&&nbx<=LEN-2&&nby>=1&&nby<=LEN-2)
					{
						if (GetBoxLiberties(nbx, nby) == CHAINBOX)
							return true;
					}
				}
			}
		}
	}
	return false;
}
void Board::GetAllCTypeBoxes(int Player,bool Msg)
{
	for (;;)//ֱ���޷�ռ��CTypeBox�˾ͽ���
	{
		if (!GetCTypeBox(Player, Msg))
			break;
	}
}
LOC Board::FindNextBox(LOC FoundBox, LOC LastBox)
{
	//����ĳ������,������������ܰ����������ꡣ��ʹ��ǰ����GetBoxType����
	int fbx = FoundBox.x * 2 - 1;
	int fby = FoundBox.y * 2 - 1;
	LOC AimBox;
	if (board[fbx + 1][fby] == EDGE && ((FoundBox.x + 1) != LastBox.x || (FoundBox.y) != LastBox.y))//����Ϊ0��
	{
		AimBox.Set(FoundBox.x + 1, FoundBox.y);
		return AimBox;
	}
	else if (board[fbx][fby + 1] == EDGE && ((FoundBox.x) != LastBox.x || (FoundBox.y + 1) != LastBox.y))//����Ϊ90��
	{
		AimBox.Set(FoundBox.x, FoundBox.y + 1);
		return AimBox;
	}
	else if (board[fbx - 1][fby] == EDGE && ((FoundBox.x - 1) != LastBox.x || (FoundBox.y) != LastBox.y))//����Ϊ180��
	{
		AimBox.Set(FoundBox.x - 1, FoundBox.y);
		return AimBox;
	}
	else if (board[fbx][fby - 1] == EDGE && ((FoundBox.x) != LastBox.x || (FoundBox.y - 1) != LastBox.y))//����Ϊ270��
	{
		AimBox.Set(FoundBox.x, FoundBox.y - 1);
		return AimBox;
	}
	AimBox.Set(0, 0);//�������
	return AimBox;
}

//���ڼ���
LOC Board::GetDoubleCrossLoc(int Player)
{
	//�õ���������˫�����Ǹ��ߵ�����
	for (int by = 1; by < LEN - 1; by = by + 2)
	{
		for (int bx = 1; bx < LEN - 1; bx = bx + 2)
		{
			if (GetLongCTypeBoxBool(bx, by))
			{
				//���ڵ�bx��by����ʵ�ʵĸ���
				int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
				for (int n = 0; n < 4; n++)
				{
					int ex = bx + Dir[n][0];
					int ey = by + Dir[n][1];
					int nbx = bx + Dir[n][0] + Dir[n][0];
					int nby = by + Dir[n][1] + Dir[n][1];
					if (board[ex][ey] == EDGE&&nbx >= 1 && nbx <= LEN - 2 && nby >= 1 && nby <= LEN - 2)
					{
						//����ex,ey���乫���ߣ�nbx,nby��doublecross��ĩ��CHAINBOX
						for (int n = 0; n < 4; n++)
						{
							int nex = nbx + Dir[n][0];
							int ney = nby + Dir[n][1];
							if (board[nex][ney] == EDGE && (nex != ex || ney != ey))//�ձ߶��Ҳ����м�Ĺ�����(ex,ey)
							{
								LOC k;
								k.Set(nex, ney);
								return k;
							}
						}
					}
				}
			}
		}
	}
	LOC k;
	k.Set(0, 0);
	return k;
}
bool Board::GetCTypeBoxLimit(int Player,bool Msg)
{
	//������ǰ�ڣ�������������ʱ��ռ������C�͸�
	for (int y = 1; y < LEN - 1; y = y + 2)
	{
		for (int x = 1; x < LEN - 1; x = x + 2)//x��
		{
			if (GetCTypeBoxBool(x, y))
			{
				int lx, ly;
				if (board[x + 1][y] == EDGE&&!GetCTypeBoxBool(x+2, y))
				{
					Move(x + 1, y, Player);
					lx = x + 1;
					ly = y;
					if (Msg)
					{
						MoveMsg(lx, ly, Player);
					}
					return true;//ռ��֮��ͷ�����
				}
				else if (board[x - 1][y] == EDGE&&!GetCTypeBoxBool(x - 2, y))
				{
					Move(x - 1, y, Player);
					lx = x - 1;
					ly = y;
					
					if (Msg)
					{
						MoveMsg(lx, ly, Player);
					}
					return true;//ռ��֮��ͷ�����
				}
				else if (board[x][y + 1] == EDGE &&!GetCTypeBoxBool(x , y+2))
				{
					Move(x, y + 1, Player);
					lx = x;
					ly = y + 1;
					
					if (Msg)
					{
						MoveMsg(lx, ly, Player);
					}
					return true;//ռ��֮��ͷ�����
				}
				else if (board[x][y - 1] == EDGE &&!GetCTypeBoxBool(x, y-2))
				{
					Move(x, y - 1, Player);
					lx = x;
					ly = y - 1;
					
					if (Msg)
					{
						MoveMsg(lx, ly, Player);
					}
					return true;//ռ��֮��ͷ�����
				}
			}
		}
	}
	return false;//���ؼ�
}

//�з�
bool Board::Move(int x, int y, int owner)
{
	bool GetBoxes = false;
	Step++;
	if (GetEdgeLegality(x, y) && (owner == RED || owner == BLUE) && x >= 0 && x <= (LEN - 1) && y >= 0 && y <= (LEN - 1))
	{
		int ex, ey;
		board[x][y] = owner;//�����������
		//���һ���ĸ�������û�б���������û�б�ռ��ĸ��ԣ�ռ������
		if (IfOddNumber(x) && IfEvenNumber(y))//X����Yż��������
		{
			if (y == 0)
			{
				ex = x; ey = y + 1;
				if (board[ex][ey] == BOX&&GetBoxLiberties(ex, ey) == 0)
				{
					board[ex][ey] = owner * 2;
					GetBoxes = true;
				}
			}
			else if (y == LEN - 1)
			{
				ex = x; ey = y - 1;
				if (board[ex][ey] == BOX&&GetBoxLiberties(ex, ey) == 0)
				{
					board[ex][ey] = owner * 2;
					GetBoxes = true;
				}
			}
			else
			{
				ex = x; ey = y + 1;
				if (board[ex][ey] == BOX&&GetBoxLiberties(ex, ey) == 0)
				{
					board[ex][ey] = owner * 2;
					GetBoxes = true;
				}
				ex = x; ey = y - 1;
				if (board[ex][ey] == BOX&&GetBoxLiberties(ex, ey) == 0)
				{
					board[ex][ey] = owner * 2;
					GetBoxes = true;
				}
			}
		}
		else if (IfEvenNumber(x) && IfOddNumber(y))//Xż��Y����������
		{
			if (x == 0)
			{
				ex = x + 1; ey = y;
				if (board[ex][ey] == BOX&&GetBoxLiberties(ex, ey) == 0)
				{
					board[ex][ey] = owner * 2;
					GetBoxes = true;
				}
			}
			else if (x == LEN - 1)
			{
				ex = x - 1; ey = y;
				if (board[ex][ey] == BOX&&GetBoxLiberties(ex, ey) == 0)
				{
					board[ex][ey] = owner * 2;
					GetBoxes = true;
				}
			}
			else
			{
				ex = x + 1; ey = y;
				if (board[ex][ey] == BOX&&GetBoxLiberties(ex, ey) == 0)
				{
					board[ex][ey] = owner * 2;
					GetBoxes = true;
				}
				ex = x - 1; ey = y;
				if (board[ex][ey] == BOX&&GetBoxLiberties(ex, ey) == 0)
				{
					board[ex][ey] = owner * 2;
					GetBoxes = true;
				}
			}
		}
		return GetBoxes;
	}
	return GetBoxes;
}
int Board::GetAllMoves(LOC Moves[MOVENUM])
{
	int MoveNum = 0;
	for (int j = 0; j<LEN; j++)
	{
		for (int i = 0; i<LEN; i++)
		{
			if (board[i][j] == EDGE)
			{
				Moves[MoveNum].Set(i, j);
				MoveNum++;											//����Ŀ����
			}
		}
	}
	return MoveNum;
}
int Board::GetFreeMoves(LOC Moves[MOVENUM])
{
	int MoveNum = 0;
	//�õ����е����ɱ�
	for (int y = 1; y < LEN - 1; y = y + 2)
	{
		//���ж�ͷ����һ�����������ı��Ƿ����ɱ�
		if (GetFreeBoxBool(1, y) && GetEdgeLegality(0, y))//��һ��Ϊ�����������罻���ı�Ϊ�ձ�
		{
			Moves[MoveNum].Set(0, y);//��������
			MoveNum++;//�����ɱ���Ŀ����1
		}
		//ѭ���ж��м�ļ�������
		for (int x = 1; x < LEN - 3; x = x + 2)//x��
		{
			if (GetFreeBoxBool(x, y) && GetFreeBoxBool(x + 2, y) && GetEdgeLegality(x + 1, y))
			{
				Moves[MoveNum].Set(x + 1, y);//��������
				MoveNum++;//�����ɱ���Ŀ����1
			}
		}
		//�ж�ĩβ�ĸ���
		if (GetFreeBoxBool(LEN - 2, y) && GetEdgeLegality(LEN - 1, y))//���һ��Ϊ����������罻���ı�Ϊ�ձ�
		{
			Moves[MoveNum].Set(LEN - 1, y);//��������
			MoveNum++;//�����ɱ���Ŀ����1
		}

		//XY�滻���ٽ���һ���ж�

		//���ж�ͷ����һ�����������ı��Ƿ����ɱ�
		if (GetFreeBoxBool(y, 1) && GetEdgeLegality(y, 0))//��һ��Ϊ�����������罻���ı�Ϊ�ձ�
		{
			Moves[MoveNum].Set(y, 0);//��������
			MoveNum++;//�����ɱ���Ŀ����1
		}
		//ѭ���ж��м�ļ�������
		for (int x = 1; x < LEN - 3; x = x + 2)//x��
		{
			if (GetFreeBoxBool(y, x) && GetFreeBoxBool(y, x + 2) && GetEdgeLegality(y, x + 1))
			{
				Moves[MoveNum].Set(y, x + 1);//��������
				MoveNum++;//�����ɱ���Ŀ����1
			}
		}
		//�ж�ĩβ�ĸ���
		if (GetFreeBoxBool(y, LEN - 2) && GetEdgeLegality(y, LEN - 1))//���һ��Ϊ����������罻���ı�Ϊ�ձ�
		{
			Moves[MoveNum].Set(y, LEN - 1);//��������
			MoveNum++;//�����ɱ���Ŀ����1
		}
	}
	return MoveNum;//�������ɱߵ�����
}
int Board::GetFilterMoves(LOC Moves[MOVENUM])
{
	int MoveNum = 0;
	for (int y = 0; y < LEN; y++)
	{
		for (int x = 0; x < LEN; x++)
		{
			if (board[x][y] == EDGE)//��Ϊ�հױ�
			{
				//Board Test(board,Step);
				int BoardSave[LEN][LEN];
				int CurrentStep = Step;
				BoardCopy(board, BoardSave);	//����һ��
				Move(x, y, RED);				//���ģ����һ������

				if (IfOddNumber(x) && IfEvenNumber(y))//X����Yż��������
				{
					if (y == 0)
					{
						if (!GetLongCTypeBoxBool(x, y + 1))//���������Ǹ�����û����Ļ�������з�Ҳû����
						{
							Moves[MoveNum].Set(x, y);
							MoveNum++;//����Ŀ����
						}
					}
					else if (y == LEN - 1)
					{
						if (!GetLongCTypeBoxBool(x, y - 1))//���������Ǹ�����û����Ļ�������з�Ҳû����
						{
							Moves[MoveNum].Set(x, y);
							MoveNum++;//����Ŀ����
						}
					}
					else
					{
						if (!GetLongCTypeBoxBool(x, y + 1) && !GetLongCTypeBoxBool(x, y - 1))//������µĸ��Ӷ�û����Ļ�������з�Ҳû����
						{
							Moves[MoveNum].Set(x, y);
							MoveNum++;//����Ŀ����
						}
					}
				}
				else//����
				{
					if (x == 0)
					{
						if (!GetLongCTypeBoxBool(x + 1, y))//����ұߵ��Ǹ�����û����Ļ�������з�Ҳû����
						{
							Moves[MoveNum].Set(x, y);
							MoveNum++;//����Ŀ����
						}
					}
					else if (x == LEN - 1)
					{
						if (!GetLongCTypeBoxBool(x - 1, y))//�����ߵ��Ǹ�����û����Ļ�������з�Ҳû����
						{
							Moves[MoveNum].Set(x, y);
							MoveNum++;//����Ŀ����
						}
					}
					else
					{
						if (!GetLongCTypeBoxBool(x + 1, y) && !GetLongCTypeBoxBool(x - 1, y))//����������ߵĸ��Ӷ�û����Ļ�������з�Ҳû����
						{
							Moves[MoveNum].Set(x, y);
							MoveNum++;//����Ŀ����
						}
					}
				}

				/*if (!GetLongCTypeBoxExist())	//�������ڳ�����Ҳ����ֻ�е�������û������
				{
				//����TURN
				Moves[MoveNum].Set(x, y);
				MoveNum++;//����Ŀ����
				}*/
				SetBoard(BoardSave);			//��ԭ
				Step = CurrentStep;				//��ԭ
			}
		}
	}

	return MoveNum;
}
int Board::GetFilterMoveNum()
{
	int MoveNum = 0;
	for (int y = 0; y < LEN; y++)
	{
		for (int x = 0; x < LEN; x++)
		{
			if (board[x][y] == EDGE)//��Ϊ�հױ�
			{
				//Board Test(board,Step);
				int BoardSave[LEN][LEN];
				int CurrentStep = Step;
				BoardCopy(board, BoardSave);	//����һ��
				Move(x, y, RED);				//���ģ����һ������

				if (IfOddNumber(x) && IfEvenNumber(y))//X����Yż��������
				{
					if (y == 0)
					{
						if (!GetLongCTypeBoxBool(x, y + 1))//���������Ǹ�����û����Ļ�������з�Ҳû����
						{
							MoveNum++;//����Ŀ����
						}
					}
					else if (y == LEN - 1)
					{
						if (!GetLongCTypeBoxBool(x, y - 1))//���������Ǹ�����û����Ļ�������з�Ҳû����
						{
							MoveNum++;//����Ŀ����
						}
					}
					else
					{
						if (!GetLongCTypeBoxBool(x, y + 1) && !GetLongCTypeBoxBool(x, y - 1))//������µĸ��Ӷ�û����Ļ�������з�Ҳû����
						{
							MoveNum++;//����Ŀ����
						}
					}
				}
				else//����
				{
					if (x == 0)
					{
						if (!GetLongCTypeBoxBool(x + 1, y))//����ұߵ��Ǹ�����û����Ļ�������з�Ҳû����
						{
							MoveNum++;//����Ŀ����
						}
					}
					else if (x == LEN - 1)
					{
						if (!GetLongCTypeBoxBool(x - 1, y))//�����ߵ��Ǹ�����û����Ļ�������з�Ҳû����
						{
							MoveNum++;//����Ŀ����
						}
					}
					else
					{
						if (!GetLongCTypeBoxBool(x + 1, y) && !GetLongCTypeBoxBool(x - 1, y))//����������ߵĸ��Ӷ�û����Ļ�������з�Ҳû����
						{
							MoveNum++;//����Ŀ����
						}
					}
				}

				/*if (!GetLongCTypeBoxExist())	//�������ڳ�����Ҳ����ֻ�е�������û������
				{
				//����TURN
				Moves[MoveNum].Set(x, y);
				MoveNum++;//����Ŀ����
				}*/
				SetBoard(BoardSave);			//��ԭ
				Step = CurrentStep;				//��ԭ
			}
		}
	}

	return MoveNum;
}
bool Board::GetLongCTypeBoxBool(int bx, int by)
{
	if (GetBoxLiberties(bx, by) == DEADBOX)//����������ӱ��뱾����һ��C�͸�
	{
		int Dir[4][2] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
		for (int n = 0; n < 4; n++)
		{
			int ex = bx + Dir[n][0];
			int ey = by + Dir[n][1];
			int nbx = bx + Dir[n][0] + Dir[n][0];//��һ�����ӵ�ʵ�ʵ�ַ
			int nby = by + Dir[n][1] + Dir[n][1];//��һ�����ӵ�ʵ�ʵ�ַ
			if (board[ex][ey] == EDGE&&nbx >= 1 && nbx <= LEN - 2 && nby >= 1 && nby <= LEN - 2)
			{
				if (GetBoxLiberties(nbx, nby) == CHAINBOX)
					return true;
			}
		}
	}
	return false;
}


//Turn��֤��Ϊ��Ч�ʣ���ֱ�ӵĿ������һ��MoveҪ����Լ1000�����ؼ�����Turn��Ҫ�����µ�Board����һ��ʮ�ֺ�ʱ��
void Board::TurnMove(TURN Turn)
{
	SetBoard(Turn.NewBoard);//����Ϊ�¾���
}
int Board::GetAllTurns(TURN Turns[MOVENUM], int Player)
{
	int TurnNum = 0;
	Board ChosenBoard(board, Step);//����ģ������
	GetAllCTypeBoxes(Player,false);
	for (int j = 0; j<LEN; j++)
	{
		for (int i = 0; i<LEN; i++)
		{
			if (board[i][j] == EDGE)
			{
				Board Test(ChosenBoard.board, ChosenBoard.Step);
				//Test.Move(i, j, Player);							//��Ҿ�����һ������
				Turns[TurnNum].Owner = Player;						//��һ�ֵ������Ǹ����
				Turns[TurnNum].SetBoard(board);						//�����¾���
				Turns[TurnNum].NewBoard[i][j] = Player;
				//Turns[TurnNum].SetCaptualInfo(board, Test.board);	//�����з�
				TurnNum++;											//����Ŀ����
			}
		}
	}
	return TurnNum;
}
int Board::GetFilterTurn(TURN Turns[MOVENUM], int Player)
{
	int TurnNum = 0;
	Board ChosenBoard(board, Step);//����ģ������
	GetAllCTypeBoxes(Player,false);
	//��ʱ��ChosenBoard�Ǳ�׼���棬��Ҫ��һ��ѡ��
	for (int y = 0; y < LEN; y++)
	{
		for (int x = 0; x < LEN; x++)
		{
			if (board[x][y] == EDGE)//��Ϊ�հױ�
			{
				Board Test(ChosenBoard.board, ChosenBoard.Step);
				Test.Move(x, y, Player);//���ģ����һ������
				if (!Test.GetLongCTypeBoxExist())//�������ڳ�����Ҳ����ֻ�е�������û������
				{
					//����TURN
					Turns[TurnNum].Owner = Player;						//��һ�ֵ������Ǹ����
					Turns[TurnNum].SetBoard(Test.board);				//�����¾���
					Turns[TurnNum].SetCaptualInfo(board, Test.board);	//�����з�
					TurnNum++;//����Ŀ����
				}
			}
		}
	}

	return TurnNum;
}

//���ú���
void cprintf(char* str, WORD color, ...) {
	WORD colorOld;
	HANDLE handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(handle, &csbi);
	colorOld = csbi.wAttributes;
	SetConsoleTextAttribute(handle, color);
	cout << str;
	SetConsoleTextAttribute(handle, colorOld);
	/*
	cprintf("H", 10);//��ɫ
	cprintf("e", 9);//��ɫ
	cprintf("l", 12);//��ɫ
	cprintf("l", 11);//��ɫ
	cprintf("o", 13);//��ɫ
	cprintf("W", 15);//��
	cprintf("o", 2);//����
	cprintf("r", 5);//��
	cprintf("l", 8);//��
	cprintf("d", 14);//��
	cprintf("!", 4);//��
	*/
}
void cprintNum(int color, int Num)
{
	char str[4];
	sprintf_s(str, "%d", Num);
	cprintf(str, color);
}
bool LocEqual(LOC a, LOC b)
{
	if (a.x == b.x&&a.y == b.y)
		return true;
	return false;
}
bool BoardEqual(int a[LEN][LEN], int b[LEN][LEN])
{
	for (int y = 0; y < LEN; y++)
	{
		for (int x = 0; x < LEN; x++)
		{
			if (a[x][y] != b[x][y])
				return false;
		}
	}
	return true;
}
bool IfOddNumber(int num)
{
	//�ж�һ�������Ƿ�������
	if (num % 2 != 0)
		return true;
	return false;
}
bool IfEvenNumber(int num)
{
	//�ж�һ�������Ƿ���ż��
	if (num % 2 == 0 || num == 0)
		return true;
	return false;
}
void BoardCopy(int Source[LEN][LEN], int Target[LEN][LEN])
{
	for (int y = 0; y < LEN; y++)
	{
		for (int x = 0; x < LEN; x++)
		{
			Target[x][y] = Source[x][y];
		}
	}
}
void MoveMsg(int x, int y, int Player)
{
	cout << "Msg: ";
	if (Player == RED)
		cprintf("Red", 12);
	else
		cprintf("Blue ", 9);
	cout << " Captual Edge";
	cprintf("(", 10);
	cprintNum(10, x);
	cprintf(",", 10);
	cprintNum(10, y);
	cprintf(")\n", 10);
}









