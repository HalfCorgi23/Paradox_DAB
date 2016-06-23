#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "time.h"
#include "windows.h"

#include "Define.h"
#include "Paradox.h"
#include "MCTS.h"

using namespace std;

//UCT for DAB

//��Ĭ��һ�仰��ÿһ���ӽڵ������ֵ�����Ƕ��ڸ��ڵ������߶��Ե����棡


UCTNode::UCTNode()
{
	//��ʼ������
	Step = 0;
	for (int i = 0; i < LEN; i++)
	{
		for (int j = 0; j < LEN; j++)
		{
			board[j][i] = 0;
		}
	}
	ExistChild = 0;
	TotalChild = 0;
}
UCTNode::UCTNode(int Player, int Array[LEN][LEN], int step,bool GetCBox, int Filter_Range)
{
	//��ʼ������
	Step = step;
	for (int i = 0; i < LEN; i++)
	{
		for (int j = 0; j < LEN; j++)
		{
			board[j][i] = Array[j][i];
		}
	}
	if (GetCBox)
		GetAllCTypeBoxes(Player, false);		//��ȫ���Ե�C�͸񣬴�ʱ��Node�Զ�ԾǨΪGotten Node


	//Ȼ��ʼ��ʼ������
	Owner = Player;								//����ڵ��ӵ�����Ǵ��������
	Times = 1;									//����ʱ������ڵ��Ѿ�������һ����
	Board Win(board, Step);						//������ǰ������жϵ�
	BoardWinner = GetBoardWinner(Win, -Player);	//��������ʤ���ߣ�ע���ʱ���������ҷ�
	ExistChild = 0;								//�Ѿ����ڵ��ӽڵ���Ϊ0

	int F = Win.GetFreeEdgeNum();
	if (F>=Filter_Range)
		TotalChild = GetFreeMoves(NodeMoves);		//��ǰ�����ܽڵ���
	else
		TotalChild = GetFilterMoves(NodeMoves);		//��ǰ�����ܽڵ���
}
UCTNode* UCTNode::ExpandUCTNode(int MC_Times, int Filter_Range)
{
	//�����½ڵ�
	Board CB(board, Step);
	CB.Move(NodeMoves[ExistChild].x, NodeMoves[ExistChild].y, Owner);
	UCTNode *NewB = new UCTNode(-Owner, CB.board, CB.Step, true, Filter_Range);

	//��һ��MC����
	CB.SetBoard(NewB->board);
	NewB->AvgValue = GetFilterMCEvalution(CB, -Owner, Owner, MC_Times, Filter_Range);
	return NewB;//����NewB�ĵ�ַ
}
UCTNode* UCTNode::ExpandUCTNodeRave(int MC_Times, int Filter_Range,int RaveTable[LEN][LEN])
{
	//�����½ڵ�
	Board CB(board, Step);
	CB.Move(NodeMoves[ExistChild].x, NodeMoves[ExistChild].y, Owner);
	UCTNode *NewB = new UCTNode(-Owner, CB.board, CB.Step, true, Filter_Range);

	//��һ��MC����
	CB.SetBoard(NewB->board);
	NewB->AvgValue = GetRaveFilterMCEvalution(CB, -Owner, Owner, MC_Times, Filter_Range,RaveTable);
	return NewB;//����NewB�ĵ�ַ
}
float UCTNode::RefreshAvgValue()
{
	float Value = 0;
	//���ȼ���ƽ������
	for (int i = 0; i < ExistChild; i++)
	{
		/*if (ChildNodes[i]->AvgValue == 1)//�������һ���ӽڵ������Ϊ1
		{
			AvgValue = 0;	//�򱾽ڵ������ض�Ϊ0
			return 0;		//����0
		}*/
		Value += ChildNodes[i]->AvgValue;
	}
	AvgValue = 1 - (Value / ExistChild);//��ǰ�ڵ��ƽ������ض���(1 - �ӽڵ��ƽ������ )
	return AvgValue;
}
float UCTNode::RefreshAvgValueWithSolver()
{
	float Value = 0;
	//���ȼ���ƽ������
	bool Fail = true;
	for (int i = 0; i < ExistChild; i++)
	{
		if (ChildNodes[i]->BoardWinner != -Owner)
		{
			Fail = false;
			break;
		}
	}
	if (Fail)
	{
		BoardWinner = -Owner;
		AvgValue = 1;
		return 1;
	}
	int VaildChildNum = ExistChild;
	for (int i = 0; i < ExistChild; i++)
	{
		if (ChildNodes[i]->BoardWinner == Owner)//�������һ���ӽڵ������Ϊ1
		{
			BoardWinner = Owner;	//��ô����ڵ��ʤ���߾�����������
			AvgValue = 0;			//��ô���ڸ��ڵ���˵���ڵ������Ϊ0
			return 0;
		}
		if (ChildNodes[i]->BoardWinner == -Owner)
			VaildChildNum--;
		else
			Value += ChildNodes[i]->AvgValue;
	}
	AvgValue = 1 - (Value / VaildChildNum);//��ǰ�ڵ��ƽ������ض���(1 - ��Ч�ӽڵ��ƽ������ )
	return AvgValue;
}
float UCTNode::GetUCBValue(int Total)
{
	//double UCBValue = AvgValue + sqrt((((log10((double)Total))*2)/(double)Times));
	return AvgValue + sqrt((((log10((float)Total)) * 2) / (float)Times));
}
float UCTNode::GetUCBRaveValue(int Total,LOC Move,int RaveTable[LEN][LEN])
{
	float k = 100;
	float UCB = AvgValue + sqrt((((log10((float)Total)) * 2) / (float)Times));
	float RAVE = (float)RaveTable[Move.x][Move.y];
	float ravek = sqrt(k / (3 * Total) + k);
	float ucbk = 1 - ravek;
	float Final = ravek*RAVE + ucbk*UCB;
	return Final;
}
float UCTProcess(UCTNode &B, int &Total, int MC_Times, int Filter_Range)
{
	B.Times++;//���ʵĴ�������һ��
	if (B.BoardWinner != 0)//�����Ϸ�Ѿ�������
	{
		if (B.BoardWinner == B.Owner)
			B.AvgValue = 0;//���������ڵ���Ϸ�����ˣ��ж����档
		else
			B.AvgValue = 1;//���������ڵ���Ϸ�����ˣ��ж�����
		Total++;//�����������Ϸ�����Ľڵ㣬�򱾴ε���������
		return B.AvgValue;
	}
	if (B.ExistChild < B.TotalChild)//�������δ���Թ��Ľڵ�
	{
		Total++;//��׼���Σ����ε������������Դ���+1��
		B.ChildNodes[B.ExistChild] = B.ExpandUCTNode(MC_Times,Filter_Range);//��չһ���ӽڵ�
		B.ExistChild++;//�ӽڵ����Ŀ����1
		B.RefreshAvgValue();//ˢ������
		return B.AvgValue;
	}
	else//˵��û��δ���Թ��Ľڵ�
	{
		int BestNodeNum = 0;
		double BestUCBValue = 0;
		double UCBValue[MOVENUM];
		for (int i = 0; i < B.TotalChild; i++)
		{
			UCBValue[i] = B.ChildNodes[i]->GetUCBValue(Total);
			if (UCBValue[i] >= BestUCBValue)
			{
				BestNodeNum = i;
				BestUCBValue = UCBValue[i];
			}
		}
		UCTProcess(*B.ChildNodes[BestNodeNum], Total, MC_Times, Filter_Range);
		B.RefreshAvgValue();
		return B.AvgValue;
	}
	return 0;
}

