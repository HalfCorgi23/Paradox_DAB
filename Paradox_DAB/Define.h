#include "stdafx.h"
#include "windows.h"

#pragma  once

//���弸������
#define RED 1
#define BLUE -1
#define EMPTY 0
#define EDGE 10
#define BOX 20
#define RED_EDGE 1
#define BLUE_EDGE -1
#define RED_BOX 2
#define BLUE_BOX -2

//�����С
#define LEN 11			//LENΪ����ĳ��ȣ�����Ϊ������X2+1
#define MOVENUM 60		//MOVENUMΪ�����ʽ�Ĵ�С��
#define BOXNUM 25		//�ܸ�����
#define BOXLEN 5		//�������鳤��

//�����������
#define FULLBOX	0		//����
#define DEADBOX 1		//����
#define CHAINBOX 2		//����
#define FREEBOX 3		//���ɸ�

//TURN�����ڱ�ʾһ���µĻغϣ�����������»غϵľ����Լ�������з�
class TURN
{
public:
	TURN();
	void SetBoard(int Array[LEN][LEN]);
	void SetCaptualInfo(int OB[LEN][LEN], int NB[LEN][LEN]);
	int Owner;
	bool Captual[LEN][LEN];//��ʹ�õ��з�
	int NewBoard[LEN][LEN];//�²����ľ���
};

//LOC�����ڱ�ʾ���꣬������Box����Edge������ʹ��
class LOC
{
public:
	int x;
	int y;
	void Set(int lx, int ly);
	
};

//Board����һ��������������
class Board{
public:
	int board[LEN][LEN];
	int Step;

	Board();//���캯��
	Board(int Array[LEN][LEN], int step);			//���캯��
	void PrintBoard();								//��ʾ����1
	void ShowBoard();								//��ʾ����2
	void ShowLiberties();							//��ʾ���и��ӵ����ɶ�
	int GetEmptyEdges();							//�õ�δ��ռ��ıߵ���Ŀ
	int GetEmptyBoxes();							//�õ�δ��ռ��ĸ��ӵ���Ŀ
	int GetEdgeOwner(int x,int y);					//�õ�ĳ���ߵ�������
	int GetBoxOwner(int x,int y);					//�õ�ĳ�����ӵ�������
	int GetBoxLiberties(int x,int y);				//�õ�ĳ�����ӵ����ɶ�
	int GetPlayerBoxes(int player);					//�õ�ĳ�����ռ��ĸ��ӵ�����
	int GetPlayerEdges(int player);					//�õ�ĳ�����ռ��ıߵ�����
	bool GetEdgeLegality(int x,int y);				//�õ�ĳ�����Ƿ�Ϸ��ĺ���
	int Winner();									//�õ�ʤ���ߣ�-1/0/1��
	void SetBoard(int Array[LEN][LEN]);				//���þ���
	void LoadBoard();								//��ȡ����

	//�����ֵ
	bool GetFreeBoxBool(int bx, int xy);			//�ж�ĳ�������Ƿ�Ϊ���ɸ�
	int GetFreeEdgeNum();							//�õ����ɱߵ�����
	bool GetCTypeBoxBool(int bx, int by);			//�жϾ������Ƿ���C�͸�
	bool GetCTypeBox(int Player,bool Msg);			//ռ��һ�������е�C�͸�
	bool GetLongCTypeBoxExist();					//�жϾ������Ƿ���ڳ�C����
	void GetAllCTypeBoxes(int Player,bool Msg);		//�Ե����е�C�͸�
	LOC FindNextBox(LOC FoundBox, LOC LastBox);		//����ĳ������

	//���ڼ���
	LOC GetDoubleCrossLoc(int Player);				//����һ��doublecross����������������ֵΪ�ñߵ�����
	bool GetCTypeBoxLimit(int Player, bool Msg);	//���ư��C�͸�

	//Move�ǵ�ǰ����Ч�ʵķ�����Ҳ���ǽ��������һ������֮ǰ��ʹ��̰������
	bool Move(int x, int y, int owner);							//ռ��һ���ߵĺ���
	int GetAllMoves(LOC Moves[MOVENUM]);						//�õ����еĿ��б�
	int GetFreeMoves(LOC Moves[MOVENUM]);						//�õ����е����ɱ�
	int GetFilterMoves(LOC Moves[MOVENUM]);						//�õ����еĹ��˿��б�(����������)
	int GetFilterMoveNum();										//�õ����еĹ��˿��бߵ�����
	bool GetLongCTypeBoxBool(int bx, int by);					//�ж�һ�������Ƿ���һ������������

	//Turn��֤��Ϊ��Ч�ʣ���ֱ�ӵĿ������һ��MoveҪ����Լ1000�����ؼ�����Turn��Ҫ�����µ�Board����һ��ʮ�ֺ�ʱ��
	void TurnMove(TURN Turn);									//һ���غ��ƶ�
	int GetAllTurns(TURN Turns[MOVENUM], int Player);			//�õ����п��еĻغ�
	int GetFilterTurn(TURN Turns[MOVENUM], int Player);			//��ý�����һ�����˵Ŀ��о���
};

//���ú���
void cprintf(char* str, WORD color, ...);
void cprintNum(int color, int Num);
bool LocEqual(LOC a, LOC b);
bool BoardEqual(int a[LEN][LEN], int b[LEN][LEN]);
void BoardCopy(int Source[LEN][LEN], int target[LEN][LEN]);
bool IfOddNumber(int num);
bool IfEvenNumber(int num);
void MoveMsg(int x, int y, int Player);