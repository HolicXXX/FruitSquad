#ifndef _PROPERTYBOX_H_
#define _PROPERTYBOX_H_

#include "HeadFile.h"

class PropertyBox : public Node
{
public:
	CREATE_FUNC(PropertyBox);
	virtual bool init();
	virtual void update(float dt);

	void resetBox(int heroindex);
private:
	Sprite* m_bg;

	Scale9Sprite* m_hpBar;
	Scale9Sprite* m_atkBar;
	Scale9Sprite* m_skillBar;

	Scale9Sprite* m_hpBarBG;
	Scale9Sprite* m_atkBarBG;
	Scale9Sprite* m_skillBarBG;
};



#endif