void UCTMove(Board &CB, int Player, bool Msg)
{
	UCTNode UCTB = UCTNode(Player, CB.board, CB.Step,true, UCT_FILTER_RANGE);//���ݵ�ǰ���洴��UCT�ĸ��ڵ�
	if (UCTB.BoardWinner == 0)
	{
		int Total = 0;//UCT�Ĵ�������
		clock_t start;	//���ü�ʱ���ı���
		start = clock();
		for (int i = 0; i < UCT_TIMES; i++)//����һ������
		{
			UCTProcess(UCTB, Total, UCT_MC_TIMES, UCT_FILTER_RANGE);

			//cout<<"UCT Iteration have finished "<<i+1<<" times"<<endl;
			double totaltime = (double)(clock() - start) / CLOCKS_PER_SEC;
			if (totaltime >= UCT_LIMIT_TIME)
				break;
		}
		//�ж��������
		int BestNodeNum = 0;
		float BestAvgValue = 0;
		int LargerTimesNodeNum = 0;
		int LargerTimesValue = 0;
		for (int i = 0; i < UCTB.ExistChild; i++)
		{
			if (UCTB.ChildNodes[i]->AvgValue >= BestAvgValue)
			{
				BestNodeNum = i;
				BestAvgValue = UCTB.ChildNodes[i]->AvgValue;
			}
			if (UCTB.ChildNodes[i]->Times >= LargerTimesValue)
			{
				LargerTimesNodeNum = i;
				LargerTimesValue = UCTB.ChildNodes[i]->Times;
			}
		}

		CB.Move(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);

		if (Msg)
		{
			MoveMsg(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);
			cprintf("========================Infomation==========================\n", 8);
			cout << "��ǰ�ڵ�ƽ������Ϊ" << 1 - UCTB.AvgValue << endl;
			if (BestNodeNum == LargerTimesNodeNum)
				cprintf("�����������������ͬ��\n", 10);
			else
				cprintf("�����ʲ���ͬ��������棡\n", 14);
			cout << "�������ڵ����Ϊ" << UCTB.ChildNodes[BestNodeNum]->Times << endl;
			cout << "�������ڵ�����Ϊ" << UCTB.ChildNodes[BestNodeNum]->AvgValue << endl;
			cout << "�����ʽڵ����Ϊ" << UCTB.ChildNodes[LargerTimesNodeNum]->Times << endl;
			cout << "�����ʽڵ�����Ϊ" << UCTB.ChildNodes[LargerTimesNodeNum]->AvgValue << endl;
			cout << "����UCT�ܵ�������Ϊ" << Total << endl;
			cprintf("============================================================\n", 8);
		}
		//�ͷ��ڴ�
		DeleteUCTTree(UCTB);
	}
	else
	{
		CB.GetAllCTypeBoxes(Player,true);
		//using evalution function
		//cout << "then using evalution function" << endl;
		LatterSituationMove(CB, Player, Msg);
	}
	
	
}
void UCTMove_2(Board &CB, int Player, bool Msg)
{
	UCTNode UCTB = UCTNode(Player, CB.board, CB.Step, true, UCT_FILTER_RANGE);//���ݵ�ǰ���洴��UCT�ĸ��ڵ�
	if (UCTB.BoardWinner == 0)
	{
		int Total = 0;//UCT�Ĵ�������
		clock_t start;	//���ü�ʱ���ı���
		start = clock();
		for (int i = 0; i < UCT_TIMES; i++)//����һ������
		{
			UCTProcess(UCTB, Total, UCT_MC_TIMES, UCT_FILTER_RANGE);

			//cout<<"UCT Iteration have finished "<<i+1<<" times"<<endl;
			double totaltime = (double)(clock() - start) / CLOCKS_PER_SEC;
			if (totaltime >= UCT_LIMIT_TIME)
				break;
		}
		//�ж��������
		int BestNodeNum = 0;
		float BestAvgValue = 0;
		int LargerTimesNodeNum = 0;
		int LargerTimesValue = 0;
		for (int i = 0; i < UCTB.ExistChild; i++)
		{
			if (UCTB.ChildNodes[i]->AvgValue >= BestAvgValue)
			{
				BestNodeNum = i;
				BestAvgValue = UCTB.ChildNodes[i]->AvgValue;
			}
			if (UCTB.ChildNodes[i]->Times >= LargerTimesValue)
			{
				LargerTimesNodeNum = i;
				LargerTimesValue = UCTB.ChildNodes[i]->Times;
			}
		}

		CB.Move(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);

		if (Msg)
		{
			MoveMsg(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);
			cprintf("========================Infomation==========================\n", 8);
			cout << "��ǰ�ڵ�ƽ������Ϊ" << 1 - UCTB.AvgValue << endl;
			if (BestNodeNum == LargerTimesNodeNum)
				cprintf("�����������������ͬ��\n", 10);
			else
				cprintf("�����ʲ���ͬ��������棡\n", 14);
			cout << "�������ڵ����Ϊ" << UCTB.ChildNodes[BestNodeNum]->Times << endl;
			cout << "�������ڵ�����Ϊ" << UCTB.ChildNodes[BestNodeNum]->AvgValue << endl;
			cout << "�����ʽڵ����Ϊ" << UCTB.ChildNodes[LargerTimesNodeNum]->Times << endl;
			cout << "�����ʽڵ�����Ϊ" << UCTB.ChildNodes[LargerTimesNodeNum]->AvgValue << endl;
			cout << "����UCT�ܵ�������Ϊ" << Total << endl;
			cprintf("============================================================\n", 8);
		}
		//�ͷ��ڴ�
		DeleteUCTTree(UCTB);
	}
	else
	{
		CB.GetAllCTypeBoxes(Player, true);
		//using evalution function
		//cout << "then using evalution function" << endl;
		LatterSituationMove(CB, Player, Msg);
	}


}
void UCTMove_T(Board &CB, int Player, bool Msg)
{
	UCTNode UCTB = UCTNode(Player, CB.board, CB.Step, true, UCT_FILTER_RANGE);//���ݵ�ǰ���洴��UCT�ĸ��ڵ�
	if (UCTB.BoardWinner == 0)
	{
		int Total = 0;//UCT�Ĵ�������
		clock_t start;	//���ü�ʱ���ı���
		start = clock();
		for (int i = 0; i < UCT_TIMES; i++)//����һ������
		{
			UCTProcess(UCTB, Total, UCT_MC_TIMES, UCT_FILTER_RANGE);

			//cout<<"UCT Iteration have finished "<<i+1<<" times"<<endl;
			double totaltime = (double)(clock() - start) / CLOCKS_PER_SEC;
			if (totaltime >= UCT_LIMIT_TIME)
				break;
		}
		//�ж��������
		int BestNodeNum = 0;
		float BestAvgValue = 0;
		int LargerTimesNodeNum = 0;
		int LargerTimesValue = 0;
		for (int i = 0; i < UCTB.ExistChild; i++)
		{
			if (UCTB.ChildNodes[i]->AvgValue >= BestAvgValue)
			{
				BestNodeNum = i;
				BestAvgValue = UCTB.ChildNodes[i]->AvgValue;
			}
			if (UCTB.ChildNodes[i]->Times >= LargerTimesValue)
			{
				LargerTimesNodeNum = i;
				LargerTimesValue = UCTB.ChildNodes[i]->Times;
			}
		}

		CB.Move(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);

		if (Msg)
		{
			MoveMsg(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);
			cprintf("========================Infomation==========================\n", 8);
			cout << "��ǰ�ڵ�ƽ������Ϊ" << 1 - UCTB.AvgValue << endl;
			if (BestNodeNum == LargerTimesNodeNum)
				cprintf("�����������������ͬ��\n", 10);
			else
				cprintf("�����ʲ���ͬ��������棡\n", 14);
			cout << "�������ڵ����Ϊ" << UCTB.ChildNodes[BestNodeNum]->Times << endl;
			cout << "�������ڵ�����Ϊ" << UCTB.ChildNodes[BestNodeNum]->AvgValue << endl;
			cout << "�����ʽڵ����Ϊ" << UCTB.ChildNodes[LargerTimesNodeNum]->Times << endl;
			cout << "�����ʽڵ�����Ϊ" << UCTB.ChildNodes[LargerTimesNodeNum]->AvgValue << endl;
			cout << "����UCT�ܵ�������Ϊ" << Total << endl;
			cprintf("============================================================\n", 8);
		}
		//�ͷ��ڴ�
		DeleteUCTTree(UCTB);
	}
	else
	{
		CB.GetAllCTypeBoxes(Player, true);
		//using evalution function
		//cout << "then using evalution function" << endl;
		LatterSituationMove(CB, Player, Msg);
	}


}

