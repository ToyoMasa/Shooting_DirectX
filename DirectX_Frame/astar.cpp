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
//  �}�b�v�O�ɏo�邩�ǂ���
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
//  �S�[���܂ł̐���R�X�g���v�Z
//---------------------------------------------------------------------------
int CAStar::GetCostToGoal(const int& startX, const int& startY, const int& goalX, const int& goalY)
{
	return std::abs(goalX - startX) + std::abs(goalY - startY);
}

//---------------------------------------------------------------------------
//  �S�[���܂ł̐���R�X�g���v�Z
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
	// �����_����p�����֐���`
	//------------------------------------------------------------

	// Node�ʒu��r�p�֐�
	auto compareNodeByTotalCost = [](NodePtr pNode1, NodePtr pNode2) -> int
	{
		return pNode1->GetTotalCost() > pNode2->GetTotalCost();
	};

	// ���X�g���Ɋ܂܂�Ă��邩�ǂ����̔���p�֐�
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

	// �X�^�[�g�m�[�h
	NodePtr startNode(new CAStarNode(startX, startY));
	// �S�[���m�[�h
	NodePtr goalNode(new CAStarNode(goalX, goalY));

	int startNodeCostToGoalNode = GetCostToGoal(*startNode, *goalNode);
	startNode->SetCostToGoalNode(startNodeCostToGoalNode);

	// �I�[�v�����X�g������o��
	openList.push_back(startNode);

	int i = 0;
	while (true)
	{
		if (i++ == 30)
		{
			return false;
		}

		// �I�[�v�����X�g����Ȃ猟�����s
		if (openList.empty())
		{
			//MessageBox(NULL, "�T���Ɏ��s", "�G���[", MB_OK);
			break;
		}

		// �ŏ��R�X�g�̃m�[�h���I�[�v�����X�g������o��
		std::sort(openList.begin(), openList.end(), compareNodeByTotalCost);
		NodePtr pBaseNode = openList.back();
		openList.pop_back();

		// �S�[���m�[�h�ƈ�v�����猟���I��
		if (*pBaseNode == *goalNode)
		{
			*goalNode = *pBaseNode;
			break;
		}

		// ���o�����m�[�h���N���[�Y���X�g�Ɉڂ�
		closeList.push_back(pBaseNode);

		// �אڃm�[�h���`�F�b�N
		// ����͏㉺���E�̂�
		for (int dy = -1; dy <= 1; ++dy)
		{
			for (int dx = -1; dx <= 1; ++dx)
			{
				// ���ʒu����
				if (dx == 0 && dy == 0) continue;

				// �΂߂��l�����Ȃ�
				//if (dx != 0 && dy != 0) continue;

				// �אڃm�[�h�ʒu
				int pAdjacentNodePosX = pBaseNode->GetPosX() + dx;
				int pAdjacentNodePosY = pBaseNode->GetPosY() + dy;

				// �ǔ���
				if (IsWall(pAdjacentNodePosX, pAdjacentNodePosY)) continue;

				// �אڃm�[�h�̊e�R�X�g
				int adjacentNodeCostFromStart = pBaseNode->GetCostFromStartNode() + 1;  // �e����q�ւ̈ړ��R�X�g��1
				int adjacentNodeCostToGoalNode = GetCostToGoal(pAdjacentNodePosX, pAdjacentNodePosY, goalX, goalY);

				// �אڃm�[�h
				NodePtr pAdjacentNode(new CAStarNode(pAdjacentNodePosX, pAdjacentNodePosY));
				pAdjacentNode->SetParentNode(pBaseNode.get())
					.SetCostFromStartNode(adjacentNodeCostFromStart)
					.SetCostToGoalNode(adjacentNodeCostToGoalNode);

				NodePtr pSearchedNode = nullptr;

				// �I�[�v�����X�g�Ɋ܂܂�Ă��邩
				pSearchedNode = isInNodeList(openList, pAdjacentNode);
				if (pSearchedNode)
				{
					// �I�[�v�����X�g�ɂ������m�[�h���אڃm�[�h�̃R�X�g����������΁A�I�[�v�����X�g�̃m�[�h���㏑��
					if (pAdjacentNode->GetTotalCost() < pSearchedNode->GetTotalCost())
					{
						*pSearchedNode = *pAdjacentNode;
					}
					continue;
				}

				// �N���[�Y���X�g�Ɋ܂܂�Ă��邩
				pSearchedNode = isInNodeList(closeList, pAdjacentNode);
				if (pSearchedNode)
				{
					// �N���[�Y���X�g�ɂ������m�[�h���אڃm�[�h�̃R�X�g����������΁A�N���[�Y���X�g����폜���ăI�[�v�����X�g�ɒǉ�
					if (pAdjacentNode->GetTotalCost() < pSearchedNode->GetTotalCost())
					{
						std::remove(closeList.begin(), closeList.end(), pSearchedNode);
						openList.push_back(pAdjacentNode);
					}
					continue;
				}

				// �ǂ���ɂ��܂܂�Ă��Ȃ���΃I�[�v�����X�g�ɒǉ�
				openList.push_back(pAdjacentNode);
			}
		}
	}

	//------------------------------------------------------------
	// ����
	//------------------------------------------------------------
	std::vector<D3DXVECTOR3> retList;
	// �S�[���m�[�h����e�m�[�h��H�邱�ƂŁA�X�^�[�g�m�[�h�܂ł̌o�H���Z�o�����
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
