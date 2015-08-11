/********************************************************************
创建于:		2014/11/14  14:21	
文件名: 	E:\Cocos3.3_Project\MyGame\Classes\commonData.h 
作  者:		alex(alex)
修改于：	2014/11/14  14:21	

目  的:		一些全局数据的定义
*********************************************************************/
#ifndef _GAME_DATA_H_
#define _GAME_DATA_H_



#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "../GamePlaying/BasePlane/BasePlane.h"
#include "GameSystem.h"

using namespace cocostudio::timeline;
USING_NS_CC;


class PhysicsUnit;
class LevelData;
class BulletFactory;
class WeaponFactory;
class PlaneFactory;
class PlaneAIFactory;
class EnterAction;
class DropsFactory;
class PlaneManager;
class GameData : public Ref
{
public: 

	static GameData* getInstance()
	{
		static GameData inStance;
		return &inStance;
	}
	void resetGameData();						//重置游戏数据
	void addScore(const int& num);						//增加分数
	void addGoldNum(const int& num);					//增加金币

	void AddEnemyPlane(PhysicsUnit* pu);		//新增敌机到敌机列表
	void DelEnemyPlane(PhysicsUnit* pu);		//从敌机列表删除死亡敌机单元
	void SortByDis();					//对敌机单元按距离由近及远排序
	void SortByHpLimit();			//对敌机单元按血上限由大到小排序

	PhysicsUnit* getEnemyPlaneByMaxHp();		//得到血量最大的敌机目标
	const Vec2& getNearTargetByPos(const Vec2& curPos);//得到离给定的坐标最近的目标的位置
	float getDistance(const Point& pos);	//点与主飞机点的距离

	std::list<PhysicsUnit*> getEnemyPlaneVec(){return m_EnemyPlaneVec;} //敌机列表
	int getEmemyNums();							//当前敌机数量

	//各种工厂的实例
	BulletFactory* getBulletFactory(){return m_bulletFactory;}
	WeaponFactory* getWeaponFactory(){return m_weaponFactory;}
	PlaneFactory* getPlaneFactory(){return m_planefactory;}
	DropsFactory* getDropsFactory(){return m_scoreFactory;}
	PlaneManager* getPlaneManager(){return m_planeManager;}
	PlaneAIFactory* getPlaneAiFactory(){return m_planeAiFactory;}
	EnterAction* getPlaneAction(){return m_PlaneAction;}

	//储存和取得我方飞机
	void setMainPlane(MainPlane* plane);
	MainPlane* getMainPlane();
	
	void GameStart();						//游戏开始(在调用该函数之前，先调用setGameType)
	void GameOver(bool iswin);				//游戏结束
	bool isGameEnd();
	bool isGamePause(){return m_gamePause;}	//游戏是否暂停
	void GamePause();						//暂停游戏
	void GameResume();						//恢复游戏
	void GameSlowDown(float duration, bool isToBlack = true);		//减速
	void GameResumeScale();		//减速
	void GameSpeedUp();						//游戏加速
	
	void addEnemyStatistics(const SelfPro& selfpro, const UnitId& KillerId);	//每击毁一架敌机，添加统计
	std::vector<ES>	getEnemyStatistics();	//击杀列表
	int getSumEnemyStatics();				//击杀总数
	void AnalyseStaticsData();				//分析击杀数据

	//测试刚体之间是否能发生实质碰撞
	bool isContact(int mask1,int mask2,int mask3, int mask4, int mask5,int mask6);

	//测试刚体之间是否能发生穿透碰撞
	bool isContact_P(int mask1,int mask2,int mask3, int mask4, int mask5,int mask6);

	//BOSS类型及评级等级记录
	void addBossRecord(int bossType, const BossRating& rating);
	std::vector<BossGrade>& getBossRecords();

	//道具
	void addProp();

	// 所有数据统计
	int *getAllStatistics( void );

	//小怪子弹的伤害
	int getBulletHurtsOfEnemyPl(float userdata);

	Size				m_winSize;

private:
	GameData();
	~GameData(); 

	CC_SYNTHESIZE(bool, m_isDisabledKey, isDisabledKey);			//禁用按键

	CC_SYNTHESIZE(int, m_Score, ScoreNum);			//分数
	CC_SYNTHESIZE(int,m_Golds,GoldNum);				//金币数
	CC_SYNTHESIZE(float,m_updateSpeed,UpdateSpeed);		//地图更新速度
	CC_SYNTHESIZE(bool,m_GameResult,GameResult);		//游戏结果
	CC_SYNTHESIZE(bool,m_Playing,isGamePlaying);		//游戏是否正在进行,即游戏结束判断的唯一标示
	CC_SYNTHESIZE(bool,m_isSpeedUp,isSpeedUp);			//游戏是否要冲刺
	CC_SYNTHESIZE(bool,m_IsBossDead,IsBossDead);		//BOSS是否死亡

	CC_SYNTHESIZE(bool,m_IsHurtByBoss,IsHurtByBoss);	//主角是否受到来自BOSS的伤害

	std::list<PhysicsUnit*>		m_EnemyPlaneVec;
	std::vector<ES>				m_StatisticsVec;		//击毁敌机统计
	std::vector<BossGrade>		m_BossGrade;			//BOSS评级记录

	EnterAction*				m_PlaneAction;
	PlaneManager*				m_planeManager;
	BulletFactory*				m_bulletFactory;
	WeaponFactory*				m_weaponFactory;
	PlaneFactory*				m_planefactory;
	DropsFactory*				m_scoreFactory;
	PlaneAIFactory*				m_planeAiFactory;

	MainPlane*					m_mainPlane;

	bool						m_gamePause;		//游戏是否暂停
	int							m_statisticsArr[GamePrKey_Max];

	 std::set<void*>			m_TargetSet;
};



#endif