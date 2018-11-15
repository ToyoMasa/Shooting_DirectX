//======================================================================
//	ライティング処理[Light.h]
//
//======================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

static const float LIGHT_NMR_X = -1.0f;
static const float LIGHT_NMR_Y = -1.0f;
static const float LIGHT_NMR_Z = 1.0f;
static const float GLOBAL_AMBIENT = 0.35f;
static const float DEFAULT_DEFUSE_COLOR = 0.7f;
static const float DEFAULT_AMBIENT_COLOR = 0.2f;

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
