//======================================================================
//	��[wall.h]
//
//======================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "scene.h"
#include "box.h"

class CWall: public CScene
{
public:
	CWall::CWall(int prioriy) :CScene(prioriy)
	{
		m_Right = D3DXVECTOR3(1, 0, 0);
		m_Up = D3DXVECTOR3(0, 1, 0);
		m_Front = D3DXVECTOR3(0, 0, 1);

		m_Normal[0] = D3DXVECTOR3(0, 1, 0);
		m_Normal[1] = D3DXVECTOR3(0, -1, 0);
		m_Normal[2] = D3DXVECTOR3(1, 0, 0);
		m_Normal[3] = D3DXVECTOR3(-1, 0, 0);
		m_Normal[4] = D3DXVECTOR3(0, 0, 1);
		m_Normal[5] = D3DXVECTOR3(0, 0, -1);
	}
	~CWall() {}

	void Init(D3DXVECTOR3 pos, float x, float y, float z, int texID);
	void Uninit();
	void Update();
	void Draw();

	D3DXVECTOR3 GetNormal(int n);
	D3DXVECTOR3 GetNormalPos(int n);
	int FindHitPlane(Sphere sphere);

	OBB GetOBB() { return m_OBB; }

	static CWall* Create(D3DXVECTOR3 pos, float X, float Y, float Z, int texID);

private:
	D3DXVECTOR3 m_Right;		// �E�����x�N�g�� (X)
	D3DXVECTOR3 m_Up;			// ������x�N�g�� (Y)
	D3DXVECTOR3 m_Front;		// �O�����x�N�g�� (Z)
	D3DXMATRIX m_World;			// ���[���h���W�ϊ��s��

	// �@���x�N�g��
	D3DXVECTOR3 m_Normal[6];
	D3DXVECTOR3 m_NormalPos[6];

	OBB m_OBB;					// �����蔻��
	CBox* m_Box;
};

#endif // !_WALL_H_

