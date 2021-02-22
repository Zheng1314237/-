#if !defined(AFX_SEARCHENGINE_H__7A7237B9_0908_45D8_B102_94E342B174A5__INCLUDED_)
#define AFX_SEARCHENGINE_H__7A7237B9_0908_45D8_B102_94E342B174A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Eveluation.h"
#include "MoveGenerator.h"

class CSearchEngine //��������Ļ���
{
public:
	CSearchEngine(); //���캯��
	virtual ~CSearchEngine(); //��������

public:
	int virtual SearchAGoodMove(BYTE position[10][9])=0;    //����һ�����麯��������������������д
	CHESSMOVE GetBestMove(){return m_cmBestMove;};			//�õ�����߷�
	UNDOMOVE GetUndoMove(){return m_umUndoMove;};			//�õ������߷�
	void SetSearchDepth(int nDepth){m_nSearchDepth = nDepth;};//�趨����������
	void SetEveluator(CEveluation* pEval){m_pEval = pEval;};  //�趨��ֵ����
	void SetMoveGenerator(CMoveGenerator* pMG){m_pMG = pMG;};//�趨�߷�������
	void SetUserChessColor(int nUserChessColor){m_nUserChessColor = nUserChessColor;};//�趨�û�Ϊ�ڷ���췽
	//void SetProgress(CProgressCtrl* Progress){ m_Progress = Progress; }//�趨����������

	//���������������ֻʵ��������5������*************************************************
	void UndoChessMove(BYTE position[10][9],CHESSMOVE* move,BYTE nChessID);//����
	void RedoChessMove(BYTE position[10][9],CHESSMOVE* move);//��ԭ

protected:
	int IsGameOver(BYTE position[10][9],int nDepth);//�ж��Ƿ��ѷ�ʤ��
	BYTE MakeMove(CHESSMOVE* move);//����ĳһ�߷���������֮�������
	void UnMakeMove(CHESSMOVE* move,BYTE nChessID);//�ָ�Ϊ�߹�֮ǰ������
	//���������������ֻʵ��������5������*************************************************

public:
	int m_nUserChessColor;//�û�������ɫ
//	CProgressCtrl* m_Progress;//����������

protected:
	BYTE CurPosition[10][9];		//����ʱ���ڼ�¼��ǰ�ڵ�����״̬������,�м����
	CHESSMOVE m_cmBestMove;			//��¼����߷�
	UNDOMOVE m_umUndoMove;          //��¼�����߷�
	CMoveGenerator* m_pMG;			//�߷�������
	CEveluation* m_pEval;			//��ֵ����
	int m_nSearchDepth;				//����������
	int m_nMaxDepth;				//��ǰ����������������
};

#endif // !defined(AFX_SEARCHENGINE_H__7A7237B9_0908_45D8_B102_94E342B174A5__INCLUDED_)
