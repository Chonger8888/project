#ifndef __GAME_SYSTEM_H__
#define __GAME_SYSTEM_H__



#include "cocos2d.h"
#include "../CsvMgr/CsvMgr.h"
#include "sqlite3/include/sqlite3.h"



USING_NS_CC;
/************************************************************************/
/* 游戏相关系统管理                                                  */
/************************************************************************/
	




/*******************************************
* 任务系统
*******************************************/
enum GamePropertyEnum
{
	GamePrKey_killMonNum,
	GamePrKey_BGradeNum,
	GamePrKey_AGradeNum,
	GamePrKey_SGradeNum,
	GamePrKey_SSGradeNum,
	GamePrKey_getRedBallNum,
	GamePrKey_getBlueBallNum,
	GamePrKey_getYellowBallNum,
	GamePrKey_RedKillerNum,
	GamePrKey_BlueKillerNum,
	GamePrKey_YellowKillerNum,
	GamePrKey_totalKm,
	GamePrKey_minKillBossTime,
	GamePrKey_RedScore,
	GamePrKey_BlueScore,
	GamePrKey_YellowScore,

	GamePrKey_Max,
};
#define TaskSystem_DayTaskNum		5		// 每日任务数量
class GameTaskSystem
{
public:

	// 此枚举与GameTaskConfig.csv一一对应
	enum DayTaskTag
	{
		DayTaskTag_Task1 = 1,	//	在远征途中击杀80个小怪。
		DayTaskTag_Task2,		//	击杀远征Boss获得3个S评级。
		DayTaskTag_Task3,		//	在远征途中接到30次红色元素球
		DayTaskTag_Task4,		//	在远征途中接到30次蓝色元素球
		DayTaskTag_Task5,		//	在远征途中接到30次黄色元素球
		DayTaskTag_Task6,		//	击杀远征Boss获得1个SS评级。
		DayTaskTag_Task7,		//	击杀远征Boss获得5个A评级。
		DayTaskTag_Task8,		//	在远征途中击杀110个小怪。
		DayTaskTag_Task9,		//	今日远征总里程达到18Km
		DayTaskTag_Task10,		//	今日远征总里程达到16Km
		DayTaskTag_Task11,		//	15秒内击杀任意1个boss
		DayTaskTag_Max			
	};

	struct DayTaskStr
	{
		int taskId;				// 任务ID
		bool isFinish;			// 任务是否完成
	};

	GameTaskSystem();

	~GameTaskSystem();

	void initGameProperty( void );

	// 获取/设置任务日期
	int getTaskDate( void );
	void setTaskDate( int date );


	void initGameTask( sqlite3* db );
	void initDayTaskTable( void );
	void createDayTaskList();

	void saveGameAboutData( void );
	void saveGamePropertyData( void );
	void saveDayTaskData( void );
	std::vector<int> getDayTaskList( void );

	// 设置任务相关属性数值
	void setGameTaskProperty(int *data);

	csvGameTaskConfig_s* getGameTaskListWithID( int id );


private:
	void updateDayTaskData( void );

	void cleanTaskProperty( void );

private:
	sqlite3*			_sqlitedb;

	std::vector<int>	_dayTaskList;

	int					_GamePropertyArr[GamePrKey_Max];
	//DayTaskSystemStr _dayTaskProperty;

};



/*******************************************
* 角色系统
*******************************************/
class GameRoleSystem
{
public:
	GameRoleSystem();
	~GameRoleSystem();

	// 初始化角色列表 锁定状态
	void initRoleInfo(  sqlite3* db );


private:
	csvRoleUpgrade_s*		_roleConfig[3];

};


/*******************************************
* 卡牌系统
*******************************************/
class GameCardSystem
{
public:
	GameCardSystem();
	~GameCardSystem();

private:

};



/*******************************************
* 物品购物系统
*******************************************/

// 此Key需要与BuyList.csv 中物品ID对应
enum BuyListKey
{	
	BuyKey_Dazhao = 1,			// 道具 大招
	BuyKey_HuDun,				// 道具 护盾
	BuyKey_ChongCi,				// 道具 冲刺

	BuyKey_Pebble1,				// 小水晶
	BuyKey_Pebble2,				// 中水晶
	BuyKey_Pebble3,				// 大水晶

	BuyKey_ScrollUpdata,		//卷轴刷新
	BuyKey_StarScroll,			//启动卷轴
	BuyKey_Role2Unlock,			//角色2解锁
	BuyKey_Role3Unlock,			//角色3解锁
	BuyKey_CardCompound,		//卡牌合成
	BuyKey_FuHuo,				//复活

	BuyKey_Max
};

enum BuyType
{
	BuyType_Gem,		// 宝石购买
	BuyType_Gold		// 金币购买
};

class GameBuySystem
{
public:
	GameBuySystem();
	~GameBuySystem();

	void initBuyData( sqlite3* db );

	void initGoodsData( sqlite3* db );
	void addGoodsNumWithKey( BuyListKey key, int num );
	void updateGoodsNumConfig( BuyListKey key );


	// 获得物品购买
	csvBuyList_s* getBuyGoodsDataWithKey( BuyListKey key );

	bool buyGoodsWithKey( BuyListKey key , int num = 1);
	bool buyGoodsWithPrice(int goodsprice);

private:
	void showBuyTips( BuyType type );

private:
	sqlite3*			_sqlitedb;

};


/*******************************************
* 灵石系统
*******************************************/

// 此Key需要与QuickStoneTable.csv 中物品ID对应
enum QuickStoneKey
{
	QuickStoneKey_F = 1,
	QuickStoneKey_E,
	QuickStoneKey_D,
	QuickStoneKey_C,
	QuickStoneKey_B,
	QuickStoneKey_A,
	QuickStoneKey_S,
	QuickStoneKey_SS,
	QuickStoneKey_Max
};
class QuickStoneSystem
{
public:
	QuickStoneSystem()
		:_sqlitedb(nullptr)
	{
	}

	~QuickStoneSystem();

	void initQuickStoneSystem( sqlite3* db );

	void implantQuickStone(int cardType, QuickStoneKey key);
	void compountQuickStone(QuickStoneKey key);
	void updateQuickStoneData(QuickStoneKey key);

	// 获得
	csvQuickStoneTable_s* getQuickStoneDataWithKey(QuickStoneKey key);
private:
	void initQuickStoneData(void);

private:
	sqlite3*			_sqlitedb;

};

#endif