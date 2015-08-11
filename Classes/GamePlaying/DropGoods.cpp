#include "DropGoods.h"
#include "Bullet/Bullet.h"
#include "Layer/PhysicsLayer.h"
#include "Layer/UIController.h"
#include "Weapon/WeaponFactory.h"
#include "Weapon/weapon.h"
#include "../GameCommon/GameData.h"
#include "../GameCommon/GameConfig.h"
#include "../GameUI/GameMainLayer.h"
#include "GameCommon/NewDataMgr.h"
#include "../GameCommon/EffectsManager.h"
#include "GameCommon/GameDefine.h"
#include "Bullet/BulletFactory.h"
#include "PhysicsUnit.h"

//凋落物资源
const char* DropRes_Gold3 = "Drops/Gold3.png";
const char* DropRes_BigPower = "Drops/power_big.png";
const char* DropRes_SmallPower = "Drops/power_small.png";

//掉落物价值
const int GoldType1Nums = 18;
const int GoldType2Nums = 88;
const int GoldType3Nums = 188;
const int addHp100 = 100;
const int addHp500 = 500;
const float magnetTime = 10.0f;	//磁铁时间


DropGold* DropGold::create(const Vec2& pos, ResTag res,  int glod, int score)
{
	DropGold* dg = new (std::nothrow) DropGold();
	auto frameName = DropsFactory::getResByTag(res);
	if (dg && dg->initWithSpriteFrameName(frameName))
	{
		dg->m_DropMod = DropMod::Place_Mod;
		dg->m_StartPos = pos;
		dg->autorelease();
		dg->initBody();
		return dg;
	}
	CC_SAFE_DELETE(dg);
	return nullptr;
}

void DropGold::onEnter()
{
	PhysicsUnit::onEnter();
	//OrbitCamera* orbit = OrbitCamera::create(rand_0_1()*2, 100, 0, 0, 720, 0, 360);
	//runAction( RepeatForever::create( orbit ) );

	InitSpeed();

	schedule(schedule_selector(DropGold::IsMainPlHasMagnet));
	schedule(schedule_selector(DropGold::MoveToMainPl));
}

void DropGold::initBody()
{
	auto body = PhysicsBody::createBox(getContentSize());
	body->setGravityEnable(false);
	if (m_DropMod == DropMod::Drop_Mod)		//掉落模式，受重力影响
	{
		body->setGravityEnable(true);
	}
	body->setRotationEnable(true);
	body->setMass(1);
	body->setCategoryBitmask(REWARD_CATEGORYBITMASK);
	body->setCollisionBitmask(REWARD_COLLISIONBITMASK);
	body->setContactTestBitmask(REWARD_CONTACTTESTBITMASK);
	setPhysicsBody(body);

}

void DropGold::InitEdgeLine()
{
	m_EdgeLineVec.push_back(20);
	m_EdgeLineVec.push_back(m_WinSize.width -20);
	m_EdgeLineVec.push_back(-100);
	m_EdgeLineVec.push_back(m_WinSize.height + 500);
}

void DropGold::DoSomethingBeforeDead()
{
	EnemyPlaneController::getInstance()->dropDestroyCallBack(this);
	if (m_isNaturalDead)
	{
		return;
	}

	if (m_isDirectDeath)
	{
		return;
	}

	StatictisNum();
	Rewards();
}

void DropGold::InitSpeed()
{
	switch (m_DropMod)
	{
	case Place_Mod:
		{
			setPosition(m_StartPos);
			//向下运动
			_physicsBody->setVelocity(Vec2(0,-700));
		}
		break;
	case Drop_Mod:
		{
			float offsetX = (rand_0_1() - 0.5) * 100;
			auto myPos = getPosition();
			if (myPos.x > m_WinSize.width * 0.9)
			{
				offsetX = -rand_0_1() * 50;
			}
			else if(myPos.x < m_WinSize.width * 0.1)
			{
				offsetX = rand_0_1() * 50;
			}
			//给一个向上的随机速度
			Vec2 vec = Vec2(offsetX, rand_0_1() * 100 + m_UpSpeed);
			_physicsBody->setVelocity(vec);
		}
		break;
	default:
		break;
	}
	
}

void DropGold::updateRewards(const int& rewardtype )
{
	//如果在加速冲刺阶段，接到(蛋)掉落物不再有影响
	bool b = GameData::getInstance()->getisSpeedUp();
	if (b)
	{
		return;
	}
}

void DropGold::ActionCallback( Node* node )
{
	node->removeFromParentAndCleanup(true);
}

void DropGold::PlayMusicWhenDead()
{
	NewAudio::getInstance()->playEff(Music_getGolds);
}

void DropGold::StatictisNum()
{
	auto data = GameData::getInstance()->getAllStatistics();
	if (m_SelfPro.idx == UnitId::eRedCard)
	{
		data[GamePrKey_getRedBallNum]++;
	}
	else if(m_SelfPro.idx == UnitId::eBlueCard)
	{
		data[GamePrKey_getBlueBallNum]++;
	}
	else if(m_SelfPro.idx == UnitId::eYellowCard)
	{
		data[GamePrKey_getYellowBallNum]++;
	}
	//GameData::getInstance()->setAllStatistics(data);

}

