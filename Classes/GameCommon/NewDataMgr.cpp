#include "NewDataMgr.h"
#include "NewAudio.h"
#include "JniHelp/CppCallJava.h"
#include "GameDefine.h"

//#ifdef WIN32
//#pragma comment(lib, "E:\\Cocos_Project\\cocos2d-x-3.3\\project\\PlaneGame2\\proj.win32\\sqlite3.lib")
//#endif

SINGLETONDATA_DEFINE(NewDataMgr);

#define File_DataBase_Name		"weme.db"


NewDataMgr::NewDataMgr()
	:_sqlitedb(nullptr)
	,mGameTaskSystem(nullptr)
	,mGameBuySystem(nullptr)
	, mQuickStoneSystem(false)
	, mRoleUpgradeIng(false)
{
	mRoleConfig.roleIndex	= 1;

	mUserData.Gold = 0;


}

NewDataMgr::~NewDataMgr()
{
	//sqlite3_close( _sqlitedb );
	
}

void NewDataMgr::initGameData( void )
{
	// 数据库
	std::string path= FileUtils::getInstance()->getWritablePath() + File_DataBase_Name;
	std::string sql;
	int result;

	if ( !_sqlitedb )
	{
		result = sqlite3_open(path.c_str(),&_sqlitedb);
	}

	if ( !mGameTaskSystem )
	{
		mGameTaskSystem = new GameTaskSystem();
		mGameTaskSystem->initGameTask( _sqlitedb );
	}

	if ( !mGameBuySystem )
	{
		mGameBuySystem = new GameBuySystem();
		mGameBuySystem->initBuyData( _sqlitedb );
	}

	if (!mQuickStoneSystem)
	{
		mQuickStoneSystem = new QuickStoneSystem();
		mQuickStoneSystem->initQuickStoneSystem(_sqlitedb);
	}

	initUserData(	_sqlitedb );
	initRoleInfo(	_sqlitedb );
	initCardInfo(	_sqlitedb );
	initCardOption( _sqlitedb );
//	initGoodsData(	_sqlitedb );

	// 音乐
#ifdef WIN32
	NewAudio::getInstance()->setMusicState( false );
	NewAudio::getInstance()->setSoundEffState( false );
#else
	NewAudio::getInstance()->setMusicState( true );
	NewAudio::getInstance()->setSoundEffState( true );
#endif

	// 观察者消息
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(NewDataMgr::MsgProConsumeLife), Msg_ConsumeLife, nullptr);

}

void NewDataMgr::initUserData( sqlite3* db )
{
	std::string sql;
	int result;
	char *zErrorMsg = 0;

	// 查询表字段
	sql = "select gold from userData_Table";
	result=sqlite3_exec(db,sql.c_str(),NULL,NULL,&zErrorMsg);

	// 表不存在
	if ( result != SQLITE_OK )
	{
		//	CCASSERT( result == SQLITE_OK, "roleinfo table not existes");
		sql="create table if not exists userData_Table("\
			"newUser integer"\
			",life integer"\
			",gold integer"\
			",gem integer"\
			")";

		result=sqlite3_exec(db,sql.c_str(),NULL,NULL,&zErrorMsg);

		if(result == SQLITE_OK)   
		{
			// 插入数据 用户基础数据
			sql = "insert into userData_Table values ( 0, 5, 0, 0 )";

			result=sqlite3_exec(db,sql.c_str(),NULL,NULL,NULL);

			if(result!=SQLITE_OK)
				CCLOG("insert err");
			
		}
	}

	// 查询语句
	sql = " select newUser ,life ,gold ,gem from userData_Table";
	sqlite3_stmt *statement;
	if ( sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, nullptr) == SQLITE_OK )
	{
		// 初始化角色列表
		while (sqlite3_step(statement) == SQLITE_ROW) 
		{
			mUserData.IsNewUser		= sqlite3_column_int(statement, 0);
			mUserData.LifeNum		= sqlite3_column_int(statement, 1);
			mUserData.Gold			= sqlite3_column_int(statement, 2);
			mUserData.Gem			= sqlite3_column_int(statement, 3);
		}
	}

	sqlite3_finalize(statement);
}

