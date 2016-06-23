#include "stdafx.h"
#include "windows.h"

#pragma  once


enum ChainType
{
	NotDefine, SingleChain, LongChain, ShortChain, Circle, PreCircle, DeadChain, DeadCircle
};

//BoxType����һ�����ڼ������ڸ������͵���
class BoxType
{
public:
	BoxType();
	LOC BoxLoc;
	int BoxOwner;			//���ӵ��������󲿷ָ���û������
	int Type;				//[0]�ѱ�ռ�� [1]���� [2]���� [3]���ɸ�
	int BelongingChainNum;		//���������͵ı�ţ����1��ʼ�Զ���š�
};

//ChainInfo��һ��������һ�����Ļ�����Ϣ����
class ChainInfo
{
public:
	ChainInfo();
	ChainType Type;
	int ChainBoxNum;
	LOC StartLoc;
	LOC EndLoc;
	bool ConditionOfPreCircle;
};

//BoxBoard����һ�����׵���
class BoxBoard :public Board
{
public:
	BoxBoard(Board NewB);						//���캯��
	BoxBoard(int Array[LEN][LEN], int step);	//���캯��
	BoxType Boxes[BOXLEN + 1][BOXLEN + 1];			//�������̵Ļ������ͣ��ɸ������,��Ŵ�1��ʼ��
	ChainInfo Chains[BOXNUM];					//���������ض����ᳬ��ȫ��������
	int GetFirstEmptyChainNum();				//���һ���յ������
	void DefineBoxesType();						//�����и��ӵĻ�����Ϣ���롣
	int GetBoxType(int bx, int by);				//�õ�ĳ�����ӵ����ͣ��������귶Χ�ڵ�ȫ���������ɸ�����[��������Ϊ1��BOXLEN]��
	void ShowBoxType();							//��ʾ���и��ӵ�����

	//������
	void ShowBelongingChain();									//��ʾ���и����������������������Ϣ
	void InheritChain(int InheritorRegNum, int AncesterRegNum);	//һ�����̲���һ����
	void RegisterChain(LOC FreeBoxLoc, LOC NextLoc);			//��һ�����ڵ����ɸ�����������е���������һ�����塣
	void RegisterCircle(LOC StartLoc, LOC NextLoc);				//��һ���������ȷ���Ƿ�Ϊһ������
	void SearchingFromBox(LOC BoxLoc);							//��һ���������ע����������������,ChianPlusӦ����û�ж���ʱ���á�
	void SearchingCircle(LOC BoxLoc);							//��һ���������ע������������������
	void ResetChainsInfo();										//�ض��������ĵ���Ϣ
	void DefineAllChains(bool ChainPlus);						//�������е���

	//˵�������Ƕ�̬ע��ġ���һ���������ൽ��һ����ʱ��������ᱻ���¶���Ϊ"NotDefine"����ÿ����Ҫע������ʱ����0��ʼ���ҵ�һ��������

	//�����ж�
	void RegisterDeadChain(LOC FreeBoxLoc, LOC NextLoc);		//��һ�����ڵ����������������������һ�����塣
	void SearchingDeadChain(LOC BoxLoc);						//��һ���������ע������������������
	void DefineDeadChain();										//������е�����
	bool GetDeadChainExist();
	bool GetDeadCircleExist();

	//ռ����
	bool CaptualAnySingleChain(int LatterPlayer);				//����̲�һ������������Ϊռ���ߡ�
	bool CaptualAnyShortChain(int LatterPlayer);				//����̲�һ������������Ϊռ���ߡ�
	bool CaptualShortestChain(int LatterPlayer);				//�̲�һ����ǰ��̵ĳ������߻�

	//����
	LOC GetOpenShortestChainLoc();
	LOC GetOpenSuitableChainLoc();

	//�����ж�����
	bool RationalState(LOC BoxNum);								//�ж�˫���Ƿ�Ϊ����״̬
	LOC GetRationalStateBoxNum();								//��õ�ǰ������˫��������״̬���Ի�õĸ���������xΪ���֣�yΪ����
	int GetBoardWinner(int LatterPlayer);						//��ñ������µ�ʤ���ߣ�����Ϊ���ַ���
};