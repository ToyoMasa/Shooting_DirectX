#include "common.h"
#include "main.h"
#include "scene3D.h"
#include "astar.h"
#include <algorithm>

std::vector<AStarNode> CAStar::m_NodesList;

void CAStar::AddNodesList(VERTEX_3D* vtx)
{
	AStarNode node;
	node.distance = 99999;
	node.step = 99999;
	node.parent = NULL;
	node.status = AStarNode::NONE;
	node.pos = vtx->pos;

	m_NodesList.push_back(node);
}

AStarNode CAStar::GetShortestNode(D3DXVECTOR3 const& pos)
{
	for (auto itr = m_NodesList.begin(); itr != m_NodesList.end(); ++itr)
	{
		if (D3DXVECTOR2((int)itr->pos.x, (int)itr->pos.z) == D3DXVECTOR2((int)pos.x, (int)pos.z))
		{
			return *itr;
		}
	}

	return *m_NodesList.begin();
}

std::vector<D3DXVECTOR3> CAStar::GetShortestWay(VERTEX_3D const& startVtx, VERTEX_3D const& endVtx)
{
	std::vector<D3DXVECTOR3> retList;

	while (1)
	{
		break;
	}

	return retList;
}

AStarNode CAStar::GetNodeInfo(D3DXVECTOR3 const& target, D3DXVECTOR3 const& end, AStarNode* parent)
{
	AStarNode ret;
	ret.pos = target;
	ret.parent = parent;
	if (parent != NULL)
	{
		ret.step = parent->step + 1;
	}

	return ret;
}