void NewDataMgr::initRoleInfo( sqlite3* db )
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
				mRoleConfig.roleIndex = id;
			}
		}
	}

	sqlite3_finalize(statement);
}

void NewDataMgr::setRoleExpWithIndex( int type )
{
	updateRoleInfoConfig(RoleDateKey_CurExp, type, (void*)_roleConfig[type-1]->CurExp);

}

void NewDataMgr::setRoleGoadeWithIndex( int type  )
{
	updateRoleInfoConfig(RoleDateKey_Grade, type, (void*)_roleConfig[type-1]->roleGrade);
}


void NewDataMgr::setRoleLockWithIndex( int type , bool islock )
{
	_roleConfig[type-1]->isLock = islock;
	updateRoleInfoConfig(RoleDateKey_isLock, type, (void*)islock);
}

void NewDataMgr::updateRoleInfoConfig( RoleDateKey key, int type, void* data )
{
	std::string sql;
	sql.append("update roleInfo_Table set ");

	switch (key)
	{

	case RoleDateKey_isLock:
		sql.append("isunlock=");
		sql.append(__String::createWithFormat("%d ",(intptr_t)data)->getCString());
		break;

	case RoleDateKey_CurExp:
		sql.append("roleExp=");
		sql.append(__String::createWithFormat("%d ",(intptr_t)data)->getCString());
		break;

	case RoleDateKey_Grade:
		sql.append("roleGrade=");
		sql.append(__String::createWithFormat("%d ",(intptr_t)data)->getCString());
		break;

	case RoleDateKey_isOption:
		{
			char* zErrorMsg = 0;
			int result=sqlite3_exec( _sqlitedb, "update roleInfo_Table set isOption=0" ,NULL,NULL,&zErrorMsg);

			if(result!=SQLITE_OK)
				CCLOG("insert err");

			sql.append("isOption=");
			sql.append(__String::createWithFormat("%d ",(intptr_t)data)->getCString());
		}
		break;
	default:
		break;
	}

	sql.append("where type=");
	sql.append(__String::createWithFormat("%d ",type)->getCString());

	if ( _sqlitedb )
	{
		int result;
		char* zErrorMsg = 0;

		result=sqlite3_exec(_sqlitedb,sql.c_str(),NULL,NULL,&zErrorMsg);
		if(result!=SQLITE_OK)
			CCLOG("insert err");
	}
}

void NewDataMgr::initCardInfo( sqlite3* db )
{
	for ( int i = 0; i<GameData_CardNum; i++ )
	{
		_cardAtkPConfig[i] = CSV_MGR()->GetCsvCardAtkPConfig()->GetByID( i );
	}

 	std::string sql;
	int result;
	char *zErrorMsg = 0;

	// 查询表字段
	sql = "select id from cardInfo_Table";
	result=sqlite3_exec(db,sql.c_str(),NULL,NULL,&zErrorMsg);

	// 表不存在
	if ( result != SQLITE_OK )
	{
		//	CCASSERT( result == SQLITE_OK, "roleinfo table not existes");
		sql="create table if not exists cardInfo_Table("\
			"id integer"\
			",cardtype integer"\
			",star1 integer"\
			",star2 integer"\
			",star3 integer"\
			",star4 integer"\
			",star5 integer"\
			",quickstoneid integer"\
			")";

		result=sqlite3_exec(db,sql.c_str(),NULL,NULL,&zErrorMsg);

		if(result == SQLITE_OK)   
		{
			for ( int i =0; i<GameData_CardNum; i++ )
			{
				// 插入数据 用户基础数据
				sql = __String::createWithFormat("insert into cardInfo_Table values (%d, %d, %d, %d, %d ,%d ,%d, %d)",
									_cardAtkPConfig[i]->id, _cardAtkPConfig[i]->CardType, 
									_cardAtkPConfig[i]->star1, 
									_cardAtkPConfig[i]->star2, 
									_cardAtkPConfig[i]->star3,  
									_cardAtkPConfig[i]->star4, 
									_cardAtkPConfig[i]->star5,
									_cardAtkPConfig[i]->quickStoneId
									)->getCString();

				result=sqlite3_exec(db,sql.c_str(),NULL,NULL,&zErrorMsg);

				if(result!=SQLITE_OK)
					CCLOG("insert err");
			}
		}
	}

	// 查询语句
	sql = " select id, cardtype ,star1, star2, star3, star4, star5, quickstoneid from cardInfo_Table";
	sqlite3_stmt *statement;
	if ( sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, nullptr) == SQLITE_OK )
	{
		// 初始化角色列表
		while (sqlite3_step(statement) == SQLITE_ROW) 
		{
			// 获得星级卡牌数
			int id			= sqlite3_column_int(statement, 0);
			_cardAtkPConfig[id]->star1 = sqlite3_column_int(statement, 2);
			_cardAtkPConfig[id]->star2 = sqlite3_column_int(statement, 3);
			_cardAtkPConfig[id]->star3 = sqlite3_column_int(statement, 4);
			_cardAtkPConfig[id]->star4 = sqlite3_column_int(statement, 5);
			_cardAtkPConfig[id]->star5 = sqlite3_column_int(statement, 6);

			_cardAtkPConfig[id]->quickStoneId = sqlite3_column_int(statement, 7);
		}
	}

	sqlite3_finalize(statement);

}

