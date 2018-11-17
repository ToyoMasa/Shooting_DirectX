//======================================================================
//	ウェイポイント　（2018/11/17)
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "scene.h"
#include "waypoint.h"

std::vector<CWayPoint*> CWayPoint::m_WayPonits;

void CWayPoint::Create(D3DXVECTOR3 pos)
{
	CWayPoint* waypoint = new CWayPoint(pos);
	m_WayPonits.push_back(waypoint);
}

void CWayPoint::Release()
{
	for (int i = 0; i != m_WayPonits.size(); ++i)
	{
		if (m_WayPonits[i] != NULL)
		{
			m_WayPonits[i]->m_NearPoints.clear();
			delete m_WayPonits[i];
		}
	}

	m_WayPonits.clear();
	
}

void CWayPoint::SetRecentPoint(CWayPoint* point)
{
	m_NearPoints.push_back(point);
}
