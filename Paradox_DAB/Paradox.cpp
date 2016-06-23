#include "stdafx.h"
#include "Define.h"
#include <iostream>
#include <stdio.h>
#include "time.h"
#include "windows.h"
#include "Paradox.h"
#include <hash_map>
#include <math.h>
#include "MCTS.h"

using namespace std;



//Some  Game Function
void MakeLaterSituation(Board &NewB,int StartPlayer,bool Show)
{
	int player = StartPlayer;
	for (; NewB.GetFreeEdgeNum()>0;)
	{
		if (NewB.GetFreeEdgeNum() == 0)
			break;
		bool Cap = false;
		player = RndFilterTurn(NewB, player,false,FILTER_RANGE);
	}
	if (Show)
		NewB.PrintBoard();
}
int GetBoardWinner(Board &CB, int LatterPlayer)
{
	LOC a[MOVENUM];
	if (CB.GetFilterMoves(a) != 0)
		return 0;
	BoxBoard Advanced(CB);
	int w = Advanced.GetBoardWinner(LatterPlayer);
	return w;
}
/*int GetBoardWinner2(Board &CB, int LatterPlayer)
{
	if (CB.GetFreeEdgeNum() != 0)
		return 0;
	BoxBoard Advanced(CB);
	
	int w = Advanced.GetBoardWinner(LatterPlayer);
	return w;
}*/

//����غϡ�����ʹ��̰��
void RndTurn(Board &CB, int Player, bool Msg)
{
	LOC Moves[MOVENUM];
	CB.GetAllCTypeBoxes(Player, Msg);
	int MoveNum = CB.GetAllMoves(Moves);//ȷ�����������TurnNum������
	if (MoveNum == 0)
	{
		cout << "RndTurn�����е�TurnNum =0" << endl;
		CB.PrintBoard();
		system("pause");
	}
	int Rnd = rand() % MoveNum;//��0-MoveNum�г�ȡһ�������
	CB.Move(Moves[Rnd].x, Moves[Rnd].y, Player);
	if (Msg)
		MoveMsg(Moves[Rnd].x, Moves[Rnd].y, Player);
}
int RndFilterTurn(Board &CB, int Player, bool Msg, int Filter_Range)
{
	LOC Moves[MOVENUM];
	CB.GetAllCTypeBoxes(Player, Msg);

	int MoveNum;
	int FreeEdge = CB.GetFreeEdgeNum();
	if (FreeEdge < Filter_Range)//����FreeEdge����С��25������¿���Filter
		MoveNum = CB.GetFilterMoves(Moves);//ȷ�����������MoveNum������
	else
		MoveNum = CB.GetFreeMoves(Moves);//ȷ�����������MoveNum������

	if (MoveNum != 0)//��ĳЩʱ�����ڳԵ���ǰ���C�͸񡣿��ܵ���MoveNum������Ϊ0.��ʱ��ֻҪ������һ����Ȼ�ͻῪʼ�ж�ʤ����
	{
		int Rnd = rand() % MoveNum;//��0-MoveNum�г�ȡһ�������
		CB.Move(Moves[Rnd].x, Moves[Rnd].y, Player);
		if (Msg)
			MoveMsg(Moves[Rnd].x, Moves[Rnd].y, Player);
		return -Player;//����
	}
	else
	{
		return Player;//������
	}
	
}

//MC
int GetMCWinner(Board &CB, int NextPlayer)
{
	int player = NextPlayer;
	while (CB.GetFreeEdgeNum() != 0)//�����������ɱߵ�ʱ��
	{
		RndTurn(CB, player, false);
		player = -player;
	}
	int W = GetBoardWinner(CB, -player);
	return W;
}
float GetMCEvalution(Board &CB, int NextPlayer, int Winner, int TIMES)
{
	Board MCB = CB;//�ȸ���һ������
	int MCE = 0;
	for (int i = 0; i<TIMES; i++)
	{
		if (GetMCWinner(MCB, NextPlayer) == Winner)
			MCE++;
	}
	float score = ((float)MCE) / ((float)TIMES);
	return score;
}
void MonteCarloMove(Board &CB, int Player, bool Msg)
{
	//����ע����ǣ�MC��ʼʱ��һ��Ҫע����û��C�͸��״̬���������C�͸����Ⱦ����Ƿ�ռ�ݡ�
	CB.GetAllCTypeBoxes(Player, Msg);//���ȳԵ����е�C�͸�
	LOC Moves[MOVENUM];
	int MoveNum = CB.GetAllMoves(Moves);		//ȷ�����������TurnNum������
	float Best = -1;							//ȷ�����۶��õĳɼ�������һ��Best
	int BestMove;
	for (int i = 0; i<MoveNum; i++)
	{
		Board MCB(CB.board, CB.Step);				//�ȴ���һ��������
		MCB.Move(Moves[i].x, Moves[i].y, Player);	//�ٶ��ҷ��ƶ���һ����
		int NextPlayer = -Player;					//��һ���ƶ��߽��ǶԷ�
		float Eva = GetMCEvalution(MCB, NextPlayer, Player, MC_TIMES);
		if (Eva >= Best)//������ڵ�����ѳɼ������¼�¼
		{
			Best = Eva;
			BestMove = i;
		}
	}
	CB.Move(Moves[BestMove].x, Moves[BestMove].y, Player);//ѡ����Ѿ���
	if (Msg)
		MoveMsg(Moves[BestMove].x, Moves[BestMove].y, Player);
}