csvCardAtkPConfig_s* NewDataMgr::getCardConfigWithId(int cardId)
{
	return CSV_MGR()->GetCsvCardAtkPConfig()->GetByID(cardId);
}

void NewDataMgr::addCardNumWithStar( int id,int starlv,  int num )
{
	int curNum = 0;
	switch ( starlv )
	{
	case 1:
		_cardAtkPConfig[id]->star1 += num;
		curNum = _cardAtkPConfig[id]->star1;
		break;
	case 2:
		_cardAtkPConfig[id]->star2 += num;
		curNum = _cardAtkPConfig[id]->star2;
		break;
	case 3:
		_cardAtkPConfig[id]->star3 += num;
		curNum = _cardAtkPConfig[id]->star3;
		break;
	case 4:
		_cardAtkPConfig[id]->star4 += num;
		curNum = _cardAtkPConfig[id]->star4;
		break;
	case 5:
		_cardAtkPConfig[id]->star5 += num;
		curNum = _cardAtkPConfig[id]->star5;
		break;
	default:
		break;
	}
	updateCardNumWithStar( id, starlv, (void*)curNum );
}

void NewDataMgr::updateCardNumWithStar( int id, int starLv , void* data )
{

	std::string sql;
	sql.append("update cardInfo_Table set ");

	switch ( starLv )
	{
	case 1:
		sql.append("star1=");
		sql.append(__String::createWithFormat("%d ",(intptr_t)data)->getCString());
		break;
	case 2:
		sql.append("star2=");
		sql.append(__String::createWithFormat("%d ",(intptr_t)data)->getCString());
		break;
	case 3:
		sql.append("star3=");
		sql.append(__String::createWithFormat("%d ",(intptr_t)data)->getCString());
		break;
	case 4:
		sql.append("star4=");
		sql.append(__String::createWithFormat("%d ",(intptr_t)data)->getCString());
		break;
	case 5:
		sql.append("star5=");
		sql.append(__String::createWithFormat("%d ",(intptr_t)data)->getCString());
		break;
	default:
		break;
	}
	 
	sql.append("where id=");
	sql.append(__String::createWithFormat("%d ",id)->getCString());

	if ( _sqlitedb )
	{
		int result;
		char* zErrorMsg = 0;

		result=sqlite3_exec(_sqlitedb,sql.c_str(),NULL,NULL,&zErrorMsg);
		if(result!=SQLITE_OK)
			CCLOG("insert err");
	}

}

int NewDataMgr::getCardMaxStarLvWithId( int id )
{
	int cardNum = 0;

	if ( _cardAtkPConfig[id]->star1 )
		cardNum = 1;
	
	if( _cardAtkPConfig[id]->star2 )
		cardNum = 2;
	
	if( _cardAtkPConfig[id]->star3 )
		cardNum = 3;
	
	if( _cardAtkPConfig[id]->star4 )
		cardNum = 4;
	
	if( _cardAtkPConfig[id]->star5 )
		cardNum = 5;

	return cardNum;
}