void DropGold::IsMainPlHasMagnet( float dt )
{
	bool isHas = false;
	auto mainpl = GameData::getInstance()->getMainPlane();
	if (mainpl)
	{
		isHas = mainpl->getIsHasMagnet();
	}

	if(isHas)
	{
		m_MagnetDis = 400;
		unschedule(schedule_selector(DropGold::IsMainPlHasMagnet));
	}
}

void DropGold::MoveToMainPl( float dt )
{
	auto mainPl = GameData::getInstance()->getMainPlane();
	if (!mainPl)
	{
		return;
	}
	auto myPos = convertToWorldSpaceAR(Vec2::ZERO);
	auto plPos = mainPl->convertToWorldSpaceAR(Vec2::ZERO) + Vec2(0,20);
	
	float dis = myPos.distance(plPos);
	if (dis < m_MagnetDis)
	{
		_physicsBody->setGravityEnable(false);
		auto newPos = MoveStep(plPos, myPos, 35);
		setPosition(newPos);
	}
}

void DropGold::setUpSpeed( float speed )
{
	m_UpSpeed = speed;
}

void DropGold::setRewardsInfo( int gold /*= 0*/, int score /*= 0*/ )
{
	m_SelfPro.goldsNum = gold;
	m_SelfPro.score = score;
}


//-------------------
DropsFactory* DropsFactory::create()
{
	DropsFactory* sf = new (std::nothrow) DropsFactory;
	if (sf && sf->init())
	{
		sf->retain();
		sf->autorelease();
		return sf;
	}
	else
	{
		CC_SAFE_DELETE(sf);
		return nullptr;
	}
	
}

bool DropsFactory::init()
{
	return true;
}

void DropsFactory::purge()
{
	this->release();
}

void DropsFactory::AddDrops(const Point& pos, DropGoodsType tp, int nums)
{
	for (int i = 0; i < nums; ++i)
	{
		switch (tp)
		{
		case DropGoodsType::DropGoods_Gold1:
			DropGoldsType1(pos);
			break;
		case DropGoodsType::DropGoods_Gold2:
			DropGoldsType2(pos);
			break;
		case DropGoodsType::DropGoods_Gold3:
			DropGoldsType3(pos);
			break;
		case DropGoodsType::DropGoods_RedWp:
			DropWithRedWp(pos);
			break;
		case DropGoodsType::DropGoods_YellowWp:
			DropWithYellowWp(pos);
			break;
		case DropGoodsType::DropGoods_BlueWp:
			DropWithBlueWp(pos);
			break;
		case DropGoodsType::DropGoods_NorWpEx:
			//加强普通子弹BUFF
			{
				int curRole = NewDataMgr::getInstance()->mRoleConfig.roleIndex;
				switch (curRole)
				{
				case 1:
					DropWithWpBuff1(pos);
					break;
				case 2:
					DropWithWpBuff2(pos);
					break;
				case 3:
					DropWithWpBuff3(pos);
					break;

				default:
					break;
				}
			}
			break;
		case DropGoodsType::DropGoods_AddHp100:
			AddHp100(pos);
			break;
		case DropGoodsType::DropGoods_AddHp500:
			AddHp500(pos);
			break;
		case DropGoodsType::DropGoods_Magnet:
			DropMagnet(pos);
			break;
		default:
			break;
		}
	}
}

void DropsFactory::DropGoldsType1(const Point& pos)
{
	auto p = getRandPos(pos);

	SelfPro sp;
	sp.goldsNum = GoldType1Nums;

	auto drop = DropGold::create(getResByTag(ResTag::Drops_Golds1),350);
	drop->setSelfPro(sp);

	UIController::getInstance()->addScore(drop,p);
}

void DropsFactory::DropGoldsType2( const Point& pos)
{
	auto p = getRandPos(pos);

	SelfPro sp;
	sp.goldsNum = GoldType2Nums;

	auto drop = DropGold::create(getResByTag(ResTag::Drops_Golds2),300);
	drop->setSelfPro(sp);

	UIController::getInstance()->addScore(drop,p);
}

void DropsFactory::DropGoldsType3( const Point& pos)
{
	auto p = getRandPos(pos);

	SelfPro sp;
	sp.goldsNum = GoldType3Nums;

	auto drop = DropGold::create(getResByTag(ResTag::Drops_Golds3),250);
	drop->setSelfPro(sp);

	UIController::getInstance()->addScore(drop,p);
}

void DropsFactory::DropWithRedWp( const Point& pos )
{
	auto p = getRandPos(pos);

	auto drop = DropRedWp::create(getResByTag(ResTag::Drops_RedWp));
	drop->setUnitId(UnitId::eRedCard);
	UIController::getInstance()->addScore(drop,p);
}

void DropsFactory::DropWithBlueWp( const Point& pos )
{
	auto p = getRandPos(pos);

	auto drop = DropBlueWp::create(getResByTag(ResTag::Drops_BlueWp));
	drop->setUnitId(UnitId::eBlueCard);
	UIController::getInstance()->addScore(drop,p);
}

