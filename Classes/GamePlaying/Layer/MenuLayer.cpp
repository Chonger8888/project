#include "MenuLayer.h"
#include "UIController.h"
#include "PhysicsLayer.h"
#include "../../GameCommon/EffectsManager.h"
#include "../../GameCommon/GameData.h"
#include "../BasePlane/BasePlane.h"
#include "../Bullet/Bullet.h"
#include "../Bullet/BulletFactory.h"
#include "../Weapon/WeaponFactory.h"
#include "../Weapon/weapon.h"
#include "../../GameCommon/GameConfig.h"
#include "RollingBg.h"
#include "../SpecialBody.h"
#include "GameUI/GameMainLayer.h"
#include "../AI.h"
#include "../BasePlane/PlaneFactory.h"
#include "../BasePlane/PlaneAction.h"
#include "../BasePlane/PlaneManager.h"
#include "Config/WeaponConfig.h"
#include "../DropGoods.h"





bool MenuLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	m_winSize = Director::getInstance()->getWinSize();
	m_isListening = false;
	m_Pos_1_State = 0;
	m_Pos_2_State = 0;
	m_Pos_3_State = 0;
	m_Pos_4_State = 0;
	m_bulletId = 1;
	return true;
}

void MenuLayer::onEnter()
{
	Layer::onEnter();
	TestMenus();

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(MenuLayer::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(MenuLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void MenuLayer::TestMenus()
{
	addTestMenu("显示/隐藏刚体",ccp(50,900),menu_selector(MenuLayer::MenuCB1));
	addTestMenu("全屏清怪@",ccp(50,800),menu_selector(MenuLayer::MenuCB2));
	addTestMenu("测试-1 ",ccp(50,600),menu_selector(MenuLayer::MenuCB3));
	addTestMenu("测试-2 ",ccp(50,500),menu_selector(MenuLayer::MenuCB4));
	addTestMenu("测试-3 ",ccp(50,400),menu_selector(MenuLayer::MenuCB5));
	addTestMenu("测试-4 ",ccp(50,300),menu_selector(MenuLayer::MenuCB6));
#ifdef WIN32
	m_isShowMenu = true;
#else
	m_isShowMenu = false;
#endif
	menuSwitch();

}

void MenuLayer::MenuCB1( Ref* sender )
{
	auto scene = Director::getInstance()->getRunningScene();
	int mask = scene->getPhysicsWorld()->getDebugDrawMask();
	if (mask == PhysicsWorld::DEBUGDRAW_NONE)
	{
		scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	}
	else
	{
		scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
	}
}

void MenuLayer::MenuCB2( Ref* sender )
{
	ExplisonScreen* exp = ExplisonScreen::create();
	exp->setPosition(360,640);
	addChild(exp);
}

void MenuLayer::MenuCB3( Ref* sender )
{
	for (int i = 0; i < 30; i++)
	{
		DropGold* testGrop = DropGold::create(	Vec2(360 + rand_minus1_1() * 200, 1500 - rand_0_1()*200),
			ResTag::Drops_Golds1, 10, 100);
		addChild(testGrop);
	}

	return;
	EnemyPlane* testEnePl = EnemyPlane::createWithPicture("plane_1.png");
	testEnePl->setPosition(Vec2(360 , 800));
	//testEnePl->startTrack();
	testEnePl->setBitmask(
		ENEMY_PLANE_CATEGORYBITMASK,
		ENEMY_PLANE_COLLISIONBITMASK,
		ENEMY_PLANE_CONTACTTESTBITMASK);
	addChild(testEnePl);

	GameData::getInstance()->getWeaponFactory()->AddWeaponWithConfig(testEnePl, 0);
	return;

	Bow* arrow = Bow::create();
	arrow->setPosition(360,150);
	addChild(arrow);
	return;

	GrenadeBullect* liudan = GrenadeBullect::create("plane_1.png");
	liudan->getPhysicsBody()->setMass(0.1);
	liudan->setBitmask(
		ENEMY_AMMO_CATEGORYBITMASK,
		ENEMY_AMMO_COLLISIONBITMASK,
		ENEMY_AMMO_CONTACTTESTBITMASK);
	addChild(liudan);
	return;

	FallingStone* stone = FallingStone::create();
	stone->setPosition(360 + rand_minus1_1() * 200, 1500);
	UIController::getInstance()->getPhysicsLayer()->addChild(stone);
	return;

	auto gd = GameData::getInstance();
	auto testPl = gd->getPlaneFactory()->createEnemyPlane(1, 1.0, 1);	//敌方飞机
	testPl->setPosition(360,800);
	//log("------ weapon id = %d",m_bulletId);
	EnemyPlane* norPl = dynamic_cast<EnemyPlane*>(testPl);
	if (norPl)
	{
		norPl->setWeaponPro(WeaponPro(m_bulletId, ResTag::Bullet_EnemyNor1, 1));
	}

	gd->getPlaneAction()->AddEnemyActionByIdx(0,testPl);			//飞机出场动作
	gd->getPlaneAiFactory()->AddEnemyAiByIdx(testPl,9);				//飞机自己的AI
	UIController::getInstance()->getPhysicsLayer()->addChild(testPl,-1000);
	return;

	

	GameData::getInstance()->getPlaneManager()->Rest(10.0f);
	return;

	auto testSp = EnemyPlane::createWithPicture("plane_1.png");
	addChild(testSp);
	testSp->setBitmask(
		ENEMY_PLANE_CATEGORYBITMASK,
		ENEMY_PLANE_COLLISIONBITMASK,
		ENEMY_PLANE_CONTACTTESTBITMASK);

	auto mv = GameData::getInstance()->getPlaneAction()->CreateBezierFromLP2RP(testSp);
	testSp->runAction(mv);

	return;

	SpecialSkillTips* tips = SpecialSkillTips::create(1);
	addChild(tips);
	return;

	CompleteGame* layer  = CompleteGame::create(1);
	UIController::getInstance()->getEffectsLayer()->addChild(layer,1000);
	return;

	auto pl = GameData::getInstance()->getMainPlane();
	if (!pl)
	{
		return;
	}

	UIController::getInstance()->getGameBgMgr()->SwitchBg(1);
	return;

	SpecialSkillTips* skilltips = SpecialSkillTips::create(2);
	addChild(skilltips);
	return;

	//第一种
	/*for (int i = 0; i < 10; i++)
	{
		Bullet1OfBoss4* bullet = Bullet1OfBoss4::create(i * 0.1 + 2.0);
		bullet->setPosition(Vec2(100 + 55 * i, 640));
		addChild(bullet);
	}*/

	//第二种
	//for (int i = 0; i < 2; ++i)
	//{
	//	Weapon* wp = Weapon::create(m_target);
	//	wp->setBullstType(BulletType::Enemy_Boss4Bezier);
	//	wp->setZorder(-100);
	//	wp->setPosition(360, 850);
	//	wp->ChangePro(0,0.3,3.0);			//设置武器的属性
	//  wp->StartShoot();
	//	addChild(wp);
	//	wp->setRotation(-45 + 90 * i);
	//}

	//第三种
	/*Bullet3OfBoss4* bullet = Bullet3OfBoss4::create();
	addChild(bullet);*/

	//第四种
	Bullet4OfBoss5* bullet = Bullet4OfBoss5::create(0.0);
	bullet->setPosition(Vec2(360, 800));
	addChild(bullet);

	return;

}

void MenuLayer::MenuCB4( Ref* sender )
{
	auto pl = GameData::getInstance()->getMainPlane();
	if (!pl)
	{
		return;
	}

	TalismanLayer* Talisman = TalismanLayer::create();
	Talisman->setduration(6);
	pl->addChild(Talisman,-1);
	return;

	m_bulletId++;
	if (m_bulletId > 18)
	{
		m_bulletId = 18;
	}
	return;
	//输出统计
	GameData::getInstance()->AnalyseStaticsData();
	auto data = GameData::getInstance()->getAllStatistics();
	return;

	CompleteGame* layer  = CompleteGame::create(2);
	UIController::getInstance()->getMainUILayer()->addChild(layer,1000);
	return;

	SpecialSkillTips* tips = SpecialSkillTips::create(3);
	addChild(tips);
	return;

	
}

void MenuLayer::MenuCB5( Ref* sender )
{
	MeteorLayer* liuxing = MeteorLayer::create();
	addChild(liuxing);
	return;

	auto mainPl = GameData::getInstance()->getMainPlane();
	if (!mainPl)
	{
		return;
	}
	auto pos = mainPl->convertToWorldSpaceAR(Vec2::ZERO);
	auto s = mainPl->getContentSize();
	BulletSurroundMainPl* bullet = BulletSurroundMainPl::create(3);
	bullet->resetHurt(-500);
	bullet->setBelongTo(UnitId::eRedCard);
	bullet->setPosition(s.width*0.5,s.height*0.5);
	mainPl->addChild(bullet);
	return;

	m_bulletId--;
	if (m_bulletId < 1)
	{
		m_bulletId = 1;
	}
	return;

	


	CompleteGame* layer  = CompleteGame::create(3);
	UIController::getInstance()->getEffectsLayer()->addChild(layer,1000);
	return;

	UIController::getInstance()->getGameBgMgr()->SwitchBg(3);
	return;


}

void MenuLayer::MenuCB6( Ref* sender )
{
	//通关动画
	int curRole = NewDataMgr::getInstance()->mRoleConfig.roleIndex;
	CompleteGame* layer = CompleteGame::create(curRole);
	UIController::getInstance()->getMainUILayer()->addChild(layer, 1000);
}

void MenuLayer::MenuCB7( Ref* sender )
{
}

void MenuLayer::MenuCB8( Ref* sender )
{
}

void MenuLayer::MenuCB9( Ref* sender )
{
	
}

void MenuLayer::MenuCB10( Ref* sender )
{

}

void MenuLayer::MenuCB11( Ref* sender )
{
	
}

void MenuLayer::MenuCB12( Ref* sender )
{

}

void MenuLayer::addTestMenu( const char* info, const Point& pos, SEL_MenuHandler callback )
{
	MenuItem* menuItem = MenuItemImage::create("menu/menuN.png","menu/menuD.png","menu/menuD.png",this,callback);
	menuItem->setPosition(pos);

	Menu* menu = Menu::create(menuItem,nullptr);
	menu->setPosition(Point::ZERO);
	addChild(menu);
	m_menuList.push_back(menu);

	//功能简介
	Label* label = Label::create(info,"",24);
	label->setAnchorPoint(ccp(0,0.5));
	label->setPosition(ccp(40,menuItem->getContentSize().height*0.5));
	menuItem->addChild(label);
}

void MenuLayer::menuSwitch()
{
	for (auto it = m_menuList.begin(); it != m_menuList.end(); it++)
	{
		Menu* menu = (*it);
		menu->setVisible(m_isShowMenu);
		menu->setEnabled(m_isShowMenu);
	}

	m_isShowMenu = !m_isShowMenu;
}

bool MenuLayer::onTouchBegan( Touch *touch, Event *unused_event )
{
	return true;
}

void MenuLayer::onTouchEnded( Touch *touch, Event *unused_event )
{
	auto pos = touch->getLocation();
	if (isTouchInArea(Vec2(0,m_winSize.height), pos, 200))
	{
		m_Pos_1_State = 1;
		m_isListening = true;
		//开始计时，3S内完成
		ActionInterval* delay = DelayTime::create(3.0f);
		CallFunc* func = CallFunc::create(this,callfunc_selector(MenuLayer::ChangeListeningState));
		ActionInterval* seq = Sequence::create(delay, func, nullptr);
		runAction(seq);
	}

	if (m_isListening)
	{
		if (isTouchInArea(Vec2(m_winSize.width * 0.5,m_winSize.height), pos, 200))
		{
			m_Pos_2_State = 1;
		}
		if (m_Pos_2_State && isTouchInArea(Vec2(m_winSize.width,0), pos, 200))
		{
			m_Pos_3_State = 1;
		}
		if (m_Pos_2_State && m_Pos_3_State && isTouchInArea(Vec2(0,0), pos, 200))
		{
			m_Pos_4_State = 1;
			if (m_Pos_1_State && m_Pos_2_State && m_Pos_3_State && m_Pos_4_State)
			{
				m_isListening = false;
				menuSwitch();
			}
		}
	}
}

bool MenuLayer::isTouchInArea( const Vec2& amiPos, const Vec2& curPos, float radius )
{
	if (curPos.x > amiPos.x - radius && curPos.x < amiPos.x + radius &&
		curPos.y > amiPos.y - radius && curPos.y < amiPos.y + radius)
	{
		return true;
	}
	return false;
}

void MenuLayer::ChangeListeningState()
{
	m_isListening = false;
	m_Pos_1_State = 0;
	m_Pos_2_State = 0;
	m_Pos_3_State = 0;
	m_Pos_4_State = 0;
}
