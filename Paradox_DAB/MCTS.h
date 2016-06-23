#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include "time.h"
#include "windows.h"

#include "Define.h"
#include "BoxBoard.h"
#include "Paradox.h"

#pragma  once

#define UCT_MC_TIMES 1
#define UCT_TIMES 15000000
#define UCT_LIMIT_TIME 32
#define UCT_FILTER_RANGE 30


//UCT
class UCTNode :public Board
{
public:
	UCTNode();//���캯��
	UCTNode(int Player, int Array[LEN][LEN], int step, bool GetCBox, int Filter_Range);//���캯��

	//UCTʹ�õ�ֵ
	int Owner;						//�������
	int Times;						//�����Թ��Ĵ���
	int BoardWinner;				//��������ʤ����
	int ExistChild;					//�ӽڵ㵱ǰ����Ŀ
	int TotalChild;					//�ӽڵ��������
	float AvgValue;					//�Ը��ڵ���ԣ�����ڵ��ƽ������ 
	LOC NodeMoves[MOVENUM];			//����ӽڵ����п��ܵ���ʽ
	UCTNode* ChildNodes[MOVENUM];	//ָ���һ���ӽڵ��ָ��

	//���ܺ���
	float RefreshAvgValue();									//����ƽ������
	float RefreshAvgValueWithSolver();
	float GetUCBValue(int Total);								//�������ڵ�ƽ����������UCBֵ
	float GetUCBRaveValue(int Total, LOC Move, int RaveTable[LEN][LEN]);
	UCTNode* ExpandUCTNode(int MC_Times, int Filter_Range);
	UCTNode* ExpandUCTNodeRave(int MC_Times, int Filter_Range, int RaveTable[LEN][LEN]);
};
float UCTProcess(UCTNode &B, int &Total, int MC_Times, int Filter_Range);
void UCTMove(Board &CB, int Player, bool Msg);
void UCTMove_1(Board &CB, int Player, bool Msg);
void UCTMove_2(Board &CB, int Player, bool Msg);
void UCTMove_T(Board &CB, int Player, bool Msg);

bool SearchingUCTNode(UCTNode* SearchingNode, int CurrentBoard[LEN][LEN], int Depth);
void DeleteUCTNode(UCTNode* Root);
void DeleteUCTTree(UCTNode Root);


void UCTMoveWithSacrifice(Board &CB, int Player, bool Msg, int branch);
void LatterSituationMove(Board &CB, int Player, bool Msg);
void GameTurnMove(Board &CB, int Player, bool Msg);
void GameTurnMove_T(Board &CB, int Player, bool Msg);
void GameTurnMove_Rave(Board &CB, int Player, bool Msg);
void RaveSolverDouble(Board &CB, int Player, bool Msg);

//Rave
float UCTProcessRave(UCTNode &B, int &Total, int MC_Times, int Filter_Range, int RaveTable[LEN][LEN]);
void UCTRaveMove(Board &CB, int Player, bool Msg);

//RaveSolverDouble
float UCTProcessRaveSolver(UCTNode &B, int &Total, int MC_Times, int Filter_Range, int RaveTable[LEN][LEN]);
void UCTRaveSolverDoubleMove(Board &CB, int Player, bool Msg);
