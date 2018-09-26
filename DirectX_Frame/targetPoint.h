//======================================================================
//	’Ê‰ß’n“_[targetPoint.h]
//
//======================================================================
#ifndef _TARGETPOINT_H_
#define _TARGETPOINT_H_

static const int TARGETPOINT_MAX = 100;

class CTargetPoint
{
public:
	CTargetPoint() 
	{
		for (int i = 0; i < TARGETPOINT_MAX; i++)
		{
			if (m_TargetPoint[i] == NULL)
			{
				m_TargetPoint[i] = this;
				break;
			}
		}
	}
	~CTargetPoint() {}

	void Init(D3DXVECTOR3 pos) { m_Pos = pos; }

	static CTargetPoint* Create(D3DXVECTOR3 pos);

private:
	D3DXVECTOR3 m_Pos;
	static CTargetPoint* m_TargetPoint[TARGETPOINT_MAX];
};

#endif // !_TARGETPOINT_H_
