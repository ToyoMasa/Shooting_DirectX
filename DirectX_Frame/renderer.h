//======================================================================
//	DirectX������
//
//======================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	static bool Init(HWND hWnd, BOOL bWindow);
	static void Uninit();
	static HRESULT DrawBegin();
	static void DrawEnd();
	static LPDIRECT3DDEVICE9 GetDevice();

private:
	static LPDIRECT3D9			m_D3D;				//Direct3D�C���^�[�t�F�[�X
	static LPDIRECT3DDEVICE9	m_D3DDevice;		//Direct3D�f�o�C�X
};

#endif // !_RENDERER_H_
