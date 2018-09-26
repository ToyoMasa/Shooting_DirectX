//======================================================================
//	フェード画面[fade.h]
//
//======================================================================
#ifndef _FADE_H_
#define _FADE_H_

class CFade
{
public:
	CFade() {}
	~CFade() {}

	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();
	static bool GetFade() { return m_FadeIn | m_FadeOut; }
	static bool GetFadeOut() { return m_FadeOut; }

	static void FadeIn();
	static void FadeOut(CMode* nextMode);

private:
	static int m_Alpha;
	static bool m_FadeIn;
	static bool m_FadeOut;
	static CMode* m_NextMode;
};

#endif // !_FADE_H_

