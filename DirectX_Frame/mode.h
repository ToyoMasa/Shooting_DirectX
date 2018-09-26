//======================================================================
//	‰æ–Ê‘JˆÚ‚ÌŠî’êƒNƒ‰ƒX[mode.h]
//
//======================================================================
#ifndef _MODE_H_
#define _MODE_H_

class CMode
{
public:
	CMode() {}
	virtual ~CMode() {}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

private:

};

#endif // !_MODE_H_
