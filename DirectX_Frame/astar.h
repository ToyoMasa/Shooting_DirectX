#ifndef _A_STAR_H_
#define _A_STAR_H_

#include <d3dx9.h>
#include <vector>
#include <memory>

class CAStarNode
{
public:
	CAStarNode(int posX, int posY)
		: m_ParentNode(nullptr)
		, _posX(posX)
		, _posY(posY)
		, m_Step(0)
		, m_Distance(0)
	{}

	// 親のノードポインタの操作
	CAStarNode& SetParentNode(CAStarNode* pNode) { m_ParentNode = pNode; return *this; }
	CAStarNode* GetParentNode(void) const { return m_ParentNode; }

	CAStarNode& SetPosX(int posX) { _posX = posX; return *this; }
	int GetPosX(void) const { return _posX; }

	CAStarNode& SetPosY(int posY) { _posY = posY; return *this; }
	int GetPosY(void) const { return _posY; }

	CAStarNode& SetCostFromStartNode(int costFromStartNode) { m_Step = costFromStartNode; return *this; }
	int GetCostFromStartNode(void) const { return m_Step; }

	CAStarNode& SetCostToGoalNode(int costToGoalNode) { m_Distance = costToGoalNode; return *this; }
	int GetCostToGoalNode(void) const { return m_Distance; }

	int GetTotalCost(void) const { return m_Step + m_Distance; }

	bool operator == (CAStarNode node)
	{
		return (this->_posX == node._posX && this->_posY == node._posY);
	}

	void operator = (CAStarNode node)
	{
		this->m_ParentNode = node.m_ParentNode;
		this->_posX = node._posX;
		this->_posY = node._posY;
		this->m_Step = node.m_Step;
		this->m_Distance = node.m_Distance;
	}

private:
	CAStarNode* m_ParentNode;       // 親ノード
	int   _posX;              // X座標
	int   _posY;              // Y座標
	int   m_Step; // スタートノードからの最小コスト
	int   m_Distance;    // ゴールノードまでの最小コスト
};

typedef std::shared_ptr<CAStarNode> NodePtr;
typedef std::vector<NodePtr> NodePtrVector;

class CAStar
{
public:
	CAStar() {}
	~CAStar() {}

	static void AddMapData(const D3DXVECTOR3& pos);
	static void SetMapSize(int x, int y);
	static void Uninit();
	static bool IsWall(int posX, int posY);
	static int GetCostToGoal(const int& startX, const int& startY, const int& goalX, const int& goalY);
	static int GetCostToGoal(const CAStarNode& start, const CAStarNode& goal);
	static std::vector<D3DXVECTOR3> GetShortestWay(D3DXVECTOR3 start, D3DXVECTOR3 goal);

private:
	static int m_MapWidth;
	static int m_MapHeight;
	static std::vector<int> m_MapData;
};

#endif // !_A_STAR_H_
