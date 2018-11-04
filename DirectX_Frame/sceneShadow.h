//======================================================================
//	�e�`�揈���w�b�_[shadow.h]
//
//======================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

class CSceneShadow : public CScene
{
public:
	//======================================================================
	//	�R���X�g���N�^
	//======================================================================
	CSceneShadow::CSceneShadow() : CScene(LAYER_EFFEKSEER)
	{

	}

	void Init();
	void Uninit();
	void Update();
	void Draw();
	void DrawWithShader()override {}
	static CSceneShadow* Create();

	void Move(D3DXVECTOR3 pos);
	void Rotate(D3DXVECTOR3 rot);
	void Rotate(D3DXMATRIX rot);
	void Scale(D3DXVECTOR3 scale);

private:
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer; // ���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9  m_IndexBuffer;  // �C���f�b�N�X�o�b�t�@
	D3DXMATRIX				m_World;
	D3DXMATRIX				m_Move;			// ���s�ړ��s��
	D3DXMATRIX				m_Rotate;		// ��]�s��
	D3DXMATRIX				m_RotX;			// X����]�s��
	D3DXMATRIX				m_RotY;			// Y����]�s��
	D3DXMATRIX				m_RotZ;			// Z����]�s��
	D3DXMATRIX				m_Scale;		// �g��k���s��

};

#endif // !_SHADOW_H_
