#include "pch.h"
#include "SearchEngine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CSearchEngine::CSearchEngine()
{
}

CSearchEngine::~CSearchEngine()
{
	delete m_pMG;//�ͷ��߷�������
	delete m_pEval;//�ͷŹ�ֵ����
}

BYTE CSearchEngine::MakeMove(CHESSMOVE* move)//����ĳһ�߷���������֮�������
{
	BYTE nChessID;

	nChessID = CurPosition[move->To.y][move->To.x];//ȡĿ��λ�����ӵı��
    CurPosition[move->To.y][move->To.x] = CurPosition[move->From.y][move->From.x];//�������ƶ���Ŀ��λ��
	CurPosition[move->From.y][move->From.x] = NOCHESS;//�����λ�����
	
	return nChessID;//���ر��Ե������ӣ����δ���Ӿ��ǳԿ��ӣ�
}

void CSearchEngine::UnMakeMove(CHESSMOVE* move, BYTE nChessID)//�ָ�Ϊ�߹�֮ǰ������
{
	CurPosition[move->From.y][move->From.x] = CurPosition[move->To.y][move->To.x];//��Ŀ��λ�����ӿ���ԭλ  	
	CurPosition[move->To.y][move->To.x] = nChessID;//��Ŀ��λ�ûָ�Ϊ��֮ǰ������
}

void CSearchEngine::RedoChessMove(BYTE position[][9],CHESSMOVE* move)//��ԭ
{
    position[move->To.y][move->To.x] = position[move->From.y][move->From.x];//�������ƶ���Ŀ��λ��
	position[move->From.y][move->From.x] = NOCHESS;//�����λ�����
}

void CSearchEngine::UndoChessMove(BYTE position[][9],CHESSMOVE* move, BYTE nChessID)//����
{
	position[move->From.y][move->From.x]=position[move->To.y][move->To.x];//��Ŀ��λ�����ӿ���ԭλ  	
	position[move->To.y][move->To.x]=nChessID;//��Ŀ��λ�ûָ�Ϊ��֮ǰ������
}

int CSearchEngine::IsGameOver(BYTE position[][9], int nDepth)//�ж���Ϸ�Ƿ����
{
	int i,j;
	BOOL RedLive=FALSE,BlackLive=FALSE;

	for(i=7;i<10;i++)//���췽�Ź��Ƿ���˧
		for(j=3;j<6;j++)
		{
			if(position[i][j]==B_KING) BlackLive=TRUE;
			if(position[i][j]==R_KING) RedLive=TRUE;
		}

	for(i=0;i<3;i++)//���ڷ��Ź��Ƿ��н�
		for(j=3;j<6;j++)
		{
			if(position[i][j]==B_KING) BlackLive=TRUE;
			if(position[i][j]==R_KING) RedLive=TRUE;
		}

	i=(m_nMaxDepth-nDepth+1)%2;//ȡ��ǰ��ż��־,������Ϊ���Է�,ż����Ϊ�û���
	
	if(!RedLive)//�췽����
		if(i)
			return 19990+nDepth; //�����㷵�ؼ���ֵ
		else
			return -19990-nDepth;//ż���㷵�ؼ�Сֵ

	if(!BlackLive)//�ڷ�����
		if(i)
			return -19990-nDepth;//�����㷵�ؼ�Сֵ
		else
			return 19990+nDepth; //ż���㷵�ؼ���ֵ
		
	return 0;//��˧���ڣ�����0
}
