#ifndef _ITEM_BUTTON_H_
#define _ITEM_BUTTON_H_

#include "cocos2d.h"

enum ButtonRole
{
	SELECT,
	USE
};

enum ItemButtonState
{
	NORMAL,
	CD,
	DISABEL,
};

enum ItemType
{
	BOOM,
	ANGEL,
	FROZEN,
	BUFF,
	DRAGON
};

class ItemButton : public cocos2d::Node
{
public:
	static ItemButton* create(ButtonRole r, ItemType t);
	virtual bool init(ButtonRole r, ItemType t);

	void bindUseItemCallBack(const std::function<void()> & func){ m_useCallback = func; }

	void setSelected(bool b);
	bool isSelected(){ return m_isSelected; }
	void resetTexture();
private:
	ButtonRole m_role;
	ItemType m_type;
	ItemButtonState m_state;

	void initName();
	std::string m_name;

	void initItem();
	cocos2d::Sprite* m_item;

	void initEff();
	bool m_isSelected;
	cocos2d::Sprite* m_eff;

	void initCD();
	float m_cdTime;
	cocos2d::Node* m_cd;
	std::function<void()> m_useCallback;
	
	void initNumberImage();
	int m_number;
	cocos2d::Sprite* m_numberImage;
};

#endif