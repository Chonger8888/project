#include "GameSystem.h"
#include "../CsvMgr/CsvMgr.h"
#include "JniHelp/CppCallJava.h"
#include "NewDataMgr.h"
#include "GameUI/CommonWidget.h"

GameTaskSystem::GameTaskSystem()
	:_sqlitedb(nullptr)
{
}

GameTaskSystem::~GameTaskSystem()
{

}

void GameTaskSystem::initGameProperty( void )
{
	std::string sql;
	int result;
	char *zErrorMsg = 0;

	CCASSERT(_sqlitedb, "sqlitedb==null");

	// 查询表字段
	sql = "select data0 from gamePro_Table";
	result=sqlite3_exec(_sqlitedb,sql.c_str(),NULL,NULL,&zErrorMsg);

	// 表不存在
	if ( result != SQLITE_OK )
	{
		//	CCASSERT( result == SQLITE_OK, "roleinfo table not existes");
		sql.clear();
		sql="create table if not exists gamePro_Table(";

		for ( int i=GamePrKey_killMonNum; i<GamePrKey_Max; i++)
		{
			sql.append( "data" );
			sql.append(  __String::createWithFormat("%d", i)->getCString() );
			sql.append( " integer");

			if ( i != GamePrKey_Max-1)
			{
				sql.append( ",");
			}

		}
		sql.append( ")");

		result=sqlite3_exec(_sqlitedb,sql.c_str(),NULL,NULL,&zErrorMsg);

		if(result == SQLITE_OK)   
		{
			// 插入数据 
			sql.clear();
			sql = sql.append( "insert into gamePro_Table values (" );
			for ( int i=GamePrKey_killMonNum; i<GamePrKey_Max; i++)
			{
				sql.append( "1" );

				if ( i != GamePrKey_Max-1)
				{
					sql.append( ", ");
				}

			}
			sql.append( ")");				

			result=sqlite3_exec(_sqlitedb,sql.c_str(),NULL,NULL,&zErrorMsg);

			if(result!=SQLITE_OK)
				CCLOG("insert err");

		}
	}

	// 插入数据 
	sql.clear();
	sql = sql.append( "select " );
	for ( int i=GamePrKey_killMonNum; i<GamePrKey_Max; i++)
	{
		sql.append( "data" );
		sql.append(  __String::createWithFormat("%d", i)->getCString() );

		if ( i != GamePrKey_Max-1)
		{
			sql.append( ",");
		}

	}
	sql.append( " from gamePro_Table");		

	sqlite3_stmt *statement;
	if ( sqlite3_prepare_v2(_sqlitedb, sql.c_str(), -1, &statement, nullptr) == SQLITE_OK )
	{
		// 初始化列表
		while (sqlite3_step(statement) == SQLITE_ROW) 
		{		
			for ( int i=GamePrKey_killMonNum; i<GamePrKey_Max; i++)
			{
				_GamePropertyArr[i]			= sqlite3_column_int(statement, i);
			}
		}
	}
	sqlite3_finalize(statement);

}


void GameTaskSystem::initGameTask( sqlite3* db )
{
	_sqlitedb = db;
	
	initGameProperty();
	initDayTaskTable();

	int today = NewDataMgr::getInstance()->convertTimeStrToNumber(CppCallJava::getInstance()->getSystemTime(), SystemTime_Day);
	int history = getTaskDate();

	// 当前天日期与历史日期对比

 	if ( today != history || _dayTaskList.empty())
	{
		setTaskDate( today );

		// 重新生成任务表
		cleanTaskProperty();
		createDayTaskList();
	}

}