void DropsFactory::DropWithYellowWp( const Point& pos )
{
	auto p = getRandPos(pos);

	auto drop = DropYellowWp::create(getResByTag(ResTag::Drops_YellowWp));
	drop->setUnitId(UnitId::eYellowCard);
	UIController::getInstance()->addScore(drop,p);
}

void DropsFactory::DropWithWpBuff1( const Vec2& pos )
{
	auto p = getRandPos(pos);

	auto drop = WeaponUpgradeDrop::create(getResByTag(ResTag::Drops_WpBuff1));
	//drop->setBuff(BuffStruct(BuffId::Buff_TempNorWp1,20.0f));

	UIController::getInstance()->addScore(drop,p);
}

void DropsFactory::DropWithWpBuff2( const Vec2& pos )
{
	auto p = getRandPos(pos);

	auto drop = WeaponUpgradeDrop::create(getResByTag(ResTag::Drops_WpBuff1));
	//drop->setBuff(BuffStruct(BuffId::Buff_TempNorWp3,20.0f));

	UIController::getInstance()->addScore(drop,p);
}

void DropsFactory::DropWithWpBuff3( const Vec2& pos )
{
	auto p = getRandPos(pos);

	auto drop = WeaponUpgradeDrop::create(getResByTag(ResTag::Drops_WpBuff1));
	//drop->setBuff(BuffStruct(BuffId::Buff_TempNorWp2,20.0f));

	UIController::getInstance()->addScore(drop,p);
}

void DropsFactory::AddHp100( const Vec2& pos )
{
	auto p = getRandPos(pos);

	auto drop = HpAdditionDrop::create(getResByTag(ResTag::Drops_AddHp100));
	drop->setAddHpNums(addHp100);

	UIController::getInstance()->addScore(drop,p);
}

void DropsFactory::AddHp500( const Vec2& pos )
{
	auto p = getRandPos(pos);

	auto drop = HpAdditionDrop::create(getResByTag(ResTag::Drops_AddHp500));
	drop->setAddHpNums(addHp500);

	UIController::getInstance()->addScore(drop,p);
}

void DropsFactory::DropMagnet( const Vec2& pos )
{
	auto p = getRandPos(pos);

	auto drop = MainPlaneMagnet::create(getResByTag(ResTag::Drops_Magnet));
	UIController::getInstance()->addScore(drop,p);
}

std::string DropsFactory::getResByTag( ResTag tag )
{
	std::string res = "";
	switch (tag)
	{
	case ResTag::Drops_WpBuff1:
		res = "WpBuff1.png";
		break;
	case ResTag::Drops_RedWp:
		res = "redWp.png";
		break;
	case ResTag::Drops_YellowWp:
		res = "yellowWp.png";
		break;
	case ResTag::Drops_BlueWp:
		res = "blueWp.png";
		break;
	case ResTag::Drops_Golds1:
		res = "DropGold1.png";
		break;
	case ResTag::Drops_Golds2:
		res = "DropGold2.png";
		break;
	case ResTag::Drops_Golds3:
		res = "DropGold3.png";
		break;
	case ResTag::Drops_AddHp100:
		res = "DropHp100.png";
		break;
	case ResTag::Drops_AddHp500:
		res = "DropHp500.png";
		break;
	case ResTag::Drops_Magnet:
		res = "DropMagnet.png";
		break;
	default:
		break;
	}
	return res;
}

cocos2d::Vec2 DropsFactory::getRandPos(const Vec2& centerPos)
{
	float randNumX = rand_0_1() - 0.5;
	float randNumY = rand_0_1() - 0.5;
	return Vec2(centerPos.x + 100 * randNumX,centerPos.y + 150 * randNumY);
}


void DropRedWp::OtherDealWhenContact( const ContactPro& rs )
{
	UIController::getInstance()->getEffectsLayer()->playEffGetRewards(1);
}

void DropRedWp::delayGive()
{
	bool b = UIController::getInstance()->getMainUILayer()->isRealDead();
	if (b)
	{
		return;
	}
	//根据卡片类型判断接到掉落物，促发什么技能
	auto cardType = NewDataMgr::getInstance()->mRoleConfig.cardArr[0].cardID;
	//log("------ cardType	= %d", cardType);
	auto cardLvl = NewDataMgr::getInstance()->mRoleConfig.cardArr[0].cardStarLv;
	cardType = cardType%3;

	//log("------ cardType	= %d", cardType);
	//log("++++++ cardlvl		= %d", cardLvl);

	int hurt = DataConfig::getCardHurts(cardType, cardLvl);

	switch (cardType)
	{
	case 1:
		{
			//花
			addFlower(cardLvl, hurt);
		}
		break;
	case 2:
		{
			//飞镖
			addFeibiao(cardLvl, hurt);
		}
		break;
	case 0:
		{
			if (cardLvl <= 2)
			{
				cardLvl = 1;
			}
			else if(cardLvl <= 4)
			{
				cardLvl = 2;
			}
			else
			{
				cardLvl = 3;
			}
			//火圈
			addFireCircle(cardLvl, hurt);
		}
		break;
	default:
		break;
	}
}

