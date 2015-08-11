#ifndef __GAMETASKCONFIG_H__
#define __GAMETASKCONFIG_H__

#include "cocos2d.h"
USING_NS_CC; 

struct csvGameTaskConfig_s{
	int				TaskID;
	std::string		TaskDes;
	std::string		TaskRewardDes;
	int				RewardNum;
	int				TaskTargetNum;
	int				TaskProgressNum;
	int				TaskState;			// 0-δ��� 1-�����δ��ȡ 2-���������ȡ
	int				TaskRewardID;		// ������ƷID
};

class CsvGameTaskConfig
{
public:

	typedef enum 
	{
	}CSVDataKey;

	CsvGameTaskConfig();
	~CsvGameTaskConfig();

	void					SetData( std::vector<std::string> &str );
	csvGameTaskConfig_s*		GetByID( int id );
	std::map<int, csvGameTaskConfig_s*> *GetAll();

protected:
	std::map<int, csvGameTaskConfig_s*>			GameTaskConfigMap;
	int		counter;
};
#endif