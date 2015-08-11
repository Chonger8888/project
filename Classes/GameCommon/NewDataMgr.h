#ifndef __NEW_ROLE_DATA_MGR__
#define __NEW_ROLE_DATA_MGR__

#include "cocos2d.h"
#include "SQLite/sqlite3.h"

#include "CsvMgr/CsvMgr.h"
#include "GameSystem.h"


USING_NS_CC;
/************************************************************************/
/* 游戏角色数据管理                                                     */
/************************************************************************/

// 单件声明模板
 #define SINGLETONDATA_DECLEAR( className )	public:\
 	static className	*getInstance(){ if( inst == NULL ) { inst = new (std::nothrow) className(); }	return inst; }\
 	static void			purgeInstance(){ if( inst != NULL ) { CC_SAFE_DELETE(inst);	inst = NULL; } }\
 										private:\
 										static className	*inst;


// 单件定义模板。在cpp开始调用											
#define SINGLETONDATA_DEFINE( className )	className	*className::inst = NULL;

#define GameData_CardArr	3
#define GameData_CardNum	9
// 游戏基础攻击
#define Game_BaseATK	100


/************************************************************************/
/* 卡牌升级规则                                                         */
/************************************************************************/

#define CardCompound_StarLv1_To_StarLv2		3		// 1星升2星
#define CardCompound_StarLv2_To_StarLv3		4		// 2星升3星
#define CardCompound_StarLv3_To_StarLv4		5		// 3星升4星
#define CardCompound_StarLv4_To_StarLv5		5		// 4星升5星

//攻击公式
#define CardAtkFormula( grade, coe1, coe2 )									( coe1 + coe2 * grade )


/************************************************************************/
/* 角色升级相关规则                                                     */
/************************************************************************/
#define  RoleGrade_Max		60
//攻击公式
#define RoleAtkFormula( grade, coe1, coe2, coe3, coe4 )						( coe1 * grade + coe2 * coe3 * coe4)
//武器数公式
#define RoleWeaponNumFormula( grade, bass, coe )							( bass + grade/10 )
//防御公式
#define RoleDefenseFormula( grade, bass, coe )								( coe * grade + bass)
//幸运公式
#define RoleLuckFormula( grade, coe )										( coe * grade )
//经验公式
#define RoleExpFormula( grade, bass, coe1, coe2, coe3 )						( coe1 * grade * grade + coe3 * grade)
//HP公式
#define RoleHpFormula( grade, bass, coe1, coe2, coe3, coe4 )				( coe1 * grade + bass + pow(coe2 , coe3) * coe4 )
/************************************************************************/
/* 生命相关                                                             */
/************************************************************************/
#define GameLife_MaxLifeNum			5				// 触发生命倒计时生命数
#define GameLife_ReplyLife_Time		(30 * 1)		// 单个生命回复时间（秒）



/************************************************************************/
/* 抽奖相关*/
/************************************************************************/
typedef enum RewardGoodsKey 
{
	RewardKey_1 = 1,			// 随机4星卡牌
	RewardKey_2,				// 随机3星卡牌
	RewardKey_3,				// 大经验水晶
	RewardKey_4,				// 金币
	RewardKey_5,				// 金币
	RewardKey_6,				// 翅膀
	// 以上为游戏外

	// 以下为游戏内
	RewardKey_7,				// 随机1星卡牌
	RewardKey_8,				// 随机2星卡牌
	RewardKey_9,				// 小经验水晶
	RewardKey_10,				// 中经验水晶
	RewardKey_11,				// 冲刺道具
	RewardKey_12,				// 护盾道具
	RewardKey_13,				// 大招道具

	// 任务奖励
	RewardKey_14,				// 金币
	RewardKey_15,				// 随机1~2星卡牌
	RewardKey_16,				// 中经验水晶
	RewardKey_17,				// 中经验水晶
	RewardKey_18,				// 中经验水晶
	RewardKey_19,				// 随机2星卡牌
	RewardKey_20,				// 随机1~2星卡牌
	RewardKey_21,				// 金币
	RewardKey_22,				// 金币
	RewardKey_23,				// 金币
	RewardKey_24,				// 大经验水晶

};
	
	
#define GameScroll_Updta_Time		(30 * 1)		// 卷轴回复时间（秒）

	
	
	
	
	
	
	
	



typedef struct GameCard
{
	// use this data
	int cardID;			// 卡牌ID 1-9
	int cardStarLv;		// 星级 1-5

}GameCard_S;

typedef struct  EntryGameConfig
{

	int		roleIndex;				// 角色索引

	GameCard_S	cardArr[3];			// 选择带入游戏的三类卡片

}EntryGameConfig_S;

typedef enum
{
	CardDateKey_isLock,
}CardDateKey;

typedef enum
{
	RoleDateKey_isLock,
	RoleDateKey_CurExp,
	RoleDateKey_Grade,
	RoleDateKey_isOption

}RoleDateKey;



enum  class UserDateKey
{
	UserKey_Gold,
	UserKey_Gem,
	UserKey_IsNewUser,
	UserKey_LifeNum,
};