void DropRedWp::onEnter()
{
	WpDrops::onEnter();

}

void DropYellowWp::delayGive()
{
	bool b = UIController::getInstance()->getMainUILayer()->isRealDead();
	if (b)
	{
		return;
	}
	//根据卡片类型判断接到掉落物，促发什么技能
	auto cardType = NewDataMgr::getInstance()->mRoleConfig.cardArr[2].cardID;
	auto cardLvl = NewDataMgr::getInstance()->mRoleConfig.cardArr[2].cardStarLv;
	cardType %= 3;
	//log("------ cardType	= %d", cardType);
	//log("++++++ cardlvl		= %d", cardLvl);

	int hurt = DataConfig::getCardHurts(cardType, cardLvl);

	switch (cardType)
	{
	case 1:
		{
			//龙
			addDragon(cardLvl, hurt);
		}
		break;
	case 2:
		{
			//激光
			addLaser(cardLvl, hurt);
		}
		break;
	case 0:
		{
			//闪电
			//addLighting(cardLvl); //写在碰撞之后无用
		}
		break;

	default:
		break;
	}
}

void DropYellowWp::OtherDealWhenContact( const ContactPro& rs )
{
	UIController::getInstance()->getEffectsLayer()->playEffGetRewards(2);
}

void DropYellowWp::onEnter()
{
	WpDrops::onEnter();
	//根据卡片类型判断接到掉落物，促发什么技能
	auto cardType = NewDataMgr::getInstance()->mRoleConfig.cardArr[2].cardID;
	auto cardLvl = NewDataMgr::getInstance()->mRoleConfig.cardArr[2].cardStarLv;
	cardType = cardType%3;

	int hurt = DataConfig::getCardHurts(cardType, cardLvl);

	switch (cardType)
	{
	case 0:
		{
			//闪电
			addLighting(cardLvl, hurt);
		}
		break;

	default:
		break;
	}
}

void DropBlueWp::delayGive()
{
	bool b = UIController::getInstance()->getMainUILayer()->isRealDead();
	if (b)
	{
		return;
	}
	//根据卡片类型判断接到掉落物，促发什么技能
	auto cardType = NewDataMgr::getInstance()->mRoleConfig.cardArr[1].cardID;
	//log("------ cardType	= %d", cardType);
	auto cardLvl = NewDataMgr::getInstance()->mRoleConfig.cardArr[1].cardStarLv;
	cardType = cardType%3;
	//log("------ cardType	= %d", cardType);
	//log("++++++ cardlvl		= %d", cardLvl);

	int hurt = DataConfig::getCardHurts(cardType, cardLvl);
	switch (cardType)
	{
	case 1:
		{
			//水泡
			addMissile(cardLvl, hurt);
		}
		break;
	case 2:
		{
			//猫
			addCat(cardLvl, hurt);
		}
		break;
	case 0:
		{
			//冰剑
			addIcePiton(cardLvl, hurt);
		}
		break;

	default:
		break;
	}
}

void DropBlueWp::OtherDealWhenContact( const ContactPro& rs )
{
	UIController::getInstance()->getEffectsLayer()->playEffGetRewards(3);
}

void DropBlueWp::onEnter()
{
	WpDrops::onEnter();
}

//-------------------
void WpDrops::Rewards()
{
	bool isSpeedUp = GameData::getInstance()->getisSpeedUp();
	if (isSpeedUp)
	{
		//冲刺阶段，不触发
		return;
	}
	auto pl = GameData::getInstance()->getMainPlane();
	if (!pl)
	{
		return;
	}
	ActionInterval* delay = DelayTime::create(1.0f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(WpDrops::delayGive));
	ActionInterval* seq = Sequence::create(delay,func, nullptr);
	pl->runAction(seq);
}

void WpDrops::delayGive()
{
	
}

void WpDrops::PlayMusicWhenDead()
{
	//播放音效
	NewAudio::getInstance()->playEff(Music_getDrops);
}

