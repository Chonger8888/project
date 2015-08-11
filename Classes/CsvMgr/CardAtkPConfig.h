#ifndef __CARDATKPCONFIG_H__
#define __CARDATKPCONFIG_H__

#include "cocos2d.h"
USING_NS_CC; 

struct csvCardAtkPConfig_s{
	int				id;
	int				CardType;
	int				CardId;
	float			CardAtkCoe1;
	float			CardAtkCoe2;
	int				islock;
	int				star1;
	int				star2;
	int				star3;
	int				star4;
	int				star5;
	int				quickStoneId;
};

class CsvCardAtkPConfig
{
public:


	CsvCardAtkPConfig();
	~CsvCardAtkPConfig();

	void					SetData( std::vector<std::string> &str );
	csvCardAtkPConfig_s*		GetByID( int id );
	std::map<int, csvCardAtkPConfig_s*> *GetAll();

protected:
	std::map<int, csvCardAtkPConfig_s*>			CardAtkPConfigMap;
	int		counter;
};
#endif