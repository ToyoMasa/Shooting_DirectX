#ifndef _EFFEKSEER_H
#define _EFFEKSEER_H

#include <Effekseer.h>
#include <EffekseerRendererDX9.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "scene.h"

struct EffectFile
{
	const wchar_t fileName[256];
};


class CEffekseer : public CScene
{
public:
	struct Transform
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 scale;
		D3DXVECTOR3 rotate;
	};

	const enum Effect
	{
		EFFECT_BLOODLOSS,
		EFFECT_MUZZLEFLASH,
		EFFECT_SMOKE,
		EFFECT_FLAME,
		EFFECT_MAX
	};
	const EffectFile fileName_[EFFECT_MAX]{
		L"data/effects/BloodLoss.efk",
		L"data/effects/muzzleflash.efk",
		L"data/effects/smoke.efk",
		L"data/effects/flame.efk",
	};


public:
	CEffekseer(Effect EffectType, int prioriy) :CScene(prioriy)
	{
		//描画管理用インスタンスの生成
		effekseerRenderer_ = EffekseerRendererDX9::Renderer::Create(CRenderer::GetDevice(), 2000);
		// エフェクト管理用インスタンスの生成
		effekseerManager_ = Effekseer::Manager::Create(2000);

		effekseerRenderer_->SetProjectionMatrix(
			Effekseer::Matrix44().PerspectiveFovLH(D3DXToRadian(90.0f),                           //視野角
			(float)SCREEN_WIDTH / SCREEN_HEIGHT,   //アスペクト比
				0.1f, 10000.0f));

		// 描画用インスタンスから描画機能を設定
		effekseerManager_->SetSpriteRenderer(effekseerRenderer_->CreateSpriteRenderer());
		effekseerManager_->SetRibbonRenderer(effekseerRenderer_->CreateRibbonRenderer());
		effekseerManager_->SetRingRenderer(effekseerRenderer_->CreateRingRenderer());
		effekseerManager_->SetTrackRenderer(effekseerRenderer_->CreateTrackRenderer());
		effekseerManager_->SetModelRenderer(effekseerRenderer_->CreateModelRenderer());

		// 描画用インスタンスからテクスチャの読込機能を設定
		// 独自拡張可能、現在はファイルから読み込んでいる。
		effekseerManager_->SetTextureLoader(effekseerRenderer_->CreateTextureLoader());
		effekseerManager_->SetModelLoader(effekseerRenderer_->CreateModelLoader());

		//左手座標系に設定
		effekseerManager_->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

		this->effectType_ = EffectType;

		this->LoadEffect();

		this->SetPosition(0.0f, 0.0f, 0.0f);
		this->SetRotate(0.0f, 0.0f, 0.0f);
		this->SetScale(1.0f, 1.0f, 1.0f);
	}
	void Init();
	void Uninit() override;
	void Update()override;
	void Draw()override;
	void DrawShadow()override {}
	void DrawWithShader()override {}

	static CEffekseer *Create(Effect EffectType, int priority = LAYER_EFFEKSEER);
	static void  CreateScene(Effect EffectType);

	void SetLocation(D3DXVECTOR3 pos);
	void SetProj();
	void SetView(D3DXVECTOR3 Pos, D3DXVECTOR3 At, D3DXVECTOR3 Up);
	void SetSpeed(float speed);
	void RepeatEffect(bool repeat);
	void Play();
	void Paused(bool paused);
	void Stop();
	void SetMatrix(D3DXMATRIX mat);
	bool GetPlaying();

	void SetVisible(bool flag);
	bool GetVisible();

	void SetPosition(float x,float y,float z);
	void SetPosition(D3DXVECTOR3 trans);
	D3DXVECTOR3 GetPosition();
	void SetRotate(float x,float y,float z);
	void SetRotate(D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRotate();
	void SetScale(float x, float y,float z);
	void SetScale(D3DXVECTOR3 scale);
	D3DXVECTOR3 GetScale();
	void SetMtxRotate(D3DXMATRIX rot) { m_Rotate = rot; }

private:
	void LoadEffect();
	
	Effekseer::Manager*					effekseerManager_;
	EffekseerRendererDX9::Renderer*		effekseerRenderer_;
	Effekseer::Effect*					effekseerEffect_;
	Effekseer::Handle					effekseerHandle_;
	CEffekseer::Effect					effectType_;
	bool repeat_ = false;				//繰りかえし再生するか。
	bool paused_ = false;				//ポーズ中か。
	bool playing_ = false;				//再生中か
	bool visible_ = false;				//表示するか
	Transform transform_;
	D3DXMATRIX	m_Rotate;
	int priority_;
};

#endif // !_EFFEKSEER_H