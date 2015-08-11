#include "PlaneManager.h"
#include "../../GameCommon/GameDefine.h"
#include "../../GameCommon/GameData.h"
#include "PlaneFactory.h"
#include "BasePlane.h"
#include "../../GameUtil/TmxUtil.h"
#include "PlaneAction.h"
#include "../Weapon/WeaponFactory.h"
#include "../AI.h"
#include "../Layer/UIController.h"
#include "../Layer/PhysicsLayer.h"
#include "GameUI/GameMainLayer.h"
#include "../../../cocos2d/cocos/editor-support/spine/extension.h"


using namespace experimental;

PlaneManager::PlaneManager()
	:m_GroupTmx(nullptr)
	,m_IsGroupOver(false)
	,m_restTime(-1)
	,m_restCount(0)
{

}

PlaneManager::~PlaneManager()
{
	if (m_GroupTmx)
	{
		m_GroupTmx->release();
	}
}

PlaneManager* PlaneManager::create()
{
	PlaneManager* pm = new (std::nothrow) PlaneManager;
	if (pm && pm->init())
	{
		pm->retain();
		pm->autorelease();
		return pm;
	}
	CC_SAFE_DELETE(pm);
	return nullptr;
}

void PlaneManager::purge()
{
	release();
}

bool PlaneManager::init()
{
	do 
	{
	} while (0);

	return true;
}

void PlaneManager::InitLvlData(const int& lvl )
{
	m_curLvl = lvl;

	GameData::getInstance()->setIsBossDead(false);
 	bool isExsit = readGroupTmx(lvl);
	if (!isExsit)
	{
		//没有下一组了，那么游戏结束
		m_IsGroupOver = true;
		return;
	}

	m_GruopVec.clear();
	m_PlaneGroup.clear();

	m_GruopVec = m_GroupTmx->getObjectGroups();
	m_GruopIt = m_GruopVec.begin();
	if (m_curLvl > 1)
	{
		Rest(10.0f);
	}
	refreshEnemyPlane();
}

bool PlaneManager::readGroupTmx(const int& gameLvl)
{
	bool isExsit = true;
	if (m_GroupTmx)
	{
		m_GroupTmx->release();
		m_GroupTmx = nullptr;
	}
	std::string fileName = __String::createWithFormat("LevelData/Level_%d.tmx",gameLvl)->getCString();

	auto tmx = cocos2d::experimental::TMXTiledMap::create(fileName);
	if (!tmx)
	{
		//如果文件不存在，那么返回false
		isExsit = false;
	}
	else
	{
		m_GroupTmx = tmx;
		m_GroupTmx->retain();
	}
	return isExsit;
}

void PlaneManager::AnalyseOneGroup( TMXObjectGroup* group )
{
	int	ActionId = getNamePro(group,1);						//找到动作ID
	bool isSpeedUp = GameData::getInstance()->getisSpeedUp();

	ValueVector& objects = group->getObjects();
	for (auto& obj : objects)
	{
		ValueMap& dict = obj.asValueMap();

		//位置
		Point pos = getPosByTmx(dict);

		std::string name = dict["name"].asString();
		std::string type = dict["type"].asString();

		//以#分割出2个字符串容器
		std::vector<std::string> idStrVec = TmxUtil::getStrVec(name);
		std::vector<std::string> rateStrVec = TmxUtil::getStrVec(type);

		//敌机
		int plId = TmxUtil::getIntDataByRate(idStrVec.at(0),rateStrVec.at(0));			//从信息解析出的敌机类型中，按概率取得ID
		
		//AI
		int AiId = TmxUtil::getIntDataByRate(idStrVec.at(1),rateStrVec.at(1));
		
		//武器
		int weaponId = TmxUtil::getIntDataByRate(idStrVec.at(2),rateStrVec.at(2));

		//子弹资源
		int bulletRes = TmxUtil::getIntDataByRate(idStrVec.at(3),rateStrVec.at(3));

		//用户数据
		float userdata = TmxUtil::getFloatDataByRate(idStrVec.at(4),rateStrVec.at(4));
		if (userdata <= 0.0f)
		{
			userdata = 1.0f;
		}

		//log("---- ammo id = %d -------",weaponId);
		auto plane = CreateWithPro(pos, plId, weaponId, bulletRes, ActionId, AiId, m_curLvl-1, userdata);

		//掉落物
		AddDropsByTmx(dict,plane);			//从信息解析出的凋落物类型中，按概率取得ID

		AddOnePlane(plane);
	}
}

void PlaneManager::refreshEnemyPlane()
{
	if (m_IsGroupOver)
	{
		return;
	}
	if (m_restTime > 0 )
	{
		m_restCount++;

		//休息
		auto delay = DelayTime::create(m_restTime);
		auto rest = CallFunc::create(this,callfunc_selector(PlaneManager::RestCallback));
		auto seq = Sequence::create(delay, rest, nullptr);
		UIController::getInstance()->getPhysicsLayer()->runAction(seq);
		return;
	}
	if (m_GruopVec.empty() || m_GruopIt == m_GruopVec.end())
	{
		//当前关卡组合全部加载完毕
		return;
	}
	addNextNormalGroup();
}

void PlaneManager::addNextNormalGroup()
{
	if (m_restCount > 0)
	{
		return;
	}
	//加载敌机组合
	TMXObjectGroup* group = (*m_GruopIt);
	AnalyseOneGroup(group);
	m_GruopIt++;
}

cocos2d::Point PlaneManager::getPosByTmx( ValueMap& dict )
{
	//位置
	float x = dict["x"].asFloat();
	float y = dict["y"].asFloat();
	Point pos = ccp(x,y);

	return pos;
}

