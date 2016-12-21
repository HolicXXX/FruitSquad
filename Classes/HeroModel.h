#ifndef _HEROMODEL_H_
#define _HEROMODEL_H_

#include "cocos2d.h"

class HeroModel : public cocos2d::Node
{
public:
	static HeroModel* create(int heroIndex);
	virtual bool init(int heroIndex);

	void resetHeroIndex(int heroIndex);
private:
	int m_heroIndex;
	cocos2d::Node* m_lockButton;
	cocos2d::Node* m_levelResetButton;
	cocos2d::Node* m_levelUpButton;
	
	cocos2d::Node* m_type;
	cocos2d::Sprite* m_model;
	cocos2d::Label* m_nameText;

	void setModel();
	void setButton();

	void levelUpEff();
	void levelResetEff();
};


#endif