bool SearchingUCTNode(UCTNode* SearchingNode, int CurrentBoard[LEN][LEN], int Player, int Depth)
{
	if (BoardEqual(SearchingNode->board, CurrentBoard) && Player == SearchingNode->Owner)//���ϲ�������
	{
		//CurrentUCTNode = SearchingNode;
		return true;
	}
	else if (SearchingNode->BoardWinner != 0 && Depth > 0 && SearchingNode->ExistChild > 0)//��δʤ�� and ���δ�� and ���ӽڵ�
	{
		for (int i = 0; i < SearchingNode->ExistChild; i++)
		{
			if (SearchingUCTNode(SearchingNode->ChildNodes[i], CurrentBoard, Player, Depth - 1))
				return true;
		}
	}
	return false;
}
void DeleteUCTNode(UCTNode* Root)
{
	if (Root->ExistChild > 0)
	{
		for (int i = 0; i < Root->ExistChild; i++)
		{
			DeleteUCTNode(Root->ChildNodes[i]);
			delete Root->ChildNodes[i];
		}
	}
}
void DeleteUCTTree(UCTNode Root)
{
	for (int i = 0; i < Root.ExistChild; i++)
	{
		DeleteUCTNode(Root.ChildNodes[i]);
		delete Root.ChildNodes[i];
	}
}