int NewDataMgr::getCardNumWithStarLv( int id, int starLv )
{
	//int id = (type -1)*GameData_CardArr+(index-1);
	int cardNum = 0;

	switch ( starLv )
	{
	case 1:
		cardNum = _cardAtkPConfig[id]->star1;
		break;

	case 2:
		cardNum = _cardAtkPConfig[id]->star2;
		break;

	case 3:
		cardNum = _cardAtkPConfig[id]->star3;
		break;

	case 4:
		cardNum = _cardAtkPConfig[id]->star4;
		break;

	case 5:
		cardNum = _cardAtkPConfig[id]->star5;
		break;
	default:
		break;
	}

	return cardNum;
}

float NewDataMgr::getCardAtk( int id, int starlv )
{
	float cardAtk = CardAtkFormula(
		starlv,
		_cardAtkPConfig[id]->CardAtkCoe1,
		_cardAtkPConfig[id]->CardAtkCoe2
		);
	return  cardAtk * 100;
}

void NewDataMgr::initCardOption( sqlite3* db )
{
	std::string sql;
	int result;
	char *zErrorMsg = 0;

	// 查询表字段
	sql = "select type from cardOption_Table";
	result=sqlite3_exec(db,sql.c_str(),NULL,NULL,&zErrorMsg);

	// 表不存在
	if ( result != SQLITE_OK )
	{
		//	CCASSERT( result == SQLITE_OK, "roleinfo table not existes");
		sql="create table if not exists cardOption_Table("\
			"type integer"\
			",cardid integer"\
			",cardStar integer"\
			")";

		result=sqlite3_exec(db,sql.c_str(),NULL,NULL,&zErrorMsg);

		if(result == SQLITE_OK)   
		{
			// 插入数据 
			sql = "insert into cardOption_Table values"\
				"(1, 1, 1),"\
				"(2, 4, 1),"\
				"(3, 7, 1)";

			result=sqlite3_exec(db,sql.c_str(),NULL,NULL,&zErrorMsg);

			if(result!=SQLITE_OK)
			CCLOG("insert err");
			
		}
	}

	// 查询语句
	sql = " select type, cardid , cardStar from cardOption_Table";
	sqlite3_stmt *statement;
	if ( sqlite3_prepare_v2(db, sql.c_str(), -1, &statement, nullptr) == SQLITE_OK )
	{
		// 初始化已选择的Card列表
		while (sqlite3_step(statement) == SQLITE_ROW) 
		{
			int m								= sqlite3_column_int(statement, 0);
			mRoleConfig.cardArr[m-1].cardID		= sqlite3_column_int(statement, 1);
			mRoleConfig.cardArr[m-1].cardStarLv	= sqlite3_column_int(statement, 2);
		}
	}

	sqlite3_finalize(statement);
}

int NewDataMgr::getRoleGradeWithId( int roleId )
{
	return _roleConfig[roleId-1]->roleGrade;
}

int NewDataMgr::getRolePriceWithId( int roleId )
{
	return _roleConfig[roleId-1]->LockPrice;
}

int NewDataMgr::getRoleAutoLockLvWithId( int roleId )
{
	return _roleConfig[roleId-1]->AutoLockGr;
}

void NewDataMgr::addRoleGradeWithId( int roleId )
{
	_roleConfig[roleId-1]->roleGrade += 1;
}

float NewDataMgr::getRoleCurExpWithId( int roleId ,int grade)
{
	return _roleConfig[roleId-1]->CurExp;
}

bool NewDataMgr::addRoleCurExpWithId( int roleId ,int exp )
{
	bool ret = false;

	int curGrade = getRoleGradeWithId(roleId );
	float nextGradeExp = getRoleExpWithId( roleId, curGrade+1);

	int residueExp = 0;

	do 
	{
		if ( 0 >= exp ) return true;

		curGrade = getRoleGradeWithId(roleId );
		nextGradeExp = getRoleExpWithId( roleId, curGrade+1);

		_roleConfig[roleId-1]->CurExp += exp;
		exp = _roleConfig[roleId-1]->CurExp - nextGradeExp;

		bool isFullGrade = false;
		if ( RoleGrade_Max <= curGrade  && _roleConfig[roleId-1]->CurExp >= getRoleExpWithId( roleId, RoleGrade_Max))
		{
			isFullGrade = true;
			_roleConfig[roleId-1]->roleGrade = RoleGrade_Max;
			_roleConfig[roleId-1]->CurExp = getRoleExpWithId( roleId, curGrade);
			return false;;
		}

		if (  _roleConfig[roleId-1]->CurExp >= nextGradeExp && curGrade <= RoleGrade_Max )
		{
			mRoleUpgradeIng = true;
			_roleConfig[roleId-1]->CurExp = 0;
			addRoleGradeWithId( roleId );
		}


	} while (1);

	return true;
}

