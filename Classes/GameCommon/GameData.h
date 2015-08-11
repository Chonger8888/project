/********************************************************************
������:		2014/11/14  14:21	
�ļ���: 	E:\Cocos3.3_Project\MyGame\Classes\commonData.h 
��  ��:		alex(alex)
�޸��ڣ�	2014/11/14  14:21	

Ŀ  ��:		һЩȫ�����ݵĶ���
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
	void resetGameData();						//������Ϸ����
	void addScore(const int& num);						//���ӷ���
	void addGoldNum(const int& num);					//���ӽ��

	void AddEnemyPlane(PhysicsUnit* pu);		//�����л����л��б�
	void DelEnemyPlane(PhysicsUnit* pu);		//�ӵл��б�ɾ�������л���Ԫ
	void SortByDis();					//�Եл���Ԫ�������ɽ���Զ����
	void SortByHpLimit();			//�Եл���Ԫ��Ѫ�����ɴ�С����

	PhysicsUnit* getEnemyPlaneByMaxHp();		//�õ�Ѫ�����ĵл�Ŀ��
	const Vec2& getNearTargetByPos(const Vec2& curPos);//�õ�����������������Ŀ���λ��
	float getDistance(const Point& pos);	//�������ɻ���ľ���

	std::list<PhysicsUnit*> getEnemyPlaneVec(){return m_EnemyPlaneVec;} //�л��б�
	int getEmemyNums();							//��ǰ�л�����

	//���ֹ�����ʵ��
	BulletFactory* getBulletFactory(){return m_bulletFactory;}
	WeaponFactory* getWeaponFactory(){return m_weaponFactory;}
	PlaneFactory* getPlaneFactory(){return m_planefactory;}
	DropsFactory* getDropsFactory(){return m_scoreFactory;}
	PlaneManager* getPlaneManager(){return m_planeManager;}
	PlaneAIFactory* getPlaneAiFactory(){return m_planeAiFactory;}
	EnterAction* getPlaneAction(){return m_PlaneAction;}

	//�����ȡ���ҷ��ɻ�
	void setMainPlane(MainPlane* plane);
	MainPlane* getMainPlane();
	
	void GameStart();						//��Ϸ��ʼ(�ڵ��øú���֮ǰ���ȵ���setGameType)
	void GameOver(bool iswin);				//��Ϸ����
	bool isGameEnd();
	bool isGamePause(){return m_gamePause;}	//��Ϸ�Ƿ���ͣ
	void GamePause();						//��ͣ��Ϸ
	void GameResume();						//�ָ���Ϸ
	void GameSlowDown(float duration, bool isToBlack = true);		//����
	void GameResumeScale();		//����
	void GameSpeedUp();						//��Ϸ����
	
	void addEnemyStatistics(const SelfPro& selfpro, const UnitId& KillerId);	//ÿ����һ�ܵл������ͳ��
	std::vector<ES>	getEnemyStatistics();	//��ɱ�б�
	int getSumEnemyStatics();				//��ɱ����
	void AnalyseStaticsData();				//������ɱ����

	//���Ը���֮���Ƿ��ܷ���ʵ����ײ
	bool isContact(int mask1,int mask2,int mask3, int mask4, int mask5,int mask6);

	//���Ը���֮���Ƿ��ܷ�����͸��ײ
	bool isContact_P(int mask1,int mask2,int mask3, int mask4, int mask5,int mask6);

	//BOSS���ͼ������ȼ���¼
	void addBossRecord(int bossType, const BossRating& rating);
	std::vector<BossGrade>& getBossRecords();

	//����
	void addProp();

	// ��������ͳ��
	int *getAllStatistics( void );

	//С���ӵ����˺�
	int getBulletHurtsOfEnemyPl(float userdata);

	Size				m_winSize;

private:
	GameData();
	~GameData(); 

	CC_SYNTHESIZE(bool, m_isDisabledKey, isDisabledKey);			//���ð���

	CC_SYNTHESIZE(int, m_Score, ScoreNum);			//����
	CC_SYNTHESIZE(int,m_Golds,GoldNum);				//�����
	CC_SYNTHESIZE(float,m_updateSpeed,UpdateSpeed);		//��ͼ�����ٶ�
	CC_SYNTHESIZE(bool,m_GameResult,GameResult);		//��Ϸ���
	CC_SYNTHESIZE(bool,m_Playing,isGamePlaying);		//��Ϸ�Ƿ����ڽ���,����Ϸ�����жϵ�Ψһ��ʾ
	CC_SYNTHESIZE(bool,m_isSpeedUp,isSpeedUp);			//��Ϸ�Ƿ�Ҫ���
	CC_SYNTHESIZE(bool,m_IsBossDead,IsBossDead);		//BOSS�Ƿ�����

	CC_SYNTHESIZE(bool,m_IsHurtByBoss,IsHurtByBoss);	//�����Ƿ��ܵ�����BOSS���˺�

	std::list<PhysicsUnit*>		m_EnemyPlaneVec;
	std::vector<ES>				m_StatisticsVec;		//���ٵл�ͳ��
	std::vector<BossGrade>		m_BossGrade;			//BOSS������¼

	EnterAction*				m_PlaneAction;
	PlaneManager*				m_planeManager;
	BulletFactory*				m_bulletFactory;
	WeaponFactory*				m_weaponFactory;
	PlaneFactory*				m_planefactory;
	DropsFactory*				m_scoreFactory;
	PlaneAIFactory*				m_planeAiFactory;

	MainPlane*					m_mainPlane;

	bool						m_gamePause;		//��Ϸ�Ƿ���ͣ
	int							m_statisticsArr[GamePrKey_Max];

	 std::set<void*>			m_TargetSet;
};



#endif