#include "BasePlane.h"
#include "../Weapon/weapon.h"
#include "../../GameCommon/GameData.h"
#include "../Layer/UIController.h"
#include "../../GameCommon/EffectsManager.h"
#include "../AI.h"
#include "../../GameUI/GameMainLayer.h"
#include "../../JniHelp/CppCallJava.h"
#include "../../GameCommon/GameDefine.h"
#include "../Layer/PhysicsLayer.h"
#include "../../GameCommon/GameConfig.h"
#include "../BossTimeOut.h"
#include "../SpecialBody.h"
#include "PlaneManager.h"
#include "../Layer/RollingBg.h"
#include "../DropGoods.h"
#include "../EnemyPlaneController.h"

using namespace experimental;

BasePlane::BasePlane()
	:m_EnterAnimation(nullptr)
	,m_SelfAi(nullptr)
	,m_enterFunc(nullptr)
	,m_ShootMusicAble(true)
	,m_HpBar(nullptr)
	,m_isNeedFlyToMainPl(false)
	,m_IsShootAble(true)
	,m_ConfigDanmuId(-1)
	,m_ConfigFIreInterval(-1)
{
	m_Musicfile = "";
}

void BasePlane::setBitmask(const int& cate,const int& collision,const int& test )
{
	_physicsBody->setCategoryBitmask(cate);
	_physicsBody->setCollisionBitmask(collision);
	_physicsBody->setContactTestBitmask(test);
}

void BasePlane::setSelfAndAttact( const SelfPro& myPro, const ContactPro& attactPro )
{
	m_SelfPro = myPro;
	m_ContactPro = attactPro;
}

void BasePlane::initBody()
{
	auto body = PhysicsBody::createBox(Size(50,50));
	body->setGravityEnable(false);
	body->setRotationEnable(false);
	body->setMass(1000000);
	this->setPhysicsBody(body);
}

void BasePlane::FuncAfterEnter()
{
	//先设置无敌，在进入屏幕后设置为非无敌
	setInvincible(1);

	//执行各种动作
	runEnterAnimation();

}

void BasePlane::AnimationWhenDead()
{
	UIController::getInstance()->getEffectsLayer()->displayDeathExplosion(this->convertToWorldSpaceAR(Point::ZERO));
}

void BasePlane::setShootAble( bool b )
{
	m_IsShootAble = b;
	//普通
	for (int i = 0; i < m_NorWpVec.size(); i++)
	{
		Weapon* wp = m_NorWpVec.at(i);
		if (wp)
		{
			wp->setShootAble(b);
		}
	}

	//临时
	for (int i = 0; i < m_TempWpVec.size(); ++i)
	{
		Weapon* wp = m_TempWpVec.at(i);
		if (wp)
		{
			wp->setShootAble(b);
		}
	}

	//特殊
	for (int i = 0; i < m_SpecialWpVec.size(); ++i)
	{
		Weapon* wp = m_SpecialWpVec.at(i);
		if (wp)
		{
			wp->setShootAble(b);
		}
	}
}

void BasePlane::OtherDealWhenContact( const ContactPro& rs )
{
	if (rs.idx == UnitId::eBullet_38)
	{
		UIController::getInstance()->getEffectsLayer()->displayLightingEff(this);
	}
}

void BasePlane::removePartical( Node* node )
{
	node->removeFromParentAndCleanup(true);
}

void BasePlane::CheckIsInScreen( float dt )
{
	CCPoint pos = this->convertToWorldSpaceAR(Point::ZERO);

	//是否超出删除线，是则删除（在可以删除的前提下）
	if (pos.y < getEdgeLine(3) && pos.y > getEdgeLine(2) && 
		pos.x < getEdgeLine(1) && pos.x > getEdgeLine(0))
	{
		//进入了屏幕,在实时检测超出屏幕
		unschedule(schedule_selector(PhysicsUnit::CheckIsInScreen));
		//不受死亡线影响(主飞机，BOSS)
		if (!m_isInvincibleAtLine)
		{
			schedule(schedule_selector(PhysicsUnit::CheckIsOutScreen));

			//在0.5s后无敌状态结束（敌方普通飞机）
			schedule(schedule_selector(BasePlane::WudiOver),0.5f,1,0.0f);
		}

		StartShoot();
		setShootMusicAble(true);

		if (m_enterFunc)
		{
			(m_enterObj->*m_enterFunc)();
		}
	}
}

void BasePlane::WudiOver(float dt)
{
	setInvincible(0);
	unschedule(schedule_selector(BasePlane::WudiOver));
}

void BasePlane::runEnterAnimation()
{
	if (m_EnterAnimation)
	{
		CallFunc* func = CallFunc::create(this,callfunc_selector(BasePlane::runSelfAction));
		ActionInterval* seq = Sequence::create(m_EnterAnimation,func,nullptr);
		runAction(seq);
	}
	else
	{
		runSelfAction();
	}
}

void BasePlane::runSelfAction()
{
	bool isSpeeddUp = GameData::getInstance()->getisSpeedUp();
	if (isSpeeddUp)
	{
		if(m_isNeedFlyToMainPl)
		{
			//向下冲刺
			FlyToMainPlaneWhenSpeedUp();
		}
	}
	else
	{
		if (m_SelfAi)
		{
			//addChild(m_SelfAi);
			m_SelfAi->StartAI();
		}
	}
}

void BasePlane::setMaxHP( int hp )
{
	m_SelfPro.maxHP = hp;
	m_SelfPro.curHP = hp;
}

void BasePlane::setBltHurts( int hurts )
{
	m_ContactPro.hurts = hurts;
}

void BasePlane::setDefense( int defense )
{
	m_SelfPro.defense = defense;
}

void BasePlane::PlayMusicWhenDead()
{
	if (!m_Musicfile.empty())
	{
		NewAudio::getInstance()->playEff(m_Musicfile);
	}
	else
	{
		int randNum = random(1,5);
		switch (randNum)
		{
		case 1:
			m_Musicfile = Music_PlDead1;
			break;
		case 2:
			m_Musicfile = Music_PlDead2;
			break;
		case 3:
			m_Musicfile = Music_PlDead3;
			break;
		case 4:
			m_Musicfile = Music_PlDead4;
			break;
		case 5:
			m_Musicfile = Music_PlDead5;
			break;

		default:
			break;
		}
		NewAudio::getInstance()->playEff(m_Musicfile);
	}
}

void BasePlane::SetFuncAfterEnterWin(Ref* obj, SEL_CallFunc func)
{
	m_enterObj = obj;
	m_enterFunc = func;
}

void BasePlane::DoSomethingBeforeDead()
{
	//判断是否本组飞机都死亡，死亡则刷新下一组
	GameData::getInstance()->getPlaneManager()->DelOnePlane(this);
	EnemyPlaneController::getInstance()->destroyCallBack(this);

	if (m_HpEff.rate < 0.0f)
	{
		return;
	}

	float randf = rand_0_1();
	if (randf <= m_HpEff.rate)
	{
		//主角加血
		UIController::getInstance()->getMainUILayer()->AddHp(m_HpEff.hp);
	}
}

void BasePlane::FlyToMainPlaneWhenSpeedUp()
{
	schedule(schedule_selector(BasePlane::CheckIsSpeedUp));
}

void BasePlane::CheckIsSpeedUp(float dt)
{
	bool b = GameData::getInstance()->getisSpeedUp();
	if (b)
	{
		setInvincible(false);
		stopAllActions();
		unschedule(schedule_selector(BasePlane::CheckIsSpeedUp));

		//冲向主飞机
		schedule(schedule_selector(BasePlane::FlyToMainPlane));
	}
}