void UCTMoveWithSacrifice(Board &CB,int Player,bool Msg,int branch)
{
	BoxBoard Dead(CB);
	bool DeadChain = Dead.GetDeadChainExist();
	bool DeadCircle = Dead.GetDeadCircleExist();
	if (DeadCircle||DeadChain)//�л���������ȴ���
	{
		LOC DCMove;
		if (DeadChain)//Ȼ��������
		{
			//���ȳԵ�̰�����ٽ��
			for (;;)
			{
				Board Test = CB;
				Test.GetCTypeBox(Player,false);
				BoxBoard Dead(Test);
				if (Dead.GetDeadChainExist())
					CB.GetCTypeBox(Player, Msg);
				else
					break;
			}
			//Ȼ��ʼ����DoubleCross
			DCMove = CB.GetDoubleCrossLoc(Player);
		}
		else
		{
			//���ȳԵ�̰�����ٽ��
			for (;;)
			{
				Board Test = CB;
				Test.GetCTypeBox(Player, false);
				BoxBoard Dead(Test);
				if (Dead.GetDeadCircleExist())
					CB.GetCTypeBox(Player, Msg);
				else
					break;
			}
			//Ȼ��ʼ����DoubleCross
			DCMove = CB.GetDoubleCrossLoc(Player);
		}
		//Ȼ��ʼ����UCTProcess
		UCTNode GreedyChild = UCTNode(Player, CB.board, CB.Step, true, UCT_FILTER_RANGE);//���ݵ�ǰ���洴��UCT�ĸ��ڵ�
		UCTNode SacrificeChild = UCTNode(-Player, CB.board, CB.Step, false, UCT_FILTER_RANGE);//���ݵ�ǰ���洴��UCT�ĸ��ڵ�
		SacrificeChild.Move(DCMove.x, DCMove.y, Player);
		for (;;)//ֱ���޷�ռ��CTypeBox�˾ͽ���
		{
			if (!SacrificeChild.GetCTypeBoxLimit(Player, false))
				break;
		}
		//��ʱ��Sacrifice��׼�������ľ�����
		SacrificeChild.TotalChild = SacrificeChild.GetFilterMoves(SacrificeChild.NodeMoves);
		Board Win(SacrificeChild.board, SacrificeChild.Step);		//������ǰ������жϵ�
		SacrificeChild.BoardWinner = GetBoardWinner(Win, Player);	//��������ʤ���ߣ�ע���ʱ�������ǶԷ�
		if (SacrificeChild.BoardWinner == 0)
		{
			//
			int Total = 0;//UCT�Ĵ�������
			int Total2 = 0;
			clock_t start;	//���ü�ʱ���ı���
			start = clock();
			for (int i = 0; i < UCT_TIMES / 2; i++)//����һ������
			{
				UCTProcess(SacrificeChild, Total, UCT_MC_TIMES, UCT_FILTER_RANGE);
				UCTProcess(GreedyChild, Total2, UCT_MC_TIMES, UCT_FILTER_RANGE);
				//cout<<"UCT Iteration have finished "<<i+1<<" times"<<endl;
				double totaltime = (double)(clock() - start) / CLOCKS_PER_SEC;
				if (totaltime >= UCT_LIMIT_TIME)
					break;
			}
			//

			//�ж��������
			int BestNodeNum = 0;
			float BestAvgValue = 0;
			for (int i = 0; i < GreedyChild.ExistChild; i++)
			{
				if (GreedyChild.ChildNodes[i]->AvgValue >= BestAvgValue)
				{
					BestNodeNum = i;
					BestAvgValue = GreedyChild.ChildNodes[i]->AvgValue;
				}
			}

			//��̰��
			if (BestAvgValue >= SacrificeChild.AvgValue)
			{
				CB.GetAllCTypeBoxes(Player, Msg);
				CB.Move(GreedyChild.NodeMoves[BestNodeNum].x, GreedyChild.NodeMoves[BestNodeNum].y, Player);
				if (Msg)
					MoveMsg(GreedyChild.NodeMoves[BestNodeNum].x, GreedyChild.NodeMoves[BestNodeNum].y, Player);
			}
			//ʹ������
			else
			{
				CB.Move(DCMove.x, DCMove.y, Player);
				for (;;)//ֱ���޷�ռ��CTypeBox�˾ͽ���
				{
					if (!CB.GetCTypeBoxLimit(Player, Msg))
						break;
				}
				if (Msg)
					MoveMsg(DCMove.x, DCMove.y, Player);
			}
			DeleteUCTTree(GreedyChild);
			DeleteUCTTree(SacrificeChild);
			if (Msg)
			{
				cprintf("========================Infomation==========================\n", 8);
				cout << "�����Ľڵ�����Ϊ" << SacrificeChild.AvgValue << endl;
				cout << "̰����ѽڵ�����Ϊ" << GreedyChild.ChildNodes[BestNodeNum]->AvgValue << endl;
				cprintf("============================================================\n", 8);
			}
		}
		else
		{

		}
	}

	else//����UCT�ƶ�
	{
		CB.GetAllCTypeBoxes(Player, Msg);
		if (branch ==1)
			UCTMove_T(CB, Player, true);
		else if (branch == 2)
			UCTMove_2(CB, Player, true);
		else if (branch == 3)
			UCTRaveMove(CB, Player, true);
		else if (branch == 4)
			UCTRaveSolverDoubleMove(CB, Player, true);
		else
			UCTMove(CB, Player, true);
	}
}
void LatterSituationMove(Board &CB, int Player, bool Msg)
{
	//�����㷨����ʱֻ�г����ͻ���
	if (CB.GetLongCTypeBoxExist())
	{
		//���д򿪵ĳ��������������벻����֮�������״̬�����Ƿ�������
		BoxBoard Dead(CB);
		int SacrificeBoxNum;
		if (Dead.GetDeadChainExist())
			SacrificeBoxNum = 2;
		if (Dead.GetDeadCircleExist())
			SacrificeBoxNum = 4;
		
		//����ȫ�����Ե���
		Dead.GetAllCTypeBoxes(Player,false);
		LOC BoxNum = Dead.GetRationalStateBoxNum();//x�����ֵģ�y�Ǻ��ֵ�

		//���ڸ��ݽ����������ܵõ��ĸ����������з���

		//�����ڵ�ǰ��ȫ���������ֿ����õ�x�������ֿ����õ�y�����ҷ�ȫ�Ժ�����õ�y+n���Է��õ�x������x-y<n���ҷ�ȫ��

		if (BoxNum.x - BoxNum.y <= SacrificeBoxNum||Dead.Winner()!=0)
		{
			CB.GetAllCTypeBoxes(Player, Msg);
			if (Dead.Winner()==0)
				LatterSituationMove(CB, Player, Msg);
		}
		else
		{
			//��������ʱ����������������
			LOC DCMove;
			if (SacrificeBoxNum==2)//Ȼ��������
			{
				//���ȳԵ�̰�����ٽ��
				for (;;)
				{
					Board Test = CB;
					Test.GetCTypeBox(Player, false);
					BoxBoard Dead(Test);
					if (Dead.GetDeadChainExist())
						CB.GetCTypeBox(Player, Msg);
					else
						break;
				}
				//Ȼ��ʼ����DoubleCross
				DCMove = CB.GetDoubleCrossLoc(Player);
			}
			else
			{
				//���ȳԵ�̰�����ٽ��
				for (;;)
				{
					Board Test = CB;
					Test.GetCTypeBox(Player, false);
					BoxBoard Dead(Test);
					if (Dead.GetDeadCircleExist())
						CB.GetCTypeBox(Player, Msg);
					else
						break;
				}
				//Ȼ��ʼ����DoubleCross
				DCMove = CB.GetDoubleCrossLoc(Player);
			}

			CB.Move(DCMove.x, DCMove.y, Player);
			for (;;)//ֱ���޷�ռ��CTypeBox�˾ͽ���
			{
				if (!CB.GetCTypeBoxLimit(Player, Msg))
					break;
			}
			MoveMsg(DCMove.x, DCMove.y, Player);
			//��������
		}

	}
	else
	{
		//ѡ�����һ����������������״̬�����Ǵ���̵ĳ����������
		CB.GetAllCTypeBoxes(Player, Msg);
		BoxBoard Test(CB);
		if (Test.RationalState(Test.GetRationalStateBoxNum()))
		{
			//��������Ծ���Ļ�,���δ���
			LOC M = Test.GetOpenSuitableChainLoc();
			CB.Move(M.x, M.y, Player);
			if (Msg)
				MoveMsg(M.x, M.y, Player);
		}
		else
		{
			//����������Ծ���Ļ�������̵���
			//��������Ծ���Ļ�
			LOC M = Test.GetOpenShortestChainLoc();
			CB.Move(M.x, M.y, Player);
			if (Msg)
				MoveMsg(M.x, M.y, Player);
		}
	}
}

