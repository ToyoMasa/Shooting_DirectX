#include "common.h"
#include "main.h"
#include "renderer.h"

//======================================================================
//	静的メンバ変数の初期化
//======================================================================
LPDIRECT3D9 CRenderer::m_D3D = NULL;
LPDIRECT3DDEVICE9 CRenderer::m_D3DDevice = NULL;

CRenderer::CRenderer()
{
	CRenderer::m_D3D = NULL;
	CRenderer::m_D3DDevice = NULL;
}

CRenderer::~CRenderer()
{
}

bool CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;

	//Direct3Dインターフェースの取得
	CRenderer::m_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (CRenderer::m_D3D == NULL)
	{
		return false;
	}

	//現在のディスプレイモードを取得
	if (FAILED(CRenderer::m_D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return false;
	}

	//デバイスのプレゼンテーションパラメータの設定
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//[デバイス作成制御]<描画>と<頂点処理>をハードウェアで行う
	if (FAILED(CRenderer::m_D3D->CreateDevice(
		D3DADAPTER_DEFAULT,						//ディスプレイアダプタ
		D3DDEVTYPE_HAL,							//デバイスタイプ
		hWnd,									//フォーカスするウィンドウへのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,	//デバイス作成制御の組み合わせ
		&d3dpp,									//デバイスのプレゼンテーションへのポインタ
		&CRenderer::m_D3DDevice)))						//デバイスインタフェースへのポインタ　※最重要
	{
		//上記の設定が失敗したら
		MessageBox(hWnd, "Direct3Dデバイスの作成に失敗しました", "エラー", MB_OK);
		return false;
	}

	//レンダ―ステートパラメータの設定
	//SRC…今から描くもの。つまりポリゴンにテクスチャを貼ったもの。
	//DEST…すでに描画されている画面のこと。
	//SRC_RGB * SRC_α + DEST_RGB * (1 - SRC_α)
	CRenderer::m_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	CRenderer::m_D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CRenderer::m_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//CRenderer::m_D3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	// 照明
	CRenderer::m_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//ポリゴンとテクスチャの色の乗算
	CRenderer::m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	CRenderer::m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);			//左辺値
	CRenderer::m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);			//右辺値

	//テクスチャのピクセルに関する設定
	//テクスチャアドレス外を参照した時にどうするか
	//WRAP = 反復（繰り返し）
	//CLAMP = 一番最後のピクセルを引き延ばす
	//MIRROR = 反転（U方向のみ、V方向のみもできる）
	CRenderer::m_D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	CRenderer::m_D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	//D3DTEXF_POINT　＝　周りの点を無理やり持ってくる→ドット絵を強調する時に使う
	//D3DTEXF_LINEAR　＝　アンチエイリアシングのようになめらかにする(基本こっち)
	CRenderer::m_D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);			//実際のテクスチャサイズより小さい時のフィルタリング
	CRenderer::m_D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);			//実際のテクスチャサイズより大きい時のフィルタリング
	CRenderer::m_D3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);			//ミップマップ（元の画像の÷２の単位で自動で作られる画像）間を補正する

	return true;
}

void CRenderer::Uninit()
{
	if (CRenderer::m_D3DDevice != NULL)
	{//デバイスの開放
		CRenderer::m_D3DDevice->Release();
		CRenderer::m_D3DDevice = NULL;
	}

	if (CRenderer::m_D3D != NULL)
	{//Direct3Dオブジェクトの開放
		CRenderer::m_D3D->Release();
		CRenderer::m_D3D = NULL;
	}
}

LPDIRECT3DDEVICE9 CRenderer::GetDevice()
{
	return CRenderer::m_D3DDevice;
}

HRESULT CRenderer::DrawBegin()
{
	//クリア処理
	CRenderer::GetDevice()->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(64, 96, 255, 255), 1.0f, 0);
	//CRenderer::GetDevice()->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	return CRenderer::GetDevice()->BeginScene();
}

void CRenderer::DrawEnd()
{
	//Direct3Dによる描画の終了
	CRenderer::GetDevice()->EndScene();

	//バックバッファとフロントバッファの入れ替え
	CRenderer::GetDevice()->Present(NULL, NULL, NULL, NULL);	//第三引数はhWndでも可
}