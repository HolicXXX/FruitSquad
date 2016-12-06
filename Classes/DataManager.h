#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_

#include <vector>
#include <functional>

class DataManager
{
public:
	static DataManager* getInstance();

	std::string getName(int index);

	void submitTeamList(const std::vector<int> & list);
	void submitTeamNumber(int t){ m_teamNum = t; }
	std::vector<int>& getTeamList(){ return m_teamList; }
	int getTeamNumber(){ return m_teamNum; }

	void registAddGoldCallBack(const std::function<void(int)> & func){ m_addGold = func; }
	void registCostGoldCallBack(const std::function<void(int)> & func){ m_costGold = func; }
	void addGold(int g);
	void costGold(int g);
	int getGoldNum(){ return m_gold; }
	void setGoldNum(int g){ m_gold = g; }
private:
	DataManager();
	~DataManager();

	int m_gold;
	std::function<void(int)> m_addGold;
	std::function<void(int)> m_costGold;

	int m_teamNum;
	std::vector<int> m_teamList;
	std::vector<std::string> m_heroNames;
};


#endif