float NewDataMgr::getRoleCurExpPercentWithId( int roleId ,int grade /*= 0*/ )
{
	int curGrade = getRoleGradeWithId(roleId );
	float curExp = getRoleCurExpWithId(roleId );
	float curGradeNeedExp = 0;//getRoleExpWithId( roleId, curGrade);
	float nextGradeExp = getRoleExpWithId( roleId, (RoleGrade_Max == curGrade)?curGrade : curGrade+1);

	float expPercent = (curExp - curGradeNeedExp) / (nextGradeExp - curGradeNeedExp);

	return expPercent*100;
}

int NewDataMgr::getWeaponNumWithId( int roleId ,int grade)
{
	return RoleWeaponNumFormula( 
		grade, 
		_roleConfig[roleId-1]->roleWeapon, 
		0);
}

float NewDataMgr::getRoleAtkWithId( int roleId ,int grade)
{
	if(roleId == 1)
	{
		return 300;
	}
	else if(roleId == 2)
	{
		return 500;
	}
	else
	{
		return 1000;
	}

/*
	int weaponNum = getWeaponNumWithId( roleId, grade );
	if ( 0 == grade )
		return RoleAtkFormula( 
		_roleConfig[roleId-1]->roleGrade,
		_roleConfig[roleId-1]->BassAtkCoe1,
		_roleConfig[roleId-1]->BassAtkCoe2,
		_roleConfig[roleId-1]->BassAtkCoe3,
		weaponNum);
	else
		return RoleAtkFormula( 
		grade,
		_roleConfig[roleId-1]->BassAtkCoe1,
		_roleConfig[roleId-1]->BassAtkCoe2,
		_roleConfig[roleId-1]->BassAtkCoe3,
		weaponNum);*/
	
}

float NewDataMgr::getRoleDefenseWithId( int roleId ,int grade)
{
	if ( 0 == grade )
		return RoleDefenseFormula( 
		_roleConfig[roleId-1]->roleGrade,
		_roleConfig[roleId-1]->BassDefense,
		_roleConfig[roleId-1]->BassDefenseCoe);
	else
		return RoleDefenseFormula( 
		grade,
		_roleConfig[roleId-1]->BassDefense,
		_roleConfig[roleId-1]->BassDefenseCoe);
}

float NewDataMgr::getRoleLuckWithId( int roleId ,int grade)
{
	float formula=0.0f;
	if ( 0 == grade )
		formula = RoleLuckFormula( 
		_roleConfig[roleId-1]->roleGrade,
		_roleConfig[roleId-1]->BassLuckCoe);
	else
		formula = RoleLuckFormula( 
		grade,
		_roleConfig[roleId-1]->BassLuckCoe);

	return formula * 100 + 0.5f;
}

float NewDataMgr::getRoleExpWithId( int roleId,int grade )
{
	if ( 0 == grade )
	{
		if ( 1 == _roleConfig[roleId-1]->roleGrade ) return 0;
		
		return  RoleExpFormula( 
			_roleConfig[roleId-1]->roleGrade,
			0,
			_roleConfig[roleId-1]->ExpCoe1,
			_roleConfig[roleId-1]->ExpCoe2,
			_roleConfig[roleId-1]->ExpCoe3);
	}
	else
	{
		if ( 1 == grade ) return 0;

		return 
			RoleExpFormula( grade,
			0,
			_roleConfig[roleId-1]->ExpCoe1,
			_roleConfig[roleId-1]->ExpCoe2,
			_roleConfig[roleId-1]->ExpCoe3);
	}
}

