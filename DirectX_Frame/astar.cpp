#include "common.h"
#include "main.h"
#include "scene3D.h"
#include "astar.h"
#include <functional>
#include <iostream>
#include <vector>
#include <queue>
#include <memory>

int CAStar::m_MapWidth = 0;
int CAStar::m_MapHeight = 0;
std::unordered_map<MapCoord, int> CAStar::m_MapData;

void CAStar::AddMapData(const D3DXVECTOR3& pos)
{
	MapCoord coord((int)pos.x, (int)pos.y);

	if (pos.y > 0.0f)
	{
		m_MapData[coord] = MAP_WALL;
	}
	else
	{
		m_MapData[coord] = MAP_ROAD;
	}
}

void CAStar::SetMapSize(int x, int y)
{
	m_MapWidth = x;
	m_MapHeight = y;
}

void CAStar::Uninit()
{
	m_MapData.clear();
}

//---------------------------------------------------------------------------
//  マップ外に出るかどうか
//---------------------------------------------------------------------------
bool CAStar::IsWall(int posX, int posY)
{
	if ((float)posX < -(m_MapWidth / 2.0f) || m_MapWidth / 2.0f <= (float)posX) return true;
	if ((float)posY < -(m_MapHeight / 2.0f) || m_MapHeight / 2.0f <= (float)posY) return true;
	MapCoord coord = { posX, posY };

	int i = m_MapData[coord];
	return (m_MapData[coord] == MAP_WALL);
}

//---------------------------------------------------------------------------
//  ゴールまでの推定コストを計算
//---------------------------------------------------------------------------
int CAStar::GetCostToGoal(const int& startX, const int& startY, const int& goalX, const int& goalY)
{
	return std::abs(goalX - startX) + std::abs(goalY - startY);
}

//---------------------------------------------------------------------------
//  ゴールまでの推定コストを計算
//---------------------------------------------------------------------------
int CAStar::GetCostToGoal(const CAStarNode& start, const CAStarNode& goal)
{
	return GetCostToGoal(start.GetPosX(), start.GetPosY(), goal.GetPosX(), goal.GetPosY());
}

bool CAStar::GetShortestWay(D3DXVECTOR3 start, D3DXVECTOR3 goal, std::vector<D3DXVECTOR3>& list)
{
	int startX = (int)start.x;
	int startY = (int)start.z;
	int goalX = (int)goal.x;
	int goalY = (int)goal.z;

	NodePtrVector openList;
	NodePtrVector closeList;

	//------------------------------------------------------------
	// ラムダ式を用いた関数定義
	//------------------------------------------------------------

	// Node位置比較用関数
	auto compareNodeByTotalCost = [](NodePtr pNode1, NodePtr pNode2) -> int
	{
		return pNode1->GetTotalCost() > pNode2->GetTotalCost();
	};

	// リスト内に含まれているかどうかの判定用関数
	auto isInNodeList = [](NodePtrVector& list, const NodePtr& node) -> NodePtr
	{
		for (NodePtrVector::iterator it = list.begin(); it != list.end(); ++it)
		{
			NodePtr nodeItem = (*it);
			if (*node == *nodeItem)
			{
				return nodeItem;
			}
		}
		return nullptr;
	};

	// スタートノード
	NodePtr startNode(new CAStarNode(startX, startY));
	// ゴールノード
	NodePtr goalNode(new CAStarNode(goalX, goalY));

	int startNodeCostToGoalNode = GetCostToGoal(*startNode, *goalNode);
	startNode->SetCostToGoalNode(startNodeCostToGoalNode);

	// オープンリストから取り出す
	openList.push_back(startNode);

	int i = 0;
	while (true)
	{
		if (i++ == 30)
		{
			return false;
		}

		// オープンリストが空なら検索失敗
		if (openList.empty())
		{
			//MessageBox(NULL, "探索に失敗", "エラー", MB_OK);
			break;
		}

		// 最小コストのノードをオープンリストから取り出す
		std::sort(openList.begin(), openList.end(), compareNodeByTotalCost);
		NodePtr pBaseNode = openList.back();
		openList.pop_back();

		// ゴールノードと一致したら検索終了
		if (*pBaseNode == *goalNode)
		{
			*goalNode = *pBaseNode;
			break;
		}

		// 取り出したノードをクローズリストに移す
		closeList.push_back(pBaseNode);

		// 隣接ノードをチェック
		// 今回は上下左右のみ
		for (int dy = -1; dy <= 1; ++dy)
		{
			for (int dx = -1; dx <= 1; ++dx)
			{
				// 同位置判定
				if (dx == 0 && dy == 0) continue;

				// 斜めを考慮しない
				//if (dx != 0 && dy != 0) continue;

				// 隣接ノード位置
				int pAdjacentNodePosX = pBaseNode->GetPosX() + dx;
				int pAdjacentNodePosY = pBaseNode->GetPosY() + dy;

				// 壁判定
				if (IsWall(pAdjacentNodePosX, pAdjacentNodePosY)) continue;

				// 隣接ノードの各コスト
				int adjacentNodeCostFromStart = pBaseNode->GetCostFromStartNode() + 1;  // 親から子への移動コストは1
				int adjacentNodeCostToGoalNode = GetCostToGoal(pAdjacentNodePosX, pAdjacentNodePosY, goalX, goalY);

				// 隣接ノード
				NodePtr pAdjacentNode(new CAStarNode(pAdjacentNodePosX, pAdjacentNodePosY));
				pAdjacentNode->SetParentNode(pBaseNode.get())
					.SetCostFromStartNode(adjacentNodeCostFromStart)
					.SetCostToGoalNode(adjacentNodeCostToGoalNode);

				NodePtr pSearchedNode = nullptr;

				// オープンリストに含まれているか
				pSearchedNode = isInNodeList(openList, pAdjacentNode);
				if (pSearchedNode)
				{
					// オープンリストにあったノードより隣接ノードのコストが小さければ、オープンリストのノードを上書き
					if (pAdjacentNode->GetTotalCost() < pSearchedNode->GetTotalCost())
					{
						*pSearchedNode = *pAdjacentNode;
					}
					continue;
				}

				// クローズリストに含まれているか
				pSearchedNode = isInNodeList(closeList, pAdjacentNode);
				if (pSearchedNode)
				{
					// クローズリストにあったノードより隣接ノードのコストが小さければ、クローズリストから削除してオープンリストに追加
					if (pAdjacentNode->GetTotalCost() < pSearchedNode->GetTotalCost())
					{
						std::remove(closeList.begin(), closeList.end(), pSearchedNode);
						openList.push_back(pAdjacentNode);
					}
					continue;
				}

				// どちらにも含まれていなければオープンリストに追加
				openList.push_back(pAdjacentNode);
			}
		}
	}

	//------------------------------------------------------------
	// 結果
	//------------------------------------------------------------
	std::vector<D3DXVECTOR3> retList;
	// ゴールノードから親ノードを辿ることで、スタートノードまでの経路が算出される
	CAStarNode* pNode = goalNode.get();
	while (true)
	{
		retList.push_back(D3DXVECTOR3((float)pNode->GetPosX(), 0.0f, (float)pNode->GetPosY()));

		if ((pNode = pNode->GetParentNode()) == nullptr)
		{
			break;
		}
	}

	list = retList;

	return true;
}
