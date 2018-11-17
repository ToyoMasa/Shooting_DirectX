#ifndef _A_STAR_H_
#define _A_STAR_H_

#include <d3dx9.h>
#include <vector>

//class CAStarGrid
//{
//public:
//	CAStarGrid() {}
//	~CAStarGrid() {}
//
//	enum STATUS
//	{
//		OPEN,
//		CLOSE,
//	};
//
//	struct Coord
//	{
//		D3DXVECTOR3 pos;
//		static D3DXVECTOR3 zero = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		static D3DXVECTOR3 one = new Coord(){ x = 1, y = 1 };
//		static D3DXVECTOR3 left = new Coord(){ x = -1, y = 0 };
//		static D3DXVECTOR3 up = new Coord(){ x = 0, y = 1 };
//		static D3DXVECTOR3 right = new Coord(){ x = 1, y = 0 };
//		static D3DXVECTOR3 down = new Coord(){ x = 0, y = -1 };
//	};
//private:
//
//};

struct AStarNode
{
	enum STATUS
	{
		NONE,
		OPEN,
		CLOSED,
	};
	STATUS status;			// ステータス
	int step;				// 移動コスト
	int distance;			// ヒューリスティックコスト
	AStarNode* parent;		// 親A*のポインタ
	D3DXVECTOR3 pos;		// 座標
	
	// スコア取得
	int GetCost()
	{
		return step + distance;
	}
};

class CAStar
{
public:
	CAStar() {}
	~CAStar() {}
	
	static void AddNodesList(VERTEX_3D* vtx);
	static std::vector<AStarNode> GetNodesList() { return m_NodesList;	}
	static void Uninit() { m_NodesList.clear(); }
	static AStarNode GetShortestNode(D3DXVECTOR3 const& pos);
	std::vector<D3DXVECTOR3> GetShortestWay(VERTEX_3D const& startVtx, VERTEX_3D const& endVtx);
	AStarNode GetNodeInfo(D3DXVECTOR3 const& target, D3DXVECTOR3 const& end, AStarNode* parent);
private:
	static std::vector<AStarNode> m_NodesList;

};

#endif // !_A_STAR_H_
