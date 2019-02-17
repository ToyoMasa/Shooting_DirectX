//======================================================================
//	チュートリアルパターンヘッダ[tutorialBase.h]
//
//======================================================================
#ifndef _TUTORIAL_BASE_H_
#define _TUTORIAL_BASE_H_

static const int TUTORIAL_FADE_FRAME = 60;
static const int TUTORIAL_DRAW_FRAME = 300;

class CScene2D;
class CPlayer;

class CTutorialBase
{
public:
	CTutorialBase() {}
	~CTutorialBase() {}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update(CPlayer* player) = 0;

	static void LoadTexture();
	static void ReleaseTexture();

protected:
	CScene2D*	m_Text;
	CScene2D*	m_Controller;
	CScene2D*	m_Pad;
	int			m_Count;
	int			m_Alpha;
};

#endif // !_TUTORIAL_BASE_H_

