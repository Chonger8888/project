#ifndef __GAME_SYSTEM_H__
#define __GAME_SYSTEM_H__



#include "cocos2d.h"
#include "../CsvMgr/CsvMgr.h"
#include "sqlite3/include/sqlite3.h"



USING_NS_CC;
/************************************************************************/
/* ��Ϸ���ϵͳ����                                                  */
/************************************************************************/
	




/*******************************************
* ����ϵͳ
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
#define TaskSystem_DayTaskNum		5		// ÿ����������
class GameTaskSystem
{
public:

	// ��ö����GameTaskConfig.csvһһ��Ӧ
	enum DayTaskTag
	{
		DayTaskTag_Task1 = 1,	//	��Զ��;�л�ɱ80��С�֡�
		DayTaskTag_Task2,		//	��ɱԶ��Boss���3��S������
		DayTaskTag_Task3,		//	��Զ��;�нӵ�30�κ�ɫԪ����
		DayTaskTag_Task4,		//	��Զ��;�нӵ�30����ɫԪ����
		DayTaskTag_Task5,		//	��Զ��;�нӵ�30�λ�ɫԪ����
		DayTaskTag_Task6,		//	��ɱԶ��Boss���1��SS������
		DayTaskTag_Task7,		//	��ɱԶ��Boss���5��A������
		DayTaskTag_Task8,		//	��Զ��;�л�ɱ110��С�֡�
		DayTaskTag_Task9,		//	����Զ������̴ﵽ18Km
		DayTaskTag_Task10,		//	����Զ������̴ﵽ16Km
		DayTaskTag_Task11,		//	15���ڻ�ɱ����1��boss
		DayTaskTag_Max			
	};

	struct DayTaskStr
	{
		int taskId;				// ����ID
		bool isFinish;			// �����Ƿ����
	};

	GameTaskSystem();

	~GameTaskSystem();

	void initGameProperty( void );

	// ��ȡ/������������
	int getTaskDate( void );
	void setTaskDate( int date );


	void initGameTask( sqlite3* db );
	void initDayTaskTable( void );
	void createDayTaskList();

	void saveGameAboutData( void );
	void saveGamePropertyData( void );
	void saveDayTaskData( void );
	std::vector<int> getDayTaskList( void );

	// �����������������ֵ
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
* ��ɫϵͳ
*******************************************/
class GameRoleSystem
{
public:
	GameRoleSystem();
	~GameRoleSystem();

	// ��ʼ����ɫ�б� ����״̬
	void initRoleInfo(  sqlite3* db );


private:
	csvRoleUpgrade_s*		_roleConfig[3];

};


/*******************************************
* ����ϵͳ
*******************************************/
class GameCardSystem
{
public:
	GameCardSystem();
	~GameCardSystem();

private:

};



/*******************************************
* ��Ʒ����ϵͳ
*******************************************/

// ��Key��Ҫ��BuyList.csv ����ƷID��Ӧ
enum BuyListKey
{	
	BuyKey_Dazhao = 1,			// ���� ����
	BuyKey_HuDun,				// ���� ����
	BuyKey_ChongCi,				// ���� ���

	BuyKey_Pebble1,				// Сˮ��
	BuyKey_Pebble2,				// ��ˮ��
	BuyKey_Pebble3,				// ��ˮ��

	BuyKey_ScrollUpdata,		//����ˢ��
	BuyKey_StarScroll,			//��������
	BuyKey_Role2Unlock,			//��ɫ2����
	BuyKey_Role3Unlock,			//��ɫ3����
	BuyKey_CardCompound,		//���ƺϳ�
	BuyKey_FuHuo,				//����

	BuyKey_Max
};

enum BuyType
{
	BuyType_Gem,		// ��ʯ����
	BuyType_Gold		// ��ҹ���
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


	// �����Ʒ����
	csvBuyList_s* getBuyGoodsDataWithKey( BuyListKey key );

	bool buyGoodsWithKey( BuyListKey key , int num = 1);
	bool buyGoodsWithPrice(int goodsprice);

private:
	void showBuyTips( BuyType type );

private:
	sqlite3*			_sqlitedb;

};


/*******************************************
* ��ʯϵͳ
*******************************************/

// ��Key��Ҫ��QuickStoneTable.csv ����ƷID��Ӧ
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

	// ���
	csvQuickStoneTable_s* getQuickStoneDataWithKey(QuickStoneKey key);
private:
	void initQuickStoneData(void);

private:
	sqlite3*			_sqlitedb;

};

#endif