void GameTaskSystem::initDayTaskTable( void )
{
	std::string sql;
	int result;
	char *zErrorMsg = 0;

	CCASSERT(_sqlitedb, "sqlitedb==null");
	
	// 查询表字段
	sql = "select data0 from dayTask_Table";
	result=sqlite3_exec(_sqlitedb,sql.c_str(),NULL,NULL,&zErrorMsg);

	// 表不存在
	if ( result != SQLITE_OK )
	{
		//	CCASSERT( result == SQLITE_OK, "roleinfo table not existes");
		sql.clear();
		sql="create table if not exists dayTask_Table("\
			"taskid integer"\
			",isFinish integer"\
			")";


		result=sqlite3_exec(_sqlitedb,sql.c_str(),NULL,NULL,&zErrorMsg);

		if(result == SQLITE_OK)   
		{
						
		}
	}

	// 查询 
	sql.clear();
	sql = sql.append( "select  taskid, isFinish from dayTask_Table");		

	sqlite3_stmt *statement;
	if ( sqlite3_prepare_v2(_sqlitedb, sql.c_str(), -1, &statement, nullptr) == SQLITE_OK )
	{
		// 初始化列表
		while (sqlite3_step(statement) == SQLITE_ROW) 
		{		
			int taskid			= sqlite3_column_int(statement, 0);
			int taskIsFinish	= sqlite3_column_int(statement, 1);

			_dayTaskList.push_back( taskid );
			auto taskItem = NewDataMgr::getInstance()->mGameTaskSystem->getGameTaskListWithID(taskid);
			taskItem->TaskState = taskIsFinish;
			
		}
	}
	sqlite3_finalize(statement);

}

void GameTaskSystem::saveGamePropertyData( void )
{
	std::string sql;
	int result;
	char *zErrorMsg = 0;

	CCASSERT(_sqlitedb, "sqlitedb==null");

	// 清空表
	sql = "delete from gamePro_Table";
	result=sqlite3_exec(_sqlitedb,sql.c_str(),NULL,NULL,&zErrorMsg);


	// 插入数据 
	sql.clear();
	sql = sql.append( "insert into gamePro_Table values (" );
	for ( int i=GamePrKey_killMonNum; i<GamePrKey_Max; i++)
	{
		sql.append( __String::createWithFormat("%d", _GamePropertyArr[i])->getCString() );

		if ( i != GamePrKey_Max-1)
		{
			sql.append( ", ");
		}

	}
	sql.append( ")");				

	result=sqlite3_exec(_sqlitedb,sql.c_str(),NULL,NULL,&zErrorMsg);

	if(result!=SQLITE_OK)
		CCLOG("insert err");

}

void GameTaskSystem::saveDayTaskData( void )
{
	std::string sql;
	int result;
	char *zErrorMsg = 0;

	CCASSERT(_sqlitedb, "sqlitedb==null");

	// 清空表
	sql = "delete from dayTask_Table";
	result=sqlite3_exec(_sqlitedb,sql.c_str(),NULL,NULL,&zErrorMsg);

	auto item = _dayTaskList.begin();
	// 插入数据 
	sql.clear();
	sql = sql.append( "insert into dayTask_Table values " );
	for ( int i=0; i<_dayTaskList.size(); i++)
	{
		int taskid = (*item);item++;
		int taskIsFinish = getGameTaskListWithID( taskid )->TaskState;
		sql.append( __String::createWithFormat("(%d, %d)",taskid, taskIsFinish)->getCString() );

		if ( i != _dayTaskList.size()-1)
		{
			sql.append( ", ");
		}

	}

	result=sqlite3_exec(_sqlitedb,sql.c_str(),NULL,NULL,&zErrorMsg);

	if(result!=SQLITE_OK)
		CCLOG("insert err");
}

void GameTaskSystem::createDayTaskList()
{
	_dayTaskList.clear();
	// 任务随机
	for ( int i = 1; i<=TaskSystem_DayTaskNum; i++ )
	{
		// 随机ID
		int randID = random(1, (int)DayTaskTag_Max-1);

		while(1)
		{
			if(std::find(_dayTaskList.begin(), _dayTaskList.end(), randID) != _dayTaskList.end())
			{
				randID = random(1, (int)DayTaskTag_Max-1);
			}
			else
			{
				_dayTaskList.push_back( randID );
				break;
			}
		}
	}
}

std::vector<int> GameTaskSystem::getDayTaskList( void )
{
	updateDayTaskData();
	return _dayTaskList;
}

