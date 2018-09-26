//======================================================================
//	DirectX初期化
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
	static LPDIRECT3D9			m_D3D;				//Direct3Dインターフェース
	static LPDIRECT3DDEVICE9	m_D3DDevice;		//Direct3Dデバイス
};

#endif // !_RENDERER_H_