//��Ϸ�ƶ��������ǰ�к����Զ��ƶ�
void GameTurnMove(Board &CB, int Player, bool Msg)
{
	//This Function is using for the game's move turn.

	Board Test = CB;
	Test.GetAllCTypeBoxes(Player,false);
	bool LatterSituation = (Test.GetFilterMoveNum() == 0);
	//bool LongCTypeBoolExist = CB.GetLongCTypeBoxExist();
	if (!LatterSituation)
	{
		//cout << "ʹ��UCT" << endl;
		UCTMoveWithSacrifice(CB, Player, Msg,0);
	}
	else//Ҳ���Ǻ��ھ�����
	{
		//Ҳ����Filter���Ѿ�����Ϊ��������£�ֻ��LongChain,Circle,PreCircle
		//cout << "then using evalution function" << endl;
		//cout << "ʹ�ú���" << endl;
		LatterSituationMove(CB, Player, Msg);
	}
}
void GameTurnMove_T(Board &CB, int Player, bool Msg)
{
	//This Function is using for the game's move turn.

	Board Test = CB;
	Test.GetAllCTypeBoxes(Player, false);
	bool LatterSituation = (Test.GetFilterMoveNum() == 0);
	//bool LongCTypeBoolExist = CB.GetLongCTypeBoxExist();
	if (!LatterSituation)
	{
		//cout << "ʹ��UCT" << endl;
		UCTMoveWithSacrifice(CB, Player, Msg,1);
	}
	else//Ҳ���Ǻ��ھ�����
	{
		//Ҳ����Filter���Ѿ�����Ϊ��������£�ֻ��LongChain,Circle,PreCircle
		//cout << "then using evalution function" << endl;
		//cout << "ʹ�ú���" << endl;
		LatterSituationMove(CB, Player, Msg);
	}
}