void WpDrops::addFeibiao( int lvl, int hurts )
{
	auto wpfactory = GameData::getInstance()->getWeaponFactory();
	auto pl = GameData::getInstance()->getMainPlane();
	if (!pl)
	{
		return;
	}
	
	switch (lvl)
	{
	case 1:
	case 2:
		//小飞镖
		for (int i = 0; i < 5; i++)
		{
			int Hurt = hurts/3;
			auto wp = wpfactory->SpecialWp_SmallDart_S(pl,Hurt,10.0,0.5);
			wp->setRotation((i-2) * 10);
			ActionInterval* rotate = RotateBy::create(0.2f,6);
			ActionInterval* rep = RepeatForever::create(rotate);
			wp->runAction(rep);
			wp->setBltSpeed(500);
		}
		break;
	case 3:
	case 4:
		{
			//3个飞镖
			auto pos = pl->convertToWorldSpaceAR(Vec2::ZERO);

			//掉落飞镖
			auto bulletfactory = GameData::getInstance()->getBulletFactory();
			{
				DartBullet* bullet = DartBullet::create(bulletfactory->getResByTag(ResTag::Bullet_Card_BigDart));
				bullet->setPosition(pos);
				bullet->setScale(2.0f);
				bullet->resetHurt(hurts * 0.6);
				bullet->setBelongTo(UnitId::eRedCard);	//设置为属于红球
				UIController::getInstance()->getPhysicsLayer()->addChild(bullet);
			}
			for (int i = 0; i < 2; i++)
			{
				int direct = (i-0.5) * 2.0f;
				DartBulletRebounce* bullet = DartBulletRebounce::create(bulletfactory->getResByTag(ResTag::Bullet_Card_SmallDart),direct);
				bullet->setPosition(pos);
				bullet->setScale(1.0f);
				bullet->resetHurt(hurts * 0.2);
				bullet->setBelongTo(UnitId::eRedCard);	//设置为属于红球
				UIController::getInstance()->getPhysicsLayer()->addChild(bullet);
			}
		}
		break;
	case 5:
		{
			int Hurt = hurts/10;

			auto bulletFac = GameData::getInstance()->getBulletFactory();
			auto pos = pl->convertToWorldSpaceAR(Vec2::ZERO);
			auto bullet = DartBulletTrace::create(bulletFac->getResByTag(ResTag::Bullet_Card_BigDart));
			bullet->setPosition(pos);
			bullet->resetHurt(Hurt);
			bullet->setBelongTo(UnitId::eRedCard);	//设置为属于红球
			UIController::getInstance()->getPhysicsLayer()->addChild(bullet);
		}
		break;

	default:
		break;
	}
}

void WpDrops::addCat( int lvl,int hurts )
{
	auto pl = GameData::getInstance()->getMainPlane();
	if (!pl)
	{
		return;
	}
	switch (lvl)
	{
	case 1:
	case 2:
		{
			auto pos = pl->convertToWorldSpaceAR(Vec2::ZERO);
			pos.y += 100;
			//掉落猫
			{
				auto bullet = CatBullet::create(true);
				bullet->setPosition(pos);
				bullet->resetHurt(hurts * 0.5);
				bullet->setBelongTo(UnitId::eBlueCard);	//设置为属于篮球
				UIController::getInstance()->getEffectsLayer()->addChild(bullet);
			}
			{
				auto bullet = CatBullet::create(false);
				bullet->setPosition(pos);
				bullet->resetHurt(hurts * 0.5);
				bullet->setBelongTo(UnitId::eBlueCard);	//设置为属于篮球
				UIController::getInstance()->getEffectsLayer()->addChild(bullet);
			}
		}
		break;
	case 3:
	case 4:
		{
			auto pos = pl->convertToWorldSpaceAR(Vec2::ZERO);
			pos.y += 100;
			//掉落猫
			{
				auto bullet = CatBullet::create(true, CatBullet::wolf);
				bullet->setPosition(pos);
				bullet->resetHurt(hurts * 0.5);
				bullet->setBelongTo(UnitId::eBlueCard);	//设置为属于篮球
				UIController::getInstance()->getEffectsLayer()->addChild(bullet);
			}
			{
				auto bullet = CatBullet::create(false,CatBullet::wolf);
				bullet->setPosition(pos);
				bullet->resetHurt(hurts * 0.5);
				bullet->setBelongTo(UnitId::eBlueCard);	//设置为属于篮球
				UIController::getInstance()->getEffectsLayer()->addChild(bullet);
			}
		}
		break;
	case 5:
		{
			auto pos = pl->convertToWorldSpaceAR(Vec2::ZERO);
			pos.y += 100;
			//掉落猫
			{
				auto bullet = CatBullet::create(true, CatBullet::tiger);
				bullet->setPosition(pos);
				bullet->resetHurt(hurts * 0.5);
				bullet->setBelongTo(UnitId::eBlueCard);	//设置为属于篮球
				UIController::getInstance()->getEffectsLayer()->addChild(bullet);
			}
			{
				auto bullet = CatBullet::create(false,CatBullet::tiger);
				bullet->setPosition(pos);
				bullet->resetHurt(hurts * 0.5);
				bullet->setBelongTo(UnitId::eBlueCard);	//设置为属于篮球
				UIController::getInstance()->getEffectsLayer()->addChild(bullet);
			}
		}
		break;

	default:
		break;

	}
}

