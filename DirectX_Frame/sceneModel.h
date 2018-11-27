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
	MODEL_ID_SKYDOME,
	MODEL_ID_RIFLE,
	MODEL_ID_SHOTGUN,
	MODEL_ID_CAPSULE,
	MODEL_ID_MAX,
} MODEL_ID;

// ���f���ǂݍ��ݗp�̃\�[�X
static const std::string MODEL_SOURCE[] =
{
	"data/models/sky.x",
	"data/models/rifle.x",
	"data/models/shotgun.x",
	"data/models/scifi_capsule.x",
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
		m_NormalmapTexture = NULL;
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
	void DrawWithShader()override;
	void SetWorld(D3DXMATRIX move);
	void SetNormalMapTexture(const std::string& texName);
	void Move(D3DXVECTOR3 pos);
	void Rotate(D3DXVECTOR3 rot);
	void Rotate(D3DXMATRIX rot);
	void Scale(D3DXVECTOR3 scale);
	bool AddTangentSpace();
	D3DXMATRIX& GetScale() { return m_Scale; }
	D3DXMATRIX& GetWorld() { return m_World; }
	LPD3DXMESH GetMesh() { return m_Mesh; }
	static CSceneModel* Create(const std::string& modelName, int layer = LAYER_OBJECT3D);
	static CSceneModel* Create(const std::string& modelName, bool isIgnore, int layer = LAYER_OBJECT3D);

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
	LPDIRECT3DTEXTURE9  m_NormalmapTexture; // �@���}�b�v�e�N�X�`��
	bool				m_isIgnoreLight;// �e��t���邩�ǂ���
};

#endif //!_SCENEMODEL_H_