//Rave
float UCTProcessRave(UCTNode &B, int &Total, int MC_Times, int Filter_Range,int RaveTable[LEN][LEN])
{
	B.Times++;//���ʵĴ�������һ��
	if (B.BoardWinner != 0)//�����Ϸ�Ѿ�������
	{
		if (B.BoardWinner == B.Owner)
			B.AvgValue = 0;//���������ڵ���Ϸ�����ˣ��ж����档
		else
			B.AvgValue = 1;//���������ڵ���Ϸ�����ˣ��ж�����
		Total++;//�����������Ϸ�����Ľڵ㣬�򱾴ε���������
		return B.AvgValue;
	}
	if (B.ExistChild < B.TotalChild)//�������δ���Թ��Ľڵ�
	{
		Total++;//��׼���Σ����ε������������Դ���+1��
		B.ChildNodes[B.ExistChild] = B.ExpandUCTNodeRave(MC_Times, Filter_Range,RaveTable);//��չһ���ӽڵ�
		B.ExistChild++;//�ӽڵ����Ŀ����1
		B.RefreshAvgValue();//ˢ������
		return B.AvgValue;
	}
	else//˵��û��δ���Թ��Ľڵ�
	{
		int BestNodeNum = 0;
		float BestUCBValue = 0;
		float UCBValue[MOVENUM];
		for (int i = 0; i < B.TotalChild; i++)
		{
			UCBValue[i] = B.ChildNodes[i]->GetUCBRaveValue(Total,B.NodeMoves[i],RaveTable);
			if (UCBValue[i] >= BestUCBValue)
			{
				BestNodeNum = i;
				BestUCBValue = UCBValue[i];
			}
		}
		UCTProcessRave(*B.ChildNodes[BestNodeNum], Total, MC_Times, Filter_Range,RaveTable);
		B.RefreshAvgValue();
		return B.AvgValue;
	}
	return 0;
}
void UCTRaveMove(Board &CB, int Player, bool Msg)
{
	UCTNode UCTB = UCTNode(Player, CB.board, CB.Step, true, UCT_FILTER_RANGE);//���ݵ�ǰ���洴��UCT�ĸ��ڵ�
	if (UCTB.BoardWinner == 0)
	{
		int Total = 0;//UCT�Ĵ�������
		clock_t start;	//���ü�ʱ���ı���
		start = clock();
		int RaveTable[LEN][LEN];
		for (int y = 0; y < LEN; y++)
		{
			for (int x = 0; x < 0; x++)
			{
				RaveTable[x][y] = 0;
			}
		}
		for (int i = 0; i < UCT_TIMES; i++)//����һ������
		{
			UCTProcessRave(UCTB, Total, UCT_MC_TIMES, UCT_FILTER_RANGE,RaveTable);

			//cout<<"UCT Iteration have finished "<<i+1<<" times"<<endl;
			double totaltime = (double)(clock() - start) / CLOCKS_PER_SEC;
			if (totaltime >= UCT_LIMIT_TIME)
				break;
		}
		//�ж��������
		int BestNodeNum = 0;
		float BestAvgValue = 0;
		int LargerTimesNodeNum = 0;
		int LargerTimesValue = 0;
		for (int i = 0; i < UCTB.ExistChild; i++)
		{
			if (UCTB.ChildNodes[i]->AvgValue >= BestAvgValue)
			{
				BestNodeNum = i;
				BestAvgValue = UCTB.ChildNodes[i]->AvgValue;
			}
			if (UCTB.ChildNodes[i]->Times >= LargerTimesValue)
			{
				LargerTimesNodeNum = i;
				LargerTimesValue = UCTB.ChildNodes[i]->Times;
			}
		}

		CB.Move(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);

		if (Msg)
		{
			MoveMsg(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);
			cprintf("========================Infomation==========================\n", 8);
			cout << "��ǰ�ڵ�ƽ������Ϊ" << 1 - UCTB.AvgValue << endl;
			if (BestNodeNum == LargerTimesNodeNum)
				cprintf("�����������������ͬ��\n", 10);
			else
				cprintf("�����ʲ���ͬ��������棡\n", 14);
			cout << "�������ڵ����Ϊ" << UCTB.ChildNodes[BestNodeNum]->Times << endl;
			cout << "�������ڵ�����Ϊ" << UCTB.ChildNodes[BestNodeNum]->AvgValue << endl;
			cout << "�����ʽڵ����Ϊ" << UCTB.ChildNodes[LargerTimesNodeNum]->Times << endl;
			cout << "�����ʽڵ�����Ϊ" << UCTB.ChildNodes[LargerTimesNodeNum]->AvgValue << endl;
			cout << "����UCT�ܵ�������Ϊ" << Total << endl;
			cprintf("============================================================\n", 8);
		}
		//�ͷ��ڴ�
		DeleteUCTTree(UCTB);
	}
	else
	{
		CB.GetAllCTypeBoxes(Player, true);
		//using evalution function
		//cout << "then using evalution function" << endl;
		LatterSituationMove(CB, Player, Msg);
	}


}
void GameTurnMove_Rave(Board &CB, int Player, bool Msg)
{
	//This Function is using for the game's move turn.

	Board Test = CB;
	Test.GetAllCTypeBoxes(Player, false);
	bool LatterSituation = (Test.GetFilterMoveNum() == 0);
	//bool LongCTypeBoolExist = CB.GetLongCTypeBoxExist();
	if (!LatterSituation)
	{
		//cout << "ʹ��UCT" << endl;
		UCTMoveWithSacrifice(CB, Player, Msg, 3);
	}
	else//Ҳ���Ǻ��ھ�����
	{
		//Ҳ����Filter���Ѿ�����Ϊ��������£�ֻ��LongChain,Circle,PreCircle
		//cout << "then using evalution function" << endl;
		//cout << "ʹ�ú���" << endl;
		LatterSituationMove(CB, Player, Msg);
	}
}