csvGameTaskConfig_s* GameTaskSystem::getGameTaskListWithID( int id )
{
	return CSV_MGR()->GetCsvGameTaskConfig()->GetByID( id );
}

void GameTaskSystem::setGameTaskProperty( int *data )
{
	_GamePropertyArr[GamePrKey_killMonNum]			+= data[GamePrKey_killMonNum]		;
	_GamePropertyArr[GamePrKey_BGradeNum]			+= data[GamePrKey_BGradeNum]		;
	_GamePropertyArr[GamePrKey_AGradeNum]			+= data[GamePrKey_AGradeNum]		;			
	_GamePropertyArr[GamePrKey_SGradeNum]			+= data[GamePrKey_SGradeNum]		;			
	_GamePropertyArr[GamePrKey_SSGradeNum]			+= data[GamePrKey_SSGradeNum]		;		
	_GamePropertyArr[GamePrKey_getRedBallNum]		+= data[GamePrKey_getRedBallNum]	;		
	_GamePropertyArr[GamePrKey_getBlueBallNum]		+= data[GamePrKey_getBlueBallNum]	;	
	_GamePropertyArr[GamePrKey_getYellowBallNum]	+= data[GamePrKey_getYellowBallNum]	;	
	_GamePropertyArr[GamePrKey_RedKillerNum]		+= data[GamePrKey_RedKillerNum]		;	
	_GamePropertyArr[GamePrKey_BlueKillerNum]		+= data[GamePrKey_BlueKillerNum]	;	
	_GamePropertyArr[GamePrKey_YellowKillerNum]		+= data[GamePrKey_YellowKillerNum]	;	
	_GamePropertyArr[GamePrKey_minKillBossTime]		= data[GamePrKey_minKillBossTime]	;	
	_GamePropertyArr[GamePrKey_RedScore]			+= data[GamePrKey_RedScore]			;
	_GamePropertyArr[GamePrKey_BlueScore]			+= data[GamePrKey_BlueScore]		;
	_GamePropertyArr[GamePrKey_YellowScore]			+= data[GamePrKey_YellowScore]		;

	_GamePropertyArr[GamePrKey_totalKm]				= (_GamePropertyArr[GamePrKey_totalKm] > data[GamePrKey_totalKm])?_GamePropertyArr[GamePrKey_totalKm] : data[GamePrKey_totalKm];
}

void GameTaskSystem::cleanTaskProperty( void )
{
	for ( int i = GamePrKey_killMonNum; i<GamePrKey_Max; i++)
	{
		_GamePropertyArr[i] = 0;
	}
}

