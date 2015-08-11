#ifndef __NEW_ROLE_DATA_MGR__
#define __NEW_ROLE_DATA_MGR__

#include "cocos2d.h"
#include "SQLite/sqlite3.h"

#include "CsvMgr/CsvMgr.h"
#include "GameSystem.h"


USING_NS_CC;
/************************************************************************/
/* ��Ϸ��ɫ���ݹ���                                                     */
/************************************************************************/

// ��������ģ��
 #define SINGLETONDATA_DECLEAR( className )	public:\
 	static className	*getInstance(){ if( inst == NULL ) { inst = new (std::nothrow) className(); }	return inst; }\
 	static void			purgeInstance(){ if( inst != NULL ) { CC_SAFE_DELETE(inst);	inst = NULL; } }\
 										private:\
 										static className	*inst;


// ��������ģ�塣��cpp��ʼ����											
#define SINGLETONDATA_DEFINE( className )	className	*className::inst = NULL;

#define GameData_CardArr	3
#define GameData_CardNum	9
// ��Ϸ��������
#define Game_BaseATK	100


/************************************************************************/
/* ������������                                                         */
/************************************************************************/

#define CardCompound_StarLv1_To_StarLv2		3		// 1����2��
#define CardCompound_StarLv2_To_StarLv3		4		// 2����3��
#define CardCompound_StarLv3_To_StarLv4		5		// 3����4��
#define CardCompound_StarLv4_To_StarLv5		5		// 4����5��

//������ʽ
#define CardAtkFormula( grade, coe1, coe2 )									( coe1 + coe2 * grade )


/************************************************************************/
/* ��ɫ������ع���                                                     */
/************************************************************************/
#define  RoleGrade_Max		60
//������ʽ
#define RoleAtkFormula( grade, coe1, coe2, coe3, coe4 )						( coe1 * grade + coe2 * coe3 * coe4)
//��������ʽ
#define RoleWeaponNumFormula( grade, bass, coe )							( bass + grade/10 )
//������ʽ
#define RoleDefenseFormula( grade, bass, coe )								( coe * grade + bass)
//���˹�ʽ
#define RoleLuckFormula( grade, coe )										( coe * grade )
//���鹫ʽ
#define RoleExpFormula( grade, bass, coe1, coe2, coe3 )						( coe1 * grade * grade + coe3 * grade)
//HP��ʽ
#define RoleHpFormula( grade, bass, coe1, coe2, coe3, coe4 )				( coe1 * grade + bass + pow(coe2 , coe3) * coe4 )
/************************************************************************/
/* �������                                                             */
/************************************************************************/
#define GameLife_MaxLifeNum			5				// ������������ʱ������
#define GameLife_ReplyLife_Time		(30 * 1)		// ���������ظ�ʱ�䣨�룩



/************************************************************************/
/* �齱���*/
/************************************************************************/
typedef enum RewardGoodsKey 
{
	RewardKey_1 = 1,			// ���4�ǿ���
	RewardKey_2,				// ���3�ǿ���
	RewardKey_3,				// ����ˮ��
	RewardKey_4,				// ���
	RewardKey_5,				// ���
	RewardKey_6,				// ���
	// ����Ϊ��Ϸ��

	// ����Ϊ��Ϸ��
	RewardKey_7,				// ���1�ǿ���
	RewardKey_8,				// ���2�ǿ���
	RewardKey_9,				// С����ˮ��
	RewardKey_10,				// �о���ˮ��
	RewardKey_11,				// ��̵���
	RewardKey_12,				// ���ܵ���
	RewardKey_13,				// ���е���

	// ������
	RewardKey_14,				// ���
	RewardKey_15,				// ���1~2�ǿ���
	RewardKey_16,				// �о���ˮ��
	RewardKey_17,				// �о���ˮ��
	RewardKey_18,				// �о���ˮ��
	RewardKey_19,				// ���2�ǿ���
	RewardKey_20,				// ���1~2�ǿ���
	RewardKey_21,				// ���
	RewardKey_22,				// ���
	RewardKey_23,				// ���
	RewardKey_24,				// ����ˮ��

};
	
	
#define GameScroll_Updta_Time		(30 * 1)		// ����ظ�ʱ�䣨�룩

	
	
	
	
	
	
	
	



typedef struct GameCard
{
	// use this data
	int cardID;			// ����ID 1-9
	int cardStarLv;		// �Ǽ� 1-5

}GameCard_S;

typedef struct  EntryGameConfig
{

	int		roleIndex;				// ��ɫ����

	GameCard_S	cardArr[3];			// ѡ�������Ϸ�����࿨Ƭ

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
	SystemTime_Normal,		// ��
	SystemTime_Year,		// ��
	SystemTime_Mon,			// ��
	SystemTime_Day,			// ��
	SystemTime_Hour,		// ʱ
	SystemTime_Min,			// ��
	SystemTime_Sec			// ��
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
	// ��ʼ����ȡ���ݿ�
	void initGameData( void );
	// ��ʼ����ɫ�б� ����״̬
	void initRoleInfo(  sqlite3* db );
	// ��ʼ�������б� ����״̬
	void initCardInfo( sqlite3* db );
	// ��ʼ���ϴ�ѡ����
	void initCardOption( sqlite3* db );
	// ��ʼ���û�����
	void initUserData( sqlite3* db );

	// �������
	csvRoleUpgrade_s* getRoleInfoWithIndex( int index );

	int getRoleIndex(void)	{ return mRoleConfig.roleIndex;	};
	void setRoleIndex( int index );	

	void setRoleLockWithIndex( int type , bool isunlock);// ��Ϊ����
	void setRoleExpWithIndex( int type );// ����
	void setRoleGoadeWithIndex( int type );// �ȼ�
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


	// �������
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

	// ���
	int getGold( void );
	void addGold( int num );
	int getGem( void );
	void addGem( int num );
	void addUserLife( int num );

	void updateUserData( UserDateKey key, int num );

	int isNewUser();
	void setNewUserState( int state );// 0 ���û�	1���û�

	// ����ӵ����ñ�
	csvBulletConfig_s* getBulletConfigWithId(int id);


	// �����Ʒ������
	csvRewardGoodsList_s* getRewardGoodsListWithKey( RewardGoodsKey key );
	void goodsRewardWithKey( RewardGoodsKey key, int pre = 0 );
	// �����Ʒ��
	csvShopList_s* getShopListWithID( int id );
	// ���С�ֱ�
	csvEnemyPlanetype_s* getEnemyPlaneWithKey( int key = 1);


	  /** XML�洢���
     @ ��cocos�洢�����
     */
	
	// �����ظ�ʱ��洢
	double  getHistoryLifeTime( void );
	void setHistoryLifeTime( double time );

	// ����ˢ��ʱ��洢
	double getScrollUpdateTime( void );
	void setScrollUpdateTime( double time );

	// ��ʷ�¸߳ɼ��洢
	int getHistoryHighResult( void );
	void setHistoryHighResult( int result );


	// ת���ַ���ʱ�䵽����
	double  convertTimeStrToNumber( std::string time ,SystemTime key = SystemTime_Normal);

	// ����������Ϣ
	void MsgProConsumeLife( Ref* sender );

	// ���XML�ַ���
	std::string getXmlStringWithKey( const char* key );
public:
	// ����
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