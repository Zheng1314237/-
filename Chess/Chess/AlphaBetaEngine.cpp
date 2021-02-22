#include "pch.h"
#include "AlphaBetaEngine.h"

using namespace std;

CAlphaBetaEngine::CAlphaBetaEngine()
{
}

CAlphaBetaEngine::~CAlphaBetaEngine()
{
}

int CAlphaBetaEngine::SearchAGoodMove(BYTE position[][9])
{
	memcpy(CurPosition,position,90);    //将当前局面复制到CurPosition

	m_nMaxDepth=m_nSearchDepth;			//设定搜索深度
	AlphaBeta(m_nMaxDepth,-20000,20000);//进行alphabeta搜索
	m_umUndoMove.cmChessMove=m_cmBestMove;
	m_umUndoMove.nChessID=MakeMove(&m_cmBestMove);	

	memcpy(position,CurPosition,90);	//将走过的棋盘传出

	return 0;
}

int CAlphaBetaEngine::AlphaBeta(int nDepth,int alpha,int beta)
{
	int score;
	int Count,i;
	BYTE type;

	i = IsGameOver(CurPosition,nDepth);//检查是否游戏结束
	if(i!=0) return i;//结束，返回估值

	if(nDepth <= 0)//已搜索到叶子节点，nDepth开始=m_nMaxDepth，每向子节点搜索一次-1搜索到叶子节点为0
		return m_pEval->Eveluate(CurPosition,(m_nMaxDepth-nDepth)%2,m_nUserChessColor);//返回估值

	//此函数找出当前局面所有可能的走法，然后放进m_pMG ->m_MoveList当中
	Count = m_pMG->CreatePossibleMove(CurPosition,nDepth,(m_nMaxDepth-nDepth)%2,m_nUserChessColor);

	if (nDepth == m_nMaxDepth)//在根节点设定进度条
	{
		//m_Progress->SetRange(0, Count);
		//m_Progress->SetStep(1);
		//m_Progress->SetPos(0);
	}

	//对所有可能的走法
	for (i = 0; i < Count; i++)
	{
		if (nDepth == m_nMaxDepth)//走进度条
		{
			//m_Progress->StepIt();//走进度条
		}

		type = MakeMove(&m_pMG->m_MoveList[nDepth][i]);//将当前局面应用此走法，变为子节点的局面
		score = -AlphaBeta(nDepth-1,-beta,-alpha);//递归搜索子节点
		UnMakeMove(&m_pMG->m_MoveList[nDepth][i],type);//将此节点的局面恢复为当前节点

		if(score > alpha)
		{
			alpha = score;//保留极大值

			//靠近根节点时保留最佳走法
			if(nDepth == m_nMaxDepth)
				m_cmBestMove = m_pMG->m_MoveList[nDepth][i];
		}
	
		if(alpha >= beta) break;//剪枝，放弃搜索剩下的节点
	}

	return alpha;//返回极大值
}