void GameTaskSystem::updateDayTaskData( void )
{
	for ( std::vector<int>::iterator item = _dayTaskList.begin() ; item != _dayTaskList.end(); item++ )
	{
		int taskId = (*item);
		auto tempTask = CSV_MGR()->GetCsvGameTaskConfig()->GetByID( taskId );

		switch ( taskId )
		{
		case DayTaskTag_Task1:
		case DayTaskTag_Task8:
			{
				tempTask->TaskProgressNum = _GamePropertyArr[GamePrKey_killMonNum];
				if ( tempTask->TaskProgressNum >= tempTask->TaskTargetNum )
				{
					if (0 == tempTask->TaskState) 
						tempTask->TaskState = 1;
				}
				else
				{
					tempTask->TaskState = 0;
				}
			}
			break;

		case DayTaskTag_Task2:
			{
				tempTask->TaskProgressNum = _GamePropertyArr[GamePrKey_SGradeNum];
				if ( tempTask->TaskProgressNum >= tempTask->TaskTargetNum )
				{
					if (0 == tempTask->TaskState) 
						tempTask->TaskState = 1;
				}
				else
				{
					tempTask->TaskState = 0;
				}
			}
			break;

		case DayTaskTag_Task3:
			{
				tempTask->TaskProgressNum = _GamePropertyArr[GamePrKey_getRedBallNum];
				if ( tempTask->TaskProgressNum >= tempTask->TaskTargetNum )
				{
					if (0 == tempTask->TaskState) 
						tempTask->TaskState = 1;
				}
				else
				{
					tempTask->TaskState = 0;
				}
			}
			break;
		case DayTaskTag_Task4:
			{
				tempTask->TaskProgressNum = _GamePropertyArr[GamePrKey_getBlueBallNum];
				if ( tempTask->TaskProgressNum >= tempTask->TaskTargetNum )
				{
					if (0 == tempTask->TaskState) 
						tempTask->TaskState = 1;
				}
				else
				{
					tempTask->TaskState = 0;
				}
			}
			break;
		case DayTaskTag_Task5:
			{
				tempTask->TaskProgressNum = _GamePropertyArr[GamePrKey_getYellowBallNum];
				if ( tempTask->TaskProgressNum >= tempTask->TaskTargetNum )
				{
					if (0 == tempTask->TaskState) 
						tempTask->TaskState = 1;
				}
				else
				{
					tempTask->TaskState = 0;
				}
			}
			break;
		case DayTaskTag_Task6:
			{
				tempTask->TaskProgressNum = _GamePropertyArr[GamePrKey_SSGradeNum];
				if ( tempTask->TaskProgressNum >= tempTask->TaskTargetNum )
				{
					if (0 == tempTask->TaskState) 
						tempTask->TaskState = 1;
				}
				else
				{
					tempTask->TaskState = 0;
				}
			}
			break;
		case DayTaskTag_Task7:
			{
				tempTask->TaskProgressNum = _GamePropertyArr[GamePrKey_AGradeNum];
				if ( tempTask->TaskProgressNum >= tempTask->TaskTargetNum )
				{
					if (0 == tempTask->TaskState) 
						tempTask->TaskState = 1;
				}
				else
				{
					tempTask->TaskState = 0;
				}
			}
			break;
		case DayTaskTag_Task10:
		case DayTaskTag_Task9:
			{
				if ( DayTaskTag_Task10 == taskId )	
					tempTask->TaskProgressNum = ( 16 <= _GamePropertyArr[GamePrKey_totalKm] )?1:0;
				else if ( DayTaskTag_Task9 == taskId )	
					tempTask->TaskProgressNum = ( 18 <= _GamePropertyArr[GamePrKey_totalKm] )?1:0;

				if ( tempTask->TaskProgressNum >= tempTask->TaskTargetNum )
				{
					if (0 == tempTask->TaskState) 
						tempTask->TaskState = 1;
				}
				else
				{
					tempTask->TaskState = 0;
				}
			}
			break;
		case DayTaskTag_Task11:
			{
				tempTask->TaskProgressNum = ( 15 >= _GamePropertyArr[GamePrKey_minKillBossTime] && 0 != _GamePropertyArr[GamePrKey_minKillBossTime])?1:0;
				if ( tempTask->TaskProgressNum )
				{
					if (0 == tempTask->TaskState) 
						tempTask->TaskState = 1;
				}
				else
				{
					tempTask->TaskState = 0;
				}
			}
			break;
		default:
			break;
		}
	}
}

int GameTaskSystem::getTaskDate( void )
{
	return UserDefault::getInstance()->getIntegerForKey("TaskDate", 0);
}

void GameTaskSystem::setTaskDate( int date )
{
	UserDefault::getInstance()->setIntegerForKey( "TaskDate", date );
}

void GameTaskSystem::saveGameAboutData( void )
{
	saveGamePropertyData();
	saveDayTaskData();
}

/*******************************************
* 物品购物系统
*******************************************/

GameBuySystem::GameBuySystem()
{
	
}

GameBuySystem::~GameBuySystem()
{

}

void GameBuySystem::initBuyData( sqlite3* db )
{
	_sqlitedb = db;

	initGoodsData( db );
}