void PlaneManager::AddDropsByTmx( ValueMap& dict,PhysicsUnit* pl )
{
	std::string drop = dict["drop"].asString();
	std::string rate = dict["dropRate"].asString();

	//凋落物
	int dropId = TmxUtil::getIntDataByRate(drop.c_str(),rate.c_str());			//从信息解析出的凋落物类型中，按概率取得ID
	DropReward dropReward;
	dropReward.DropTp = (DropGoodsType)dropId;
	auto selfpro = pl->getSelfPro();
	selfpro.rewardVec.push_back(dropReward);
	pl->setSelfPro(selfpro);
}

int PlaneManager::getNamePro(TMXObjectGroup* group,const int& idx)
{
	std::string name = group->getGroupName();
	auto vec = TmxUtil::SplitToInt(name,',');		
	int	val = vec.at(idx);

	return val;
}

void PlaneManager::AddNextLevel()
{
	//加载下一TMX文件
	InitLvlData(++m_curLvl);
}

BasePlane* PlaneManager::CreateWithPro(const Vec2& pos, int plId, int weaponId, int bulletRes, int actionId, int AIid, int gameLvl, float userdata )
{
	auto gd = GameData::getInstance();
	auto pl = gd->getPlaneFactory()->createEnemyPlane(plId, userdata, gameLvl);	//敌方飞机
	pl->setPosition(pos);

	EnemyPlane* norPl = dynamic_cast<EnemyPlane*>(pl);
	if (norPl)
	{
		norPl->setWeaponPro(WeaponPro( weaponId, (ResTag)bulletRes, userdata ));
	}

	gd->getPlaneAction()->AddEnemyActionByIdx(actionId,pl);			//飞机出场动作
	gd->getPlaneAiFactory()->AddEnemyAiByIdx(pl,AIid);				//飞机自己的AI
	UIController::getInstance()->getPhysicsLayer()->addChild(pl,-1000);

	return pl;
}

void PlaneManager::Rest( float dur )
{
	m_restTime = dur;
}

void PlaneManager::RestCallback()
{
	m_restCount--;
	m_restTime = -1;
	if (m_restCount < 1)
	{
		refreshEnemyPlane();
	}
}

void PlaneManager::AddOnePlane( PhysicsUnit* pl)
{
	m_PlaneGroup.push_back(pl);
}

void PlaneManager::DelOnePlane(PhysicsUnit* pl)
{
	auto it = m_PlaneGroup.begin();
	for (it; it != m_PlaneGroup.end();++it)
	{
		PhysicsUnit* plane = (*it);
		if (plane == pl)
		{
			m_PlaneGroup.erase(it);
			break;
		}
	}

	//如果当前飞机全部被击杀, 那么加载下一组
	if (m_PlaneGroup.empty())
	{
		if (m_restCount < 1)		//如果在休息阶段，就不加载
		{
			refreshEnemyPlane();
		}
	}
}

void PlaneManager::AddGiftPlaneGroup()
{
	GiftPlaneManager* giftPlMgr = GiftPlaneManager::create();
	UIController::getInstance()->getPhysicsLayer()->addChild(giftPlMgr);
}

void PlaneManager::resetData()
{
	//数据重置
	m_restCount = 0;		
	m_restTime = -1;
	m_IsGroupOver = false;
}


//---------------------------------

void GiftPlaneManager::onEnter()
{
	Node::onEnter();
	addNextGiftGroup();
}

void GiftPlaneManager::addGiftPlaneGroup()
{
	bool isSpeedUp = GameData::getInstance()->getisSpeedUp();
	if (isSpeedUp)
	{
		return;	//冲刺时不出现运输机
	}

	int lvl = GameData::getInstance()->getPlaneManager()->getCurLvl();
	lvl = lvl%4;
	switch (lvl)
	{
	case 1:
		m_PlaneId = 1;
		break;
	case 2:
		m_PlaneId = 6;
		break;
	case 3:
		m_PlaneId = 12;
		break;
	case 0:
		m_PlaneId = 16;
		break;
	default:
		break;
	}
	m_ActionId = random(9,18);

	{
		auto node = Node::create();
		addChild(node);

		auto delay = DelayTime::create(0.3f);
		auto next = CallFunc::create(this, callfunc_selector(GiftPlaneManager::addOneGiftPlane));
		auto seq = Sequence::create(next, delay, nullptr);
		auto rep = Repeat::create(seq, 5);
		auto rm = RemoveSelf::create(true);
		auto seq1 = Sequence::create(rep, rm, nullptr);
		node->runAction(seq1);
	}
}

void GiftPlaneManager::addNextGiftGroup()
{
	addGiftPlaneGroup();

	auto node = Node::create();
	addChild(node);

	auto delay = DelayTime::create(5.0f + rand_0_1() * 3.0f);
	auto next = CallFunc::create(this, callfunc_selector(GiftPlaneManager::addNextGiftGroup));
	auto rm = RemoveSelf::create(true);
	auto seq = Sequence::create(delay, next, rm, nullptr);
	node->runAction(seq);
}

void GiftPlaneManager::addOneGiftPlane()
{
	int lvl = GameData::getInstance()->getPlaneManager()->getCurLvl();
	auto pl = GameData::getInstance()->getPlaneFactory()->createEnemyPlane(m_PlaneId, 0.5f, lvl);

	GameData::getInstance()->getPlaneAction()->AddEnemyActionByIdx(m_ActionId,pl);

	UIController::getInstance()->getPhysicsLayer()->addChild(pl);

	//添加凋落物
	int randNum = random(1,10);
	if (randNum <= 3)
	{
		DropReward dropReward;
		dropReward.DropTp = (DropGoodsType)randNum;
		auto selfpro = pl->getSelfPro();
		selfpro.rewardVec.push_back(dropReward);
		pl->setSelfPro(selfpro);
	}
}

