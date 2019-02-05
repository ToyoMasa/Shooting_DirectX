//======================================================================
//	チュートリアルパターンヘッダ[tutorialBase.h]
//
//======================================================================
#ifndef _TUTORIAL_BASE_H_
#define _TUTORIAL_BASE_H_

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
	int			m_Count;
	int			m_Alpha;
};

#endif // !_TUTORIAL_BASE_H_

