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

	virtual void Init(int id);
	virtual void Uninit();
	void SetLightColor(float diff1, float diff2, float diff3, float diff4, float amb1, float amb2, float amb3, float amb4);
	void SetEnable(BOOL enable);
	static CLight* Create(int id);

protected:
	D3DLIGHT9 m_Light;
	int	m_LightID;
};

class CSpotLight : public CLight
{
public:
	CSpotLight();
	~CSpotLight();

	void Init(
		int id,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 dir,
		float range,
		float theta,
		float phi);
	void Uninit();
	void SetPos(D3DXVECTOR3 pos);
	void SetDir(D3DXVECTOR3 dir);
	void SetRange(float range);
	void SetTheta(float theta);
	void SetPhi(float phi);
	static CSpotLight* Create(
		int id,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 dir,
		float range,
		float theta,
		float phi);

};

#endif // !_LIGHT_H_