float NewDataMgr::getRoleHpWithId( int roleId ,int grade /*= 0*/ )
{
	int roleDefense = getRoleDefenseWithId( roleId, grade );
	if ( 0 == grade )
	{
		return  RoleHpFormula( 
			_roleConfig[roleId-1]->roleGrade,
			_roleConfig[roleId-1]->BassHp,
			_roleConfig[roleId-1]->BassHpCoe1,
			roleDefense,
			_roleConfig[roleId-1]->BassHpCoe2,
			_roleConfig[roleId-1]->BassHpCoe3);
	}
	else
	{
		return 
			RoleHpFormula( 
			grade,
			_roleConfig[roleId-1]->BassHp,
			_roleConfig[roleId-1]->BassHpCoe1,
			roleDefense,
			_roleConfig[roleId-1]->BassHpCoe2,
			_roleConfig[roleId-1]->BassHpCoe3);
	}
}

std::string NewDataMgr::getXmlStringWithKey( const char* key )
{
#if 0
	CCLog("------NewRoleDataMgr::getXmlStringWithKey :%s", key);
	if ( _xmlString.empty() )
	{
		_xmlString = FileUtils::getInstance()->getValueMapFromFile("strings.xml");
	}
	CCLog("------NewRoleDataMgr::getXmlStringWithKeyEnd :%s", key);

	return _xmlString[key].asString();
#else

	auto xmlString = FileUtils::getInstance()->getValueMapFromFile("strings.xml");

	return xmlString[key].asString();

#endif
}


void NewDataMgr::setCardOptionWithIndex( int type, int index, int star )
{
	mRoleConfig.cardArr[type-1].cardID = index;
	mRoleConfig.cardArr[type-1].cardStarLv = star;
	updateCardOptionConfig( type, index ,star);
}

void NewDataMgr::updateCardOptionConfig( int type, int index, int star )
{
	std::string sql;
	sql.append("update cardOption_Table set ");

	sql.append("cardid=");
	sql.append(__String::createWithFormat("%d ",index)->getCString());
	sql.append(",cardStar=");
	sql.append(__String::createWithFormat("%d ",star)->getCString());
	sql.append("where type=");
	sql.append(__String::createWithFormat("%d ",type)->getCString());	

	if ( _sqlitedb )
	{
		int result;
		char* zErrorMsg = 0;

		result=sqlite3_exec(_sqlitedb,sql.c_str(),NULL,NULL,&zErrorMsg);
		if(result!=SQLITE_OK)
			CCLOG("insert err");
	}
}

void NewDataMgr::setCardImplantQuickStone(int type, int quickStoneid)
{
	for (int i = 0; i < GameData_CardNum; i++)
	{
		if ( _cardAtkPConfig[i]->CardType == type )
		{
			_cardAtkPConfig[i]->quickStoneId = quickStoneid;
		}
	}
	updateCardImplantQuickStone( type, quickStoneid);
}

void NewDataMgr::updateCardImplantQuickStone(int type, int qickStoneid)
{
	std::string sql;
	sql.append("update cardInfo_Table set ");
	
	sql.append("quickstoneid=");
	sql.append(__String::createWithFormat("%d ", qickStoneid)->getCString());

	sql.append("where cardtype=");
	sql.append(__String::createWithFormat("%d ", type)->getCString());

	if (_sqlitedb)
	{
		int result;
		char* zErrorMsg = 0;

		result = sqlite3_exec(_sqlitedb, sql.c_str(), NULL, NULL, &zErrorMsg);
		if (result != SQLITE_OK)
			CCLOG("insert err");
	}
}
bool NewDataMgr::getRoleUnlockState( int index )
{
	return _roleConfig[index-1]->isLock;
}

int NewDataMgr::getGold( void )
{
	return mUserData.Gold;
}

void NewDataMgr::addGold( int num )
{
	mUserData.Gold += num;
	if ( 0 > mUserData.Gold ) mUserData.Gold = 0;
	updateUserData( UserDateKey::UserKey_Gold , mUserData.Gold );
}

int NewDataMgr::getGem( void )
{
	return mUserData.Gem;
}

void NewDataMgr::addGem( int num )
{
	mUserData.Gem += num; 
	if ( 0 > mUserData.Gem ) mUserData.Gem = 0;
	updateUserData( UserDateKey::UserKey_Gem , mUserData.Gem );

}

csvRoleUpgrade_s* NewDataMgr::getRoleInfoWithIndex( int index )
{
	return _roleConfig[index-1];
}

