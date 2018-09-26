#ifndef _BULLET_H_
#define _BULLET_H_

class CBullet : public CScene
{
public:
	CBullet::CBullet(int priority) : CScene(priority)
	{
		m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Forward =	D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Right = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Speed = 1.0f;
		m_Type = SCENE_TYPE_BULLET;
	}
	~CBullet(){}

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	D3DXVECTOR3 m_Forward;
	D3DXVECTOR3 m_Right;
	float m_Speed;
};

#endif // !_BULLET_H_

