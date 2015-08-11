#include "GameData.h"
#include "AudioEngine.h"
#include "../GamePlaying/Bullet/BulletFactory.h"
#include "../GamePlaying/Weapon/WeaponFactory.h"
#include "../GamePlaying/BasePlane/BasePlane.h"
#include "../GamePlaying/BasePlane/PlaneAction.h"
#include "../GamePlaying/DropGoods.h"
#include "../GamePlaying/BasePlane/PlaneManager.h"
#include "../GamePlaying/Layer/UIController.h"
#include "GameUI/GameMainLayer.h"
#include "GamePlaying/Layer/PhysicsLayer.h"
#include "EffectsManager.h"
#include "GamePlaying/BasePlane/PlaneFactory.h"
#include "GamePlaying/AI.h"
#include "NewDataMgr.h"
#include "GameConfig.h"
#include "GameSystem.h"
#include "../GamePlaying/Layer/RollingBg.h"

GameData::GameData()
{
	m_winSize = Director::getInstance()->getWinSize();

	setisDisabledKey(false);

	m_bulletFactory = BulletFactory::create();
	m_weaponFactory = WeaponFactory::create();
	m_planefactory = PlaneFactory::create();
	m_planeAiFactory = PlaneAIFactory::create();
	m_PlaneAction = EnterAction::create();
	m_scoreFactory = DropsFactory::create();
	m_planeManager = PlaneManager::create();


	m_mainPlane = nullptr;

	m_Playing = false;
	m_gamePause = false;
	m_GameResult = false;
	m_IsHurtByBoss = false;
	m_IsBossDead = false;

	resetGameData();
}

GameData::~GameData()
{
	m_bulletFactory->purge();
	m_weaponFactory->purge();
	m_planefactory->purge();
	m_planeAiFactory->purge();
	m_planeManager->purge();
	m_PlaneAction->purge();
	m_scoreFactory->purge();

}

void GameData::addScore(const int& num )
{
	m_Score += num;
	UIController::getInstance()->getMainUILayer()->updateUIScore( m_Score );

}

void GameData::AddEnemyPlane( PhysicsUnit* pu )
{
	m_EnemyPlaneVec.push_back(pu);
}

void GameData::DelEnemyPlane( PhysicsUnit* pu )
{
	std::list<PhysicsUnit*>::iterator it = m_EnemyPlaneVec.begin();
	for (it; it != m_EnemyPlaneVec.end();)
	{
		PhysicsUnit* physicsunit = (*it);
		if (physicsunit == pu)
		{
			it = m_EnemyPlaneVec.erase(it);
			//log(" enemyPlane del ");
			break;;
		}
		else
		{
			++it;
		}
	}
}

void GameData::SortByDis()
{
	std::list<PhysicsUnit*>::iterator it_i = m_EnemyPlaneVec.begin();
	std::list<PhysicsUnit*>::iterator it_j = m_EnemyPlaneVec.begin();
	for (it_i; it_i != m_EnemyPlaneVec.end(); ++it_i)
	{
		for (it_j = it_i; it_j != m_EnemyPlaneVec.end(); ++it_j)
		{
			Point pos_i = (*it_i)->getPosition();
			Point pos_j = (*it_j)->getPosition();
			float dis_i = getDistance(pos_i);
			float dis_j = getDistance(pos_j);
			if (dis_i > dis_j)
			{
				PhysicsUnit* temp = (*it_i);
				(*it_i) = (*it_j);
				(*it_j) = temp;

			}
		}
	}
	

	//输出距离
	//for (it_i = m_EnemyPlaneVec.begin(); it_i != m_EnemyPlaneVec.end(); ++it_i)
	//{
	//	Point pos_i = (*it_i)->getPosition();
	//	float dis_i = getDistance(pos_i);
	//	//log(" dis = %f ",dis_i);
	//}
	//log("----\n");
}

float GameData::getDistance( const Point& pos )
{
	Vec2 vec = Vec2(m_winSize.width * 0.5, 0);
	BasePlane* plane = GameData::getInstance()->getMainPlane();
	if (plane)
	{
		vec = plane->getPosition();
	}
	float dis = pos.distance(vec);
	return dis;
}

void GameData::setMainPlane( MainPlane* plane )
{
	m_mainPlane = plane;
}

MainPlane* GameData::getMainPlane()
{
	return m_mainPlane;
}

void GameData::GameStart()
{
	resetGameData();
}

void GameData::GameOver(bool iswin)
{
	setGameResult(iswin);

	if (m_Playing)
	{
		m_Playing = false;
		if (!iswin)
		{
			UIController::getInstance()->getMainUILayer()->showBuyFuHuoWidget();
		}
		else
		{
			//如果是胜利，那么主飞机飞出屏幕
			auto pl = GameData::getInstance()->getMainPlane();
			if (pl)
			{
				auto mianpl = (MainPlane*)pl;
				mianpl->FlyUp();
			}
		}
	}
}

