//======================================================================
//	3D���f���w�b�_[sceneModel]
//
//======================================================================
#ifndef _SCENEMODEL_H_
#define _SCENEMODEL_H_

#include <string>
#include <fbxsdk.h>
#pragma comment (lib, "libfbxsdk-mt.lib")

// ���f���Ǘ��pID
typedef enum
{
	MODEL_ID_UFO = 0,		// UFO
	MODEL_ID_XBOT,
	MODEL_ID_SKYBOX,
	MODEL_ID_SKYDOME,
	MODEL_ID_PLAYER,
	MODEL_ID_ENEMY01,
	MODEL_ID_TARGET,
	MODEL_ID_RIFLE,
	MODEL_ID_MAX,
} MODEL_ID;

// ���f���ǂݍ��ݗp�̃\�[�X
static const std::string MODEL_SOURCE[] =
{
	"data/models/player_ufo.x",
	"data/models/xbot2.x",
	"data/models/skybox.x",
	"data/models/sky.x",
	"data/models/player.x",
	"data/models/enemy01.x",
	"data/models/target.x",
	"data/models/rifle.x",
};

class CSceneModel : public CScene
{
public:
	//======================================================================
	//	�R���X�g���N�^
	//======================================================================
	CSceneModel::CSceneModel(int priority) : CScene(priority)
	{
		D3DXMatrixIdentity(&m_Target);
		D3DXMatrixIdentity(&m_World);
		D3DXMatrixIdentity(&m_Move);
		D3DXMatrixIdentity(&m_Rotate);
		D3DXMatrixIdentity(&m_RotX);
		D3DXMatrixIdentity(&m_RotY);
		D3DXMatrixIdentity(&m_RotZ);
		D3DXMatrixIdentity(&m_Scale);
		m_MaterialNum = 0;
		m_TextureNum = 0;
		m_Texture = NULL;
		m_Material = NULL;
		m_isIgnoreLight = false;
	}

	//======================================================================
	//	�f�X�g���N�^
	//======================================================================
	CSceneModel::~CSceneModel()
	{
	}

	void Init(const std::string& modelName);
	void Uninit();
	void Update();
	void Draw();
	void SetWorld(D3DXMATRIX move);
	void Move(D3DXVECTOR3 pos);
	void Rotate(D3DXVECTOR3 rot);
	void Rotate(D3DXMATRIX rot);
	void Scale(D3DXVECTOR3 scale);
	D3DXMATRIX GetScale() { return m_Scale; }
	D3DXMATRIX GetWorld() { return m_World; }
	static CSceneModel* Create(const std::string& modelName);
	static CSceneModel* Create(const std::string& modelName, bool isIgnore);

private:
	D3DXMATRIX			m_Target;
	D3DXMATRIX			m_World;		// ���[���h�ϊ��s��
	D3DXMATRIX			m_Move;			// ���s�ړ��s��
	D3DXMATRIX			m_Rotate;		// ��]�s��
	D3DXMATRIX			m_RotX;			// X����]�s��
	D3DXMATRIX			m_RotY;			// Y����]�s��
	D3DXMATRIX			m_RotZ;			// Z����]�s��
	D3DXMATRIX			m_Scale;		// �g��k���s��
	LPD3DXMESH			m_Mesh;			// ���b�V��
	DWORD				m_MaterialNum;	// �}�e���A����
	int					m_TextureNum;	// �e�N�X�`����
	LPD3DXBUFFER		m_Material;		// �}�e���A��
	LPDIRECT3DTEXTURE9 *m_Texture;		// �e�N�X�`��
	bool				m_isIgnoreLight;// �e��t���邩�ǂ���
};

#endif //!_SCENEMODEL_H_