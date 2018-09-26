#include "common.h"
#include "root.h"

D3DXVECTOR3 CRoot::m_RootPos[100];
int CRoot::m_NextRoot[100];

void CRoot::Set()
{
	m_RootPos[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_RootPos[1] = D3DXVECTOR3(0.0f, 0.0f, 10.0f);
	m_RootPos[2] = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
	m_RootPos[3] = D3DXVECTOR3(0.0f, 0.0f, -10.0f);
	m_RootPos[4] = D3DXVECTOR3(10.0f, 0.0f, 0.0f);

	m_NextRoot[0] = 1;
	m_NextRoot[1] = 2;
	m_NextRoot[2] = 3;
	m_NextRoot[3] = 4;
	m_NextRoot[4] = 1;

	m_RootPos[5] = D3DXVECTOR3(-20.0f, 0.0f, -3.5f);
	m_RootPos[6] = D3DXVECTOR3(-20.0f, 0.0f, 8.5f);
	m_RootPos[7] = D3DXVECTOR3(-9.0f, 0.0f, 8.5f);
	m_RootPos[8] = D3DXVECTOR3(-20.0f, 0.0f, 8.5f);

	m_NextRoot[5] = 6;
	m_NextRoot[6] = 7;
	m_NextRoot[7] = 8;
	m_NextRoot[8] = 5;

	m_RootPos[9]  = D3DXVECTOR3(-11.0f, 0.0f, -16.0f);
	m_RootPos[10] = D3DXVECTOR3(-22.0f, 0.0f, -16.0f);
	m_RootPos[11] = D3DXVECTOR3(-22.0f, 0.0f, -22.0f);
	m_RootPos[12] = D3DXVECTOR3(-11.0f, 0.0f, -22.0f);

	m_NextRoot[9]  = 10;
	m_NextRoot[10] = 11;
	m_NextRoot[11] = 12;
	m_NextRoot[12] = 9;

	m_RootPos[13] = D3DXVECTOR3(20.0f, 0.0f, 20.0f);
	m_RootPos[14] = D3DXVECTOR3(10.0f, 0.0f, 20.0f);
	m_RootPos[15] = D3DXVECTOR3(15.0f, 0.0f, 20.0f);
	m_RootPos[16] = D3DXVECTOR3(15.0f, 0.0f, 7.0f);
	m_RootPos[17] = D3DXVECTOR3(10.0f, 0.0f, 7.0f);
	m_RootPos[18] = D3DXVECTOR3(20.0f, 0.0f, 7.0f);
	m_RootPos[19] = D3DXVECTOR3(15.0f, 0.0f, 7.0f);
	m_RootPos[20] = D3DXVECTOR3(15.0f, 0.0f, 20.0f);

	m_NextRoot[13] = 14;
	m_NextRoot[14] = 15;
	m_NextRoot[15] = 16;
	m_NextRoot[16] = 17;
	m_NextRoot[17] = 18;
	m_NextRoot[18] = 19;
	m_NextRoot[19] = 20;
	m_NextRoot[20] = 13;

}
