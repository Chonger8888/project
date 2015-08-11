#include "RoleUpgrade.h"
#include "CsvCommon.h"

using namespace std;
using namespace cocos2d;

CsvRoleUpgrade::CsvRoleUpgrade() 
{
}

CsvRoleUpgrade::~CsvRoleUpgrade() 
{
	for ( map<int, csvRoleUpgrade_s*>::iterator it = RoleUpgradeMap.begin(); it != RoleUpgradeMap.end(); it++ ) {
		delete it->second;
	}
}

void CsvRoleUpgrade::SetData( vector<string> &str ) 
{
	csvRoleUpgrade_s   *item = new csvRoleUpgrade_s;
	counter = 0;

	item->roleId		= StringToInt( str[counter] );
	counter++; 
	item->roleGrade		= StringToInt( str[counter] );
	counter++; 
	item->roleWeapon	= StringToInt( str[counter] );

	counter++; 
	item->BassAtkCoe1	= StringToFloat( str[counter] );
	counter++; 
	item->BassAtkCoe2	= StringToFloat( str[counter] );
	counter++; 
	item->BassAtkCoe3	= StringToFloat( str[counter] );

	counter++; 
	item->BassDefense	= StringToInt( str[counter] );
	counter++; 
	item->BassDefenseCoe = StringToFloat( str[counter] );

	counter++; 
	item->BassLuckCoe	= StringToFloat( str[counter] );

	counter++; 
	item->BassHp		= StringToFloat( str[counter] );
	counter++; 
	item->BassHpCoe1	= StringToFloat( str[counter] );
	counter++; 
	item->BassHpCoe2	= StringToFloat( str[counter] );
	counter++; 
	item->BassHpCoe3	= StringToFloat( str[counter] );

	counter++; 
	item->ExpCoe1		= StringToFloat( str[counter] );
	counter++; 
	item->ExpCoe2		= StringToFloat( str[counter] );
	counter++; 
	item->ExpCoe3		= StringToFloat( str[counter] );

	counter++; 
	item->isLock		= StringToInt( str[counter] );
	counter++; 
	item->AutoLockGr	= StringToInt( str[counter] );
	counter++; 

	item->CurExp = StringToInt( str[counter] );
	counter++; 

	item->LockPrice	 = StringToInt( str[counter] );
	counter++; 

	item->isOption	 = StringToInt( str[counter] );
	counter++; 

	RoleUpgradeMap.insert( pair<int,csvRoleUpgrade_s*>( atoi( str[0].c_str() ), item ) ); 
}

csvRoleUpgrade_s *CsvRoleUpgrade::GetByID( int id ) 
{
	map<int, csvRoleUpgrade_s*>::iterator	it = RoleUpgradeMap.find( id ); 
	if( it == RoleUpgradeMap.end() ){ return NULL; } 
	return it->second;
}

map<int, csvRoleUpgrade_s*> *CsvRoleUpgrade::GetAll() 
{
	return &RoleUpgradeMap;
}

