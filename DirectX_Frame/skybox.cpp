//======================================================================
//	スカイボックス　（2018/06/06)
//	Author : 豊村 昌俊
//======================================================================
#include "common.h"
#include "main.h"
#include "scene.h"
#include "sceneModel.h"
#include "texture.h"
#include "skybox.h"
#include "player.h"

void CSkyBox::Init() 
{
	m_Model = CSceneModel::Create(MODEL_SOURCE[MODEL_ID_SKYDOME], true, LAYER_BACKGROUND);
	m_Model->Scale(D3DXVECTOR3(300.0f, 300.0f, 300.0f));
}

void CSkyBox::Uninit()
{
	if (m_Model != NULL)
	{
		m_Model = NULL;
	}

	if (m_Player != NULL)
	{
		m_Player = NULL;
	}
}

void CSkyBox::Update()
{
	if (m_Player != NULL)
	{
		D3DXVECTOR3 newPos = m_Player->GetPos();
		newPos.y = -1.0f;
		m_Model->Move(newPos);
	}
}

void CSkyBox::Draw()
{

}

CSkyBox* CSkyBox::Create(CPlayer* player)
{
	CSkyBox* skybox = new CSkyBox(player);
	skybox->Init();

	return skybox;
}
