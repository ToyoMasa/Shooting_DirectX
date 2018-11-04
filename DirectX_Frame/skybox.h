//======================================================================
//	スカイボックスヘッダ[skybox]
//
//======================================================================
#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "scene.h"
class CSceneModel;
class CPlayer;

class CSkyBox : public CScene
{
public:
	CSkyBox(CPlayer* player):CScene(0)
	{
		m_Player = player;
		m_Model = CSceneModel::Create(MODEL_SOURCE[MODEL_ID_SKYDOME], true);
	}

	~CSkyBox(){}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawWithShader()override {}
	static CSkyBox* Create(CPlayer* player);

private:
	CSceneModel* m_Model;
	CPlayer* m_Player;
};

#endif // !_SKYBOX_H_

