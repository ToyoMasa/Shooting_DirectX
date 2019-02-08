//======================================================================
//	�Q�[���Ǘ��w�b�_[manager]
//
//======================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

class CInputKeyboard;
class CInputMouse;
class CCamera;
class CLight;
class CMode;

static const float DRAW_DIST = 40.0f;

class CManager
{
public:
	CManager() {}
	~CManager() {}

	static bool Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	static void Uninit();
	static void Update();
	static void Draw();
	static void DrawShadow();

	static CInputKeyboard *GetInputKeyboard(void) { return m_InputKeyboard; }
	static CInputMouse *GetInputMouse(void) { return m_InputMouse; }
	static void SetMode(CMode* mode);
	static CMode* GetMode() { return m_Mode; }
	static void SetCamera(CCamera* camera) { m_UsingCamera = camera; }
	static CCamera* GetCamera() { return m_UsingCamera; }

	static void ChangeDebugMode();
	static void DebugCameraControll();
	static bool GetDebug() { return m_isDebug; }
	static bool GetFogEnable() { return m_isFogEnable; }
	static void SetSkinMeshDrawDist(float dist) { m_SkinMeshDrawDist = dist; }
	static float GetSkinMeshDrawDist() { return m_SkinMeshDrawDist; }

	static void GetCurrentRendertarget(
		LPDIRECT3DSURFACE9& currentsurface,		// ���݂̕`��T�[�t�F�X��ۑ����邽�߂̃T�[�t�F�X
		LPDIRECT3DSURFACE9& currentzbuffer, 	// ���݂̂y�o�b�t�@��ۑ����邽�߂̃T�[�t�F�X
		D3DVIEWPORT9& currentvp);				// ���݂̃r���[�|�[�g��ۑ����邽�߂̂���
	static void SetRenderTarget(
		LPDIRECT3DSURFACE9 rendersurface,	// �����_�[�^�[�Q�b�g
		LPDIRECT3DSURFACE9 zbuffer,			// �y�o�b�t�@
		D3DVIEWPORT9&		vp); 			// �r���[�|�[�g
	static void CreateShadow();
	static D3DXMATRIX GetLightCamera() { return m_LightCamera; }
	static D3DXMATRIX GetLightProjection() { return m_LightProjection; }

	static LPDIRECT3DTEXTURE9 GetShadowTexture() { return m_ShadowTex; }
	static void DrawQuad(LPDIRECT3DDEVICE9 device, float x, float y, float width, float height, LPDIRECT3DTEXTURE9 tex);

private:
	static CInputKeyboard*	m_InputKeyboard;	// �L�[�{�[�h�ւ̃|�C���^
	static CInputMouse*		m_InputMouse;		// �}�E�X�ւ̃|�C���^
	static CLight*			m_Light;
	static CMode*			m_Mode;
	static CCamera*			m_UsingCamera;
	static CCamera*			m_DebugCamera;
	static CCamera*			m_TempCamera;
	// ���e�e�N�X�`���}�b�v�쐬�p
	static LPDIRECT3DTEXTURE9	m_ShadowTex;				// ���e�V���h�E�e�N�X�`��
	static LPDIRECT3DSURFACE9	m_ShadowSurface;			// ���e�V���h�E�e�N�X�`���T�[�t�F�X	
	static LPDIRECT3DSURFACE9	m_ShadowZbufferSurface;		// ���e�e�N�X�`���փ����_�����O����ۂ̂y�o�b�t�@�i�[�x�o�b�t�@�j
	static D3DXMATRIX			m_LightCamera;				// ���C�g�̈ʒu���J�����̈ʒu�ɂ������̃r���[�s��
	static D3DXMATRIX			m_LightProjection;			// �v���W�F�N�V�����ϊ��s������(���e�V���h�E�p)
	static bool				m_isDebug;
	static bool				m_isFogEnable;
	static float			m_SkinMeshDrawDist;
};

#endif // !_MANAGER_H_
