#include "DataManager.h"

static DataManager* _datamanager = nullptr;

DataManager::DataManager()
{
	m_teamNum = 0;
	m_heroNames.push_back("orange");
	m_heroNames.push_back("coconut");
	m_heroNames.push_back("apple");
	m_heroNames.push_back("pitaya");
	m_heroNames.push_back("guava");
	m_heroNames.push_back("watermelon");
	m_heroNames.push_back("pineapple");
	m_heroNames.push_back("pear");
	m_heroNames.push_back("cherry");
	m_heroNames.push_back("strawberry");
	m_heroNames.push_back("mangosteen");
	m_heroNames.push_back("evilorange");
	m_heroNames.push_back("evilapple");

	m_gold = 0;
	m_addGold = nullptr;
}

DataManager::~DataManager()
{

}

DataManager* DataManager::getInstance()
{
	if (!_datamanager)
	{
		_datamanager = new DataManager();
	}
	return _datamanager;
}

void DataManager::submitTeamList(const std::vector<int> & list)
{
	m_teamList = list;
}

std::string DataManager::getName(int index)
{
	std::string ret = m_heroNames[index];
	return ret;
}

void DataManager::addGold(int g)
{
	if (m_addGold)
	{
		m_gold += g;
		m_addGold(g);
	}
}
void DataManager::costGold(int g)
{
	if (m_costGold)
	{
		m_gold -= g;
		m_costGold(g);
	}
}