void GameBuySystem::initGoodsData( sqlite3* db )
{
	std::string sql;
	int result;
	char *zErrorMsg = 0;

	CCASSERT(db, "sqlitedb==null");

	// 查询表字段
	sql = "select data1 from initGoods_Table";
	result=sqlite3_exec(db,sql.c_str(),NULL,NULL,&zErrorMsg);

	// 表不存在
	if ( result != SQLITE_OK )
	{
		//	CCASSERT( result == SQLITE_OK, "roleinfo table not existes");

		// 建表
		sql.clear();
		sql="create table if not exists initGoods_Table(";

		for ( int i=BuyKey_Dazhao; i<BuyKey_Max; i++)
		{
			sql.append( "data" );
			sql.append(  __String::createWithFormat("%d", i)->getCString() );
			sql.append( " integer");

			if ( i != BuyKey_Max-1)
			{
				sql.append( ",");
			}

		}
		sql.append( ")");

		result=sqlite3_exec(db,sql.c_str(),NULL,NULL,&zErrorMsg);

		if(result == SQLITE_OK)   
		{
			// 插入数据 
			sql.clear();
			sql = sql.append( "insert into initGoods_Table values (" );
			for ( int i=BuyKey_Dazhao; i<BuyKey_Max; i++)
			{
				sql.append( "0" );

				if ( i != BuyKey_Max-1)
				{
					sql.append( ", ");
				}

			}
			sql.append( ")");				

			result=sqlite3_exec(db,sql.c_str(),NULL,NULL,&zErrorMsg);

			if(result!=SQLITE_OK)
				CCLOG("insert err");

		}		
	}


	// 查询数据 
	sql.clear();
	sql = sql.append( "select " );
	for ( int i=BuyKey_Dazhao; i<BuyKey_Max; i++)
	{
		sql.append( "data" );
		sql.append(  __String::createWithFormat("%d", i)->getCString() );

		if ( i != BuyKey_Max-1)
		{
			sql.append( ",");
		}

	}
	sql.append( " from initGoods_Table");		

	sqlite3_stmt *statement;
	if ( sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, nullptr) == SQLITE_OK )
	{
		// 初始化列表
		while (sqlite3_step(statement) == SQLITE_ROW) 
		{		
			for ( int i=BuyKey_Dazhao; i<BuyKey_Max; i++)
			{
				int num			= sqlite3_column_int(statement, i-1);
				getBuyGoodsDataWithKey((BuyListKey )i)->number = num;
			}
		}
	}
	sqlite3_finalize(statement);
}

void GameBuySystem::addGoodsNumWithKey( BuyListKey key, int num )
{
	CSV_MGR()->GetCsvBuyList()->GetByID( (int)key )->number += num;
	updateGoodsNumConfig( key );
}

void GameBuySystem::updateGoodsNumConfig( BuyListKey key )
{
	std::string sql;
	sql.append("update initGoods_Table set  ");

	sql.append("data");
	sql.append(__String::createWithFormat("%d= %d ",key, CSV_MGR()->GetCsvBuyList()->GetByID( (int)key )->number)->getCString());

	if ( _sqlitedb )
	{
		int result;
		char* zErrorMsg = 0;

		result=sqlite3_exec(_sqlitedb,sql.c_str(),NULL,NULL,&zErrorMsg);
		if(result!=SQLITE_OK)
			CCLOG("insert err");
	}
}

csvBuyList_s* GameBuySystem::getBuyGoodsDataWithKey( BuyListKey key )
{
	return CSV_MGR()->GetCsvBuyList()->GetByID( (int)key );
}

bool GameBuySystem::buyGoodsWithKey( BuyListKey key , int num /*= 1*/)
{
	int myGold = NewDataMgr::getInstance()->getGold();
	int myGem = NewDataMgr::getInstance()->getGem();

	auto item = getBuyGoodsDataWithKey( key );
	int price = item->Price*num;
	if ( BuyType_Gold == item->buyType )
	{
		if ( price > myGold )
		{
			showBuyTips( BuyType_Gold );
			return false;
		}
		else
		{
			NewDataMgr::getInstance()->addGold( -price );
			addGoodsNumWithKey(key, num);
		}
	}
	else if ( BuyType_Gem == item->buyType )
	{
		if ( price > myGem )
		{
			showBuyTips( BuyType_Gem );
			return false;
		}
		else
		{
			NewDataMgr::getInstance()->addGem( -price );
			addGoodsNumWithKey(key, num);
		}
	}

	return true;
}