void BasePlane::FlyToMainPlane(float dt)
{
	bool isSpeedUp = GameData::getInstance()->getisSpeedUp();
	if (!isSpeedUp)
	{
		unschedule(schedule_selector(BasePlane::FlyToMainPlane));

		ActionInterval* mv = MoveTo::create(2.0f, Vec2(360,-100));
		runAction(mv);
		return;
	}
	stopAllActions();
	
	//向下移动
	Point myPos = this->convertToWorldSpaceAR(Point::ZERO);
	myPos.y -= 30;

	setPosition(myPos);
}

void BasePlane::setDeadMusic( const std::string& file )
{
	m_Musicfile = file;
}

void BasePlane::setShootMusicAble( bool b )
{
	m_ShootMusicAble = b;
}

void BasePlane::EnemyPlaneStatistics()
{
	GameData::getInstance()->addEnemyStatistics(m_SelfPro, m_Killer);
}

cocos2d::Vec2 BasePlane::getArmatPosOffset()
{
	return Vec2(0,0);
}

Size BasePlane::getSkinSize()
{
	return m_SkinSize;
}

void BasePlane::ShowHpBar()
{
	m_HpBar = HpBar::create(this);
	m_HpBar->setPosition(0, m_SkinSize.height - 20);
	addChild(m_HpBar);
}

void BasePlane::FuncWhenHpChanged()
{
	if (!m_HpBar)
	{
		return;
	}
	float percent = m_SelfPro.curHP * 1.0 / m_SelfPro.maxHP * 100;
	m_HpBar->RefreshHpBar(percent);
}

void BasePlane::AddOneWp( Weapon* weapon,WeaponType wt )
{
	weapon->setWeaponType(wt);

	switch (wt)
	{
	case WeaponType::Normal:
		m_NorWpVec.push_back(weapon);
		break;
	case WeaponType::Replace:
		//替换武器，先停止普通武器，再加上替换的武器
		if (!m_NorWpVec.empty())
		{
			for (auto it = m_NorWpVec.begin(); it < m_NorWpVec.end(); it++)
			{
				Weapon* wp = (*it);
				wp->setShootAble(false);
			}
		}
		m_TempWpVec.push_back(weapon);
		break;
	case WeaponType::Addition:
		//额外的特殊武器，不影响原来的所有武器
		m_SpecialWpVec.push_back(weapon);
		break;
	default:
		break;
	}
	weapon->StartShoot();
	addChild(weapon);
}

void BasePlane::AddWpVec( std::vector<Weapon*> wpVec,WeaponType wt )
{
	switch (wt)
	{
	case WeaponType::Normal:
		for (auto it = wpVec.begin(); it < wpVec.end(); it++)
		{
			Weapon* wp = (*it);
			wp->setWeaponType(wt);
			wp->StartShoot();
			addChild(wp);
			m_NorWpVec.push_back(wp);
		}
		break;
	case WeaponType::Replace:
		//替换武器，先停止普通武器，再加上替换的武器
		if (!m_NorWpVec.empty())
		{
			for (auto it = m_NorWpVec.begin(); it < m_NorWpVec.end(); it++)
			{
				Weapon* wp = (*it);
				wp->setShootAble(false);
			}
		}
		for (auto it = wpVec.begin(); it < wpVec.end(); it++)
		{
			Weapon* wp = (*it);
			wp->setWeaponType(wt);
			wp->StartShoot();
			addChild(wp);
			m_TempWpVec.push_back(wp);
		}
		break;
	case WeaponType::Addition:
		//额外的特殊武器，不影响原来的所有武器
		for (auto it = wpVec.begin(); it < wpVec.end(); it++)
		{
			Weapon* wp = (*it);
			wp->setWeaponType(wt);
			wp->StartShoot();
			addChild(wp);
			m_SpecialWpVec.push_back(wp);
		}
		break;
	default:
		break;
	}

}

void BasePlane::RemoveOneWeapon( Weapon* weapon )
{
	auto wt = weapon->getWeaponType();
	switch (wt)
	{
	case WeaponType::Normal:
		for (auto it = m_NorWpVec.begin(); it < m_NorWpVec.end(); it++)
		{
			Weapon* wp = (*it);
			if (wp == weapon)
			{
				m_NorWpVec.erase(it);
				break;
			}
		}
		break;
	case WeaponType::Replace:
		for (auto it = m_TempWpVec.begin(); it < m_TempWpVec.end(); it++)
		{
			Weapon* wp = (*it);
			if (wp == weapon)
			{
				m_TempWpVec.erase(it);
				break;
			}
		}
		break;
	case WeaponType::Addition:
		for (auto it = m_SpecialWpVec.begin(); it < m_SpecialWpVec.end(); it++)
		{
			Weapon* wp = (*it);
			if (wp == weapon)
			{
				m_SpecialWpVec.erase(it);
				break;
			}
		}
		break;
	default:
		break;
	}
	if (weapon)
	{
		weapon->removeFromParentAndCleanup(true);
	}
}

void BasePlane::resumeNorWp()
{
	//先删除临时普通武器
	for (auto it = m_TempWpVec.begin(); it != m_TempWpVec.end(); ++it)
	{
		Weapon* wp = (*it);
		if (wp)
		{
			wp->removeFromParentAndCleanup(true);
		}
	}
	m_TempWpVec.clear();

	//再恢复原有普通武器的发射
	for (auto it = m_NorWpVec.begin(); it < m_NorWpVec.end(); it++)
	{
		auto wp = (*it);
		wp->setShootAble(true);
	}

	//特殊武器不用管
}

void BasePlane::clearWeapon()
{
	for (auto it = m_SpecialWpVec.begin(); it != m_SpecialWpVec.end(); ++it)
	{
		Weapon* weapon = (*it);
		weapon->removeFromParentAndCleanup(true);
	}
	for (auto it = m_NorWpVec.begin(); it != m_NorWpVec.end(); ++it)
	{
		Weapon* weapon = (*it);
		weapon->removeFromParentAndCleanup(true);
	}
	for (auto it = m_TempWpVec.begin(); it != m_TempWpVec.end(); ++it)
	{
		Weapon* weapon = (*it);
		weapon->removeFromParentAndCleanup(true);
	}
	m_SpecialWpVec.clear();
	m_TempWpVec.clear();
	m_NorWpVec.clear();
}

void BasePlane::onExit()
{
	PhysicsUnit::onExit();
	clearWeapon();
}

void BasePlane::addRewards( float HpPct, int wardNum, DropGoodsType dgt )
{
	auto drop = DropReward(HpPct, wardNum, dgt);
	m_SelfPro.rewardVec.push_back(drop);
}

void BasePlane::RemoveMyself()
{
	unschedule(schedule_selector(PhysicsUnit::CheckDead));
	m_isRemoving = true;

	//死亡之前，留下遗言
	DoSomethingBeforeDead();

	deadLog();

	if (m_SelfPro.idx == UnitId::eMainPlane)
	{
		GameData::getInstance()->setMainPlane(nullptr);
	}

	GameData::getInstance()->DelEnemyPlane(this);
	this->removeFromParentAndCleanup(true);
}

void BasePlane::setNeedFlyToMainPl( bool b )
{
	m_isNeedFlyToMainPl = b;
}

void BasePlane::StartShoot()
{
	//普通
	for (int i = 0; i < m_NorWpVec.size(); i++)
	{
		Weapon* wp = m_NorWpVec.at(i);
		if (wp)
		{
			wp->StartShoot();
		}
	}

	//临时
	for (int i = 0; i < m_TempWpVec.size(); ++i)
	{
		Weapon* wp = m_TempWpVec.at(i);
		if (wp)
		{
			wp->StartShoot();
		}
	}

	//特殊
	for (int i = 0; i < m_SpecialWpVec.size(); ++i)
	{
		Weapon* wp = m_SpecialWpVec.at(i);
		if (wp)
		{
			wp->StartShoot();
		}
	}
}

