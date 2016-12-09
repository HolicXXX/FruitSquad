#ifndef _ORANGESKILL_H_
#define _ORANGESKILL_H_

#include "Level.h"
#include "BoxBase.h"

class orangeSkill : public BoxBase
{
public:
	static orangeSkill* create(Level l, Vec2 dir);
	virtual bool init(Level l, Vec2 dir);
	virtual void update(float dt);

	virtual void initEff();
	virtual void setBox();
	virtual Rect getBox(){ return m_hitRect; }
	virtual DeBuff* getDeBuff();
private:
	Level m_level;
	float m_speed;
	Rect m_hitRect;
	std::string m_aniName;

	void initAni();
	Armature* m_ani;
	void initDir();
	float m_rotation;
	Vec2 m_dir;

};


#endif