int NewDataMgr::isNewUser()
{
	return mUserData.IsNewUser;
}

void NewDataMgr::setNewUserState( int state )
{
	mUserData.IsNewUser = state;
	updateUserData( UserDateKey::UserKey_IsNewUser , state );
}

void NewDataMgr::addUserLife( int num )
{
	mUserData.LifeNum += num;
	if( 0 > mUserData.LifeNum )mUserData.LifeNum = 0 ;
	updateUserData( UserDateKey::UserKey_LifeNum , mUserData.LifeNum );

}


void NewDataMgr::updateUserData(  UserDateKey key, int num  )
{
	std::string sql;
	sql.append("update userData_Table set  ");

	switch ( key )
	{
	case UserDateKey::UserKey_Gem:
		sql.append("gem=");
		break;
	case UserDateKey::UserKey_Gold:
		sql.append("gold=");
		break;
	case UserDateKey::UserKey_IsNewUser:
		sql.append("newUser=");
		break;
	case UserDateKey::UserKey_LifeNum:
		sql.append("life=");
		break;
	default:
		break;
	}

	sql.append(__String::createWithFormat("%d ",num)->getCString());

	if ( _sqlitedb )
	{
		int result;
		char* zErrorMsg = 0;

		result=sqlite3_exec(_sqlitedb,sql.c_str(),NULL,NULL,&zErrorMsg);
		if(result!=SQLITE_OK)
			CCLOG("insert err");
	}
}

double NewDataMgr::getHistoryLifeTime( void )
{
	double time = UserDefault::getInstance()->getDoubleForKey("getHistoryLifeTime", 0);

	return time;
}

void NewDataMgr::setHistoryLifeTime( double time )
{
	UserDefault::getInstance()->setDoubleForKey( "getHistoryLifeTime", time );
}

double NewDataMgr::getScrollUpdateTime( void )
{
	double time = UserDefault::getInstance()->getDoubleForKey("getScrollUpdateTime", 0);

	return time;
}

void NewDataMgr::setScrollUpdateTime( double time )
{
	UserDefault::getInstance()->setDoubleForKey( "getScrollUpdateTime", time );
}

int NewDataMgr::getHistoryHighResult( void )
{
	int data = UserDefault::getInstance()->getIntegerForKey("getHistoryHighResult", 0);

	return data;
}

void NewDataMgr::setHistoryHighResult( int result )
{
	UserDefault::getInstance()->setDoubleForKey( "getHistoryHighResult", result );

}


void NewDataMgr::MsgProConsumeLife( Ref* sender )
{
	NewDataMgr::getInstance()->addUserLife( -1 );

	if ( NewDataMgr::getInstance()->mUserData.LifeNum < GameLife_MaxLifeNum && 0 == NewDataMgr::getInstance()->getHistoryLifeTime() )
	{
		NewDataMgr::getInstance()->setHistoryLifeTime( convertTimeStrToNumber(CppCallJava::getInstance()->getSystemTime() ) );
	}
}

double NewDataMgr::convertTimeStrToNumber( std::string time, SystemTime key )
{
	char* token = (char*)"||";
	int nend=0;
	int nbegin=0;
	int  day, mon, year, hour, min, sec; 

	// 年
	nend = time.find(token, nbegin);
	if ( -1 != nend)
	{
		std::string temp = time.substr( nbegin, nend - nbegin);
		year = atoi( temp.c_str() );
		nbegin = nend + strlen(token);

		if ( SystemTime_Year == key ) return year;
	}

	// 月
	nend = time.find(token, nbegin);
	if ( -1 != nend)
	{
		mon = atoi( time.substr( nbegin, nend - nbegin).c_str() );
		nbegin = nend + strlen(token);

		if ( SystemTime_Mon == key ) return mon;
	}

	// 日
	nend = time.find(token, nbegin);
	if ( -1 != nend)
	{
		day = atoi( time.substr( nbegin, nend - nbegin).c_str() );
		nbegin = nend + strlen(token);

		if ( SystemTime_Day == key ) return day;
	}

	// 时
	nend = time.find(token, nbegin);
	if ( -1 != nend)
	{
		hour = atoi( time.substr( nbegin, nend - nbegin).c_str() );
		nbegin = nend + strlen(token);

		if ( SystemTime_Hour == key ) return hour;
	}

	// 分
	nend = time.find(token, nbegin);
	if ( -1 != nend)
	{
		min = atoi( time.substr( nbegin, nend - nbegin).c_str() );
		nbegin = nend + strlen(token);

		if ( SystemTime_Min == key ) return min;
	}

	// 秒
	nend = time.find(token, nbegin);
	if ( -1 != nend)
	{
		sec = atoi( time.substr( nbegin, nend - nbegin).c_str() );

		if ( SystemTime_Sec == key ) return sec;
	}

	CCLOG("convertTimeStrToNumber day:%d hour:%d min:%d sec:%d",day, hour, min, sec);
	return day*24  + hour*60 + min*60 + sec;
}