void GameData::resetGameData()
{
	m_Score = 0;
	m_Golds = 0;
	m_updateSpeed = 1.0;

	m_mainPlane = nullptr;
	m_isSpeedUp = false;
	m_Playing = true;
	m_planeManager->resetData();

	//清空敌机
	m_EnemyPlaneVec.clear();
	m_StatisticsVec.clear();
	m_BossGrade.clear();

	memset(m_statisticsArr, 0, sizeof(int)*GamePrKey_Max );
}

void GameData::SortByHpLimit()
{
	//log("begin target");
	std::list<PhysicsUnit*>::iterator it_i = m_EnemyPlaneVec.begin();
	std::list<PhysicsUnit*>::iterator it_j = m_EnemyPlaneVec.begin();
	for (it_i; it_i != m_EnemyPlaneVec.end(); ++it_i)
	{
		for (it_j = it_i; it_j != m_EnemyPlaneVec.end(); ++it_j)
		{
			PhysicsUnit* pu_i = (*it_i);
			PhysicsUnit* pu_j = (*it_j);
			if (pu_i || pu_j)
			{
				continue;
			}
			float blood_i = (*it_i)->getSelfPro().maxHP;
			float blood_j = (*it_j)->getSelfPro().maxHP;
			if (blood_i < blood_i)
			{
				PhysicsUnit* temp = (*it_i);
				(*it_i) = (*it_j);
				(*it_j) = temp;
			}
		}
	}
}

PhysicsUnit* GameData::getEnemyPlaneByMaxHp()
{
	if (m_EnemyPlaneVec.empty())
	{
		return nullptr;
	}
	//log("********************** size = %d",m_EnemyPlaneVec.size());
	auto it = m_EnemyPlaneVec.begin();
	//查询是否有被锁定的目标，有的话返回该目标
	for (it; it != m_EnemyPlaneVec.end(); ++it)
	{
		PhysicsUnit* pu = (*it);
		if (pu->getContactPro().isTarget && pu->isLiving())
		{
			//log("+++locked+++");
			return pu;
		}
	}

	//没有被标记的目标，那么现在标记一个(先按血量高低排序)
	SortByHpLimit();
	it = m_EnemyPlaneVec.begin();
	for (it; it != m_EnemyPlaneVec.end(); ++it)
	{
		PhysicsUnit* pu = (*it); 

		bool isWudi = pu->getSelfPro().isInvincible;
		bool islive = pu->isLiving();
		if (!isWudi && islive)
		{
			ContactPro rs = pu->getContactPro();
			rs.isTarget = true;
			pu->setContactPro(rs);
			return pu;
		}
	}
	return nullptr;
}

const Vec2& GameData::getNearTargetByPos( const Vec2& curPos )
{
	Vec2 nearPos = Vec2(0,0);
	float dis = 100000.0f;
	auto plVec = m_EnemyPlaneVec;
	for (auto it = plVec.begin(); it != plVec.end(); it++)
	{
		auto pl = (*it);
		if (pl)
		{
			Vec2 pos = pl->convertToWorldSpaceAR(Vec2::ZERO);
			float TempDis = pos.distance(curPos);
			if (TempDis < dis)
			{
				dis = TempDis;
				nearPos = pos;
			}
		}
		else
		{
			log("");
		}
	}
	return nearPos;
}

void GameData::GamePause()
{
	//最好是在Node完全draw出来再调用，或者延迟调用

	m_gamePause = true;

	m_TargetSet = Director::getInstance()->getScheduler()->pauseAllTargets();
	Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(0);
}

void GameData::GameResume()
{
	m_gamePause = false;

	Director::getInstance()->getScheduler()->resumeTargets(m_TargetSet);
	Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(1);
	m_TargetSet.clear();
}

void GameData::GameSlowDown(float duration, bool isToBlack)
{
	//减速比例
	float rate = 0.3;

	Director::getInstance()->getScheduler()->setTimeScale(rate);

	ActionInterval* delay = DelayTime::create(duration*rate);
	CallFunc* func = CallFunc::create(this,callfunc_selector(GameData::GameResumeScale));
	ActionInterval* seq = Sequence::create(delay,func,nullptr);

	Node* node = UIController::getInstance()->getPhysicsLayer();
	node->runAction(seq);

	if (isToBlack)
	{
		//屏幕变黑
		UIController::getInstance()->getPhysicsLayer()->ToBlack(duration);
	}
}

void GameData::GameResumeScale()
{
	Director::getInstance()->getScheduler()->setTimeScale(1.0f);
}

void GameData::addGoldNum(const int& num )
{
	m_Golds += num;

	UIController::getInstance()->getMainUILayer()->updateUIGold( num );
}

void GameData::GameSpeedUp()
{
	//Director::getInstance()->getScheduler()->setTimeScale(1.5f);
	GameData::getInstance()->setUpdateSpeed(50.0f);
}

int GameData::getEmemyNums()
{
	return m_EnemyPlaneVec.size();
}