bool GameBuySystem::buyGoodsWithPrice(int goodsprice)
{
	int myGold = NewDataMgr::getInstance()->getGold();
	int myGem = NewDataMgr::getInstance()->getGem();

	int buyType = BuyType_Gold;
	int price = goodsprice;
	if (BuyType_Gold == buyType)
	{
		if (price > myGold)
		{
			showBuyTips(BuyType_Gold);
			return false;
		}
		else
		{
			NewDataMgr::getInstance()->addGold(-price);
		}
	}
	else if (BuyType_Gem == buyType)
	{
		if (price > myGem)
		{
			showBuyTips(BuyType_Gem);
			return false;
		}
		else
		{
			NewDataMgr::getInstance()->addGem(-price);
		}
	}

	return true;
}

void GameBuySystem::showBuyTips( BuyType type )
{
	auto widget = CommonWidget::create();
	if ( BuyType_Gem == type )
		widget->setContent( "宝石不足" );
	else if ( BuyType_Gold == type )
		widget->setContent( "金币不足" );

	Director::getInstance()->getRunningScene()->addChild( widget );
}



/*******************************************
* 角色系统
*******************************************/
void GameRoleSystem::initRoleInfo( sqlite3* db )
{
	for ( int i = 0; i<3; i++ )
	{
		_roleConfig[i] = CSV_MGR()->GetCsvRoleUpgrade()->GetByID( i+1 );
	}

	std::string sql;
	int result;
	char *zErrorMsg = 0;

	// 查询表字段
	sql = "select type from roleInfo_Table";
	result=sqlite3_exec(db,sql.c_str(),NULL,NULL,&zErrorMsg);

	// 表不存在
	if ( result != SQLITE_OK )
	{
		//	CCASSERT( result == SQLITE_OK, "roleinfo table not existes");
		sql="create table if not exists roleInfo_Table("\
			"type integer"\
			",roleGrade integer"\
			",isunlock integer"\
			",roleExp integer"\
			",isOption integer"
			")";

		result=sqlite3_exec(db,sql.c_str(),NULL,NULL,&zErrorMsg);

		if(result == SQLITE_OK)   
		{
			for ( int i =0; i<3; i++ )
			{
				// 插入数据 用户基础数据
				sql.clear();
				sql = __String::createWithFormat("insert into roleInfo_Table values( %d, %d, %d, %d, %d )",
					_roleConfig[i]->roleId,
					_roleConfig[i]->roleGrade,
					_roleConfig[i]->isLock,
					_roleConfig[i]->CurExp,
					_roleConfig[i]->isOption)->getCString();

				result=sqlite3_exec(db,sql.c_str(),NULL,NULL,&zErrorMsg);

				if(result!=SQLITE_OK)
					CCLOG("insert err");
			}
		}
	}

	// 查询语句
	sql = " select type, roleGrade, isunlock, roleExp, isOption from roleInfo_Table";
	sqlite3_stmt *statement;
	if ( sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, nullptr) == SQLITE_OK )
	{
		// 初始化角色列表
		while (sqlite3_step(statement) == SQLITE_ROW) 
		{
			// 获得解锁状态
			int id			= sqlite3_column_int(statement, 0);
			int grade		= sqlite3_column_int(statement, 1);
			int islock		= sqlite3_column_int(statement, 2);
			int exp			= sqlite3_column_int(statement, 3);
			int isOption	= sqlite3_column_int(statement, 4);

			_roleConfig[id-1]->roleGrade	= grade;
			_roleConfig[id-1]->CurExp		= exp;
			_roleConfig[id-1]->isLock		= islock;
			_roleConfig[id-1]->isOption		= isOption;

			if ( 1 == isOption )
			{
//				mRoleConfig.roleIndex = id;
			}
		}
	}

	sqlite3_finalize(statement);
}



/*******************************************
* 灵石系统
*******************************************/
void QuickStoneSystem::initQuickStoneSystem(sqlite3* db)
{
	_sqlitedb = db;

	initQuickStoneData();
}

