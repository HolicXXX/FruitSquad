#ifndef _SHOP_LAYER_H_
#define _SHOP_LAYER_H_

#include "HeadFile.h"

class ShopLayer : public Layer
{
public:
	static ShopLayer* create();
	virtual bool init();
	virtual void onExit();

	virtual void update(float dt);
private:
	void initShopBG();
	Sprite* m_bg;
	Sprite* m_returnButton;

	std::vector<std::string> m_pages;
	int m_focus;

	void resetTitle(int p);
	Sprite* m_title;
	Sprite* m_titleLabel;

	void initGems();
	Node* m_gems;
	void costGems(Ref*);

	void initPageView();
	PageView* m_page;
	ListView* getPage(int index);
	void buttonCallBack(int page, int index);

	ListView* m_listView;
	void initListView();
	void checkBoxCallBack(Ref*, CheckBoxEventType);

	void initAni();
	Armature* m_leftAni;
	Armature* m_rightAni;

	Sprite* m_wcloud1;
	Sprite* m_wcloud2;
	Sprite* m_wcloud3;
	Sprite* m_wcloud4;
	Sprite* m_bcloud1;
	Sprite* m_bcloud2;
	Sprite* m_bcloud3;
	Sprite* m_bcloud4;
	void initBackGround();
	void bgUpdate(float dt);
};



#endif