void BasePlane::setFireConfig( int danmuId, float interval )
{
	m_ConfigDanmuId = danmuId;
	m_ConfigFIreInterval = interval;
}

void BasePlane::StartFire()
{
	if(m_ConfigDanmuId < 0 )
	{
		return;
	}

	if (m_ConfigDanmuId == 10000)
	{
		//攻击AI
		GameData::getInstance()->getPlaneAiFactory()->AddEnemyAiByIdx(this,7);	
		runSelfAction();
	}
	else
	{
		//发弹幕
		GameData::getInstance()->getWeaponFactory()->AddWeaponWithConfig(this, m_ConfigDanmuId);
	}

	if (m_ConfigFIreInterval > 0)
	{
		auto delay = DelayTime::create(m_ConfigFIreInterval);
		auto next = CallFunc::create(this, callfunc_selector(BasePlane::StartFire));
		auto seq = Sequence::create(delay, next, nullptr);
		auto rep = RepeatForever::create(seq);
		runAction(rep);
	}
}

//---------------------------------
MainPlane::MainPlane()
{
	m_SpeedUpNode = nullptr;
	m_SpeedUpTail = nullptr;
	m_NorAnimationNode = nullptr;
	m_MoveAnimationNodeL = nullptr;
	m_MoveAnimationNodeR = nullptr;
	m_isProtect = false;
	m_IsHasMagnet = false;
	m_isMoveAniPlayingL = false;
	m_isMoveAniPlayingR = false;
	m_isFinished = true;
	m_isTouchEnabled = true;
	m_shootMusic = "";
	m_TempPosX = 0.0f;
	m_WpLvl = 1;
}

MainPlane* MainPlane::create( PlaneType pt )
{
	MainPlane* plane = new (std::nothrow) MainPlane;
	if(plane && plane->initWithFile("weapon.png"))
	{
		plane->setPlaneType(pt);
		plane->InitSkin();
		plane->initBody();
		plane->ShowHpBar();
		plane->autorelease();
		return plane;
	}
	CC_SAFE_DELETE(plane);
	return nullptr;
}