void QuickStoneSystem::initQuickStoneData(void)
{
	std::string sql;
	int result;
	char *zErrorMsg = 0;

	CCASSERT(_sqlitedb, "sqlitedb==null");

	// 查询表字段
	sql = "select data1 from QuiteStone_Table";
	result = sqlite3_exec(_sqlitedb, sql.c_str(), NULL, NULL, &zErrorMsg);

	// 表不存在
	if (result != SQLITE_OK)
	{
		// 建表
		sql.clear();
		sql = "create table if not exists QuiteStone_Table(";

		for (int i = QuickStoneKey_F; i < QuickStoneKey_Max; i++)
		{
			sql.append("data");
			sql.append(__String::createWithFormat("%d", i)->getCString());
			sql.append(" integer");

			if (i != QuickStoneKey_Max - 1)
			{
				sql.append(",");
			}

		}
		sql.append(")");

		result = sqlite3_exec(_sqlitedb, sql.c_str(), NULL, NULL, &zErrorMsg);

		if (result == SQLITE_OK)
		{
			// 插入数据 
			sql.clear();
			sql = sql.append("insert into QuiteStone_Table values (");
			for (int i = QuickStoneKey_F; i < QuickStoneKey_Max; i++)
			{
				sql.append( StringUtils::format("%d", getQuickStoneDataWithKey((QuickStoneKey)i)->haveNum));

				if (i != QuickStoneKey_Max - 1)
				{
					sql.append(", ");
				}

			}
			sql.append(")");

			result = sqlite3_exec(_sqlitedb, sql.c_str(), NULL, NULL, &zErrorMsg);

			if (result != SQLITE_OK)
				CCLOG("insert err");

		}
	}


	// 查询数据 
	sql.clear();
	sql = sql.append("select ");
	for (int i = QuickStoneKey_F; i < QuickStoneKey_Max; i++)
	{
		sql.append("data");
		sql.append(__String::createWithFormat("%d", i)->getCString());

		if (i != QuickStoneKey_Max - 1)
		{
			sql.append(",");
		}

	}
	sql.append(" from QuiteStone_Table");

	sqlite3_stmt *statement;
	if (sqlite3_prepare_v2(_sqlitedb, sql.c_str(), -1, &statement, nullptr) == SQLITE_OK)
	{
		// 初始化列表
		while (sqlite3_step(statement) == SQLITE_ROW)
		{
			for (int i = QuickStoneKey_F; i < QuickStoneKey_Max; i++)
			{
				int num = sqlite3_column_int(statement, i - 1);
				getQuickStoneDataWithKey((QuickStoneKey)i)->haveNum = num;
			}
		}
	}
	sqlite3_finalize(statement);
}

csvQuickStoneTable_s* QuickStoneSystem::getQuickStoneDataWithKey(QuickStoneKey key)
{
	return CSV_MGR()->GetCsvQuickStoneTable()->GetByID(key);
}

void QuickStoneSystem::compountQuickStone(QuickStoneKey key)
{
	int needNum = getQuickStoneDataWithKey(key)->compoundNeedNum;

	getQuickStoneDataWithKey(QuickStoneKey(key - 1))->haveNum -= needNum;
	getQuickStoneDataWithKey(key)->haveNum += 1;

	updateQuickStoneData(key);
	updateQuickStoneData((QuickStoneKey)(key - 1));
}

void QuickStoneSystem::implantQuickStone(int cardType, QuickStoneKey key)
{
	getQuickStoneDataWithKey(key)->haveNum -= 1;
	NewDataMgr::getInstance()->setCardImplantQuickStone(cardType, key);
}

void QuickStoneSystem::updateQuickStoneData(QuickStoneKey key)
{
	std::string sql;
	sql.append("update QuiteStone_Table set ");

	sql.append(__String::createWithFormat("data%d = %d", key, getQuickStoneDataWithKey(key)->haveNum)->getCString());
	
	if (_sqlitedb)
	{
		int result;
		char* zErrorMsg = 0;

		result = sqlite3_exec(_sqlitedb, sql.c_str(), NULL, NULL, &zErrorMsg);
		if (result != SQLITE_OK)
			CCLOG("insert err");
	}
}
