#ifndef _SKILL_BUTTON_H_
#define _SKILL_BUTTON_H_

#include "cocos2d.h"

enum SkillButtonState
{
	NORMAL,
	CD,
	DISABEL,
};
enum SkillButtonEffect
{
	NONE,
	FROZEN,
	SLEEP,
	FORBIDDEN
};

class SkillButton : public cocos2d::Node
{
public:
	static SkillButton* create(int index);
	virtual bool init(int index);

	void bindSkillCallBack(const std::function<bool()> & func){ m_callback = func; }

	SkillButtonState getState(){ return m_state; }

	void setEffect(SkillButtonEffect eff, float t = 0);
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
	SkillButtonEffect m_effState;
	cocos2d::Sprite* m_eff;

	SkillButtonState m_state;
	void resetTexture();
	cocos2d::Sprite* m_skill;
	cocos2d::Node* m_cd;
};

#endif