void WpDrops::addLaser( int lvl,int hurts )
{
	auto pl = GameData::getInstance()->getMainPlane();
	if (!pl)
	{
		return;
	}
	int Hurt = hurts/10;

	switch (lvl)
	{
	case 1:
	case 2:
		{
			auto mainLaser = GameData::getInstance()->getWeaponFactory()->
				SpecialWP_Laser_S(pl,Hurt,5.0f,1.0f);
		}
		break;
	case 3:
	case 4:
		{
			auto skinSize = pl->getContentSize();

			auto mainLaser = LaserBullet::create(2);
			mainLaser->setBelongTo(UnitId::eYellowCard);	//设置为属于黄球
			mainLaser->setPosition(Vec2(skinSize.width*0.5, skinSize.height*0.5));
			mainLaser->resetHurt(Hurt * 0.6);
			pl->addChild(mainLaser,-1);

			auto outside3 = OutsideLaser::create(Vec2(-80,0),-1);
			outside3->setPosition(Vec2(skinSize.width*0.5 - 80, skinSize.height*0.5 - 50));
			outside3->resetHurt(Hurt * 0.2);
			outside3->setBelongTo(UnitId::eYellowCard);	//设置为属于黄球
			pl->addChild(outside3);

			auto outside6 = OutsideLaser::create(Vec2(80,0),1);
			outside6->setPosition(Vec2(skinSize.width*0.5 + 80, skinSize.height*0.5 - 50));
			outside6->resetHurt(Hurt * 0.2);
			outside6->setBelongTo(UnitId::eYellowCard);	//设置为属于黄球
			pl->addChild(outside6);
		}
		break;
	case 5:
		{
			auto skinSize = pl->getContentSize();

			auto mainLaser = LaserBullet::create(3);
			mainLaser->setBelongTo(UnitId::eYellowCard);	//设置为属于黄球
			mainLaser->setPosition(Vec2(skinSize.width*0.5, skinSize.height*0.5));
			mainLaser->setScale(1.3);
			mainLaser->resetHurt(Hurt * 0.6);
			pl->addChild(mainLaser,-1);

			auto outside3 = OutsideLaser::create(Vec2(-120,0),-1);
			outside3->setPosition(Vec2(skinSize.width*0.5 - 120, skinSize.height*0.5 - 50));
			outside3->resetHurt(Hurt * 0.2);
			outside3->setBelongTo(UnitId::eYellowCard);	//设置为属于黄球
			pl->addChild(outside3);

			auto outside6 = OutsideLaser::create(Vec2(120,0),1);
			outside6->setPosition(Vec2(skinSize.width*0.5 + 120, skinSize.height*0.5 - 50));
			outside6->resetHurt(Hurt * 0.2);
			outside6->setBelongTo(UnitId::eYellowCard);	//设置为属于黄球
			pl->addChild(outside6);
		}
		break;
	default:
		break;
	}

}

void WpDrops::addFlower( int lvl,int hurts )
{
	auto pl = GameData::getInstance()->getMainPlane();
	if (!pl)
	{
		return;
	}
	switch (lvl)
	{
	case 1:
	case 2:
		{
			auto pos = pl->convertToWorldSpaceAR(Vec2::ZERO);
			auto bullet = ThreePetalsBullet::create("particla/Petals/Lvl_1/flower_lvl1.png");
			bullet->setPosition(pos);
			bullet->setBelongTo(UnitId::eRedCard);	//设置为属于红球
			bullet->resetHurt(hurts);
			bullet->setScale(1.5f);
			UIController::getInstance()->getEffectsLayer()->addChild(bullet);
		}
		break;
	case 3:
	case 4:
		{
			auto pos = pl->convertToWorldSpaceAR(Vec2::ZERO);
			auto bullet = ThreePetalsBullet::create("particla/Petals/flower.png");
			bullet->setPosition(pos);
			bullet->setScale(2.5f);
			bullet->setBelongTo(UnitId::eRedCard);	//设置为属于红球
			bullet->resetHurt(hurts);
			UIController::getInstance()->getEffectsLayer()->addChild(bullet);
		}
		break;
	case 5:
		{
			int Hurt = hurts/10;

			auto pos = pl->convertToWorldSpaceAR(Vec2::ZERO);
			FlowerBullet* bullet = FlowerBullet::create();
			bullet->setPosition(pos);
			bullet->resetHurt(Hurt);
			bullet->setBelongTo(UnitId::eRedCard);	//设置为属于红球
			UIController::getInstance()->getPhysicsLayer()->addChild(bullet,-1);
		}
		break;

	default:
		break;
	}
}

