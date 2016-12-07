#ifndef _ORANGESKILL_H_
#define _ORANGESKILL_H_

#include "cocos2d.h"
#include "Level.h"

class orangeSkill : public cocos2d::Node
{
public:
	static orangeSkill* create(Level l, cocos2d::Vec2 dir);
	virtual bool init(Level l, cocos2d::Vec2 dir);
	virtual void update(float dt);

	void setBox();
	cocos2d::Rect getBox(){ return m_hitRect; }
private:
	Level m_level;
	float m_speed;
	cocos2d::Rect m_hitRect;

	void initAni();
	void initDir();
	float m_rotation;
	cocos2d::Vec2 m_dir;

};


#endif