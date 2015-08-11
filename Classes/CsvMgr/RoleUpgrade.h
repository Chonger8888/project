#ifndef __ROLEUPGRADE_H__
#define __ROLEUPGRADE_H__

#include "cocos2d.h"
USING_NS_CC; 

struct csvRoleUpgrade_s{
	int				roleId;
	int				roleGrade;
	int				roleWeapon;
	float			BassAtkCoe1;
	float			BassAtkCoe2;
	float			BassAtkCoe3;
	int				BassDefense;
	float			BassDefenseCoe;
	float			BassLuckCoe;
	float			BassHp;
	float			BassHpCoe1;
	float			BassHpCoe2;
	float			BassHpCoe3;
	float			ExpCoe1;
	float			ExpCoe2;
	float			ExpCoe3;
	int				isLock;
	int				AutoLockGr;
	int				CurExp;
	int				LockPrice;
	int				isOption;
};

class CsvRoleUpgrade
{
public:

	typedef enum 
	{
	}CSVDataKey;

	CsvRoleUpgrade();
	~CsvRoleUpgrade();

	void					SetData( std::vector<std::string> &str );
	csvRoleUpgrade_s*		GetByID( int id );
	std::map<int, csvRoleUpgrade_s*> *GetAll();

protected:
	std::map<int, csvRoleUpgrade_s*>			RoleUpgradeMap;
	int		counter;
};
#endif