void WpDrops::addMissile( int lvl,int hurts )
{
	auto pl = GameData::getInstance()->getMainPlane();
	if (!pl)
	{
		return;
	}
	switch (lvl)
	{
	case 1:
	case 2:
		{
			{
				auto wp = GameData::getInstance()->getWeaponFactory()->SpecialWP_IceMisile_S(pl,hurts*0.5,10.0f,0.5f);
				wp->setOffset(Vec2(-30,20));
				wp->setRotation(-70);
			}
			{
				auto wp = GameData::getInstance()->getWeaponFactory()->SpecialWP_IceMisile_S(pl,hurts*0.5,10.0f,0.5f);
				wp->setOffset(Vec2(30,20));
				wp->setRotation(70);
			}
		}
		break;
	case 3:
	case 4:
		{
			//2个小导弹在两边
			{
				auto wp = GameData::getInstance()->getWeaponFactory()->SpecialWP_IceMisile_S(pl,hurts*0.25,10.0f,0.5f);
				wp->setOffset(Vec2(-30,20));
				wp->setRotation(-70);
			}
			{
				auto wp = GameData::getInstance()->getWeaponFactory()->SpecialWP_IceMisile_S(pl,hurts*0.25,10.0f,0.5f);
				wp->setOffset(Vec2(30,20));
				wp->setRotation(70);
			}
			//1个大导弹在中间
			{
				auto wp = GameData::getInstance()->getWeaponFactory()->SpecialWP_IceMisile_S(pl,hurts*0.5,10.0f,0.5f);
				wp->setOffset(Vec2(0,50));
				wp->setBltScaleX(3.0);
				wp->setBltScaleY(3.0);
			}
		}
		break;
	case 5:
		{
			{
				auto wp = GameData::getInstance()->getWeaponFactory()->SpecialWP_IceMisile_S(pl,hurts*0.25,10.0f,1.0f);
				wp->setOffset(Vec2(-20,-40));
				wp->setDelayShoot(0.3f);
				wp->setRotation(-90);
			}
			{
				auto wp = GameData::getInstance()->getWeaponFactory()->SpecialWP_IceMisile_S(pl,hurts*0.25,10.0f,1.0f);
				wp->setOffset(Vec2(20,-40));
				wp->setDelayShoot(0.3f);
				wp->setRotation(90);
			}
			{
				auto wp = GameData::getInstance()->getWeaponFactory()->SpecialWP_IceMisile_S(pl,hurts*0.25,10.0f,1.0f);
				wp->setOffset(Vec2(-30,20));
				wp->setRotation(-50);
				wp->setBltScaleX(1.5);
				wp->setBltScaleY(1.5);
			}
			{
				auto wp = GameData::getInstance()->getWeaponFactory()->SpecialWP_IceMisile_S(pl,hurts*0.25,10.0f,1.0f);
				wp->setOffset(Vec2(30,20));
				wp->setRotation(50);
				wp->setBltScaleX(1.5);
				wp->setBltScaleY(1.5);
			}
		}
		break;

	default:
		break;
	}


	
}

void WpDrops::addDragon( int lvl,int hurts )
{
	auto pl = GameData::getInstance()->getMainPlane();
	if (!pl)
	{
		return;
	}
	hurts /= 5;
	switch (lvl)
	{
	case 1:
	case 2:
		{
			auto pos = pl->convertToWorldSpaceAR(Vec2::ZERO);
			FireDragon* bullet = FireDragon::create(Flaming::eSmall);
			bullet->setPosition(pos);
			bullet->resetHurt(hurts);
			bullet->setBelongTo(UnitId::eYellowCard);	//设置为属于黄球
			UIController::getInstance()->getPhysicsLayer()->addChild(bullet,-1);
		}
		break;
	case 3:
	case 4:
		{
			auto pos = pl->convertToWorldSpaceAR(Vec2::ZERO);
			FireDragon* bullet = FireDragon::create(Flaming::eMiddle);
			bullet->setPosition(pos);
			bullet->resetHurt(hurts);
			bullet->setBelongTo(UnitId::eYellowCard);	//设置为属于黄球
			UIController::getInstance()->getPhysicsLayer()->addChild(bullet,-1);
		}
		break;
	case 5:
		{
			auto pos = pl->convertToWorldSpaceAR(Vec2::ZERO);
			FireDragon* bullet = FireDragon::create(Flaming::eBig);
			bullet->setPosition(pos);
			bullet->resetHurt(hurts);
			bullet->setBelongTo(UnitId::eYellowCard);	//设置为属于黄球
			UIController::getInstance()->getPhysicsLayer()->addChild(bullet,-1);
		}
		break;

	default:
		break;
	}
}

void WpDrops::addFireCircle( int lvl,int hurts )
{
	auto mainPl = GameData::getInstance()->getMainPlane();
	if (!mainPl)
	{
		return;
	}

	auto pos = mainPl->convertToWorldSpaceAR(Vec2::ZERO);
	auto s = mainPl->getContentSize();
	BulletSurroundMainPl* bullet = BulletSurroundMainPl::create(lvl);
	bullet->setPosition(s.width*0.5,s.height*0.5);
	bullet->resetHurt(hurts);
	bullet->setBelongTo(UnitId::eRedCard);
	mainPl->addChild(bullet);
}