void MainPlane::onEnter()
{
	BasePlane::onEnter();
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(MainPlane::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(MainPlane::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(MainPlane::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	schedule(schedule_selector(MainPlane::checkIsMoveing));

	Revive();
	ShootMusic();
}

void MainPlane::DoSomethingBeforeDead()
{
	CppCallJava::getInstance()->vibrate(350);
}

void MainPlane::AnimationWhenDead()
{
	UIController::getInstance()->getEffectsLayer()->displayDeathExplosion(this->convertToWorldSpaceAR(Point::ZERO));
}

void MainPlane::initBody()
{
	auto body = PhysicsBody::createCircle(20);
	body->setPositionOffset(Vec2(0,20));
	body->setGravityEnable(false);
	body->setRotationEnable(false);
	body->setMass(100000);
	this->setPhysicsBody(body);
}

void MainPlane::addSpeedUpEffect(float dur)
{
	if (!m_SpeedUpNode)
	{
		m_SpeedUpNode = Node::create();
		m_SpeedUpNode->setPosition(getContentSize().width*0.5,getContentSize().height);
		addChild(m_SpeedUpNode,1);

		//护罩
		SpeedUpProtect* sp = SpeedUpProtect::create();
		sp->setPosition(0,-getContentSize().height*0.3);
		m_SpeedUpNode->addChild(sp);

		//喷尾
		m_SpeedUpTail = Node::create();
		m_SpeedUpTail->setPosition(35,-250);
		addChild(m_SpeedUpTail,-1);
		sp->addTail(m_SpeedUpTail);
		m_SpeedUpTail->setScale(1.5f);

		//还原
		{
			ActionInterval* scale = ScaleTo::create(1.0f,1.5f);
			runAction(scale);
		}

		GameData::getInstance()->GameSpeedUp();		//游戏加速

		if (dur > 0)
		{
			ActionInterval* delay = DelayTime::create(dur);
			CallFunc* func = CallFunc::create(this,callfunc_selector(MainPlane::removeSpeedUpEffect));
			ActionInterval* seq = Sequence::create(delay, func ,nullptr);
			m_SpeedUpNode->runAction(seq);
		}
		setShootAble(false);
		setTouchAble(false);
		setShootMusicAble(false);
	}
}

void MainPlane::removeSpeedUpEffect()
{
	if (m_SpeedUpNode)
	{
		ExplisonScreen* exp = ExplisonScreen::create();
		exp->setPosition(360,640);
		UIController::getInstance()->getEffectsLayer()->addChild(exp);

		GameData::getInstance()->setisSpeedUp(false);
		GameData::getInstance()->setUpdateSpeed(1.0f);
		setShootAble(true);
		setTouchAble(true);
		setShootMusicAble(true);

		m_SpeedUpNode->removeFromParentAndCleanup(true);
		m_SpeedUpNode = nullptr;
		m_SpeedUpTail->removeFromParentAndCleanup(true);
		m_SpeedUpTail = nullptr;

		//放大
		{
			ActionInterval* scale = ScaleTo::create(1.0f,1.0f);
			runAction(scale);
		}
	}
}

void MainPlane::InitEdgeLine()
{
	m_EdgeLineVec.push_back(-200);
	m_EdgeLineVec.push_back(m_WinSize.width + 200);
	m_EdgeLineVec.push_back(-100);
	m_EdgeLineVec.push_back(m_WinSize.height);
}

void MainPlane::FlyUp()
{
	m_isProtect = true;
	addSpeedUpEffect();

	//停止发射子弹
	NotificationCenter::getInstance()->postNotification(Msg_StopMainPlBullet,nullptr);
	setTouchAble(false);
	setInvincible(true);

	ActionInterval* moveUp = MoveBy::create(1.0,ccp(0,1700));
	CallFunc* func = CallFunc::create(this,callfunc_selector(MainPlane::FlyUpCallback));
	ActionInterval* seq = Sequence::create(moveUp,func,nullptr);
	runAction(seq);
}

void MainPlane::FlyUpCallback()
{
	GameData::getInstance()->setUpdateSpeed(1.0f);		//恢复地图滚动速度

	//通关动画
	int curRole = NewDataMgr::getInstance()->mRoleConfig.roleIndex;
	CompleteGame* layer  = CompleteGame::create(curRole);
	UIController::getInstance()->getMainUILayer()->addChild(layer,1000);
}

void MainPlane::Revive()
{
	//执行出场动作,在此期间无敌
	ActionInterval* delay = DelayTime::create(2.0);//2S无敌
	CallFuncN* funcN = CallFuncN::create(this,callfuncN_selector(MainPlane::WudiOver));
	ActionInterval* seq = Sequence::create(delay, funcN, nullptr);
	runAction(seq);
}

void MainPlane::WudiOver( Node* node )
{
	//如果在冲刺，那么一直无敌
	bool isSpeedup = GameData::getInstance()->getisSpeedUp();
	if (isSpeedup)
	{
		return;
	}

	BasePlane* pl = (BasePlane*)node;
	pl->setInvincible(1);
}

void MainPlane::OtherDealWhenContact( const ContactPro& rs )
{
	//log("******************* mainpl Hp = %d,   Hurts = %d ****************", m_SelfPro.curHP, rs.hurts);
	if(m_SpeedUpNode)
	{
		return;
	}
	
	if(rs.hurts < 0)
	{
		if (m_isProtect)
		{
			return;
		}
	}

	UIController::getInstance()->getMainUILayer()->AddHp(rs.hurts);

	//判断伤害是否来自BOSS
	if (rs.idx == UnitId::ePlaneBoss || rs.belongTo == UnitId::ePlaneBoss)
	{
		GameData::getInstance()->setIsHurtByBoss(true);
	}
}

void MainPlane::AddProtect()
{
	if (m_isProtect)
	{
		return;
	}
	m_isProtect = true;
	auto s = getContentSize();
	auto protect = PlaneProtect::create(0.0f);
	protect->setPosition(s.width*0.5,s.height*0.55);
	addChild(protect,1,20150416);
}

void MainPlane::ShootMusic()
{
	CallFunc* shoot = CallFunc::create(this,callfunc_selector(MainPlane::PlayShootMusic));
	ActionInterval* delay = DelayTime::create(0.1 + rand_0_1()*0.1);
	CallFuncN* funcN = CallFuncN::create(this,callfuncN_selector(MainPlane::ShootMusicCB));
	ActionInterval* seq = Sequence::create(shoot,delay,funcN,nullptr);

	Node* node = Node::create();
	addChild(node);
	node->runAction(seq);
}

void MainPlane::ShootMusicCB(Node* node)
{
	if (node)
	{
		node->removeFromParentAndCleanup(true);
	}
	ShootMusic();
}

void MainPlane::PlayShootMusic()
{
	if (m_ShootMusicAble)
	{
		if (!m_shootMusic.empty())
		{
			NewAudio::getInstance()->playEffWithPar(m_shootMusic);
		}
		else
		{
			NewAudio::getInstance()->playEffWithPar(Music_shootAmmo4);
		}
	}
}

void MainPlane::setShootMusic( const std::string& musicfile )
{
	m_shootMusic = musicfile;
}

void MainPlane::TintWhenHurt( float dur /*= 1.0f*/ )
{
	ActionInterval* tintTo = TintTo::create(0.1,255,255,255);
	ActionInterval* tintRed = TintTo::create(0.1,150,0,0);
	ActionInterval* seq = Sequence::create(tintRed,tintTo,nullptr);
	ActionInterval* repeat = Repeat::create(seq,(int)dur/0.2);
	CallFunc* funcN = CallFunc::create(this,callfunc_selector(MainPlane::resumeColor));
	ActionInterval* seq1 = Sequence::create(repeat,funcN,nullptr);
	runAction(seq1);
}

void MainPlane::resumeColor()
{

}

void MainPlane::addMagnet()
{
	m_IsHasMagnet = true;
	auto file = GameData::getInstance()->getDropsFactory()->getResByTag(ResTag::Drops_Magnet);
	auto sp = Sprite::createWithSpriteFrameName(file);
	sp->setAnchorPoint(Vec2(0.5,1));
	sp->setPosition(Vec2(getContentSize().width*0.5, getContentSize().height*1.0));
	sp->setTag(6151936);
	sp->setRotation(180);
	addChild(sp);

	auto rot = RotateTo::create(0.05, 185);
	auto rotback = RotateTo::create(0.05, 175);
	auto shake = Sequence::create(rot, rotback, nullptr);
	auto shakeRep = Repeat::create(shake,3);
	auto delay = DelayTime::create(0.5);
	auto seq = Sequence::create(shakeRep, delay, nullptr);
	auto rep = RepeatForever::create(seq);
	sp->runAction(rep);
}

void MainPlane::removeMagnet()
{
	m_IsHasMagnet = false;
	auto node = getChildByTag(6151936);
	if (node)
	{
		node->removeFromParentAndCleanup(true);
	}
}

bool MainPlane::onTouchBegan( Touch *touch, Event *unused_event )
{
	return true;
}

void MainPlane::onTouchMoved( Touch *touch, Event *unused_event )
{
	bool isPause = GameData::getInstance()->isGamePause();
	if (isPause)
	{
		return;
	}
	if (!m_isTouchEnabled)
	{
		return;
	}
	Vec2 prePos = touch->getPreviousLocation();
	Vec2 pos = touch->getLocation();

	m_PrePos = prePos;
	m_CurPos = pos;

	float dis = pos.x - prePos.x;
	if (dis > 5)
	{
		if (!m_isMoveAniPlayingR)
		{
			playMoveAnimationR();		//右移动画
		}
	}
	else if(dis < -5)
	{
		if (!m_isMoveAniPlayingL)
		{
			playMoveAnimationL();		//左移动画
		}
	}
	

	Vec2 newPos= pos - prePos;
	Vec2 myPos = convertToWorldSpaceAR(Vec2::ZERO);
	newPos = myPos + newPos;


	if (newPos.y > m_WinSize.height * 0.95)
	{
		newPos.y = m_WinSize.height * 0.95;
	}
	else if (newPos.y < m_WinSize.height * 0.1)
	{
		newPos.y = m_WinSize.height * 0.1;
	}
	if (newPos.x > m_WinSize.width - 30)
	{
		newPos.x = m_WinSize.width - 30;
	}
	else if (newPos.x < 30)
	{
		newPos.x = 30;
	}

	setPosition(newPos);
}

void MainPlane::onTouchEnded( Touch *touch, Event *unused_event )
{
	playNorAnimation();			//普通动画
}

void MainPlane::InitSkin()
{
	//不能直接用this来播放，否则武器在设置位置的时候，莫名其妙的偏移
	m_NorAnimationNode = Sprite::create("weapon.png");
	m_NorAnimationNode->setPosition(getContentSize().width*0.5, getContentSize().height*0.5);
	addChild(m_NorAnimationNode);
	m_NorAnimationNode->runAction(getNorAnimation());

	{
		//向左移动动画
		m_MoveAnimationNodeL = Sprite::create("weapon.png");
		m_MoveAnimationNodeL->setPosition(getContentSize().width*0.5, getContentSize().height*0.5);
		addChild(m_MoveAnimationNodeL);
		m_MoveAnimationNodeL->setVisible(false);
		auto moveAnimat = getMoveAnimationL();
		m_MoveAnimationNodeL->runAction(moveAnimat);
	}
	{
		//向右移动动画
		m_MoveAnimationNodeR = Sprite::create("weapon.png");
		m_MoveAnimationNodeR->setPosition(getContentSize().width*0.5, getContentSize().height*0.5);
		addChild(m_MoveAnimationNodeR);
		m_MoveAnimationNodeR->setVisible(false);
		auto moveAnimat = getMoveAnimationR();
		m_MoveAnimationNodeR->runAction(moveAnimat);
	}

	setSkinSize(Size(100,100));
}

void MainPlane::playMoveAnimationL()
{
	m_isMoveAniPlayingL = true;
	m_isFinished = false;

	m_NorAnimationNode->setVisible(false);
	m_MoveAnimationNodeL->setVisible(true);
	m_MoveAnimationNodeR->setVisible(false);

	m_MoveAnimationNodeL->stopAllActions();
	auto animat = getMoveAnimationL();
	auto finish = CallFunc::create(this, callfunc_selector(MainPlane::AnimatFinished));
	auto seq = Sequence::create(animat, finish, nullptr);
	m_MoveAnimationNodeL->runAction(seq);
}

void MainPlane::playMoveAnimationR()
{
	m_isMoveAniPlayingR = true;
	m_isFinished = false;

	m_NorAnimationNode->setVisible(false);
	m_MoveAnimationNodeL->setVisible(false);
	m_MoveAnimationNodeR->setVisible(true);

	m_MoveAnimationNodeR->stopAllActions();
	auto animat = getMoveAnimationR();
	auto finish = CallFunc::create(this, callfunc_selector(MainPlane::AnimatFinished));
	auto seq = Sequence::create(animat, finish, nullptr);
	m_MoveAnimationNodeR->runAction(seq);
}

void MainPlane::playMoveAnimationBackL()
{
	m_isFinished = false;

	auto animat = getMoveAnimationL(false);
	auto back = animat->reverse();
	auto func = CallFunc::create(this, callfunc_selector(MainPlane::playNorAnimation));
	auto seq = Sequence::create(back, func, nullptr);
	m_MoveAnimationNodeL->runAction(seq);
}

void MainPlane::playMoveAnimationBackR()
{
	m_isFinished = false;

	auto animat = getMoveAnimationR(false);
	auto back = animat->reverse();
	auto func = CallFunc::create(this, callfunc_selector(MainPlane::playNorAnimation));
	auto seq = Sequence::create(back, func, nullptr);
	m_MoveAnimationNodeR->runAction(seq);
}

void MainPlane::playNorAnimation()
{
	//执行完移动动画，先判断是否手指仍在移动；如果在移动，那么继续播放，否则播放正常动画
	m_isMoveAniPlayingL = false;
	m_isMoveAniPlayingR = false;
	m_isFinished = true;

	m_NorAnimationNode->stopAllActions();
	m_NorAnimationNode->runAction(getNorAnimation());

	m_NorAnimationNode->setVisible(true);
	m_MoveAnimationNodeL->setVisible(false);
	m_MoveAnimationNodeR->setVisible(false);
}

ActionInterval* MainPlane::getMoveAnimationL(bool isHead)
{
	std::string qianzhui;
	switch (m_PlaneType)
	{
	case PlaneType::PlaneType_Wind:
		qianzhui = "PlAnimationL_";
		break;
	case PlaneType::PlaneType_Fire:
		qianzhui = "Pl2MoveL_";
		break;
	case PlaneType::PlaneType_Electric:
		qianzhui = "Pl3MoveL_";
		break;		

	default:
		break;
	}
	Animate* moveAnimat = nullptr;
	if (isHead)
	{
		moveAnimat = EffectsManager::getFramesAnimation(qianzhui, 8, 1.0f/50, 0, 1);
	}
	else
	{
		moveAnimat = EffectsManager::getFramesAnimation(qianzhui, 16, 1.0f/50, 8, 1);
	}
	return moveAnimat;
}

ActionInterval* MainPlane::getMoveAnimationR(bool isHead)
{
	std::string qianzhui;
	switch (m_PlaneType)
	{
	case PlaneType::PlaneType_Wind:
		qianzhui = "PlAnimationR_";
		break;
	case PlaneType::PlaneType_Fire:
		qianzhui = "Pl2MoveR_";
		break;
	case PlaneType::PlaneType_Electric:
		qianzhui = "Pl3MoveR_";
		break;
	default:
		break;
	}
	Animate* moveAnimat = nullptr;
	if (isHead)
	{
		moveAnimat = EffectsManager::getFramesAnimation(qianzhui, 8, 1.0f/50, 0, 1);
	}
	else
	{
		moveAnimat = EffectsManager::getFramesAnimation(qianzhui, 16, 1.0f/50, 8, 1);
	}
	return moveAnimat;
}

ActionInterval* MainPlane::getNorAnimation()
{
	std::string qianzhui;
	switch (m_PlaneType)
	{
	case PlaneType::PlaneType_Wind:
		qianzhui = "Lead_wind_";
		break;
	case PlaneType::PlaneType_Fire:
		qianzhui = "MainPl2_";
		break;
	case PlaneType::PlaneType_Electric:
		qianzhui = "MainPl3_";
		break;
	default:
		break;
	}

	Animate* animate = EffectsManager::getFramesAnimation(qianzhui, 8, 0.1f, 0, REPEAT_FORVER);
	return animate;
}

void MainPlane::checkIsMoveing( float dt )
{
	//在判断没有移动的情况（在move状态，却没有移动距离）
	float dis = m_TempPosX - m_CurPos.x;

	//log("-------dis = %f", dis);

	if (fabsf(dis) < 0.5)
	{
		if(m_isMoveAniPlayingL && m_isFinished)
		{
			playMoveAnimationBackL();
		}
		else if(m_isMoveAniPlayingR && m_isFinished)
		{
			playMoveAnimationBackR();
		}
	}
	m_TempPosX = m_CurPos.x;
}

void MainPlane::setHaveProtect(bool b)
{
	m_isProtect = b;
}

void MainPlane::AnimatFinished()
{
	m_isFinished = true;
}

void MainPlane::WeaponUpgrade()
{
	if (!m_IsShootAble)
	{
		return;
	}
	m_WpLvl++;
	if (m_WpLvl > 5)
	{
		m_WpLvl = 5;
	}
	clearWeapon();
	GameData::getInstance()->getWeaponFactory()->AddMainPlaneWp((int)m_PlaneType, m_WpLvl, this);
}

void MainPlane::WeaponDowngrade()
{
	if (!m_IsShootAble)
	{
		return;
	}
	m_WpLvl--;
	if (m_WpLvl < 1)
	{
		m_WpLvl = 1;
	}
	clearWeapon();
	GameData::getInstance()->getWeaponFactory()->AddMainPlaneWp((int)m_PlaneType, m_WpLvl, this);
}



//-----------------------------
void BaseBoss::FuncAfterEnter()
{
	//先警告BOSS即将进入
	UIController::getInstance()->getPhysicsLayer()->BossIn();
	
	ActionInterval* mvMid = MoveTo::create(2.0f, Vec2(m_WinSize.width * 0.5, m_WinSize.height * 0.8));
	CallFunc* func = CallFunc::create(this,callfunc_selector(BaseBoss::BossDelayIn));
	ActionInterval* seq = Sequence::create(mvMid, func, nullptr);
	runAction(seq);
}

void BaseBoss::BossDelayIn()
{
	//如果有冲刺，那么冲刺结束
	auto mainPl = GameData::getInstance()->getMainPlane();
	MainPlane* pl = (MainPlane*)mainPl;
	pl->removeSpeedUpEffect();
	
	addAi();
	addRating();
	addTimeOut();
}

void BaseBoss::BossExplosion()
{
	Size s = getContentSize();

	//Vec2 pos = Vec2( s.width * ( rand_0_1()*0.8 + 0.2 ),s.height*(rand_0_1() * 0.8));
	Vec2 pos = Vec2( s.width * 0.5,s.height*0.6);
	
	ExplosionSprite* es = ExplosionSprite::create();
	es->setPosition(pos);
	es->setScale(4.0f);
	addChild(es);
}

void BaseBoss::displayBossExplosion()
{
	Node* antionNode = getChildByTag(20150302);
	if (antionNode)
	{
		antionNode->removeFromParentAndCleanup(true);
	}
	antionNode = Node::create();
	addChild(antionNode,0,20150302);

	/*ActionInterval* delay = DelayTime::create(5.0f + rand_0_1() * 0.02f);
	ActionInterval* seq = Sequence::create(delay,func,nullptr);
	ActionInterval* rep = Repeat::create(seq,cocos2d::random(2,10));*/
	ActionInterval* delay2 = DelayTime::create(1.4f);
	CallFunc* func1 = CallFunc::create(this,callfunc_selector(BaseBoss::BossExplosion));
	CallFunc* func2 = CallFunc::create(this,callfunc_selector(BaseBoss::shake));
	ActionInterval* seq2 = Sequence::create(func1,delay2,func2,nullptr);

	antionNode->runAction(seq2);
}

void BaseBoss::OtherDealWhenContact( const ContactPro& rs )
{
	auto wudi = m_SelfPro.isInvincible;
	if (wudi)
	{
		return;
	}
	m_Armat->setColor(ccc3(255,rand_0_1()*255,0));
	m_Armat->setOpacity(200);
	ActionInterval* delay = DelayTime::create(0.1f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(BaseBoss::resumeColor));
	ActionInterval* seq = Sequence::create(delay, func ,nullptr);
	runAction(seq);

	//受击特效
	if (m_SelfPro.curHP > 0)
	{
		float randNum = rand_0_1();
		if (randNum > 0.5)
		{
			PlaneProtect::ContactEff(this, 100.0f, rs.pos,BeHitEffRadius(), BeHitEffOffset());
		}
	}
}

void BaseBoss::resumeColor()
{
	m_Armat->setColor(ccc3(255,255,255));
	m_Armat->setOpacity(255);
}

void BaseBoss::shake()
{
	UIController::getInstance()->getEffectsLayer()->NodeShake(this,1.0f);
}

void BaseBoss::RealDead()
{
	setDirectDeath();
}

void BaseBoss::DoSomethingBeforeDead()
{
	BasePlane::DoSomethingBeforeDead();
}

void BaseBoss::addRating()
{
	m_ratingNode = CalculateRating::create();
	m_ratingNode->startRating();
	addChild(m_ratingNode);
}

void BaseBoss::FuncHpEmpty()
{
	NotificationCenter::getInstance()->postNotification(Msg_BossDead, nullptr);

	//不能碰撞
	setBitmask(ENEMY_PLANE_CATEGORYBITMASK,0,0);
	GameData::getInstance()->setIsBossDead(true);
	m_ratingNode->showRating();

	setHpEmptyAndInvincible();
	GameData::getInstance()->GameSlowDown(1.0f);

	displayBossExplosion();
	m_Ai->DeathAction();

	//切换地图,要放在加载关卡之前
	if (m_BgId > 0)
	{
		UIController::getInstance()->getGameBgMgr()->SwitchBg(m_BgId);
	}

	//开始加载下一个关卡
	GameData::getInstance()->getPlaneManager()->AddNextLevel();
	GameData::getInstance()->setIsHurtByBoss(false);	//重置是否收到boss伤害
	
}

float BaseBoss::BeHitEffRadius()
{
	return 0.7;
}

cocos2d::Vec2 BaseBoss::BeHitEffOffset()
{
	return Vec2(0, 0);
}

void BaseBoss::SwitchBgWhenDead( int BgId )
{
	m_BgId = BgId;
}

void BaseBoss::InitSkinWithArmat( const std::string& name )
{
	auto s = getContentSize();
	m_Armat = Armature::create(name);
	m_Armat->setPosition(Vec2(s.width * 0.5, s.height * 0.5) + getArmatPosOffset());
	addChild(m_Armat);
	m_Armat->getAnimation()->playWithIndex(0);

	setSkinSize(m_Armat->getContentSize());
}

void BaseBoss::FuncWhenHpChanged()
{
	if (!m_HpBar)
	{
		return;
	}
	int lifeNum = m_HpBar->getMaxLife();
	int eachMaxHp = m_SelfPro.maxHP / lifeNum;		//每条生命的值
	int leftLife = m_SelfPro.curHP / eachMaxHp;		//剩余多少管血
	int curHpOfLife = m_SelfPro.curHP % eachMaxHp;
	float percent = curHpOfLife * 1.0 / eachMaxHp * 100;
	m_HpBar->RefreshHpBar(percent);
	m_HpBar->NextLife(leftLife);
}



//-----------------------------
IronManBoss* IronManBoss::create( const std::string& artFile, int lvl )
{
	IronManBoss* Iman = new (std::nothrow) IronManBoss;
	
	if (Iman && Iman->initWithFile("weapon.png"))
	{
		Iman->InitSkinWithArmat(artFile);
		Iman->initBody();
		Iman->ShowHpBar();
		Iman->autorelease();
		Iman->setBossLvl(lvl);
		return Iman;
	}
	else
	{
		CC_SAFE_DELETE(Iman);
		return nullptr;
	}
}


void IronManBoss::addAi()
{
	PlaneAI* ai = IronManBossAI::create(this);
	addChild(ai);
	m_Ai = ai;

	ai->StartAI(m_BossLvl);

	//让其可以碰撞
	setBitmask(ENEMY_PLANE_CATEGORYBITMASK,
		ENEMY_PLANE_COLLISIONBITMASK,
		ENEMY_PLANE_CONTACTTESTBITMASK);

	getHpBar()->setHpBarOffset(Vec2(0,-140));
}

void IronManBoss::initBody()
{
	auto body = PhysicsBody::createBox(Size(300,200));
	body->setPositionOffset(Vec2(0,100));
	body->setGravityEnable(false);
	body->setRotationEnable(false);
	body->setMass(20000);
	this->setPhysicsBody(body);
	setBitmask(ENEMY_PLANE_CATEGORYBITMASK,0,0);
	setInvincibleAtLine(true);	//一定要设置，否则会发生自然死亡
}

void IronManBoss::addTimeOut()
{
	BossTimeOut* timeCd = BossTimeOut::create();
	timeCd->setMaxTime(20.0f);
	timeCd->setInterval(3.0f);
	addChild(timeCd);
}

void IronManBoss::OtherDealWhenContact( const ContactPro& rs )
{
	BaseBoss::OtherDealWhenContact(rs);
	
}

cocos2d::Vec2 IronManBoss::getArmatPosOffset()
{
	return Vec2(0,0);
}


//--------------------------
CalculateRating* CalculateRating::create()
{
	CalculateRating* cr = new (std::nothrow) CalculateRating;
	if (cr && cr->init())
	{
		cr->autorelease();
		return cr;
	}
	else
	{
		CC_SAFE_DELETE(cr);
		return nullptr;
	}
}

void CalculateRating::updateTime( float dt )
{
	m_SurvivlaTime += 1.0f;
}

BossRating CalculateRating::getRating()
{
	bool isHurtByBoss = GameData::getInstance()->getIsHurtByBoss();
	BossRating br;
	if (m_SurvivlaTime <= 10.0f)
	{
		if (isHurtByBoss)
		{
			br = BossRating::e_A;
		}
		else
		{
			br = BossRating::e_SS;
		}
	}
	else if(m_SurvivlaTime < 20.0f)
	{
		if (isHurtByBoss)
		{
			br = BossRating::e_A;
		}
		else
		{
			br = BossRating::e_S;
		}
	}
	else if(m_SurvivlaTime < 30.0f)
	{
		br = BossRating::e_A;
	}
	else
	{
		br = BossRating::e_B;
	}
	return br;
}

void CalculateRating::showRating()
{
	if (m_isShow)
	{
		return;
	}
	m_isShow = true;
	BossRating br = getRating();
	SurvivalTimeRecord();

	unschedule(schedule_selector(CalculateRating::updateTime));
	GameData::getInstance()->addBossRecord(m_BossType,br);

	std::string BgFile;
	std::string gradeFile;
	std::string discribFile;

	//分数
	int score = 0;
	switch (br)
	{
	case BossRating::e_SS:
		gradeFile = "Rating/rating_doubles.png";
		discribFile = "Rating/perfect.png";
		score = 20000;
		break;
	case BossRating::e_S:
		gradeFile = "Rating/rating_s.png";
		discribFile = "Rating/cool.png";
		score = 15000;
		break;
	case BossRating::e_A:
		gradeFile = "Rating/rating_a.png";
		discribFile = "Rating/great.png";
		score = 10000;
		break;
	case BossRating::e_B:
		gradeFile = "Rating/rating_b.png";
		discribFile = "Rating/good.png";
		score = 7000;
		break;
	default:
		break;
	}

	GameData::getInstance()->addScore(score);

	auto s = Director::getInstance()->getWinSize();

	auto Bg = Sprite::create("Rating/ratingBg.png");
	Bg->setPosition(-s.width * 0.5, s.height * 0.65);
	UIController::getInstance()->getPhysicsLayer()->addChild(Bg);

	auto bgSize = Bg->getContentSize();

	auto grade = Sprite::create(gradeFile);
	grade->setPosition(bgSize.width*0.75, bgSize.height*0.65);
	Bg->addChild(grade);
	grade->setOpacity(0);

	auto discrib = Sprite::create(discribFile);
	discrib->setPosition(bgSize.width*0.3, bgSize.height*0.35);
	Bg->addChild(discrib);

	{
		auto delay = DelayTime::create(1.5);
		auto fadein = FadeIn::create(0.2f);
		auto scale = ScaleBy::create(0.2f,2.0);
		auto scaleEase = EaseSineIn::create(scale);
		auto scaleBack = scaleEase->reverse();

		auto seq1 = Sequence::create(delay, scaleEase, scaleBack, nullptr);
		auto seq2 = Sequence::create(delay, fadein, nullptr);

		grade->runAction(seq1);
		grade->runAction(seq2);
	}

	{
		auto mvIn = MoveTo::create(1.0,Vec2(s.width * 0.5, s.height * 0.65));
		auto easeIn = EaseElasticIn::create(mvIn);
		auto stay = DelayTime::create(3.0f);
		auto mvOut = MoveTo::create(0.2,Vec2(s.width * 1.5, s.height * 0.65));
		auto rm = RemoveSelf::create(true);
		auto seq = Sequence::create(easeIn, stay, mvOut, rm, nullptr);
		Bg->runAction(seq);
	}
}

void CalculateRating::startRating()
{
	schedule(schedule_selector(CalculateRating::updateTime),1.0f);
}

void CalculateRating::removeRatingSp( Node* node )
{
	node->removeFromParentAndCleanup(true);
}

void CalculateRating::setBossType( int tp )
{
	m_BossType = tp;
}

void CalculateRating::SurvivalTimeRecord()
{
	auto data = GameData::getInstance()->getAllStatistics();
	if (m_SurvivlaTime < data[GamePrKey_minKillBossTime] || 0 == data[GamePrKey_minKillBossTime] )
	{
		data[GamePrKey_minKillBossTime] = m_SurvivlaTime;
		//GameData::getInstance()->setAllStatistics(data);
	}
}


//-----------------------------
BoneWarriorBoss* BoneWarriorBoss::create( const std::string& artFile, int lvl )
{
	BoneWarriorBoss* Warrior = new (std::nothrow) BoneWarriorBoss;

	if (Warrior && Warrior->initWithFile("weapon.png"))
	{
		Warrior->InitSkinWithArmat(artFile);
		Warrior->initBody();
		Warrior->setBossLvl(lvl);
		Warrior->ShowHpBar();
		Warrior->autorelease();
		return Warrior;
	}
	else
	{
		CC_SAFE_DELETE(Warrior);
		return nullptr;
	}
}

void BoneWarriorBoss::addAi()
{
	PlaneAI* ai = BoneWarriorAI::create(this);
	addChild(ai);
	m_Ai = ai;
	ai->StartAI(m_BossLvl);

	setBitmask(ENEMY_PLANE_CATEGORYBITMASK,
		ENEMY_PLANE_COLLISIONBITMASK,
		ENEMY_PLANE_CONTACTTESTBITMASK);

	getHpBar()->setHpBarOffset(Vec2(0,-350));
	getHpBar()->setHpBarLengthScale(0.7);
}

void BoneWarriorBoss::initBody()
{
	auto body = PhysicsBody::createBox(Size(300,400),PHYSICSBODY_MATERIAL_DEFAULT,Vec2(0,50));
	body->setGravityEnable(false);
	body->setRotationEnable(false);
	body->setMass(20000);
	this->setPhysicsBody(body);
	setBitmask(ENEMY_PLANE_CATEGORYBITMASK,0,0);
	setInvincibleAtLine(true);
}

void BoneWarriorBoss::onEnter()
{
	BaseBoss::onEnter();
}

void BoneWarriorBoss::OtherDealWhenContact( const ContactPro& rs )
{
	BaseBoss::OtherDealWhenContact(rs);
	
}

//-------------------------------
RenzheBoss* RenzheBoss::create( const std::string& artFile, int lvl )
{
	RenzheBoss* renzhe = new (std::nothrow) RenzheBoss;

	if (renzhe && renzhe->initWithFile("weapon.png"))
	{
		renzhe->InitSkinWithArmat(artFile);
		renzhe->initBody();
		renzhe->setBossLvl(lvl);
		renzhe->ShowHpBar();
		renzhe->autorelease();
		return renzhe;
	}
	else
	{
		CC_SAFE_DELETE(renzhe);
		return nullptr;
	}
}
void RenzheBoss::FuncAfterEnter()
{
	//先警告BOSS即将进入
	UIController::getInstance()->getPhysicsLayer()->BossIn();

	ActionInterval* mvMid = MoveTo::create(2.0f, Vec2(m_WinSize.width * 0.5, m_WinSize.height * 0.7));
	CallFunc* func = CallFunc::create(this,callfunc_selector(BaseBoss::BossDelayIn));
	ActionInterval* seq = Sequence::create(mvMid, func, nullptr);
	runAction(seq);
}

void RenzheBoss::addAi()
{
	PlaneAI* ai = RenzheBossAI::create(this);
	addChild(ai);
	m_Ai = ai;

	ai->StartAI(m_BossLvl);

	//让其可以碰撞
	setBitmask(ENEMY_PLANE_CATEGORYBITMASK,
		ENEMY_PLANE_COLLISIONBITMASK,
		ENEMY_PLANE_CONTACTTESTBITMASK);

	getHpBar()->setHpBarOffset(Vec2(0,-120));
}

void RenzheBoss::initBody()
{
	auto body = PhysicsBody::createBox(Size(300,300),PHYSICSBODY_MATERIAL_DEFAULT,Vec2(-30,50));
	body->setGravityEnable(false);
	body->setRotationEnable(false);
	body->setMass(20000);
	this->setPhysicsBody(body);
	setBitmask(ENEMY_PLANE_CATEGORYBITMASK,0,0);
	setInvincibleAtLine(true);	//一定要设置，否则会发生自然死亡
}

//-------------------------------------------
PoseidonBoss* PoseidonBoss::create( const std::string& artFile, int lvl )
{
	PoseidonBoss* poseidon = new (std::nothrow) PoseidonBoss;

	if (poseidon && poseidon->initWithFile("weapon.png"))
	{
		poseidon->InitSkinWithArmat(artFile);
		poseidon->initBody();
		poseidon->setBossLvl(lvl);		
		poseidon->ShowHpBar();
		poseidon->autorelease();
		return poseidon;
	}
	else
	{
		CC_SAFE_DELETE(poseidon);
		return nullptr;
	}
}

void PoseidonBoss::initBody()
{
	auto body = PhysicsBody::createBox(Size(150,350),PHYSICSBODY_MATERIAL_DEFAULT,Vec2(-30,100));
	body->setGravityEnable(false);
	body->setRotationEnable(false);
	body->setMass(20000);
	this->setPhysicsBody(body);
	setBitmask(ENEMY_PLANE_CATEGORYBITMASK,0,0);
	setInvincibleAtLine(true);	//一定要设置，否则会发生自然死亡
}

void PoseidonBoss::addAi()
{
	PlaneAI* ai = PoseidonBossAI::create(this);
	addChild(ai);
	m_Ai = ai;

	ai->StartAI(m_BossLvl);

	//让其可以碰撞
	setBitmask(ENEMY_PLANE_CATEGORYBITMASK,
		ENEMY_PLANE_COLLISIONBITMASK,
		ENEMY_PLANE_CONTACTTESTBITMASK);

	getHpBar()->setHpBarOffset(Vec2(0,-120));
}

void PoseidonBoss::FuncAfterEnter()
{
	//先警告BOSS即将进入
	UIController::getInstance()->getPhysicsLayer()->BossIn();

	ActionInterval* mvMid = MoveTo::create(2.0f, Vec2(m_WinSize.width * 0.5, m_WinSize.height * 0.7));
	CallFunc* func = CallFunc::create(this,callfunc_selector(BaseBoss::BossDelayIn));
	ActionInterval* seq = Sequence::create(mvMid, func, nullptr);
	runAction(seq);
}

cocos2d::Vec2 PoseidonBoss::BeHitEffOffset()
{
	return Vec2(0, 30);
}

//--------------------------------------
HpBar::HpBar()
{
	m_HpBar = nullptr;
	m_Plane = nullptr;
	m_MaxLife = 1;
	m_LeftLife = 1;
}

HpBar* HpBar::create(BasePlane* plane)
{
	HpBar* bar = new (std::nothrow)HpBar;
	if (bar && bar->initWithFile("Image/HpBarBg1.png"))
	{
		bar->m_Plane = plane;
		bar->autorelease();
		return bar;
	}
	else
	{
		CC_SAFE_DELETE(bar);
		return nullptr;
	}
}

void HpBar::onEnter()
{
	Sprite::onEnter();
	ShowHpBar();
}

void HpBar::ShowHpBar()
{
	auto skinSize = m_Plane->getSkinSize();
	auto s = getContentSize();
	setScaleX(skinSize.width/s.width * 0.6);
	setBlendFunc(BlendFunc::DISABLE);
	setColor(Color3B::BLACK);

	Sprite* sp = Sprite::create("Image/HpBarBg1.png");
	sp->setColor(Color3B::YELLOW);
	m_HpBar = ProgressTimer::create(sp);
	m_HpBar->setType(kCCProgressTimerTypeBar);  
	m_HpBar->setBarChangeRate(Vec2(1,0));
	m_HpBar->setPercentage(100.0f);
	m_HpBar->setPosition(ccp(s.width*0.5,s.height*0.5));
	m_HpBar->setMidpoint(Vec2(0,0.5));
	addChild(m_HpBar);
}

void HpBar::setHpBarOffset( const Vec2& offset )
{
	auto pos = getPosition();
	this->setPosition(pos + offset);
}

void HpBar::RefreshHpBar(float percent)
{
	if (!m_HpBar)
	{
		return;
	}
	m_HpBar->setPercentage(percent);
}

void HpBar::SetHpBarVisible( bool visible )
{
	this->setVisible(visible);
}

void HpBar::setHpBarLengthScale( float scale )
{
	float curScale = getScaleX();
	setScaleX(scale * curScale);
}

void HpBar::setLifeNum( int lifeNum )
{
	m_MaxLife = lifeNum;
	m_LeftLife = lifeNum - 1;

	setBlendFunc(BlendFunc::ADDITIVE);
	m_BgColor = Color3B::RED;
	m_BarColor = Color3B::YELLOW;
}

void HpBar::NextLife(int newNum)
{
	if (m_LeftLife != newNum)
	{
		if (newNum < 1)
		{
			setBlendFunc(BlendFunc::DISABLE);
			m_BarColor = Color3B::RED;
			m_BgColor = Color3B::BLACK;
		}
		else
		{
			setBlendFunc(BlendFunc::ADDITIVE);
			m_BarColor = m_BgColor;

			int colorId = newNum % 3;
			if (colorId == 0)
			{
				m_BgColor = Color3B::GREEN;
			}
			else if(colorId == 1)
			{
				m_BgColor = Color3B::YELLOW;
			}
			else if(colorId == 2)
			{
				m_BgColor = Color3B::RED;
			}
		}
		m_LeftLife = newNum;
	}
	changeColor();
}

void HpBar::changeColor()
{
	setColor(m_BgColor);
	m_HpBar->setColor(m_BarColor);
}

//---------------------------------------
EnemyPlane* EnemyPlane::createWithArmature( const std::string& file,const std::string& TrackId )
{
	EnemyPlane* plane = new (std::nothrow) EnemyPlane;
	if (plane && plane->init())
	{
		plane->InitSkinWithArmature(file);
		plane->initBody();
		plane->ShowHpBar();
		plane->initTrack(TrackId);
		plane->autorelease();
		return plane;
	}
	else
	{
		CC_SAFE_DELETE(plane);
		return nullptr;
	}
}

EnemyPlane* EnemyPlane::createWithPicture( const std::string& file, const std::string& TrackId )
{
	EnemyPlane* plane = new (std::nothrow) EnemyPlane;
	if (plane && plane->init())	//这里不直接设置图片，而是将图片设为其子节点，时为了防止this的动作停止时，皮肤动作也停止
	{
		plane->InitSkinWithPicture(file);
		plane->initBody();
		plane->ShowHpBar();
		plane->initTrack(TrackId);
		plane->autorelease();
		return plane;
	}
	else
	{
		CC_SAFE_DELETE(plane);
		return nullptr;
	}
}

void EnemyPlane::InitSkinWithPicture( const std::string& file )
{
	m_Picture = Sprite::createWithSpriteFrameName(file);
	addChild(m_Picture);
	setSkinSize(m_Picture->getContentSize());
}

void EnemyPlane::InitSkinWithArmature( const std::string& name )
{
	auto s = getContentSize();
	m_Armat = PlaneArmature::create(name, this);
	m_Armat->setPosition(Vec2(s.width * 0.5, s.height * 0.5) + getArmatPosOffset());
	addChild(m_Armat);
	m_Armat->playRestAnimation();

	setSkinSize(m_Armat->getContentSize());
}

void EnemyPlane::addWeapon()
{
	GameData::getInstance()->getWeaponFactory()->AddEnemyWp(m_WeaponPro, this);
}

void EnemyPlane::setFlipXonEnter()
{
	m_isNeedFlipX = true;
}

EnemyPlane::~EnemyPlane()
{
	m_trackMgr->purge();
}

void EnemyPlane::onEnter()
{
	BasePlane::onEnter();
	if (m_isNeedFlipX)
	{
		//图片默认朝左，根据创建的位置判断是否需要翻转
		auto pos = convertToWorldSpaceAR(Vec2::ZERO);
		if (pos.x < m_WinSize.width * 0.5)
		{
			m_Picture->setFlippedX(true);
		}
	}
}

void EnemyPlane::OtherDealWhenContact( const ContactPro& rs )
{
	int randNum = random(1,10);
	if (randNum > 4)
	{
		return;
	}
	auto s = getSkinSize();
	auto pos = Vec2(0, -s.height*0.25);
	auto partc1 = EffectsManager::addParticle("particla/NorPlContact/NorPlContact1.plist",this,pos);
	auto partc2 = EffectsManager::addParticle("particla/NorPlContact/NorPlContact2.plist",this,pos);
	if (partc1)
	{
		partc1->setPositionType(ParticleSystem::PositionType::GROUPED);
	}
	if (partc2)
	{
		partc2->setPositionType(ParticleSystem::PositionType::GROUPED);
	}

	{
		auto mv = MoveBy::create(0.05f, Vec2(0,5));
		auto back = mv->reverse();
		auto seq = Sequence::create(mv, back, nullptr);
		runAction(seq);
	}
	{
		auto scale = ScaleTo::create(0.05f, 1.0f, 0.95f);
		auto back = ScaleTo::create(0.05f, 1.0f, 1.0f);
		auto seq = Sequence::create(scale, back, nullptr);
		runAction(seq);
	}
	{
		auto tint = TintTo::create(0.05, 200, 0, 0);
		auto back = TintTo::create(0.05, 255, 255, 255);
		auto seq = Sequence::create(tint, back, nullptr);
		if (m_Picture)
		{
			m_Picture->runAction(seq);
		}
		else
		{
			m_Armat->runAction(seq);
		}
	}
}

void EnemyPlane::initTrack(const std::string& TrackId)
{
	m_trackMgr = TrackManager::create(this, TrackId);
}

void EnemyPlane::startTrack()
{
	if (m_trackMgr)
	{
		m_trackMgr->startAction();
	}
}