//Filter MC
int GetFilterMCWinner(Board &CB, int NextPlayer, int Filter_Range)
{
	int player = NextPlayer;
	while (CB.GetFreeEdgeNum() !=0)//�����������ɱߵ�ʱ��
	{
		player = RndFilterTurn(CB, player, false, Filter_Range);
		//CB.PrintBoard();
		//system("pause");
	}
	//cout << "Finish" << endl;
	int W = GetBoardWinner(CB, -player);
	return W;
}
float GetFilterMCEvalution(Board &CB, int NextPlayer, int Winner, int TIMES, int Filter_Range)
{
	Board MCB = CB;//�ȸ���һ������
	int MCE = 0;
	for (int i = 0; i<TIMES; i++)
	{
		if (GetFilterMCWinner(MCB, NextPlayer, Filter_Range) == Winner)
			MCE++;
	}
	float score = ((float)MCE) / ((float)TIMES);
	//MCB.ShowBoard();
	//cout << "This one score is " << score << endl;
	return score;
}
float GetFilterMCSingleEva(Board &CB, int NextPlayer, int Winner, int Filter_Range)
{
	Board MCB = CB;//�ȸ���һ������
	if (GetFilterMCWinner(MCB, NextPlayer, Filter_Range) == Winner)
		return 1;
	return 0;
}
void FilterMCMove(Board &CB, int Player, bool Msg)
{
	//����ע����ǣ�MC��ʼʱ��һ��Ҫע����û��C�͸��״̬���������C�͸����Ⱦ����Ƿ�ռ�ݡ�
	CB.GetAllCTypeBoxes(Player,Msg);//���ȳԵ����е�C�͸�
	LOC Moves[MOVENUM];
	int MoveNum;
	int FreeEdge = CB.GetFreeEdgeNum();
	if (FreeEdge < FILTER_RANGE)//����FreeEdge��������4������¿���Filter
		MoveNum = CB.GetFilterMoves(Moves);//ȷ�����������MoveNum������
	else
		MoveNum = CB.GetFreeMoves(Moves);//ȷ�����������MoveNum������

	float Best = -1;							//ȷ�����۶��õĳɼ�������һ��Best
	int BestMove;
	for (int i = 0; i<MoveNum; i++)
	{
		Board MCB(CB.board, CB.Step);				//�ȴ���һ��������
		MCB.Move(Moves[i].x, Moves[i].y, Player);	//�ٶ��ҷ��ƶ���һ����
		int NextPlayer = -Player;					//��һ���ƶ��߽��ǶԷ�
		float Eva = GetFilterMCEvalution(MCB, NextPlayer, Player, MC_TIMES,FILTER_RANGE);
		if (Eva >= Best)//������ڵ�����ѳɼ������¼�¼
		{
			Best = Eva;
			BestMove = i;
		}
	}
	CB.Move(Moves[BestMove].x, Moves[BestMove].y, Player );//ѡ����Ѿ���
	if (Msg)
		MoveMsg(Moves[BestMove].x, Moves[BestMove].y, Player);
}

float GetRaveFilterMCEvalution(Board &CB, int NextPlayer, int Winner, int TIMES, int Filter_Range,int RaveTable[LEN][LEN])
{
	Board MCB = CB;//�ȸ���һ������
	int MCE = 0;
	for (int i = 0; i<TIMES; i++)
	{
		if (GetFilterMCWinner(MCB, NextPlayer, Filter_Range) == Winner)
		{
			MCE++;
			for (int y = 0; y < LEN; y++)
			{
				for (int x = 0; x < 0; x++)
				{
					if (MCB.board[x][y] == Winner)
						RaveTable[x][y]++;
				}
			}
		}
	}
	float score = ((float)MCE) / ((float)TIMES);
	//MCB.ShowBoard();
	//cout << "This one score is " << score << endl;
	return score;
}
