#include "CardAtkPConfig.h"
#include "CsvCommon.h"

using namespace std;
using namespace cocos2d;

CsvCardAtkPConfig::CsvCardAtkPConfig() 
{
}

CsvCardAtkPConfig::~CsvCardAtkPConfig() 
{
	for ( map<int, csvCardAtkPConfig_s*>::iterator it = CardAtkPConfigMap.begin(); it != CardAtkPConfigMap.end(); it++ ) {
		delete it->second;
	}
}

void CsvCardAtkPConfig::SetData( vector<string> &str ) 
{
	csvCardAtkPConfig_s   *item = new (std::nothrow) csvCardAtkPConfig_s;
	counter = 0;

	item->id = StringToInt( str[counter]);
	counter++; 
	item->CardType = StringToInt( str[counter]);
	counter++; 
	item->CardId = StringToFloat( str[counter] );
	counter++; 
	item->CardAtkCoe1 = StringToFloat( str[counter] );
	counter++; 
	item->CardAtkCoe2 = StringToFloat( str[counter] );
	counter++; 
	item->islock = StringToInt( str[counter]);
	counter++; 

	item->star1 = StringToInt( str[counter] );
	counter++; 
	item->star2 = StringToInt( str[counter] );
	counter++; 
	item->star3 = StringToInt( str[counter] );
	counter++; 
	item->star4 = StringToInt( str[counter] );
	counter++; 
	item->star5 = StringToInt( str[counter] );
	counter++;
	item->quickStoneId = StringToInt(str[counter]);
	counter++;

	CardAtkPConfigMap.insert( pair<int,csvCardAtkPConfig_s*>( atoi( str[0].c_str() ), item ) ); 
}

csvCardAtkPConfig_s *CsvCardAtkPConfig::GetByID( int id ) 
{
	map<int, csvCardAtkPConfig_s*>::iterator	it = CardAtkPConfigMap.find( id ); 
	if( it == CardAtkPConfigMap.end() ){ return NULL; } 
	return it->second;
}

map<int, csvCardAtkPConfig_s*> *CsvCardAtkPConfig::GetAll() 
{
	return &CardAtkPConfigMap;
}