csvRewardGoodsList_s* NewDataMgr::getRewardGoodsListWithKey( RewardGoodsKey key )
{
	return CSV_MGR()->GetCsvRewardGoodsList()->GetByID( key );
}

void NewDataMgr::goodsRewardWithKey( RewardGoodsKey key, int pre/* = 0 */ )
{
	int goodsNum = getRewardGoodsListWithKey( key )->GoodsNum;
	switch ( key )
	{

	case RewardKey_1:
		addCardNumWithStar( pre-1, 4 , goodsNum);
		break;
	case RewardKey_2:
		addCardNumWithStar( pre-1, 3 , goodsNum);
		break;			
	case RewardKey_3:
	case RewardKey_24:
		mGameBuySystem->addGoodsNumWithKey(BuyKey_Pebble3, goodsNum);
		break;			
	case RewardKey_4:
	case RewardKey_5:
	case RewardKey_14:
	case RewardKey_21:
	case RewardKey_22:
	case RewardKey_23:
		addGold( goodsNum );
		break;				
	case RewardKey_6:
		addUserLife(goodsNum);
		break;				
			// 以上为游戏外

			// 以下为游戏内
	case RewardKey_7:
	case RewardKey_15:
		addCardNumWithStar( pre, 1 , goodsNum);
		break;				
	case RewardKey_8:
	case RewardKey_19:
	case RewardKey_20:
		addCardNumWithStar( pre, 2 , goodsNum);
		break;				
	case RewardKey_9:
		mGameBuySystem->addGoodsNumWithKey(BuyKey_Pebble1, goodsNum);
		break;				
	case RewardKey_10:
	case RewardKey_16:
	case RewardKey_17:
	case RewardKey_18:
		mGameBuySystem->addGoodsNumWithKey(BuyKey_Pebble2, goodsNum);
		break;				
	case RewardKey_11:
		mGameBuySystem->addGoodsNumWithKey(BuyKey_ChongCi, goodsNum);
		break;				
	case RewardKey_12:
		mGameBuySystem->addGoodsNumWithKey(BuyKey_HuDun, goodsNum);
		break;				
	case RewardKey_13:
		mGameBuySystem->addGoodsNumWithKey(BuyKey_Dazhao, goodsNum);
		break;		

		
	default:
		break;

	}
}

csvEnemyPlanetype_s* NewDataMgr::getEnemyPlaneWithKey( int key )
{
	return CSV_MGR()->GetCsvEnemyPlanetype()->GetByID( key );

}

csvShopList_s* NewDataMgr::getShopListWithID( int id )
{
	return CSV_MGR()->GetCsvShopList()->GetByID( id );

}

void NewDataMgr::setRoleIndex( int index )
{
	if( getRoleUnlockState( index ))
	{ 
		mRoleConfig.roleIndex = index; 
		updateRoleInfoConfig( RoleDateKey_isOption, index, (void*)1 );
	}
}

bool NewDataMgr::isFullGradeRoleWithId( int roleId )
{
	if ( RoleGrade_Max <= getRoleGradeWithId( roleId ) && getRoleExpWithId(roleId, RoleGrade_Max) <= getRoleCurExpWithId( roleId ))
	{
		return true;
	}
	return false;
}

csvBulletConfig_s* NewDataMgr::getBulletConfigWithId(int id)
{
	return CSV_MGR()->GetCsvBulletConfig()->GetByID(id);
}