void WpDrops::addIcePiton( int lvl,int hurts )
{
	auto mainPl = GameData::getInstance()->getMainPlane();
	if (!mainPl)
	{
		return;
	}
	auto pos = mainPl->convertToWorldSpaceAR(Vec2::ZERO);

	switch (lvl)
	{
	case 1:
	case 2:
		for (int i = 0; i < 2; ++i)
		{
			AirazorBullet* bullet = AirazorBullet::create("ammo/sword/sword1.png",(i - 0.5)*60.0f,false);
			bullet->setPosition(pos);
			bullet->setBelongTo(UnitId::eBlueCard);	//设置为属于篮球
			bullet->resetHurt(hurts/2);
			UIController::getInstance()->getPhysicsLayer()->addChild(bullet);
		}
		break;
	case 3:
	case 4:
		for (int i = 0; i < 3; ++i)
		{
			AirazorBullet* bullet = AirazorBullet::create("ammo/sword/sword2.png", (i - 1)*40.0f,false);
			bullet->setPosition(pos);
			bullet->setBelongTo(UnitId::eBlueCard);	//设置为属于篮球
			bullet->resetHurt(hurts/3);
			UIController::getInstance()->getPhysicsLayer()->addChild(bullet);
		}
		break;
	case 5:
		for (int i = 0; i < 2; ++i)
		{
			AirazorBullet* bullet = AirazorBullet::create("ammo/sword/sword3.png", (i - 0.5)*40.0f,true);
			bullet->setPosition(pos);
			bullet->setBelongTo(UnitId::eBlueCard);	//设置为属于篮球
			bullet->resetHurt(hurts/4);
			UIController::getInstance()->getPhysicsLayer()->addChild(bullet);
		}
		for (int i = 0; i < 2; ++i)
		{
			AirazorBullet* bullet = AirazorBullet::create("ammo/sword/sword3.png", (i - 0.5)*120.0f,true);
			bullet->setPosition(pos);
			bullet->setBelongTo(UnitId::eBlueCard);	//设置为属于篮球
			bullet->resetHurt(hurts/4);
			UIController::getInstance()->getPhysicsLayer()->addChild(bullet);
		}
		break;

	default:
		break;
	}
}

void WpDrops::addLighting( int lvl,int hurts )
{
	switch (lvl)
	{
	case 1:
	case 2:
		{
			BuffStruct buff(BuffId::Buff_LightingLvl1,5.0f);
			m_ContactPro.buffPro = buff;
		}
		break;
	case 3:
	case 4:
		{
			BuffStruct buff(BuffId::Buff_LightingLvl2,8.0f);
			m_ContactPro.buffPro = buff;
		}
		break;
	case 5:
		{
			BuffStruct buff(BuffId::Buff_LightingLvl3,10.0f);
			m_ContactPro.buffPro = buff;
		}
		break;

	default:
		break;
	}
}

//磁铁
void MainPlaneMagnet::onEnter()
{
	DropGold::onEnter();

	BuffStruct buff(BuffId::Buff_Magnet,magnetTime);
	m_ContactPro.buffPro = buff;
}

void MainPlaneMagnet::OtherDealWhenContact( const ContactPro& rs )
{
	//磁铁特效
	GiftEff("GiftEffMagnet_", 18, "animation/GiftEff/GiftEffMagnet.png", Vec2(-20, 0));
}

void MainPlaneMagnet::GiftEff( const std::string& qianzhui, int num, const std::string& picFile,const Vec2& pos )
{
	auto pl = GameData::getInstance()->getMainPlane();
	if (!pl)
	{
		return;
	}
	auto s = pl->getContentSize();

	auto aniSp = Sprite::create("weapon.png");
	aniSp->setPosition(Vec2(s.width * 0.5, s.height*0.5 + 20) + pos);
	pl->addChild(aniSp);
	aniSp->setScale(1.0f);

	{
		auto animat = EffectsManager::getFramesAnimation(qianzhui, num , 1.0f/12, 0, 1, true);
		auto rm = RemoveSelf::create(true);
		auto seq = Sequence::create(animat, rm, nullptr);
		aniSp->runAction(seq);
	}

	auto pic = Sprite::create(picFile);
	pic->setPosition(s.width * 0.5, s.height*0.5 + 100);
	pic->setOpacity(0);
	pic->setScale(0.1);
	pl->addChild(pic,1);

	{
		auto delay = DelayTime::create(0.3f);
		auto fadein = FadeIn::create(0.1f);
		auto scale = ScaleTo::create(0.1f, 1.0f);

		auto scaleBack = ScaleTo::create(0.1f, 0.8f);
		auto scaleBig = ScaleTo::create(0.1f, 0.6f);
		auto seq = Sequence::create(scaleBack, scaleBig, nullptr);
		auto rep = Repeat::create(seq, 2);
		auto fadeOut = FadeOut::create(0.5f);
		auto rm = RemoveSelf::create(true);

		auto seq1 = Sequence::create(delay, fadein, scale, rep, fadeOut, rm, nullptr);
		pic->runAction(seq1);
	}
}


//----------------------------------
void WeaponUpgradeDrop::setBuff( const BuffStruct& buff )
{
	m_ContactPro.buffPro = buff;
}

void WeaponUpgradeDrop::OtherDealWhenContact( const ContactPro& rs )
{
	auto mainpl = GameData::getInstance()->getMainPlane();
	if (mainpl)
	{
		mainpl->WeaponUpgrade();
	}
	//武器升级特效
	MainPlaneMagnet::GiftEff("GiftEffWpUpgrade_", 19, "animation/GiftEff/GiftEffWpUpgrade.png");
}

//---------------------------------------
void HpAdditionDrop::OtherDealWhenContact( const ContactPro& rs )
{
	//加血凋落物特效
	MainPlaneMagnet::GiftEff("GiftEffAddHp_", 21, "animation/GiftEff/GiftEffAddHp.png");
}

void HpAdditionDrop::setAddHpNums( int hp )
{
	m_ContactPro.hurts = hp;
}