void GameData::addEnemyStatistics(const SelfPro& selfpro, const UnitId& KillerId)
{
	if (selfpro.idx < UnitId::ePlane_1)
	{
		log("---------------------------- PLANE ID IS ERROR : id = %d--------------------",(int)selfpro.idx);
		return;
	}
	if (KillerId != UnitId::DefaultIdx)
	{
		ES data(selfpro.idx, KillerId, selfpro.score);
		m_StatisticsVec.push_back(data);
	}
}

std::vector<ES> GameData::getEnemyStatistics()
{
	return m_StatisticsVec;
}

int GameData::getSumEnemyStatics()
{
	return m_StatisticsVec.size();
}

void GameData::AnalyseStaticsData()
{
	auto data = GameData::getInstance()->getAllStatistics();
	for (auto it = m_StatisticsVec.begin(); it != m_StatisticsVec.end(); it++)
	{
		ES es = (*it);
		if (es.KillerId == UnitId::eRedCard)
		{
			//红球击杀
			m_statisticsArr[GamePrKey_RedKillerNum] += 1;
			m_statisticsArr[GamePrKey_RedScore] += es.Score;
		}
		else if(es.KillerId == UnitId::eBlueCard)
		{
			//篮球击杀
			m_statisticsArr[GamePrKey_BlueKillerNum] += 1;
			m_statisticsArr[GamePrKey_BlueScore] += es.Score;
		}
		else if(es.KillerId == UnitId::eYellowCard)
		{
			//黄球击杀
			m_statisticsArr[GamePrKey_YellowKillerNum] += 1;
			m_statisticsArr[GamePrKey_YellowScore] += es.Score;
		}
	}

	//击杀总数
	m_statisticsArr[GamePrKey_killMonNum] = m_StatisticsVec.size();

	//评级
	for (auto it = m_BossGrade.begin(); it != m_BossGrade.end(); it++)
	{
		BossGrade bg = (*it);
		if (bg.rateing == BossRating::e_SS)
		{
			m_statisticsArr[GamePrKey_SSGradeNum] += 1;
		}
		else if (bg.rateing == BossRating::e_S)
		{
			m_statisticsArr[GamePrKey_SGradeNum] += 1;
		}
		else if (bg.rateing == BossRating::e_A)
		{
			m_statisticsArr[GamePrKey_AGradeNum] += 1;
		}
		if (bg.rateing == BossRating::e_B)
		{
			m_statisticsArr[GamePrKey_BGradeNum] += 1;
		}
	}
	//GameData::getInstance()->setAllStatistics(m_statisticsArr);
}

bool GameData::isContact( int mask1,int mask2,int mask3, int mask4, int mask5,int mask6 )
{
	bool b = false;
	int v1 = mask1 & mask5;
	int v2 = mask1 & mask6;
	int v3 = mask4 & mask2;
	int v4 = mask4 & mask3;
	int v = mask1 & mask1;
	if (v1 && v2 && v3 && v4)
	{
		b = true;
	}
	return b;
}

bool GameData::isContact_P( int mask1,int mask2,int mask3, int mask4, int mask5,int mask6 )
{
	bool b = false;
	int v1 = mask1 & mask5;
	int v2 = mask1 & mask6;
	int v3 = mask4 & mask2;
	int v4 = mask4 & mask3;

	if (!v1 && v2 && !v3 && v4)
	{
		b = true;
	}
	return b;
}

bool GameData::isGameEnd()
{
	bool b = m_planeManager->getIsGroupOver();

	if (b && m_EnemyPlaneVec.empty())
	{
		return true;
	}

	return false;
}

void GameData::addBossRecord( int bossType, const BossRating& rating )
{
	BossGrade bg;
	bg.bossType = bossType;
	bg.rateing = rating;

	m_BossGrade.push_back(bg);
}

std::vector<BossGrade>& GameData::getBossRecords()
{
	return m_BossGrade;
}

void GameData::addProp()
{
	int chongciNum = NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey(BuyListKey::BuyKey_ChongCi)->number;
	
	if (chongciNum > 0)
	{
		if (m_mainPlane)
		{
			MainPlane* pl = (MainPlane*)m_mainPlane;
			pl->addSpeedUpEffect();
			NewDataMgr::getInstance()->mGameBuySystem->addGoodsNumWithKey(BuyListKey::BuyKey_ChongCi,-1);

			ActionInterval* moveTo = MoveTo::create(1.0f,Vec2(m_winSize.width * 0.5,m_winSize.height * 0.3));
			m_mainPlane->runAction(moveTo);
		}
	}


}

int *GameData::getAllStatistics( void )
{ 
	return m_statisticsArr; 
}

int GameData::getBulletHurtsOfEnemyPl(float userdata)
{
	//计算子弹的伤害
	int curMapId = UIController::getInstance()->getGameBgMgr()->getCurBgId();
	int curLvl = m_planeManager->getCurLvl();

	//公式
	float val1 = NewDataMgr::getInstance()->getEnemyPlaneWithKey()->AtkCoe1;
	float val2 = NewDataMgr::getInstance()->getEnemyPlaneWithKey()->AtkCoe2;
	int ammoHurts = -(pow((userdata * curLvl * curMapId), val1) + val2 * userdata);

	return ammoHurts;
}


