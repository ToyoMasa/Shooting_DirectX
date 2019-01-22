//======================================================================
//	ウェイポイント[waypoint.h]
//
//======================================================================
#ifndef _WAYPOINT_H_
#define _WAYPOINT_H_

#include <vector>
#include <string>
#include <d3dx9.h>

static const int INIT_COST = 50000;

class CBillBoard;

class CWayPoint
{
public:
	CWayPoint(D3DXVECTOR3 pos) { m_Pos = pos; }
	~CWayPoint() {}

	static void Init();
	static void Uninit();
	static void Add(D3DXVECTOR3 pos);
	static void AddNearPoint(int id, int addid);
	static void CreateGraph();
	static std::vector<CWayPoint*>& GetWayPoints() { return m_WayPonits; }
	static int GetNextPoint(const int s, const int e);
	static D3DXVECTOR3 GetWayPointPos(const int& index);
	static int SearchShortestPoint(const D3DXVECTOR3& pos);
	static void Debug();
	static void Debug(bool debug);
	static void Save(std::string textname);
	static void Load(std::string textname);
	static int GetIDonScreen(D3DXVECTOR3 mouse);
	static void DebugColorReset();
	static void SetSelectDebugColor(int id);
	static void ChangeDebugColor(int id, D3DCOLOR color);
	void SetRecentPoint(int id);

private:
	static std::vector<CWayPoint*>	m_WayPonits;
	static std::vector<CBillBoard*>	m_PointsDebug;
	static int**					m_EdgeCost;
	static int**					m_ShortestPath;
	static int						m_EdgeCostSize;
	static int						m_ShortestPathSize;
	int								m_ID;
	D3DXVECTOR3						m_Pos;
	std::vector<int>				m_NearPoints;
};

#endif //! _WAYPOINT_H_
