#include "common.h"
#include "targetPoint.h"

CTargetPoint *CTargetPoint::m_TargetPoint[TARGETPOINT_MAX] = { NULL };

CTargetPoint* CTargetPoint::Create(D3DXVECTOR3 pos)
{
	CTargetPoint* targetPoint = new CTargetPoint();
	targetPoint->Init(pos);

	return targetPoint;
}
