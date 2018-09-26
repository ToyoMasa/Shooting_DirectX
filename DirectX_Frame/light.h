//======================================================================
//	ライティング処理[Light.h]
//
//======================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

class CLight : public CGameObject
{
public:
	CLight();
	~CLight();

	void Init(int id);
	void Uninit();
	void SetLight(float diff1, float diff2, float diff3, float diff4, float amb1, float amb2, float amb3, float amb4);
	static CLight* Create(int id);

private:
	D3DLIGHT9 m_Light;
	int	m_LightID;
};

#endif // !_LIGHT_H_
