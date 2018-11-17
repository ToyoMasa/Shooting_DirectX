//======================================================================
//	ウェイポイント[waypoint.h]
//
//======================================================================
#ifndef _WAYPOINT_H_
#define _WAYPOINT_H_

#include <vector>
#include <d3dx9.h>

class CWayPoint
{
public:
	CWayPoint(D3DXVECTOR3 pos) { m_Pos = pos; }
	~CWayPoint() {}

	static void Create(D3DXVECTOR3 pos);
	static void Release();
	static std::vector<CWayPoint*>& GetWayPoints() { return m_WayPonits; }
	void SetRecentPoint(CWayPoint* point);

private:
	D3DXVECTOR3						m_Pos;
	std::vector<CWayPoint*>			m_NearPoints;
	static std::vector<CWayPoint*>	m_WayPonits;
};

#endif //! _WAYPOINT_H_
