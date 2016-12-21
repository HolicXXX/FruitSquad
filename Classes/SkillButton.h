#ifndef _SKILL_BUTTON_H_
#define _SKILL_BUTTON_H_

#include "cocos2d.h"
#include "Level.h"

class SkillButton : public cocos2d::Node
{
public:
	static SkillButton* create(int index);
	virtual bool init(int index);
	virtual void update(float dt);

	void bindSkillCallBack(const std::function<bool()> & func){ m_callback = func; }

	ButtonState getState(){ return m_state; }

	void setEffect(ButtonEffect eff, float t = 0);
	bool isCD(){ return m_isCD; }
	void resetButton();
private:
	int m_index;
	std::string m_name;
	bool m_isCD;
	void setCDTime();
	float m_cdTime;
	std::function<bool()> m_callback;

	float m_effTime;
	ButtonEffect m_effState;
	cocos2d::Sprite* m_eff;
	float m_effCount;

	ButtonState m_state;
	void resetTexture();
	cocos2d::Sprite* m_skill;
	cocos2d::Node* m_cd;
};

#endif