enum SystemTime
{
	SystemTime_Normal,		// 年
	SystemTime_Year,		// 年
	SystemTime_Mon,			// 月
	SystemTime_Day,			// 日
	SystemTime_Hour,		// 时
	SystemTime_Min,			// 分
	SystemTime_Sec			// 秒
};

typedef struct UserData
{
	int IsNewUser;
	int LifeNum;
	int Gold;
	int Gem;
}UserData;


class NewDataMgr : public Ref
{
public:

	SINGLETONDATA_DECLEAR(NewDataMgr);

	NewDataMgr();

	~NewDataMgr();


public:
	// 初始化读取数据库
	void initGameData( void );
	// 初始化角色列表 锁定状态
	void initRoleInfo(  sqlite3* db );
	// 初始化卡牌列表 开启状态
	void initCardInfo( sqlite3* db );
	// 初始化上次选择卡牌
	void initCardOption( sqlite3* db );
	// 初始化用户数据
	void initUserData( sqlite3* db );

	// 主角相关
	csvRoleUpgrade_s* getRoleInfoWithIndex( int index );

	int getRoleIndex(void)	{ return mRoleConfig.roleIndex;	};
	void setRoleIndex( int index );	

	void setRoleLockWithIndex( int type , bool isunlock);// 真为解锁
	void setRoleExpWithIndex( int type );// 经验
	void setRoleGoadeWithIndex( int type );// 等级
	void updateRoleInfoConfig( RoleDateKey key, int type, void* data );

	int   getRoleGradeWithId( int roleId );
	int   getRolePriceWithId( int roleId );
	int   getRoleAutoLockLvWithId( int roleId );
	void  addRoleGradeWithId( int roleId );
	int	  getWeaponNumWithId( int roleId ,int grade = 0);
	float getRoleAtkWithId( int roleId ,int grade = 0);
	float getRoleDefenseWithId( int roleId ,int grade = 0);
	float getRoleLuckWithId( int roleId ,int grade = 0);
	float getRoleExpWithId( int roleId ,int grade = 0);
	float getRoleHpWithId( int roleId ,int grade = 0);
	float getRoleCurExpWithId( int roleId ,int grade = 0);
	bool getRoleUnlockState( int index );
	bool addRoleCurExpWithId( int roleId ,int exp );
	float getRoleCurExpPercentWithId( int roleId ,int grade = 0);
	bool isFullGradeRoleWithId( int roleId );


	// 卡牌相关
	csvCardAtkPConfig_s* getCardConfigWithId(int cardId);
	void addCardNumWithStar( int id, int starlv, int num );
	void updateCardNumWithStar( int id, int starLv , void* data );
	int getCardMaxStarLvWithId( int id );
	int getCardNumWithStarLv( int id, int starLv );
	float getCardAtk( int id, int starlv );

	void setCardOptionWithIndex( int type, int index, int star );
	void updateCardOptionConfig( int type, int index, int star );
	void setCardImplantQuickStone(int type, int quickStoneid);
	void updateCardImplantQuickStone(int type, int qickStoneid);

	// 金币
	int getGold( void );
	void addGold( int num );
	int getGem( void );
	void addGem( int num );
	void addUserLife( int num );

	void updateUserData( UserDateKey key, int num );

	int isNewUser();
	void setNewUserState( int state );// 0 新用户	1老用户

	// 获得子弹配置表
	csvBulletConfig_s* getBulletConfigWithId(int id);


	// 获得物品奖励表
	csvRewardGoodsList_s* getRewardGoodsListWithKey( RewardGoodsKey key );
	void goodsRewardWithKey( RewardGoodsKey key, int pre = 0 );
	// 获得商品表
	csvShopList_s* getShopListWithID( int id );
	// 获得小怪表
	csvEnemyPlanetype_s* getEnemyPlaneWithKey( int key = 1);


	  /** XML存储相关
     @ 与cocos存储相关联
     */
	
	// 生命回复时间存储
	double  getHistoryLifeTime( void );
	void setHistoryLifeTime( double time );

	// 卷轴刷新时间存储
	double getScrollUpdateTime( void );
	void setScrollUpdateTime( double time );

	// 历史新高成绩存储
	int getHistoryHighResult( void );
	void setHistoryHighResult( int result );


	// 转换字符串时间到数字
	double  convertTimeStrToNumber( std::string time ,SystemTime key = SystemTime_Normal);

	// 消耗生命消息
	void MsgProConsumeLife( Ref* sender );

	// 获得XML字符串
	std::string getXmlStringWithKey( const char* key );
public:
	// 任务
	GameTaskSystem*		mGameTaskSystem;
	GameBuySystem*		mGameBuySystem;
	QuickStoneSystem*	mQuickStoneSystem;

	EntryGameConfig_S mRoleConfig;

	UserData	mUserData;

	bool mRoleUpgradeIng;
private:
	ValueMap _xmlString;

	sqlite3*	_sqlitedb;
	
	csvRoleUpgrade_s*		_roleConfig[3];

	//csvAtkConfig_s*			_roleAtkConfig[3];
	csvCardAtkPConfig_s*	_cardAtkPConfig[GameData_CardNum];

};









#endif