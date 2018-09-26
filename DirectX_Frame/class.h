#ifndef _CLASS_H_

class CGameObject
{
public:
	virtual ~CGameObject() {}
	virtual void Init() {}
	virtual void Uninit() = 0;
	void Release();
private:

};

#endif // !_CLASS_H_