//Rave Double Solver
float UCTProcessRaveSolver(UCTNode &B, int &Total, int MC_Times, int Filter_Range, int RaveTable[LEN][LEN])
{
	B.Times++;//���ʵĴ�������һ��
	if (B.BoardWinner != 0)//�����Ϸ�Ѿ�������
	{
		if (B.BoardWinner == B.Owner)
			B.AvgValue = 0;//���������ڵ���Ϸ�����ˣ��ж����档
		else
			B.AvgValue = 1;//���������ڵ���Ϸ�����ˣ��ж�����
		Total++;//�����������Ϸ�����Ľڵ㣬�򱾴ε���������
		return B.AvgValue;
	}
	if (B.ExistChild < B.TotalChild)//�������δ���Թ��Ľڵ�
	{
		Total++;//��׼���Σ����ε������������Դ���+1��
		B.ChildNodes[B.ExistChild] = B.ExpandUCTNodeRave(MC_Times, Filter_Range, RaveTable);//��չһ���ӽڵ�
		B.ExistChild++;//�ӽڵ����Ŀ����1
		B.RefreshAvgValueWithSolver();//ˢ������
		return B.AvgValue;
	}
	else//˵��û��δ���Թ��Ľڵ�
	{
		int BestNodeNum = 0;
		float BestUCBValue = 0;
		float UCBValue[MOVENUM];
		for (int i = 0; i < B.TotalChild; i++)
		{
			UCBValue[i] = B.ChildNodes[i]->GetUCBRaveValue(Total, B.NodeMoves[i], RaveTable);
			if (UCBValue[i] >= BestUCBValue)
			{
				BestNodeNum = i;
				BestUCBValue = UCBValue[i];
			}
		}
		UCTProcessRaveSolver(*B.ChildNodes[BestNodeNum], Total, MC_Times, Filter_Range,RaveTable);
		B.RefreshAvgValue();
		return B.AvgValue;
	}
	return 0;
}
void UCTRaveSolverDoubleMove(Board &CB, int Player, bool Msg)
{
	UCTNode UCTB = UCTNode(Player, CB.board, CB.Step, true, UCT_FILTER_RANGE);//���ݵ�ǰ���洴��UCT�ĸ��ڵ�
	if (UCTB.BoardWinner == 0)
	{
		int Total = 0;//UCT�Ĵ�������
		clock_t start;	//���ü�ʱ���ı���
		start = clock();
		int RaveTable[LEN][LEN];
		for (int y = 0; y < LEN; y++)
		{
			for (int x = 0; x < 0; x++)
			{
				RaveTable[x][y] = 0;
			}
		}
		for (int i = 0; i < UCT_TIMES; i++)//����һ������
		{
			UCTProcessRaveSolver(UCTB, Total, 2, UCT_FILTER_RANGE, RaveTable);

			//cout<<"UCT Iteration have finished "<<i+1<<" times"<<endl;
			double totaltime = (double)(clock() - start) / CLOCKS_PER_SEC;
			if (totaltime >= UCT_LIMIT_TIME)
				break;
		}
		//�ж��������
		int BestNodeNum = 0;
		float BestAvgValue = 0;
		int LargerTimesNodeNum = 0;
		int LargerTimesValue = 0;
		for (int i = 0; i < UCTB.ExistChild; i++)
		{
			if (UCTB.ChildNodes[i]->AvgValue >= BestAvgValue)
			{
				BestNodeNum = i;
				BestAvgValue = UCTB.ChildNodes[i]->AvgValue;
			}
			if (UCTB.ChildNodes[i]->Times >= LargerTimesValue)
			{
				LargerTimesNodeNum = i;
				LargerTimesValue = UCTB.ChildNodes[i]->Times;
			}
		}

		CB.Move(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);

		if (Msg)
		{
			MoveMsg(UCTB.NodeMoves[BestNodeNum].x, UCTB.NodeMoves[BestNodeNum].y, Player);
			cprintf("========================Infomation==========================\n", 8);
			cout << "��ǰ�ڵ�ƽ������Ϊ" << 1 - UCTB.AvgValue << endl;
			cout << "�������ڵ����Ϊ" << UCTB.ChildNodes[BestNodeNum]->Times << endl;
			cout << "�������ڵ�����Ϊ" << UCTB.ChildNodes[BestNodeNum]->AvgValue << endl;
			cout << "����UCT�ܵ�������Ϊ" << Total << endl;
			cprintf("============================================================\n", 8);
		}
		//�ͷ��ڴ�
		DeleteUCTTree(UCTB);
	}
	else
	{
		CB.GetAllCTypeBoxes(Player, true);
		//using evalution function
		//cout << "then using evalution function" << endl;
		LatterSituationMove(CB, Player, Msg);
	}


}
void RaveSolverDouble(Board &CB, int Player, bool Msg)
{
	//This Function is using for the game's move turn.

	Board Test = CB;
	Test.GetAllCTypeBoxes(Player, false);
	bool LatterSituation = (Test.GetFilterMoveNum() == 0);
	//bool LongCTypeBoolExist = CB.GetLongCTypeBoxExist();
	if (!LatterSituation)
	{
		//cout << "ʹ��UCT" << endl;
		UCTMoveWithSacrifice(CB, Player, Msg, 4);
	}
	else//Ҳ���Ǻ��ھ�����
	{
		//Ҳ����Filter���Ѿ�����Ϊ��������£�ֻ��LongChain,Circle,PreCircle
		//cout << "then using evalution function" << endl;
		//cout << "ʹ�ú���" << endl;
		LatterSituationMove(CB, Player, Msg);
	}
}
