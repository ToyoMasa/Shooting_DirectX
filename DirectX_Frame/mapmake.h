#ifndef _MAPMAKE_H_
#define _MAPMAKE_H_

#include "mode.h"

class CCamera;

class CModeMapMake : public CMode
{
public:
	CModeMapMake() : CMode() {}
	~CModeMapMake() {}

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void ChangeMap();

private:
	static CCamera*	m_Camera;
};

#endif // !_MAPMAKE_H_
