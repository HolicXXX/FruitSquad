#include "TeamList.h"
USING_NS_CC;

TeamList* TeamList::create()
{
	auto tl = new TeamList();
	if (tl && tl->init())
	{
		tl->autorelease();
		return tl;
	}
	CC_SAFE_DELETE(tl);
	return nullptr;
}

bool TeamList::init()
{
	if (!Node::init())
	{
		return false;
	}
	m_count = 0;
	//data
	m_maxNum = 1;
	m_mustChoosePos = m_mustChooseIndex = -1;
	for (int i = 0; i < m_maxNum;i++)
	{
		m_index.push_back(-1);
	}

	initBG();
	resetIcon();

	return true;
}

void TeamList::addHero(int index)
{
	if (m_mustChooseIndex >= 0)
	{
		if (index != m_mustChooseIndex)
		{
			auto find = isFind(index);
			if (find != -1)
			{
				for (int i = find; i < m_maxNum; i++)
				{
					if (i + 1 == m_maxNum)
					{
						m_index.at(i) = -1;
						break;
					}
					if (i + 1 == m_mustChoosePos)
					{
						if (i + 2 == m_maxNum)
						{
							m_index.at(i) = -1;
							break;
						}
						else
						{
							m_index.at(i) = m_index.at(i + 2);
							i++;
						}
					}
					else
					{
						m_index.at(i) = m_index.at(i + 1);
					}
				}
			}
			else
			{
				reSort(true, index);
			}
		}
	}
	else
	{
		auto find = isFind(index);
		if (find != -1)
		{
			for (int i = find; i < m_maxNum; i++)
			{
				if (i + 1 == m_maxNum)
				{
					m_index.at(i) = -1;
				}
				else
				{
					m_index.at(i) = m_index.at(i + 1);
				}
			}
			m_count--;
		}
		else
		{
			reSort(false, index);
		}
	}
	resetIcon();
}

void TeamList::reSort(bool must, int index)
{
	if (must)
	{
		if (m_count == m_maxNum)
		{
			for (int i = 0; i < m_maxNum; i++)
			{
				if (i + 1 == m_maxNum)
				{
					m_index.at(i) = index;
					break;
				}
				if (i + 1 == m_mustChoosePos)
				{
					if (i + 2 == m_maxNum)
					{
						m_index.at(i) = index;
						break;
					}
					else
					{
						m_index.at(i) = m_index.at(i + 2);
						i++;
					}
				}
				else
				{
					m_index.at(i) = m_index.at(i + 1);
				}
			}
			
		}
		else
		{
			for (int i = 0; i < m_maxNum; i++)
			{
				if (m_index.at(i) == -1)
				{
					m_index.at(i) = index;
					break;
				}
			}
			m_count++;
		}
	}
	else
	{
		if (m_count == m_maxNum)
		{
			for (int i = 0; i < m_maxNum; i++)
			{
				if (i + 1 != m_maxNum)
					m_index.at(i) = m_index.at(i + 1);
				else
					m_index.at(i) = index;
			}
		}
		else
		{
			for (int i = 0; i < m_maxNum; i++)
			{
				if (m_index.at(i) == -1)
				{
					m_index.at(i) = index;
					break;
				}
			}
			m_count++;
		}
	}
	
}

int TeamList::isFind(int index)
{
	int find = -1;
	for (int i = 0; i < m_maxNum; ++i)
	{
		if (m_index.at(i) == index)
		{
			find = i; 
			break;
		}
	}
	return find;
}

void TeamList::resetIcon()
{
	std::vector<std::string> m_iconStr(13);
	m_iconStr[0] = "orange";
	m_iconStr[1] = "coconut";
	m_iconStr[2] = "apple";
	m_iconStr[3] = "pitaya";
	m_iconStr[4] = "guava";
	m_iconStr[5] = "watermelon";
	m_iconStr[6] = "pineapple";
	m_iconStr[7] = "pear";
	m_iconStr[8] = "cherry";
	m_iconStr[9] = "strawberry";
	m_iconStr[10] = "mangosteen";
	m_iconStr[11] = "evilorange";
	m_iconStr[12] = "evilapple";

	if (m_mustChooseIndex >= 0)
	{
		if (m_index.at(m_mustChoosePos) != m_mustChooseIndex)
		{
			if (m_mustChooseIndex == 1 || m_mustChooseIndex == 5)
			{
				m_mustChoosePos = 0;
			}
			else if (m_mustChooseIndex == 4 || (m_mustChooseIndex >= 7 && m_mustChooseIndex <= 10))
			{
				m_mustChoosePos = m_maxNum - 1;
			}
			else
			{
				m_mustChoosePos = 1;
			}
			m_index.at(m_mustChoosePos) = m_mustChooseIndex;
		}
	}
	for (int i = 0; i < m_maxNum; ++i)
	{
		if (m_listBG.at(i)->getChildByTag(2) != nullptr)
		{
			m_listBG.at(i)->getChildByTag(2)->removeFromParent();
		}
		if (m_index.at(i) != -1)
		{
			auto icon = Sprite::create(StringUtils::format("gamescene/hero/%s_small.png", m_iconStr.at(m_index.at(i)).c_str()));
			icon->setTag(2);
			m_listBG.at(i)->addChild(icon);
		}
	}
}

void TeamList::getTouched(Touch* t)
{
	auto detect = detectTouch(t);
	if (detect != m_mustChooseIndex)
		addHero(m_index.at(detect));
}

int TeamList::detectTouch(Touch* t)
{
	for (int i = 0; i < m_maxNum;i++)
	{
		auto node = m_listBG.at(i);
		auto pos = node->convertTouchToNodeSpace(t);
		auto size = static_cast<Sprite*>(node->getChildByTag(2))->getContentSize();
		auto rc = Rect{ -Vec2(size / 2), size };
		if (rc.containsPoint(pos))
			return i;
	}
}

void TeamList::initBG()
{
	for (int i = 0; i < 4; ++i)
	{
		auto node = Node::create();
		node->setTag(i);
		auto bg = Sprite::create("gamescene/hero_list_bg.png");
		bg->setTag(1);
		node->addChild(bg);
		if (i >= m_maxNum)
		{
			auto disable = Sprite::create("gamescene/hero_list_disable.png");
			disable->setTag(2);
			node->addChild(disable);
		}
		node->setPosition(Vec2(i * bg->getContentSize().width, 0));
		this->addChild(node);
		m_listBG.pushBack(node);
	}
	auto bgSize = static_cast<Sprite*>(m_listBG.at(0)->getChildByTag(1))->getContentSize();
	m_listRect = Rect{ -Vec2(bgSize / 2), Size(bgSize.width * 4, bgSize.height) };
}