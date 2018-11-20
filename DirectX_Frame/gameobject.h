#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

class CGameObject
{
public:
	virtual ~CGameObject() {}
	virtual void Init() {}
	virtual void Uninit() = 0;
	void Release();
private:

};

#endif // !_GAMEOBJECT_H_
