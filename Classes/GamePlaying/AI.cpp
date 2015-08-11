#include "AI.h"
#include "BasePlane/BasePlane.h"
#include "Weapon/weapon.h"
#include "Weapon/WeaponFactory.h"
#include "../GameCommon/GameData.h"
#include "../GameCommon/EffectsManager.h"
#include "../GameCommon/GameConfig.h"
#include "../GameCommon/GameDefine.h"
#include "../../cocos2d/cocos/editor-support/cocostudio/CCArmature.h"
#include "GameCommon/GlobalSchedule.h"
#include "Bullet/BulletFactory.h"
#include "Layer/UIController.h"
#include "Layer/PhysicsLayer.h"
#include "Bullet/Bullet.h"
#include "BasePlane/PlaneManager.h"


bool PlaneAI::init()
{
	if (!Node::init())
	{
		return false;
	}
	m_speed = 0.3;
	m_winSize = Director::getInstance()->getWinSize();

	return true;
}

float PlaneAI::getShootInterval()
{
	auto curLvl = GameData::getInstance()->getPlaneManager()->getCurLvl();
	float interval = m_ShootInterval - curLvl * 0.1;		//随着关卡等级提高，发射间隔减少
	return interval;
}

void PlaneAI::StartShoot()
{
	auto func = CallFunc::create(this, callfunc_selector(PlaneAI::shoot));
	auto delay = DelayTime::create(getShootInterval());
	auto seq = Sequence::create(func, delay, nullptr);
	auto rep = RepeatForever::create(seq);
	runAction(rep);
}

void PlaneAI::shoot()
{
	EnemyPlane* pl = dynamic_cast<EnemyPlane*>(m_target);
	if (!pl)
	{
		return;
	}

	//飞机皮肤由图片创建
	auto pic = pl->getPicture();
	if (pic)
	{
		pl->addWeapon();
		return;
	}

	//飞机皮肤由骨骼创建
	auto armat = pl->getPlaneArmature();
	if (armat)
	{
		armat->playShootAnimation();
	}
}

bool EnemyAiOfMoveDown::init()
{
	if (!PlaneAI::init())
	{
		return false;
	}

	m_speed = 0.3;
	m_direc = (cocos2d::random(0,1) - 0.5);
	auto body = m_target->getPhysicsBody();
	body->setLinearDamping(5.0f);
	body->setGravityEnable(true);
	return true;
}

void EnemyAiOfMoveDown::StartAI(int lvl)
{
	m_AIlvl = lvl;
	StartShoot();
	schedule(schedule_selector(EnemyAiOfMoveDown::FloatDown),m_speed);
}

void EnemyAiOfMoveDown::FloatDown(float dt)
{
	auto body = m_target->getPhysicsBody();
	body->applyForce(Vec2(rand_minus1_1()*0.3,-rand_0_1()) * 100,Vec2(0,0));
}

//-----------------------------

bool EnemyAiOfFlyToTarget::init()
{
	if (!PlaneAI::init())
	{
		return false;
	}

	m_speed = 2.0f;

	return true;
}

void EnemyAiOfFlyToTarget::StartAI(int lvl)
{
	m_AIlvl = lvl;
	StartShoot();
	schedule(schedule_selector(EnemyAiOfFlyToTarget::FlyToTarget));
}

void EnemyAiOfFlyToTarget::FlyToTarget( float t )
{
	auto mainPl = GameData::getInstance()->getMainPlane();
	if (mainPl)
	{
		//朝向目标
		Vec2 myPos = m_target->convertToWorldSpaceAR(Vec2::ZERO);
		Vec2 targetPos = mainPl->convertToWorldSpaceAR(Vec2::ZERO);

		Vec2 movePos = MoveStep(targetPos,myPos,m_speed);

		m_target->setPosition(movePos);
	}
	else
	{
		//直线向下
		Vec2 myPos = m_target->convertToWorldSpaceAR(Vec2::ZERO);
		Vec2 targetPos = Vec2(myPos.x, -10000);

		Vec2 movePos = MoveStep(targetPos,myPos,m_speed);
		m_target->setPosition(movePos);
	}
}

//---------------------------
bool EnemyAiOfHoverAttack::init()
{
	if (!PlaneAI::init())
	{
		return false;
	}
	m_aimSp = nullptr;
	m_speed = 2.0f;

	return true;
}

void EnemyAiOfHoverAttack::StartAI(int lvl)
{
	m_AIlvl = lvl;
	StartShoot();
	Hover();
}

void EnemyAiOfHoverAttack::Hover()
{
	float dirct = cocos2d::random(0,1) - 0.5;
	auto pos = m_target->convertToWorldSpaceAR(Vec2::ZERO);
	float disX;
	if (dirct > 0)
	{
		//向右
		 disX = m_winSize.width * 0.95 - pos.x;
	}
	else
	{
		disX = m_winSize.width * 0.05 - pos.x;
	}
	disX = disX * cocos2d::random(0.5, 0.9);
	ActionInterval* moveby = MoveBy::create(1.0f + m_speed * 0.1f,ccp(disX,0));
	ActionInterval* moveback = moveby->reverse();
	ActionInterval* seq = Sequence::create(moveby,moveback,nullptr);
	ActionInterval* repeat = Repeat::create(seq,cocos2d::random(1,2));
	CallFunc* func = CallFunc::create(this,callfunc_selector(EnemyAiOfHoverAttack::AimMove));
	ActionInterval* seq1 = Sequence::create(repeat,func,nullptr);
	m_target->runAction(seq1);
}

void EnemyAiOfHoverAttack::AimMove()
{
	auto pl = GameData::getInstance()->getMainPlane();
	if (!pl)
	{
		Hover();		//主飞机不存在那就继续悬停
		return;
	}

	Size s = m_target->getContentSize();
	if (!m_aimSp)
	{
		m_aimSp = Sprite::create("Image/aim.png");
		m_aimSp->setPosition(s.width*0.5,s.height*0.5);
		m_target->addChild(m_aimSp);
	}

	m_aimSp->setVisible(true);

	//需要移动的距离
	Vec2 myPos = m_target->convertToWorldSpaceAR(Vec2::ZERO);
	Vec2 plPos = pl->convertToWorldSpaceAR(Vec2::ZERO);

	//实际目标位置
	m_aimPos = myPos - plPos;

	//偏上位置
	Vec2 offsetPos = Vec2(plPos.x, plPos.y + 100) - myPos;

	ActionInterval* moveby = MoveBy::create(0.7 - m_speed*0.1,offsetPos);
	ActionInterval* fadein = FadeIn::create(0.07f - m_speed*0.01);
	ActionInterval* fadeout = FadeOut::create(0.07f - m_speed*0.01);
	ActionInterval* seq = Sequence::create(fadein,fadeout,nullptr);
	ActionInterval* repeat = Repeat::create(seq,5);
	CallFunc* func = CallFunc::create(this,callfunc_selector(EnemyAiOfHoverAttack::AimMoveCallback));
	ActionInterval* seq1 = Sequence::create(moveby,repeat,func,nullptr);
	m_aimSp->runAction(seq1);
}

void EnemyAiOfHoverAttack::AimMoveCallback()
{
	m_aimSp->removeFromParentAndCleanup(true);
	m_aimSp = nullptr;

	//敌机冲向目标
	MoveToMainPl();
}

void EnemyAiOfHoverAttack::MoveToMainPl()
{
	auto pl = GameData::getInstance()->getMainPlane();
	if (!pl)
	{
		Hover();		//主飞机不存在那就继续悬停
		return;
	}

	//飞机皮肤由骨骼创建
	EnemyPlane* enePl = dynamic_cast<EnemyPlane*>(m_target);
	auto armat = enePl->getPlaneArmature();
	if (armat)
	{
		armat->playHackAnimation();
	}

	ActionInterval* moveTo = MoveBy::create(0.7f - m_speed*0.1,-m_aimPos);
	ActionInterval* moveback = moveTo->reverse();
	CallFunc* func = CallFunc::create(this,callfunc_selector(EnemyAiOfHoverAttack::Hover));
	ActionInterval* seq = Sequence::create(moveTo,moveback,func,nullptr);
	m_target->runAction(seq);
}

//------------------------
bool EnemyAiOfFloat::init()
{
	if (!PlaneAI::init())
	{
		return false;
	}
	m_speed = 2.0f;

	return true;
}

void EnemyAiOfFloat::StartAI(int lvl)
{
	m_AIlvl = lvl;
	m_OriginPos = m_target->convertToWorldSpaceAR(Vec2::ZERO);

	StartShoot();
	RandFloat();

}

void EnemyAiOfFloat::RandFloat()
{
	Vec2 myPos = m_target->convertToWorldSpaceAR(Vec2::ZERO);
	Vec2 endPos = myPos;
	if (myPos.x > m_winSize.width * 0.85)
	{
		endPos.x = m_winSize.width * 0.5  - rand_0_1() * m_winSize.width * 0.35;
	}
	else if(myPos.x < m_winSize.width * 0.15)
	{
		endPos.x = m_winSize.width * 0.5  + rand_0_1() * m_winSize.width * 0.35;
	}
	else
	{
		int randNum = cocos2d::random(0,1);
		if (randNum) //向右
		{
			endPos.x = endPos.x + ( m_winSize.width * 0.9 - endPos.x ) * cocos2d::random(0.5f,1.0f);
		}
		else
		{
			endPos.x = endPos.x - ( endPos.x - m_winSize.width * 0.1 ) * cocos2d::random(0.5f,1.0f);
		}
	}

	ActionInterval* moveby = MoveTo::create(2.0f - m_speed * 0.1f,Vec2(endPos.x, m_OriginPos.y));
	CallFunc* next = CallFunc::create(this,callfunc_selector(EnemyAiOfFloat::RandFloat));
	ActionInterval* seq = Sequence::create(moveby,next,nullptr);
	m_target->runAction(seq);
}

//-----------------------------------

ActionInterval* BossAI::randMove()
{
	auto mypos = m_target->convertToWorldSpaceAR(Vec2::ZERO);
	Vec2 endpos;
	if (m_direct < 0)
	{
		//向左
		endpos = Vec2((m_winSize.width * 0.25 - mypos.x) * random(0.5f, 1.0f),0);
	}
	else
	{
		//向右
		endpos = Vec2((m_winSize.width * 0.75 - mypos.x) * random(0.5f, 1.0f),0);
	}

	ActionInterval* move = MoveBy::create( 3.0f, endpos);

	return move;
}

ActionInterval* BossAI::MoveToMidRight()
{
	Vec2 pos = Vec2(m_winSize.width * 0.63, m_winSize.height * 0.8);
	auto dis = getDis(pos);

	ActionInterval* mvto = MoveTo::create(0.5f + dis/300 * 0.5, pos );
	return mvto;
}

ActionInterval* BossAI::MoveToMid()
{
	Vec2 pos = Vec2(m_winSize.width * 0.5, m_winSize.height * 0.8);
	auto dis = getDis(pos);

	ActionInterval* mvto = MoveTo::create(0.5f + dis/300 * 0.5, pos );
	return mvto;
}

ActionInterval* BossAI::MoveToEdge()
{
	//先快速移动到起点
	int RandNum = cocos2d::random(0,1);
	float Randf = RandNum * 1.0f;
	Randf -= 0.5f;
	Vec2 startPos = Vec2( m_winSize.width*(0.5 + Randf * 0.4), m_winSize.height * 0.8);
	ActionInterval* mvStartPos = MoveTo::create(0.5f,startPos);

	//再移动到屏幕的一端
	ActionInterval* mvToEdge = MoveTo::create(2.5f, Vec2(m_winSize.width*(0.5 - Randf * 0.4),
		m_winSize.height * 0.8));
	ActionInterval* seq = Sequence::create(mvStartPos,mvToEdge,nullptr);

	return seq;
}

ActionInterval* BossAI::MoveDown()
{
	//找到主角的位置
	auto mypos = m_target->convertToWorldSpaceAR(Vec2::ZERO);
	ActionInterval* mv = MoveBy::create(0.5f,Vec2(0, -mypos.y * 0.8));
	ActionInterval* back = mv->reverse();
	ActionInterval* seq = Sequence::create(mv, back, nullptr);

	return seq;
}

ActionInterval* BossAI::MoveToMainPl()
{
	//找到主角的位置
	auto pl = GameData::getInstance()->getMainPlane();
	Vec2 pos;
	if (!pl)
	{
		pos = Vec2(360,100);
	}
	else
	{
		pos = pl->convertToWorldSpaceAR(Vec2::ZERO);
		pos.y -= 50;
	}
	auto mypos = m_target->convertToWorldSpaceAR(Vec2::ZERO);
	ActionInterval* mv = MoveTo::create(0.5f,pos);
	ActionInterval* back = MoveTo::create(0.5f,mypos);
	ActionInterval* seq = Sequence::create(mv, back, nullptr);

	return seq;
}

float BossAI::getDis( const Vec2& pos )
{
	auto mypos = m_target->convertToWorldSpaceAR(Vec2::ZERO);
	return mypos.distance(pos);
}

void BossAI::playArmat( int idx,Ref *target, SEL_FrameEventCallFunc callFunc,float speedScale /*= 1.0f*/ )
{
	BaseBoss* boss = (BaseBoss*)m_target;
	auto armat = boss->getArmature();
	armat->getAnimation()->playWithIndex(idx);
	armat->getAnimation()->setFrameEventCallFunc(target,callFunc);  
	armat->getAnimation()->setSpeedScale(speedScale);
}

void BossAI::playArmat( int idx, float speedScale /*= 1.0f*/ )
{
	BaseBoss* boss = (BaseBoss*)m_target;
	auto armat = boss->getArmature();
	armat->getAnimation()->playWithIndex(idx);
	armat->getAnimation()->setSpeedScale(speedScale);
}

void BossAI::changeDirect()
{
	ActionInterval* delay = DelayTime::create(0.8 + rand_0_1()*1.5f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(BossAI::changeDirCB));
	ActionInterval* seq = Sequence::create(delay, func, nullptr);
	runAction(seq);
}

void BossAI::changeDirCB()
{
	m_direct *= -1.0f;

	changeDirect();
}

void BossAI::DeathAction()
{
	m_target->stopAllActions();
	m_target->setShootAble(false);
	this->stopAllActions();
	playArmat(0);
	DeadArmature();
}

void BossAI::DeadArmature()
{
	BaseBoss* boss = dynamic_cast<BaseBoss*>(m_target);
	auto armt = boss->getArmature();
	armt->getAnimation()->pause();
	stopAllActions();
	boss->getHpBar()->SetHpBarVisible(false);

	//移出屏幕 + 变小
	auto s = Director::getInstance()->getWinSize();
	auto myPos = this->convertToWorldSpaceAR(Vec2::ZERO);
	_ccBezierConfig bezc;

	if (myPos.x > 360)
	{
		bezc.controlPoint_1 = Vec2(-100, 50);
		bezc.controlPoint_2 = Vec2(- 400, -200 );
		bezc.endPosition = Vec2(- 800, -300 );
	}
	else
	{
		bezc.controlPoint_1 = Vec2(100, 50);
		bezc.controlPoint_2 = Vec2(400, -200 );
		bezc.endPosition = Vec2(800, -300 );
	}

	float stay = 2.5f;
	ActionInterval* delay = DelayTime::create(stay);
	ActionInterval* bezier = BezierBy::create(2.0f,bezc);
	ActionInterval* scale = ScaleTo::create(2.0f,0.2f);
	CallFunc* realDead = CallFunc::create(m_target,callfunc_selector(BaseBoss::RealDead));//艹，这里不要大意，一定要传BOSS对象，而非this
	ActionInterval* rotate = RotateBy::create(2.0,180);
	ActionInterval* seq = Sequence::create(DelayTime::create(stay),bezier,realDead,nullptr);
	ActionInterval* seq1 = Sequence::create(delay,scale,nullptr);
	ActionInterval* seq2 = Sequence::create(DelayTime::create(stay),rotate,nullptr);
	ActionInterval* fadeOut = FadeOut::create(7.0f);
	ActionInterval* tint = TintTo::create(2.0f,150,30,30);

	m_target->runAction(seq);
	m_target->runAction(seq2);
	armt->runAction(seq1);
	//armt->runAction(fadeOut);
	armt->runAction(tint);
}

int BossAI::getBossHurts()
{
	int hurts = m_target->getContactPro().hurts;
	return hurts;
}

//----------------------------------
PlaneAIFactory* PlaneAIFactory::create()
{
	PlaneAIFactory* pf = new (std::nothrow) PlaneAIFactory;
	if (pf && pf->init())
	{
		pf->retain();
		pf->autorelease();
		return pf;
	}
	else
	{
		CC_SAFE_DELETE(pf);
		return nullptr;
	}
}

void PlaneAIFactory::purge()
{
	this->release();
}

bool PlaneAIFactory::init()
{
	return true;
}

PlaneAI* PlaneAIFactory::AddEnemyAiByIdx(BasePlane* pl, int id )
{
	PlaneAI* ai = nullptr;
	switch (id)
	{
	case 1:
		{
			//慢速飘落
			ai = EnemyAiOfMoveDown::create(pl);
			ai->setSpeed(0.4f);
		}
		break;
	case 2:
		{
			//快速飘落
			ai = EnemyAiOfMoveDown::create(pl);
			ai->setSpeed(0.2f);
		}
		break;
	case 3:
		{
			//慢速跟随主飞机
			ai = EnemyAiOfFlyToTarget::create(pl);
			ai->setSpeed(2.0f);
		}
		break;
	case 4:
		{
			//快速跟随主飞机
			ai = EnemyAiOfFlyToTarget::create(pl);
			ai->setSpeed(3.0f);
		}
		break;
	case 5:
		{
			//慢速悬浮左右移动
			ai = EnemyAiOfFloat::create(pl);
			ai->setSpeed(2.0f);
		}
		break;
	case 6:
		{
			//快速悬浮左右移动
			ai = EnemyAiOfFloat::create(pl);
			ai->setSpeed(4.0f);
		}
		break;
	case 7:
		{
			//慢速悬浮左右移动 + 攻击
			ai = EnemyAiOfHoverAttack::create(pl);
			ai->setSpeed(1.0f);
		}
		break;
	case 8:
		{
			//快速悬浮左右移动 + 攻击
			ai = EnemyAiOfHoverAttack::create(pl);
			ai->setSpeed(2.0f);
		}
		break;
	case 9:
		{
			ai = EnemyAiofStay::create(pl);
		}
		break;
	case 10:
		{
			ai = EnemyAiofFree::create(pl);
		}
		break;
	default:
		break;

	}
	if (ai)
	{
		pl->setSelfAi(ai);
		pl->addChild(ai);
	}
	return ai;
}

void IronManBossAI::initAction()
{
	switch (m_AIlvl)
	{
	case 1:
		{
			//m_ActionVec.push_back(5);
			m_ActionVec.push_back(0);
			m_ActionVec.push_back(1);
			m_ActionVec.push_back(2);
			m_ActionVec.push_back(3);
			m_ActionVec.push_back(4);
			m_ActionVec.push_back(5);
		}
		break;
	case 2:
		{
			m_ActionVec.push_back(2);
			m_ActionVec.push_back(3);
			m_ActionVec.push_back(5);
			m_ActionVec.push_back(1);
			m_ActionVec.push_back(3);
			m_ActionVec.push_back(2);
			m_ActionVec.push_back(5);
			m_ActionVec.push_back(1);
			m_ActionVec.push_back(0);
			m_ActionVec.push_back(5);
			m_ActionVec.push_back(2);
			m_ActionVec.push_back(3);
			m_ActionVec.push_back(1);
			m_ActionVec.push_back(0);
			m_ActionVec.push_back(5);
			m_ActionVec.push_back(2);
			m_ActionVec.push_back(3);
			m_ActionVec.push_back(0);
			m_ActionVec.push_back(1);
			m_ActionVec.push_back(5);
		}
		break;
	case 3:
		{
			m_ActionVec.push_back(6);
			m_ActionVec.push_back(1);
			m_ActionVec.push_back(4);
			m_ActionVec.push_back(7);
			m_ActionVec.push_back(1);
			m_ActionVec.push_back(6);
			m_ActionVec.push_back(4);
			m_ActionVec.push_back(1);
			m_ActionVec.push_back(7);
			m_ActionVec.push_back(4);
		}
		break;

	default:
		break;
	}
	

	m_it = m_ActionVec.begin();
}

void IronManBossAI::NextAction()
{
	playArmat(0);
	auto action = randMove();
	m_target->runAction(action);
	
	ActionInterval* delay = DelayTime::create(1.0f + rand_0_1() * 2);
	CallFunc* next = CallFunc::create(this,callfunc_selector(IronManBossAI::NewAction));
	ActionInterval* seq = Sequence::create(delay, next, nullptr);
	m_target->runAction(seq);
}

void IronManBossAI::Action1()
{
	auto action = MoveToMidRight();
	CallFunc* shangtang = CallFunc::create(this,callfunc_selector(IronManBossAI::PlayArmatWithReady));
	ActionInterval* seq = Sequence::create(action,shangtang,nullptr);

	//子弹
	ActionInterval* ready = DelayTime::create(1.2f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(IronManBossAI::PlayArmatWith1Sword));
	ActionInterval* delay2 = DelayTime::create(0.3f);
	ActionInterval* seq1 = Sequence::create(func,delay2,nullptr);
	ActionInterval* rep = Repeat::create(seq1,10);
	CallFunc* func1 = CallFunc::create(this,callfunc_selector(IronManBossAI::NextAction));
	ActionInterval* seq2 = Sequence::create(seq,ready,rep,func1,nullptr);

	m_target->runAction(seq2);
	
}

void IronManBossAI::Action2()
{
	//移动
	auto action = MoveToMidRight();
	PlayArmatWithReady();

	//子弹
	ActionInterval* ready = DelayTime::create(0.6f);
	ActionInterval* delay = DelayTime::create(2.0f);
	CallFunc* func1 = CallFunc::create(this,callfunc_selector(IronManBossAI::PlayArmatWith3Dart));
	ActionInterval* shootDelay = DelayTime::create(0.35);
	CallFunc* func2 = CallFunc::create(this,callfunc_selector(IronManBossAI::PlayArmatWith2Dart));

	ActionInterval* seq = Sequence::create(func1,shootDelay,func2,delay,nullptr);
	ActionInterval* rep = Repeat::create(seq,2);

	CallFunc* func3 = CallFunc::create(this,callfunc_selector(IronManBossAI::NextAction));
	ActionInterval* seq2 = Sequence::create(action, ready, rep, func3, nullptr);
	m_target->runAction(seq2);
}

void IronManBossAI::Action3()
{
	auto action = randMove();

	PlayArmatWithReady();

	//子弹
	ActionInterval* ready = DelayTime::create(1.5f);
	ActionInterval* delay = DelayTime::create(1.0f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(IronManBossAI::PlayArmatWith3Sword));
	ActionInterval* seq = Sequence::create(func, delay, nullptr);
	ActionInterval* rep = Repeat::create(seq,3);
	ActionInterval* spa = Spawn::create(action, rep, nullptr);

	CallFunc* next = CallFunc::create(this,callfunc_selector(IronManBossAI::NextAction));
	ActionInterval* seq2 = Sequence::create(ready, spa, next, nullptr);
	m_target->runAction(seq2);
}

void IronManBossAI::Action4()
{
	//冲向目标1
	ActionInterval* action = randMove();
	CallFunc* attack = CallFunc::create(this,callfunc_selector(IronManBossAI::PlayArmatWithMvAttack1));
	ActionInterval* seq = Sequence::create(action, attack, nullptr);
	m_target->runAction(seq);
}

void IronManBossAI::Action4_1()
{
	//冲向目标2
	ActionInterval* action = randMove();
	CallFunc* attack = CallFunc::create(this,callfunc_selector(IronManBossAI::PlayArmatWithMvAttack2));
	ActionInterval* seq = Sequence::create(action, attack, nullptr);
	m_target->runAction(seq);
}

void IronManBossAI::Action5()
{
	//朝下冲
	ActionInterval* action = randMove();
	CallFunc* attack = CallFunc::create(this,callfunc_selector(IronManBossAI::PlayArmatWithMvDown));
	ActionInterval* seq = Sequence::create(action, attack, nullptr);
	m_target->runAction(seq);
}

void IronManBossAI::Action6()
{
	auto action = MoveToMidRight();

	PlayArmatWithReady();

	//子弹
	ActionInterval* ready = DelayTime::create(1.5f);
	ActionInterval* delay = DelayTime::create(1.0f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(IronManBossAI::PlayArmatWithMidExpl));
	ActionInterval* seq = Sequence::create(func, delay, nullptr);
	ActionInterval* rep = Repeat::create(seq,3);
	ActionInterval* spa = Spawn::create(action, rep, nullptr);

	CallFunc* next = CallFunc::create(this,callfunc_selector(IronManBossAI::NextAction));
	ActionInterval* seq2 = Sequence::create(ready, spa, next, nullptr);
	m_target->runAction(seq2);
}

void IronManBossAI::Action7()
{
	ActionInterval* action = MoveToMidRight();
	CallFunc* shangtang = CallFunc::create(this,callfunc_selector(IronManBossAI::PlayArmatWithReady));
	ActionInterval* seq = Sequence::create(action,shangtang,nullptr);

	//子弹
	ActionInterval* ready = DelayTime::create(1.2f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(IronManBossAI::PlayArmatWithRandExpl));
	ActionInterval* delay2 = DelayTime::create(2.0f);
	ActionInterval* seq1 = Sequence::create(func,delay2,nullptr);
	ActionInterval* rep = Repeat::create(seq1,6);
	CallFunc* func1 = CallFunc::create(this,callfunc_selector(IronManBossAI::NextAction));
	ActionInterval* seq2 = Sequence::create(seq,ready,rep,func1,nullptr);

	m_target->runAction(seq2);
}

void IronManBossAI::shoot1Sword()
{
	NewAudio::getInstance()->playEffWithPar(Music_Boss1Shoot1,false,0.3f,nullptr);
	Vec2 mypos = m_target->convertToWorldSpaceAR(Vec2::ZERO);
	Vec2 gunPos = mypos + getGunPos();

	Vec2 AmiPos = Vec2(mypos.x, -1280);
	auto pl = GameData::getInstance()->getMainPlane();
	if (pl)
	{
		AmiPos = pl->convertToWorldSpaceAR(Vec2::ZERO);
	}

	//计算旋转角度
	Vec2 newVec = AmiPos - mypos;
	float angle = 270 - newVec.getAngle() * Pi2Angle;
	setGunRotate(angle);

	Weapon* wp = Weapon::create(m_target);
	wp->setBullstType(BulletType::Enemy_BossDart);
	wp->setShootTimes(1);
	wp->setBltHurts(getBossHurts());
	m_target->AddOneWp(wp,WeaponType::Addition);						
	wp->setOffset(getShootPos());

	float rot = rand_minus1_1() * 25 + angle;
	wp->setRotation(180 + rot);

}

void IronManBossAI::shoot3Sword()
{
	NewAudio::getInstance()->playEff(Music_Boss1Shoot1);

	Vec2 mypos = m_target->convertToWorldSpaceAR(Vec2::ZERO);
	Vec2 gunPos = mypos + getGunPos();
	
	Vec2 AmiPos = Vec2(mypos.x, -1280);
	auto pl = GameData::getInstance()->getMainPlane();
	if (pl)
	{
		AmiPos = pl->convertToWorldSpaceAR(Vec2::ZERO);
	}
	
	//计算旋转角度
	Vec2 newVec = AmiPos - mypos;
	float angle = 270 - newVec.getAngle() * Pi2Angle;
	setGunRotate(angle);

	//计算旋转后的发射点
	newVec.normalize();
	newVec *= 100;
	Vec2 shootPos = getGunPos() + newVec;

	for(int i = 0; i < 3; ++i)
	{
		Weapon* wp = Weapon::create(m_target);
		wp->setBullstType(BulletType::Enemy_BossDart);
		wp->setShootTimes(1);
		wp->setBltHurts(getBossHurts());
		m_target->AddOneWp(wp,WeaponType::Addition);
		auto offset = Vec2((1-i) * 20, -i%2 * 50) + shootPos;
		wp->setOffset(offset);
		wp->setRotation(180 + angle + (i-1)*10);
	}
}

void IronManBossAI::shoot3Dart()
{
	NewAudio::getInstance()->playEff(Music_Boss1Shoot1);
	auto GunPos = getShootPos();
	for(int i = 0; i < 3; ++i)
	{
		Weapon* wp = Weapon::create(m_target);
		wp->setBullstType(BulletType::Enemy_BossDart);
		wp->setShootTimes(1);
		wp->setBltSpeed(1500);
		wp->setBltLinearDamping(1.2f);
		wp->setBltHurts(getBossHurts());
		wp->setRotation((i-1)*20 + 180);
		
		Vec2 offset = Vec2(Vec2((1-i) * 15, 0) + GunPos);
		if (i == 1)
		{
			offset =(Vec2((1-i) * 15, 0) + GunPos);
		}
		wp->setPosition(offset);
		m_target->AddOneWp(wp, WeaponType::Addition);
	}
}

void IronManBossAI::shoot2Dart()
{
	NewAudio::getInstance()->playEff(Music_Boss1Shoot1);
	
	for(int i = 0; i < 2; ++i)
	{
		Weapon* wp = Weapon::create(m_target);
		wp->setBullstType(BulletType::Enemy_BossDart);
		wp->setShootTimes(1);
		wp->setBltSpeed(1500);
		wp->setBltLinearDamping(1.2f);
		wp->setBltHurts(getBossHurts());
		wp->setRotation((i-0.5)*25 + 180);

		Vec2 offset = Vec2(Vec2((i-0.5) * 25, 0) + getShootPos());
		wp->setPosition(offset);

		m_target->AddOneWp(wp, WeaponType::Addition);
	}
}

void IronManBossAI::shootExplosionInMid()
{
	auto mypos = m_target->convertToWorldSpaceAR(Vec2::ZERO);

	setGunRotate(0);

	ExplosionBullet* bullet = ExplosionBullet::create(0);
	bullet->setPosition(getShootPos() + mypos);
	UIController::getInstance()->getPhysicsLayer()->addChild(bullet);
}

void IronManBossAI::shootExplosionRand()
{
	auto mypos = m_target->convertToWorldSpaceAR(Vec2::ZERO);

	float angle = random(-1,1)*30;
	setGunRotate(angle);

	ExplosionBullet* bullet = ExplosionBullet::create(angle);
	bullet->setPosition(getShootPos() + mypos);
	UIController::getInstance()->getPhysicsLayer()->addChild(bullet);
}

void IronManBossAI::StartAI(int lvl)
{
	m_AIlvl = lvl;

	m_direct = cocos2d::random(0,1) - 0.5;

	changeDirect();

	initAction();
	NewAction();
	
}

void IronManBossAI::NewAction()
{
	playArmat(0);
	int tag = 0;
	if (m_it == m_ActionVec.end())
	{
		//随机
		switch (m_AIlvl)
		{
		case 1:
			tag = 5;
			break;
		case 2:
			{
				float randi = rand_0_1() * 1.25;
				if (randi < 0.25)
				{
					tag = 2;
				}
				else if(randi < 0.5)
				{
					tag = 1;
				}
				else if(randi < 0.75)
				{
					tag = 5;
				}
				else if(randi < 1.0)
				{
					tag = 3;
				}
			}
			break;
		case 3:
			{
				float randi = rand_0_1() * 1.25;
				if (randi < 0.25)
				{
					tag = 6;
				}
				else if(randi < 0.5)
				{
					tag = 1;
				}
				else if(randi < 0.75)
				{
					tag = 4;
				}
				else if(randi < 1.0)
				{
					tag = 7;
				}
			}
			break;

		default:
			break;
		}
	}
	else
	{
		tag = (*m_it);
		m_it++;
	}
	switch (tag)
	{
	case 0:
		NewAction();
		break;
	case 1:
		Action1();
		break;
	case 2:
		Action2();
		break;
	case 3:
		Action3();
		break;
	case 4:
		Action4();
		break;
	case 5:
		Action5();
		break;
	case 6:
		Action6();
		break;
	case 7:
		Action7();
		break;
	default:
		break;
	}
}

void IronManBossAI::onFrameEventOfIdex0( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	
}

void IronManBossAI::PlayArmatWith1Sword()
{
	playArmat(1,this,frameEvent_selector(IronManBossAI::onFrameEventOf1Sword),3.0);
}

void IronManBossAI::PlayArmatWith3Sword()
{
	setGunRotate(0);
	playArmat(1,this,frameEvent_selector(IronManBossAI::onFrameEventOf3Sword));
}

void IronManBossAI::PlayArmatWith3Dart()
{
	setGunRotate(0);
	playArmat(1,this,frameEvent_selector(IronManBossAI::onFrameEventOf3Dart),1.0f);
}

void IronManBossAI::PlayArmatWith2Dart()
{
	setGunRotate(0);
	playArmat(1,this,frameEvent_selector(IronManBossAI::onFrameEventOf2Dart),1.0f);
}

void IronManBossAI::PlayArmatWithReady()
{
	NewAudio::getInstance()->playEff(Music_Boss1Ready);
	setGunRotate(0);
	playArmat(2,this,nullptr,1.2f);
}

void IronManBossAI::PlayArmatWithMvAttack1()
{
	setGunRotate(0);
	playArmat(3,this,frameEvent_selector(IronManBossAI::onFrameEventOfAttack1));
}

void IronManBossAI::PlayArmatWithMvAttack2()
{
	playArmat(3,this,frameEvent_selector(IronManBossAI::onFrameEventOfAttack2));
}

void IronManBossAI::PlayArmatWithMvDown()
{
	playArmat(3,this,frameEvent_selector(IronManBossAI::onFrameEventOfMoveDown));
}

void IronManBossAI::PlayArmatWithMidExpl()
{
	playArmat(1,this,frameEvent_selector(IronManBossAI::onFrameEventOfMidExpl));
}

void IronManBossAI::PlayArmatWithRandExpl()
{
	playArmat(1,this,frameEvent_selector(IronManBossAI::onFrameEventOfRandExpl));
}

void IronManBossAI::onFrameEventOf3Dart( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "shoot")
	{
		shoot3Dart();
	}
	else if(evt == "end")  
	{  
		playArmat(0);
	}  
}

void IronManBossAI::onFrameEventOf2Dart( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "shoot")
	{
		shoot2Dart();
	}
	else if(evt == "end")  
	{  
		playArmat(0);
	}  
}

void IronManBossAI::onFrameEventOf1Sword( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "shoot")
	{
		shoot1Sword();
	}
	else if(evt == "end")  
	{  
		playArmat(0);
	}  
}

void IronManBossAI::onFrameEventOf3Sword( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "shoot")
	{
		shoot3Sword();
	}
	else if(evt == "end")  
	{  
		playArmat(0);
	}  
}

void IronManBossAI::onFrameEventOfAttack1( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "attack")
	{
		NewAudio::getInstance()->playEff(Music_Boss1Shoot2);
		ActionInterval* mvtoPl = MoveToMainPl();
		CallFunc* func = CallFunc::create(this,callfunc_selector(IronManBossAI::Action4_1));
		ActionInterval* seq = Sequence::create(mvtoPl,func,nullptr);
		m_target->runAction(seq);
	}
	else if(evt == "end")  
	{  
		playArmat(0);
	}  
}

void IronManBossAI::onFrameEventOfAttack2( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "attack")
	{
		NewAudio::getInstance()->playEff(Music_Boss1Shoot2);
		ActionInterval* mvtoPl = MoveToMainPl();
		CallFunc* func = CallFunc::create(this,callfunc_selector(IronManBossAI::NextAction));
		ActionInterval* seq = Sequence::create(mvtoPl,func,nullptr);
		m_target->runAction(seq);
	}
	else if(evt == "end")  
	{  
		playArmat(0);
	}  
}

void IronManBossAI::onFrameEventOfMoveDown( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "attack")
	{
		NewAudio::getInstance()->playEff(Music_Boss1Shoot2);
		ActionInterval* mvtoPl = MoveDown();
		CallFunc* func = CallFunc::create(this,callfunc_selector(IronManBossAI::NextAction));
		ActionInterval* seq = Sequence::create(mvtoPl,func,nullptr);
		m_target->runAction(seq);
	}
	else if(evt == "end")  
	{  
		playArmat(0);
	}  
}

void IronManBossAI::onFrameEventOfMidExpl( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "shoot")
	{
		shootExplosionInMid();
	}
	else if(evt == "end")  
	{  
		playArmat(0);
	}  
}

void IronManBossAI::onFrameEventOfRandExpl( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "shoot")
	{
		shootExplosionRand();
	}
	else if(evt == "end")  
	{  
		playArmat(0);
	}  
}

void IronManBossAI::setGunRotate( float r )
{
	BaseBoss* boss = (BaseBoss*)m_target;
	auto armat = boss->getArmature();
	Bone* bone = armat->getBone("Layer4");
	bone->setRotation(r);
}

cocos2d::Vec2 IronManBossAI::getShootPos()
{
	return Vec2(-100, -100);
}

cocos2d::Vec2 IronManBossAI::getGunPos()
{
	return Vec2(-100, 50);
}


//----------------------------
void BoneWarriorAI::StartAI(int lvl)
{
	m_AIlvl = lvl;
	m_times = 0;

	m_direct = cocos2d::random(0,1) - 0.5;
	changeDirect();
	initAction();
	FixPos();

	ActionInterval* delay = DelayTime::create(30.0f);
	CallFunc* next = CallFunc::create(this,callfunc_selector(BoneWarriorAI::addLiudan));
	ActionInterval* seq = Sequence::create(delay,next,nullptr);
	runAction(seq);
}

ActionInterval* BoneWarriorAI::randMove()
{
	auto mypos = m_target->convertToWorldSpaceAR(Vec2::ZERO);
	Vec2 endpos;
	if (m_direct < 0)
	{
		//向左
		endpos = Vec2(m_winSize.width * 0.3 - mypos.x,0);
	}
	else
	{
		//向右
		endpos = Vec2(m_winSize.width * 0.6 - mypos.x,0);
	}

	ActionInterval* move = MoveBy::create( 3.0f, endpos);

	return move;
}

void BoneWarriorAI::changeDirect()
{
	ActionInterval* delay = DelayTime::create(0.8 + rand_0_1()*1.5f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(BoneWarriorAI::changeDirCB));
	ActionInterval* seq = Sequence::create(delay, func, nullptr);
	runAction(seq);
}

void BoneWarriorAI::changeDirCB()
{
	m_direct *= -1.0f;

	changeDirect();
}

void BoneWarriorAI::shoot2BezierBullet()
{
	for (int i = 0; i < 2; i++)
	{
		Weapon* wp = Weapon::create(m_target);
		wp->setBullstType(BulletType::Enemy_Bezier);
		wp->setBltBelongTo(UnitId::ePlaneBoss);
		wp->setBltScaleX(1.5);
		wp->setBltScaleY(1.5);
		wp->setShootTimes(1);
		wp->setBltHurts(getBossHurts());
		wp->setOffset(Vec2(120,0));
		wp->setRotation(180 + (i-0.5) * 60);
		m_target->AddOneWp(wp, WeaponType::Addition);
	}
}

void BoneWarriorAI::shoot1JumpBullet()
{
	Weapon* wp = Weapon::create(m_target);
	wp->setBullstType(BulletType::Enemy_BossRebound);
	wp->setBltBelongTo(UnitId::ePlaneBoss);
	wp->setBltScaleX(1.5);
	wp->setBltGravityAble(true);
	wp->setBltScaleY(1.5);
	wp->setShootTimes(1);
	wp->setBltHurts(getBossHurts());
	wp->setOffset(Vec2(120,0));
	wp->setRotation(180 + m_direct * (90 + rand_minus1_1() * 60));
	//wp->setRotation(rand_minus1_1() * 360);
	m_target->AddOneWp(wp, WeaponType::Addition);

}

void BoneWarriorAI::Action1()
{
	auto mv = randMove();
	m_target->runAction(mv);

	ActionInterval* delay = DelayTime::create(1.5f + rand_minus1_1() * 0.5);
	ActionInterval* delay1 = DelayTime::create(5.0);
	CallFunc* func = CallFunc::create(this,callfunc_selector(BoneWarriorAI::ShootReboundDown));
	CallFunc* func1 = CallFunc::create(this,callfunc_selector(BoneWarriorAI::NextAction));
	ActionInterval* seq = Sequence::create(delay,func,delay1,func1,nullptr);
	runAction(seq);
}

void BoneWarriorAI::Action2()
{
	if (m_times < 4)
	{
		auto delay = DelayTime::create(3.0f);
		auto shoot = CallFunc::create(this,callfunc_selector(BoneWarriorAI::ShootReboundRandom));
		auto next = CallFunc::create(this,callfunc_selector(BoneWarriorAI::Action2));
		ActionInterval* seq2 = Sequence::create(shoot, delay, next, nullptr);
		runAction(seq2);

		m_times++;
	}
	else
	{
		//下一步
		m_times = 0;
		NextAction();
	}
	
}

void BoneWarriorAI::Action3()
{
	//一边移动一边发射
	auto action = randMove();
	m_target->runAction(action);

	ActionInterval* delay1 = DelayTime::create(1.5f + rand_minus1_1() * 0.5f);
	ActionInterval* delay2 = DelayTime::create(5.0f);
	CallFunc* shoot = CallFunc::create(this,callfunc_selector(BoneWarriorAI::Shoot6Rebound));
	CallFunc* next = CallFunc::create(this,callfunc_selector(BoneWarriorAI::NextAction));
	ActionInterval* seq = Sequence::create(delay1, shoot, delay2, next, nullptr);
	runAction(seq);
}

void BoneWarriorAI::Action4()
{
	//从两边发射流弹
	if (m_times < 10)
	{
		int yushu = m_times%2;
		if (yushu == 0)
		{
			//左边
			auto func = CallFunc::create(this,callfunc_selector(BoneWarriorAI::ShootLiudanL));
			auto delay = DelayTime::create(0.5);
			auto next = CallFunc::create(this,callfunc_selector(BoneWarriorAI::Action4));
			auto seq = Sequence::create(func, delay, next, nullptr);
			runAction(seq);
		}
		else
		{
			//右边
			auto func = CallFunc::create(this,callfunc_selector(BoneWarriorAI::ShootLiudanR));
			auto delay = DelayTime::create(0.5);
			auto next = CallFunc::create(this,callfunc_selector(BoneWarriorAI::Action4));
			auto seq = Sequence::create(func, delay, next, nullptr);
			runAction(seq);
		}
	}
	else
	{
		//下一步
		m_times = 0;
		NextAction();
	}
	
}

void BoneWarriorAI::Action5()
{
	if (m_times < 5)
	{
		auto func = CallFunc::create(this,callfunc_selector(BoneWarriorAI::ShootReboundToMainPlane));
		auto delay = DelayTime::create(3.0);
		auto next = CallFunc::create(this,callfunc_selector(BoneWarriorAI::Action5));
		auto seq = Sequence::create(func, delay, next, nullptr);
		runAction(seq);
	}
	else
	{
		//下一步
		m_times = 0;
		NextAction();
	}
}

void BoneWarriorAI::ActionRest()
{
	auto action = randMove();
	m_target->runAction(action);

	ActionInterval* delay = DelayTime::create(1.0f + rand_0_1() * 2);
	CallFunc* next = CallFunc::create(this,callfunc_selector(BoneWarriorAI::NextAction));
	ActionInterval* seq = Sequence::create(delay, next, nullptr);
	runAction(seq);
}

void BoneWarriorAI::NextAction()
{
	int tag = 0;
	if (m_it == m_ActionVec.end())
	{
		//随机
		switch (m_AIlvl)
		{
		case 1:
			tag = 1;
			break;
		case 2:
			{
				float randi = rand_0_1() * 1.25;
				if (randi < 0.5)
				{
					tag = 2;
				}
				else if(randi < 1.0)
				{
					tag = 1;
				}
			}
			break;
		case 3:
			{
				float randi = rand_0_1() * 1.25;
				if (randi < 0.5)
				{
					tag = 3;
				}
				else if(randi < 1.0)
				{
					tag = 4;
				}
			}
			break;

		default:
			break;
		}
	}
	else
	{
		tag = (*m_it);
		m_it++;
	}
	switch (tag)
	{
	case 0:
		ActionRest();	//休息动作
		break;
	case 1:
		Action1();
		break;
	case 2:
		Action2();
		break;
	case 3:
		Action3();
		break;
	case 4:
		Action5();
		break;
	/*case 5:
		Action5();*/
		break;
	default:
		break;
	}
}

void BoneWarriorAI::initAction()
{
	m_ActionVec.push_back(1);

	m_it = m_ActionVec.begin();
}

void BoneWarriorAI::FixPos()
{
	m_target->stopAllActions();
	auto mv = MoveToMidRight();
	CallFunc* func = CallFunc::create(this,callfunc_selector(BoneWarriorAI::NextAction));
	ActionInterval* seq = Sequence::create(mv, func, nullptr);
	m_target->runAction(seq);
}

void BoneWarriorAI::addLiudan()
{
	CallFunc* func = CallFunc::create(this,callfunc_selector(BoneWarriorAI::addLiudanCB));
	ActionInterval* delay = DelayTime::create(1.0 + rand_minus1_1() * 0.3);
	CallFunc* next = CallFunc::create(this,callfunc_selector(BoneWarriorAI::addLiudan));
	ActionInterval* seq = Sequence::create(func,delay,next,nullptr);
	runAction(seq);
}

void BoneWarriorAI::addLiudanCB()
{
	Bullet* bullet = GameData::getInstance()->getBulletFactory()->createBullet(nullptr,BulletType::Enemy_Grenade);
	UIController::getInstance()->getPhysicsLayer()->addChild(bullet);
}

void BoneWarriorAI::ShootReboundDown()
{
	for (int i = 0; i < 2; i++)
	{
		Weapon* wp = Weapon::create(m_target);
		wp->setBullstType(BulletType::Enemy_BossRebound);
		wp->setBltBelongTo(UnitId::ePlaneBoss);
		wp->setBltScaleX(1.5);
		wp->setBltGravityAble(true);
		wp->setBltScaleY(1.5);
		wp->setBltOtherData((void*)2);
		wp->setShootTimes(1);
		wp->setBltHurts(getBossHurts());
		wp->setRotation(-45 + i * 90);
		m_target->AddOneWp(wp, WeaponType::Addition);
	}

}

void BoneWarriorAI::ShootReboundRandom()
{
	Weapon* wp = Weapon::create(m_target);
	wp->setBullstType(BulletType::Enemy_BossRebound);
	wp->setBltBelongTo(UnitId::ePlaneBoss);
	wp->setBltGravityAble(true);
	wp->setBltScaleX(1.5);
	wp->setBltScaleY(1.5);
	wp->setShootTimes(1);
	wp->setBltHurts(getBossHurts());
	wp->setRotation(360 * rand_0_1());
	m_target->AddOneWp(wp, WeaponType::Addition);
}

void BoneWarriorAI::Shoot6Rebound()
{
	for (int i = 0; i < 3; i++)
	{
		Weapon* wp = Weapon::create(m_target);
		wp->setBullstType(BulletType::Enemy_BossRebound);
		wp->setBltBelongTo(UnitId::ePlaneBoss);
		wp->setBltGravityAble(true);
		wp->setBltScaleX(1.5);
		wp->setBltScaleY(1.5);
		wp->setBltOtherData((void*)3);
		wp->setShootTimes(1);
		wp->setBltHurts(getBossHurts());
		wp->setRotation(-45 * (i+1));
		m_target->AddOneWp(wp, WeaponType::Addition);
	}
	for (int i = 0; i < 3; i++)
	{
		Weapon* wp = Weapon::create(m_target);
		wp->setBullstType(BulletType::Enemy_BossRebound);
		wp->setBltOtherData((void*)3);
		wp->setBltScaleX(1.5);
		wp->setBltScaleY(1.5);
		wp->setBltGravityAble(true);
		wp->setShootTimes(1);
		wp->setBltHurts(getBossHurts());
		wp->setRotation(45 * (i+1));
		m_target->AddOneWp(wp, WeaponType::Addition);
	}
}

void BoneWarriorAI::ShootReboundToMainPlane()
{
	Vec2 endPos = Vec2(m_winSize.width * 0.5, 0);
	auto mainPl = GameData::getInstance()->getMainPlane();
	if (mainPl)
	{
		endPos = mainPl->convertToWorldSpaceAR(Vec2::ZERO);
	}

	Weapon* wp = Weapon::create(m_target);
	wp->setBullstType(BulletType::Enemy_BossRebound);
	wp->setBltBelongTo(UnitId::ePlaneBoss);
	wp->setBltScaleX(1.5);
	wp->setBltScaleY(1.5);
	wp->setBltGravityAble(true);
	wp->setShootTimes(1);
	wp->setBltHurts(getBossHurts());
	m_target->AddOneWp(wp, WeaponType::Addition);

	auto myPos = wp->convertToWorldSpaceAR(Vec2::ZERO);
	auto newVec = endPos - myPos;
	float angle = newVec.getAngle() * RAD_DEG;
	wp->setRotation(-angle + 90);
}

void BoneWarriorAI::ShootLiudanL()
{
	auto pos = m_target->convertToWorldSpaceAR(Vec2::ZERO);

	Bullet* bullet = GameData::getInstance()->getBulletFactory()->createBullet(nullptr,BulletType::Enemy_Grenade);
	bullet->setPosition(Vec2(pos.x,pos.y));
	bullet->setBelongTo(UnitId::ePlaneBoss);
	UIController::getInstance()->getPhysicsLayer()->addChild(bullet);
}

void BoneWarriorAI::ShootLiudanR()
{
	auto pos = m_target->convertToWorldSpaceAR(Vec2::ZERO);

	Bullet* bullet = GameData::getInstance()->getBulletFactory()->createBullet(nullptr,BulletType::Enemy_Grenade);
	bullet->setPosition(Vec2(pos.x + 200,pos.y));
	bullet->setBelongTo(UnitId::ePlaneBoss);
	UIController::getInstance()->getPhysicsLayer()->addChild(bullet);
}

//
//void BoneWarriorAI::DeathAction()
//{
//	m_target->stopAllActions();
//	DeadArmature();
//}
//
//void BoneWarriorAI::DeadArmature()
//{
//	BaseBoss* boss = dynamic_cast<BaseBoss*>(m_target);
//	auto armt = boss->getArmature();
//	armt->getAnimation()->pause();
//	stopAllActions();
//	boss->setHpBarVisible(false);
//
//	//移出屏幕 + 变小
//	auto s = Director::getInstance()->getWinSize();
//	auto myPos = this->convertToWorldSpaceAR(Vec2::ZERO);
//	_ccBezierConfig bezc;
//
//	if (myPos.x > 360)
//	{
//		bezc.controlPoint_1 = Vec2(-100, 50);
//		bezc.controlPoint_2 = Vec2(- 400, -200 );
//		bezc.endPosition = Vec2(- 800, -300 );
//	}
//	else
//	{
//		bezc.controlPoint_1 = Vec2(100, 50);
//		bezc.controlPoint_2 = Vec2(400, -200 );
//		bezc.endPosition = Vec2(800, -300 );
//	}
//
//	float stay = 2.5f;
//	ActionInterval* delay = DelayTime::create(stay);
//	ActionInterval* bezier = BezierBy::create(2.0f,bezc);
//	ActionInterval* scale = ScaleTo::create(2.0f,0.2f);
//	CallFunc* realDead = CallFunc::create(m_target,callfunc_selector(BaseBoss::RealDead));//艹，这里不要大意，一定要传BOSS对象，而非this
//	ActionInterval* rotate = RotateBy::create(2.0,180);
//	ActionInterval* seq = Sequence::create(DelayTime::create(stay),bezier,realDead,nullptr);
//	ActionInterval* seq1 = Sequence::create(delay,scale,nullptr);
//	ActionInterval* seq2 = Sequence::create(DelayTime::create(stay),rotate,nullptr);
//	ActionInterval* fadeOut = FadeOut::create(7.0f);
//	ActionInterval* tint = TintTo::create(2.0f,150,30,30);
//
//	m_target->runAction(seq);
//	m_target->runAction(seq2);
//	armt->runAction(seq1);
//	//armt->runAction(fadeOut);
//	armt->runAction(tint);
//}


//-------------------------------------
void RenzheBossAI::StartAI( int lvl )
{
	m_AIlvl = lvl;
	m_direct = cocos2d::random(0,1) - 0.5;

	m_Dart = CreateDart(true);
	m_Dart->setPosition(getDartLocalPos());
	m_target->addChild(m_Dart);
	m_Dart->setVisible(false);

	changeDirect();
	initAction();
	MoveToMidRight();
	NewAction();

	//addCloud();
}

void RenzheBossAI::initAction()
{
	switch (m_AIlvl)
	{
	case 1:
		{
			m_sandanCountMax = 2;
			m_sandanSpeed = 400;
			m_sandanNum= 4;

			m_ActionVec.push_back(5);
			m_ActionVec.push_back(0);
			m_ActionVec.push_back(3);
			m_ActionVec.push_back(0);
			m_ActionVec.push_back(5);
			m_ActionVec.push_back(3);
			m_ActionVec.push_back(0);
			m_ActionVec.push_back(5);
			m_ActionVec.push_back(0);
			m_ActionVec.push_back(5);
			m_ActionVec.push_back(0);
			m_ActionVec.push_back(3);
			m_ActionVec.push_back(5);
		}
		break;
	case 2:
		{
			m_sandanCountMax = 6;
			m_sandanSpeed = 600;
			m_sandanNum= 6;

			m_ActionVec.push_back(2);
			m_ActionVec.push_back(5);
			m_ActionVec.push_back(0);
			m_ActionVec.push_back(1);
			m_ActionVec.push_back(2);
			m_ActionVec.push_back(0);
			m_ActionVec.push_back(5);
			m_ActionVec.push_back(1);
			m_ActionVec.push_back(0);
			m_ActionVec.push_back(5);
			m_ActionVec.push_back(0);
			m_ActionVec.push_back(2);
			m_ActionVec.push_back(1);
			m_ActionVec.push_back(0);
			m_ActionVec.push_back(5);
			m_ActionVec.push_back(2);
			m_ActionVec.push_back(0);
			m_ActionVec.push_back(1);
			m_ActionVec.push_back(5);
		}
		break;
	case 3:
		{
			m_sandanCountMax = 6;
			m_sandanSpeed = 1000;
			m_sandanNum= 15;

			m_ActionVec.push_back(4);
			m_ActionVec.push_back(5);
			m_ActionVec.push_back(0);
			m_ActionVec.push_back(1);
			m_ActionVec.push_back(2);
			m_ActionVec.push_back(0);
			m_ActionVec.push_back(5);
			m_ActionVec.push_back(4);
			m_ActionVec.push_back(0);
			m_ActionVec.push_back(1);
			m_ActionVec.push_back(0);
			m_ActionVec.push_back(2);
			m_ActionVec.push_back(4);
		}
		break;

	default:
		break;
	}

	m_it = m_ActionVec.begin();
}

void RenzheBossAI::NewAction()
{
	playArmat(0);
	int tag = 0;
	if (m_it == m_ActionVec.end())
	{
		//随机
		switch (m_AIlvl)
		{
		case 1:
			{
				float randi = rand_0_1() * 1.25;
				if (randi < 0.5)
				{
					tag = 3;
				}
				else if(randi < 1.0)
				{
					tag = 5;
				}
			}
			break;
		case 2:
			{
				float randi = rand_0_1() * 1.25;
				if (randi < 0.33)
				{
					tag = 1;
				}
				else if(randi < 0.66)
				{
					tag = 5;
				}
				else if(randi < 1.0)
				{
					tag = 2;
				}
			}
			break;
		case 3:
			{
				float randi = rand_0_1() * 1.25;
				if (randi < 0.25)
				{
					tag = 4;
				}
				else if(randi < 0.5)
				{
					tag = 5;
				}
				else if(randi < 0.75)
				{
					tag = 1;
				}
				else if(randi < 1.0)
				{
					tag = 2;
				}
			}
			break;

		default:
			break;
		}
	}
	else
	{
		tag = (*m_it);
		m_it++;
	}
	switch (tag)
	{
	case 0:
		ActionRest();
		break;
	case 1:
		ActionShootDart1();
		break;
	case 2:
		ActionShootDart2();
		break;
	case 3:
		ActionShootDart3();
		break;
	case 4:
		ActionShootDart4();
		break;
	case 5:
		ActionShootExecration();
		break;
	default:
		break;
	}
}

cocos2d::Vec2 RenzheBossAI::getDartLocalPos()
{
	return Vec2(-190,145);
}

cocos2d::Vec2 RenzheBossAI::getDartWorldPos()
{
	auto bossPos = m_target->convertToWorldSpaceAR(Vec2::ZERO);
	return bossPos + getDartLocalPos();
}

PhysicsUnit* RenzheBossAI::CreateDart(bool isWudi)
{
	int hurt = m_target->getContactPro().hurts;

	auto bullet = DartOfBoss3::create();
	bullet->setBelongTo(UnitId::ePlaneBoss);
	bullet->setInvincibleAtLine(isWudi);

	auto pro = bullet->getContactPro();
	pro.hurts = hurt;
	bullet->setContactPro(pro);

	auto rot = RotateBy::create(0.6f,-720);
	auto rep = RepeatForever::create(rot);
	bullet->runAction(rep);

	return bullet;
}

PointArray* RenzheBossAI::getPointArray1()
{
	auto dartPos = getDartWorldPos();
	auto endPos = Vec2(m_winSize.width*0.5, 150);

	//计算出相对位移
	auto relativePos = endPos - dartPos;
	float dis = dartPos.distance(endPos);

	auto arr = PointArray::create(10);
	arr->addControlPoint(Vec2(0, 0));
	arr->addControlPoint(Vec2(relativePos.x - dis * 0.27,		relativePos.y * 0.45));
	arr->addControlPoint(Vec2(relativePos.x + dis * 0.28,		relativePos.y));
	arr->addControlPoint(Vec2(relativePos.x - dis * 0.27,		relativePos.y));
	arr->addControlPoint(Vec2(relativePos.x + dis * 0.28,		relativePos.y * 0.45));
	arr->addControlPoint(Vec2(0, 0));

	return arr;
}

PointArray* RenzheBossAI::getPointArray2()
{
	auto arr = PointArray::create(10);
	return arr;
}

void RenzheBossAI::DartAction1()
{
	m_Dart->setVisible(true);

	auto mainPl = GameData::getInstance()->getMainPlane();
	Vec2 mainplPos = Vec2(m_winSize.width*0.5, 100);
	if (mainPl)
	{
		mainplPos = mainPl->convertToWorldSpaceAR(Vec2::ZERO);
		//mainplPos.y += 150;
	}

	auto dartPos = getDartWorldPos();

	//计算出相对位移
	auto relativePos = mainplPos - dartPos;
	float dis = dartPos.distance(mainplPos);

	//auto midPos = dartPos.getMidpoint(mainplPos);
	auto midPos = Vec2((mainplPos.x + dartPos.x)*0.75, (mainplPos.y + dartPos.y)*0.75);
	auto midPosVec = mainplPos - midPos;
	auto faxianL = midPosVec;		//顺时针法线
	auto faxianR = midPosVec;		//逆时针法线
	faxianL.rotate(Vec2(0,0),-90 * DEG_RAD);
	faxianR.rotate(Vec2(0,0),90 * DEG_RAD);

	faxianL.normalize();
	faxianR.normalize();

	faxianL *= (dis * 0.5);
	faxianR *= (dis * 0.5);

	ccBezierConfig bezier;

	{
		bezier.controlPoint_1 = Vec2(0, 0);
		//bezier.controlPoint_2 = Vec2(relativePos.x * 0.75 - dis * 0.7,		relativePos.y * 0.75);
		//bezier.controlPoint_2 = faxianL;
		bezier.controlPoint_2 = Vec2(relativePos.x - 450,							relativePos.y*0.75);
		bezier.endPosition = Vec2(relativePos.x,							relativePos.y);
	}
	auto flyout = BezierBy::create(0.6, bezier);
	auto easeout = CCEaseSineIn::create(flyout);

	{
		bezier.controlPoint_1 = Vec2(0, 0);
		//bezier.controlPoint_2 = Vec2(-(relativePos.x * 0.25 - dis * 0.7),	-relativePos.y * 0.25);
		bezier.controlPoint_2 = faxianR;
		bezier.endPosition =	Vec2(-relativePos.x,						-relativePos.y);
	}
	auto flyback = BezierBy::create(0.8, bezier);
	auto easeback = CCEaseSineIn::create(flyback);

	auto delay = DelayTime::create(0.5f);
	auto seq = Sequence::create(flyout, delay, easeback, nullptr);
	m_Dart->runAction(seq);

	m_action1Time = 0.5 + 0.5 + 1.0f;
}

void RenzheBossAI::DartAction2()
{
	m_Dart->setVisible(true);

	auto arr = getPointArray1();
	auto mv = CardinalSplineBy::create(2.5f, arr, 0.5);
	m_Dart->runAction(mv);

	m_action2Time = 2.0f;
}

void RenzheBossAI::DartAction3()
{
	m_Dart->setVisible(true);

	auto mainPl = GameData::getInstance()->getMainPlane();
	Vec2 mainplPos = Vec2(m_winSize.width*0.5, 100);
	if (mainPl)
	{
		mainplPos = mainPl->convertToWorldSpaceAR(Vec2::ZERO);
	}

	auto dartPos = getDartWorldPos();

	//计算出相对位移
	auto relativePos = mainplPos - dartPos;

	auto mv = MoveBy::create(0.6f, relativePos);
	auto ease = EaseSineIn::create(mv);
	auto back = ease->reverse();
	auto seq = Sequence::create(ease, back, nullptr);
	m_Dart->runAction(seq);

	m_action3Time = 0.6*2 + 1.0f;
}

void RenzheBossAI::DartAction4()
{
	auto dartL = CreateDart();
	auto Lpos = Vec2(150, m_winSize.height*0.5);
	dartL->setPosition(Lpos);
	dartL->setOpacity(0);
	dartL->runAction(FadeIn::create(1.0f));
	UIController::getInstance()->getPhysicsLayer()->addChild(dartL);

	auto dartR = CreateDart();
	auto Rpos = Vec2(m_winSize.width - 150, m_winSize.height*0.5);
	dartR->setPosition( Rpos );
	dartR->setOpacity(0);
	dartR->runAction(FadeIn::create(1.0f));
	UIController::getInstance()->getPhysicsLayer()->addChild(dartR);

	ccBezierConfig bezier;

	{
		bezier.controlPoint_1 = Lpos;
		bezier.controlPoint_2 = Vec2(m_winSize.width * 0.5, 150);
		bezier.endPosition = Vec2(m_winSize.width * 1.2, -150);

		auto fly = BezierTo::create(1.0, bezier);
		auto delay = DelayTime::create(2.0f);
		auto seq = Sequence::create(delay, fly, nullptr); 
		dartL->runAction(seq);
	}
	{
		bezier.controlPoint_1 = Rpos;
		bezier.controlPoint_2 = Vec2(m_winSize.width * 0.5, 150);
		bezier.endPosition = Vec2(-m_winSize.width * 0.2, -150);

		auto fly = BezierTo::create(1.0, bezier);
		auto delay = DelayTime::create(2.0f);
		auto seq = Sequence::create(delay, fly, nullptr); 
		dartR->runAction(seq);
	}

	m_action4Time = 1.0+ 2.0;
}

void RenzheBossAI::ShootSandan()
{
	if (m_AIlvl == 3)
	{
		float dur = 3.0f;
		for (int i = 0; i < 2; i++)
		{
			Weapon* wp = Weapon::create(m_target);
			wp->setBullstType(BulletType::Enemy_default);
			wp->setBltResTag(ResTag::Bullet_BossSmallDart);
			wp->setBltBelongTo(UnitId::ePlaneBoss);
			wp->setBltScaleX(0.8);
			wp->setBltScaleY(0.8);
			wp->setBltSpeed(300);
			wp->setShootTimes(40);
			wp->setShootSpeed(0.08f);
			wp->setBltHurts(getBossHurts());
			wp->setPosition(getExecrationPos());
			wp->setBltLinearDamping(0.08 + i * 0.08);
			wp->setRotation(180 * i);
			m_target->AddOneWp(wp, WeaponType::Addition);

			ActionInterval* rot = RotateBy::create(1.0f,360);
			ActionInterval* rep = RepeatForever::create(rot);
			wp->runAction(rep);

			ActionInterval* delay = DelayTime::create(dur);
			CallFunc* func = CallFunc::create(this,callfunc_selector(RenzheBossAI::resumeArmat));
			ActionInterval* seq = Sequence::create(delay, func, nullptr);
			runAction(seq);
		}

		return;
	}

	if (m_sandanCount >= m_sandanCountMax)
	{
		m_sandanCount = 0;
		resumeArmat();
		return;
	}
	m_sandanCount++;

	if (m_shootDirect == -1)
	{
		m_shootDirect = 1;
	}
	else
	{
		m_shootDirect = -1;
	}
	int randRot = random(-10,10);

	
	for (int i = 0; i < m_sandanNum; i++)
	{
		Weapon* wp = Weapon::create(m_target);
		wp->setBullstType(BulletType::Enemy_default);
		wp->setBltResTag(ResTag::Bullet_BossSmallDart);
		wp->setBltBelongTo(UnitId::ePlaneBoss);
		wp->setBltScaleX(0.8);
		wp->setBltScaleY(0.8);
		wp->setBltSpeed(m_sandanSpeed);
		wp->setDelayShoot(i * 0.08);
		wp->setShootTimes(1);
		wp->setBltHurts(getBossHurts());
		m_target->AddOneWp(wp, WeaponType::Addition);

		if (m_AIlvl == 3)
		{
			wp->setRotation(180 + m_shootDirect * (i-1) * 12 + randRot);
			wp->setBltLinearDamping(0.8);
		}
		else
		{
			wp->setRotation(180 + m_shootDirect * (i-1) * 9 + randRot);
		}
	}

	ActionInterval* delay = DelayTime::create(0.5);
	CallFunc* func = CallFunc::create(this,callfunc_selector(RenzheBossAI::ShootSandan));
	ActionInterval* seq = Sequence::create(delay, func, nullptr);
	runAction(seq);
}

void RenzheBossAI::addCloud()
{
	{
		auto sp = Sprite::create("Effect/cloud/cloud.png");
		sp->setPosition(m_winSize.width * 0.5, m_winSize.height * 0.5);
		sp->setScale(5);
		UIController::getInstance()->getEffectsLayer()->addChild(sp,10000);

		sp->setOpacity(0);
		auto fadein = FadeIn::create(1.0f);
		auto delay = DelayTime::create(5.0f);
		auto fadeout = FadeOut::create(3.0f);
		auto rm = RemoveSelf::create();
		auto seq = Sequence::create(fadein, delay, fadeout, rm, nullptr);
		sp->runAction(seq);
	}

	{
		auto delay = DelayTime::create(20.0f);
		auto func = CallFunc::create(this,callfunc_selector(RenzheBossAI::addCloud));
		auto seq = Sequence::create(delay, func, nullptr);
		runAction(seq);
	}
}

void RenzheBossAI::onFrameEventOfShoot1( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "shoot")
	{
		DartAction1();
	}
	else if (evt == "end")
	{
		PlayArmatKeep(1.1);	//保持姿势
	}
}

void RenzheBossAI::onFrameEventOfShoot2( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "shoot")
	{
		DartAction2();
	}
	else if (evt == "end")
	{
		PlayArmatKeep(0.8);	//保持姿势
	}
}

void RenzheBossAI::onFrameEventOfShoot3( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "shoot")
	{
		DartAction3();
	}
	else if (evt == "end")
	{
		PlayArmatKeep(2.5);	//保持姿势
	}
}

void RenzheBossAI::onFrameEventOfCall2Darts( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "shoot")
	{
		DartAction4();
	}
	else if (evt == "end")
	{
		playArmat(1,this, frameEvent_selector(RenzheBossAI::onFrameEventOfShoot3));
	}
}

void RenzheBossAI::onFrameEventOfExecration( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "shoot")
	{
		ExecrationParticle();
		pauseArmat();
	}
	else if (evt == "end")
	{
		//log("nianznou over");
		removeExecrationParticle();
		NewAction();
	}
}

void RenzheBossAI::onFrameEventOfHuishou( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "end")
	{
		//log("huishou over");
		if (m_Dart)
		{
			m_Dart->setVisible(false);
		}
		NewAction();
	}
}

void RenzheBossAI::onFrameEventOfKeep( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "end")
	{
		//log("keep over");
		PlayArmatHuishou();
	}
}

void RenzheBossAI::onFrameEventOfRest( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	//不做任何事，只为替换帧监听事件
}

void RenzheBossAI::onFrameEventOfReady( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "pause")
	{
		//暂停
		pauseArmat();
	}
}

void RenzheBossAI::PlayArmatRest()
{
	playArmat(0,this, frameEvent_selector(RenzheBossAI::onFrameEventOfRest));
}

void RenzheBossAI::PlayArmatKeep(float speed)
{
	playArmat(3,this, frameEvent_selector(RenzheBossAI::onFrameEventOfKeep),speed);
}

void RenzheBossAI::PlayArmatShoot1()
{
	setArmatFunc(this, frameEvent_selector(RenzheBossAI::onFrameEventOfShoot1));
	resumeArmat();
}

void RenzheBossAI::PlayArmatShoot2()
{
	setArmatFunc(this, frameEvent_selector(RenzheBossAI::onFrameEventOfShoot2));
	resumeArmat();
}

void RenzheBossAI::PlayArmatShoot3()
{
	setArmatFunc(this, frameEvent_selector(RenzheBossAI::onFrameEventOfShoot3));
	resumeArmat();
}

void RenzheBossAI::PlayArmatCall2Darts()
{
	playArmat(2,this, frameEvent_selector(RenzheBossAI::onFrameEventOfCall2Darts),2.0);
}

void RenzheBossAI::PlayArmatExecration()
{
	//在发射散弹的过程中要保证只播一次，那么就要减缓播放速率
	playArmat(2,this, frameEvent_selector(RenzheBossAI::onFrameEventOfExecration),3.0f);
}

void RenzheBossAI::PlayArmatHuishou()
{
	playArmat(4,this, frameEvent_selector(RenzheBossAI::onFrameEventOfHuishou));
}

void RenzheBossAI::ActionRest()
{
	PlayArmatRest();

	auto action = randMove();
	auto next = CallFunc::create(this,callfunc_selector(RenzheBossAI::NewAction));
	auto seq = Sequence::create(action, next, nullptr);
	m_target->runAction(seq);

}

void RenzheBossAI::ActionShootDart1()
{
	auto action = MoveToMidRight();
	auto func = CallFunc::create(this,callfunc_selector(RenzheBossAI::Ready1));
	auto seq  = Sequence::create(action, func, nullptr);
	m_target->runAction(seq);
}

void RenzheBossAI::ActionShootDart2()
{
	auto action = MoveToMidRight();
	auto func = CallFunc::create(this,callfunc_selector(RenzheBossAI::Ready2));
	auto seq  = Sequence::create(action, func, nullptr);
	m_target->runAction(seq);
}

void RenzheBossAI::ActionShootDart3()
{
	auto action = MoveToMidRight();
	auto func = CallFunc::create(this,callfunc_selector(RenzheBossAI::Ready3));
	auto seq  = Sequence::create(action, func, nullptr);
	m_target->runAction(seq);
}

void RenzheBossAI::ActionShootDart4()
{
	auto action = MoveToMidRight();
	auto func = CallFunc::create(this,callfunc_selector(RenzheBossAI::PlayArmatCall2Darts));
	auto seq  = Sequence::create(action, func, nullptr);
	m_target->runAction(seq);
}

void RenzheBossAI::ActionShootExecration()
{
	auto action = MoveToMid();
	auto func = CallFunc::create(this,callfunc_selector(RenzheBossAI::PlayArmatExecration));
	auto seq  = Sequence::create(action, func, nullptr);
	m_target->runAction(seq);
}

void RenzheBossAI::pauseArmat()
{
	BaseBoss* boss = (BaseBoss*)m_target;
	auto armat = boss->getArmature();
	armat->getAnimation()->pause();	
}

void RenzheBossAI::resumeArmat()
{
	m_Dart->runAction(MoveBy::create(0.1f,-getReadyOffset()));	//从准备位置运动到发射位置
	m_Dart->setZOrder(1);

	BaseBoss* boss = (BaseBoss*)m_target;
	auto armat = boss->getArmature();
	armat->getAnimation()->resume();
}

void RenzheBossAI::Ready( Ref* obj, SEL_CallFunc func )
{
	playArmat(1,this, frameEvent_selector(RenzheBossAI::onFrameEventOfReady));

	//修正飞镖位置，让其渐现
	m_Dart->setPosition(getDartLocalPos() + getReadyOffset());
	m_Dart->setZOrder(-1);
	m_Dart->setOpacity(0);
	m_Dart->setVisible(true);
	ActionInterval* fadein = FadeIn::create(1.0f);
	m_Dart->runAction(fadein);

	ActionInterval* delay = DelayTime::create(2.0f);
	CallFunc* begin = CallFunc::create(obj,func);
	ActionInterval* seq = Sequence::create(delay, begin, nullptr);
	runAction(seq);
}

void RenzheBossAI::Ready1()
{
	Ready(this,callfunc_selector(RenzheBossAI::PlayArmatShoot1));
}

void RenzheBossAI::Ready2()
{
	Ready(this,callfunc_selector(RenzheBossAI::PlayArmatShoot2));
}

void RenzheBossAI::Ready3()
{
	Ready(this,callfunc_selector(RenzheBossAI::PlayArmatShoot3));
}

void RenzheBossAI::Ready4()
{

}

void RenzheBossAI::setArmatFunc(cocos2d::Ref *target, SEL_FrameEventCallFunc callFunc, float speed)
{
	BaseBoss* boss = (BaseBoss*)m_target;
	auto armat = boss->getArmature();
	armat->getAnimation()->setFrameEventCallFunc(target,callFunc);  
	armat->getAnimation()->setSpeedScale(speed);
}

cocos2d::Vec2 RenzheBossAI::getReadyOffset()
{
	return Vec2(32,35);
}

ActionInterval* RenzheBossAI::MoveToMidRight()
{
	Vec2 pos = Vec2(m_winSize.width * 0.75, m_winSize.height * 0.75);
	auto dis = getDis(pos);

	ActionInterval* mvto = MoveTo::create(0.5f + dis/300 * 0.5, pos );
	return mvto;
}

ActionInterval* RenzheBossAI::MoveToMid()
{
	Vec2 pos = Vec2(m_winSize.width * 0.5, m_winSize.height * 0.75);
	auto dis = getDis(pos);

	ActionInterval* mvto = MoveTo::create(0.5f + dis/300 * 0.5, pos );
	return mvto;
}

void RenzheBossAI::ExecrationParticle()
{
	auto node = m_target->getChildByTag(15611940);
	if (!node)
	{
		node = Node::create();
		node->setPosition(getExecrationPos());
		m_target->addChild(node,0,15611940);
	}
	
	auto partc1 = EffectsManager::addParticle("particla/BossDeadExpl/2/test1.plist",node,Vec2(0,0),false);
	auto partc2 = EffectsManager::addParticle("particla/BossDeadExpl/2/test2.plist",node,Vec2(0,0),false);
	auto partc3 = EffectsManager::addParticle("particla/BossDeadExpl/2/test3.plist",node,Vec2(0,0),false);
	if (partc1)
	{
		partc1->setScale(2.5);
	}
	if (partc2)
	{
		partc2->setScale(2.5);
	}
	if (partc3)
	{
		partc3->setScale(2.5);
	}

	auto delay = DelayTime::create(0.7);
	auto func = CallFunc::create(this,callfunc_selector(RenzheBossAI::ShootSandan));
	auto seq = Sequence::create(delay, func, nullptr);
	runAction(seq);
}

void RenzheBossAI::removeExecrationParticle()
{
	auto node = m_target->getChildByTag(15611940);
	if (node)
	{
		node->removeFromParentAndCleanup(true);
	}
}

void RenzheBossAI::DeathAction()
{
	BossAI::DeathAction();

	//飞镖要删除
	m_Dart->setDirectDeath();
}

cocos2d::Vec2 RenzheBossAI::getExecrationPos()
{
	return Vec2(-55,75);
}

void PoseidonBossAI::StartAI( int lvl )
{
	m_AIlvl = lvl;
	m_bezierTimes = 0;
	m_direct = cocos2d::random(0,1) - 0.5;

	changeDirect();
	initAction();
	MoveToMidRight();
	NewAction();

	//PlayAnimation1();
	//Action2();
	//Action3();
	//Action4();
	//Action5();
	//Action6();
	
}

void PoseidonBossAI::initAction()
{

}

void PoseidonBossAI::NewAction()
{
	PlayAnimationRest();
	int tag = 0;

	//随机
	switch (m_AIlvl)
	{
	case 1:
		{
			float randi = rand_0_1() * 1.25;
			if (randi < 0.5)
			{
				tag = 1;
			}
			else if(randi < 1.0)
			{
				tag = 2;
			}
		}
		break;
	case 2:
		{
			float randi = rand_0_1() * 1.25;
			if (randi < 0.33)
			{
				tag = 1;
			}
			else if(randi < 0.66)
			{
				tag = 3;
			}
			else if(randi < 1.0)
			{
				tag = 4;
			}
		}
		break;
	case 3:
		{
			float randi = rand_0_1() * 1.4;
			if (randi < 0.2)
			{
				tag = 4;
			}
			else if(randi < 0.4)
			{
				tag = 5;
			}
			else if(randi < 0.6)
			{
				tag = 1;
			}
			else if(randi < 0.8)
			{
				tag = 6;
			}
			else if(randi < 1.0)
			{
				tag = 3;
			}
			else if(randi < 1.2)
			{
				tag = 2;
			}
		}
		break;

	default:
		break;
	}
	switch (tag)
	{
	case 0:
		Action0();
		break;
	case 1:
		Action1();
		break;
	case 2:
		Action2();
		break;
	case 3:
		Action3();
		break;
	case 4:
		Action4();
		break;
	case 5:
		Action5();
		break;
	case 6:
		Action6();
		break;

	default:
		break;
	}
}

void PoseidonBossAI::ShootOneLineBullet()
{
	for (int i = 0; i < 6; i++)
	{
		Bullet1OfBoss4* bullet = Bullet1OfBoss4::create(ResTag::Bullet_BossShuiPaoSmall, i * 1.0 + 2.0);
		bullet->setPosition(Vec2(100 + 105 * i, 750));
		bullet->setBelongTo(UnitId::ePlaneBoss);
		bullet->resetHurt(getBossHurts());
		UIController::getInstance()->getPhysicsLayer()->addChild(bullet);
	}
}

void PoseidonBossAI::ShootTwoBezierBulletL()
{
	Weapon* wp = Weapon::create(m_target);
	wp->setBullstType(BulletType::Enemy_Boss4Bezier);
	wp->setBltBelongTo(UnitId::ePlaneBoss);
	wp->setPosition(-90, 0);
	wp->setRotation(-45);
	wp->setShootTimes(10);
	wp->setShootSpeed(0.2f);
	wp->setBltHurts(getBossHurts());
	m_target->AddOneWp(wp, WeaponType::Normal);
	wp->setOffset(getLeftHandPos());
}

void PoseidonBossAI::ShootTwoBezierBulletR()
{
	Weapon* wp = Weapon::create(m_target);
	wp->setBullstType(BulletType::Enemy_Boss4Bezier);
	wp->setBltBelongTo(UnitId::ePlaneBoss);
	wp->setRotation(45);
	wp->setShootTimes(10);
	wp->setShootSpeed(0.2f);
	wp->setBltHurts(getBossHurts());
	m_target->AddOneWp(wp,WeaponType::Normal);
	wp->setOffset(getRightHandPos());

}

void PoseidonBossAI::ShootOneShuiPaoBullet()
{
	Bullet3OfBoss4* bullet = Bullet3OfBoss4::create();
	bullet->setBelongTo(UnitId::ePlaneBoss);
	bullet->resetHurt(getBossHurts());
	UIController::getInstance()->getPhysicsLayer()->addChild(bullet);
}

void PoseidonBossAI::Shoot3ShuiPaoBullet()
{
	Vec2 offset = Vec2(0,-100);
	{
		Bullet4OfBoss5* bullet = Bullet4OfBoss5::create(0.0);
		bullet->setPosition(Vec2(0, 0) + offset);
		bullet->setBelongTo(UnitId::ePlaneBoss);
		bullet->resetHurt(getBossHurts());
		m_target->addChild(bullet);
	}
	{
		Bullet4OfBoss5* bullet = Bullet4OfBoss5::create(1.0);
		bullet->setPosition(Vec2(-200, 350) + offset);
		bullet->setBelongTo(UnitId::ePlaneBoss);
		bullet->resetHurt(getBossHurts());
		m_target->addChild(bullet);
	}
	{
		Bullet4OfBoss5* bullet = Bullet4OfBoss5::create(2.0);
		bullet->setPosition(Vec2(200, 350) + offset);
		bullet->setBelongTo(UnitId::ePlaneBoss);
		bullet->resetHurt(getBossHurts());
		m_target->addChild(bullet);
	}
}

void PoseidonBossAI::ShootRotateBullet()
{
	for (int i = 0; i < 8; i++)
	{
		Weapon* wp = Weapon::create(m_target);
		wp->setBullstType(BulletType::Enemy_default);
		wp->setBltResTag(ResTag::Bullet_BossShuiPaoSmall);
		wp->setBltScaleX(0.5);
		wp->setBltScaleY(0.5);
		wp->setBltBelongTo(UnitId::ePlaneBoss);
		wp->setShootTimes(25);
		wp->setBltHurts(getBossHurts());
		wp->setBltSpeed(400);
		wp->setRotation(i * 8 );
		m_target->AddOneWp(wp,WeaponType::Normal);
		wp->setOffset(getRightHandPos());
		wp->runAction(RepeatForever::create(RotateBy::create(2.0,800)));
	}
}

void PoseidonBossAI::SHootTriangleBullet(float angle)
{
	Vec2 shootPos = getLeftHandPos() + Vec2(-85, -20);
	int count = 0;
	{
		Weapon* wp = Weapon::create(m_target);
		wp->setBullstType(BulletType::Enemy_default);
		wp->setBltResTag(ResTag::Bullet_BossShuiPaoSmall);
		wp->setBltBelongTo(UnitId::ePlaneBoss);
		wp->setShootTimes(1);
		wp->setBltHurts(getBossHurts());
		wp->setBltScaleX(0.5);
		wp->setBltScaleY(0.5);
		wp->setBltSpeed(500);
		wp->setDelayShoot(count * 0.1);
		wp->setRotation(0 + angle);
		m_target->AddOneWp(wp,WeaponType::Normal);
		wp->setOffset(shootPos);
	}
	count++;

	for (int i = 0; i < 2; i++)
	{
		Weapon* wp = Weapon::create(m_target);
		wp->setBullstType(BulletType::Enemy_default);
		wp->setBltResTag(ResTag::Bullet_BossShuiPaoSmall);
		wp->setShootTimes(1);
		wp->setBltHurts(getBossHurts());
		wp->setBltSpeed(500);
		wp->setBltScaleX(0.5);
		wp->setBltScaleY(0.5);
		wp->setDelayShoot(count * 0.1);
		wp->setRotation(angle + 8 * (i-0.5));
		m_target->AddOneWp(wp,WeaponType::Normal);
		wp->setOffset(shootPos);
	}
	count++;

	for (int i = 0; i < 3; i++)
	{
		Weapon* wp = Weapon::create(m_target);
		wp->setBullstType(BulletType::Enemy_default);
		wp->setBltResTag(ResTag::Bullet_BossShuiPaoSmall);
		wp->setShootTimes(1);
		wp->setBltHurts(getBossHurts());
		wp->setBltSpeed(500);
		wp->setBltScaleX(0.5);
		wp->setBltScaleY(0.5);
		wp->setDelayShoot(count * 0.1);
		wp->setRotation(angle + 10 * (i-1));
		m_target->AddOneWp(wp,WeaponType::Normal);
		wp->setOffset(shootPos);
	}
}

void PoseidonBossAI::PlayAnimation1()
{
	playArmat(1,this,frameEvent_selector(PoseidonBossAI::onFrameEventOfShoot1));
}

void PoseidonBossAI::PlayAnimation2L()
{
	playArmat(1,this,frameEvent_selector(PoseidonBossAI::onFrameEventOfShoot2R),1.5);
}

void PoseidonBossAI::PlayAnimation2R()
{
	playArmat(2,this,frameEvent_selector(PoseidonBossAI::onFrameEventOfShoot2L),1.5);
}

void PoseidonBossAI::PlayAnimation3()
{
	playArmat(1,this,frameEvent_selector(PoseidonBossAI::onFrameEventOfShoot3));
}

void PoseidonBossAI::PlayAnimation4()
{
	playArmat(1,this,frameEvent_selector(PoseidonBossAI::onFrameEventOfShoot4));
}

void PoseidonBossAI::PlayAnimation5()
{
	playArmat(1,this,frameEvent_selector(PoseidonBossAI::onFrameEventOfShoot5));
}

void PoseidonBossAI::PlayAnimation6()
{
	playArmat(2,this,frameEvent_selector(PoseidonBossAI::onFrameEventOfShoot6));
}

void PoseidonBossAI::PlayAnimationRest()
{
	playArmat(0);
}

void PoseidonBossAI::PlayAnimationRest( float dur )
{
	playArmat(0);
	auto delay = DelayTime::create(dur);
	auto next = CallFunc::create(this,callfunc_selector(PoseidonBossAI::NewAction));
	auto seq = Sequence::create(delay, next, nullptr);
	runAction(seq);
}

void PoseidonBossAI::onFrameEventOfShoot1( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "shoot")
	{
		//PauseDur(2.0f);
		ShootOneLineBullet();
	}
	else if(evt == "end")
	{
		PlayAnimationRest(5.5f + rand_0_1() * 0.5);
	}
}

void PoseidonBossAI::onFrameEventOfShoot2L( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "shoot")
	{
		ShootTwoBezierBulletL();
		PauseDur(1.5f);
	}
	else if(evt == "end")
	{
		Action2();
	}
}

void PoseidonBossAI::onFrameEventOfShoot2R( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "shoot")
	{
		ShootTwoBezierBulletR();
		PauseDur(1.5f);
	}
	else if(evt == "end")
	{
		Action2();
	}
}

void PoseidonBossAI::onFrameEventOfShoot3( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "shoot")
	{
		PauseDur(1.0f);
		ShootOneShuiPaoBullet();
	}
	else if(evt == "end")
	{
		PlayAnimationRest(0.5f + rand_0_1() * 0.5);
	}
}

void PoseidonBossAI::onFrameEventOfShoot4( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "shoot")
	{
		PauseDur(2.0f);
		Shoot3ShuiPaoBullet();
	}
	else if(evt == "end")
	{
		PlayAnimationRest(3.5f + rand_0_1() * 0.5);
	}
}

void PoseidonBossAI::onFrameEventOfShoot5( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "shoot")
	{
		ShootRotateBullet();
	}
	else if(evt == "end")
	{
		PlayAnimationRest(1.5f + rand_0_1() * 0.5);
	}
}

void PoseidonBossAI::onFrameEventOfShoot6( Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex )
{
	if (evt == "shoot")
	{
		auto delay = DelayTime::create(0.5);
		auto shoot = CallFunc::create(this,callfunc_selector(PoseidonBossAI::ShootTriangBulletSomeTimes));
		auto seq = Sequence::create(shoot, delay, nullptr);
		auto rep = Repeat::create(seq, 5);
		auto next = CallFunc::create(this,callfunc_selector(PoseidonBossAI::NewAction));
		auto seq1 = Sequence::create(rep, next, nullptr);
		m_target->runAction(seq1);
		PauseDur(3.0);
	}
	else if(evt == "end")
	{
		Action0();
	}
}

void PoseidonBossAI::pauseArmat()
{
	BaseBoss* boss = (BaseBoss*)m_target;
	auto armat = boss->getArmature();
	armat->getAnimation()->pause();	
}

void PoseidonBossAI::resumeArmat()
{
	BaseBoss* boss = (BaseBoss*)m_target;
	auto armat = boss->getArmature();
	armat->getAnimation()->resume();	
}

void PoseidonBossAI::changeSpeed( float scale )
{
	BaseBoss* boss = (BaseBoss*)m_target;
	auto armat = boss->getArmature();
	armat->getAnimation()->setSpeedScale(scale);	
}
void PoseidonBossAI::PauseDur( float dt )
{
	auto dur = DelayTime::create(dt);
	auto pause = CallFunc::create(this,callfunc_selector(PoseidonBossAI::pauseArmat));
	auto resume = CallFunc::create(this,callfunc_selector(PoseidonBossAI::resumeArmat));
	auto seq = Sequence::create(pause, dur, resume, nullptr);
	runAction(seq);
}

void PoseidonBossAI::Action0()
{
	PlayAnimationRest(2.0 + rand_0_1()*1.0);
}

void PoseidonBossAI::Action1()
{
	PlayAnimation1();
}

void PoseidonBossAI::Action2()
{
	if (m_bezierTimes > 5)
	{
		m_bezierTimes = 0;
		PlayAnimationRest(1.5f + rand_0_1() * 0.5);
		return;
	}
	int yushu = m_bezierTimes%2;
	if ( yushu == 0)
	{
		PlayAnimation2L();
	}
	else
	{
		PlayAnimation2R();
	}
	m_bezierTimes++;
}

void PoseidonBossAI::Action3()
{
	PlayAnimation3();
}

void PoseidonBossAI::Action4()
{
	PlayAnimation4();
}

void PoseidonBossAI::Action5()
{
	PlayAnimation5();
}

void PoseidonBossAI::Action6()
{
	if (m_bezierTimes > 5)
	{
		m_bezierTimes = 0;
		PlayAnimationRest(3.5f + rand_0_1() * 0.5);
		return;
	}
	PlayAnimation6();
	m_bezierTimes++;
}

void PoseidonBossAI::ShootTriangBulletSomeTimes()
{
	float randAngle = random(0,60);
	SHootTriangleBullet(0 + randAngle);
	SHootTriangleBullet(60 + randAngle);
	SHootTriangleBullet(120 + randAngle);
	SHootTriangleBullet(180 + randAngle);
	SHootTriangleBullet(240 + randAngle);
	SHootTriangleBullet(300 + randAngle);
}

cocos2d::Vec2 PoseidonBossAI::getLeftHandPos()
{
	return Vec2(-80, 240);
}

cocos2d::Vec2 PoseidonBossAI::getRightHandPos()
{
	return Vec2(135, 170);
}



//----------------------
bool EnemyAiofStay::init()
{
	return PlaneAI::init();
}

void EnemyAiofStay::StartAI( int lvl )
{
	m_AIlvl = lvl;
	StartShoot();
	Stay();
}

void EnemyAiofStay::Stay()
{
	//先移动到屏幕的某个位置
	auto mypos = m_target->convertToWorldSpaceAR(Vec2::ZERO);

	auto mv = MoveTo::create(1.0 + rand_0_1() * 0.1, Vec2(m_winSize.width * (0.5 + rand_minus1_1()*0.35), mypos.y));
	auto ease = EaseSineOut::create(mv);
	auto swing = MoveBy::create(0.5 + rand_0_1() * 0.2, Vec2(20, 0));
	auto back = swing->reverse();
	auto seq = Sequence::create(swing, back, nullptr);
	auto rep = RepeatForever::create(seq);
	auto seq1 = Sequence::create(ease, rep, nullptr);
	m_target->runAction(seq1);
}

//----------------------
bool EnemyAiofFree::init()
{
	return PlaneAI::init();
}

void EnemyAiofFree::StartAI( int lvl )
{
	m_AIlvl = lvl;
	StartShoot();
	MoveFree();
}

void EnemyAiofFree::MoveFree()
{
	PointArray* arr = PointArray::create(100);
	auto mypos = m_target->convertToWorldSpaceAR(Vec2::ZERO);
	arr->addControlPoint(mypos);

	for (int i = 0; i < 99; ++i)
	{
		Vec2 pos = Vec2(m_winSize.width * (0.5 + rand_minus1_1() * 0.4),  m_winSize.height * (0.5 + rand_0_1() * 0.4));
		arr->addControlPoint(pos);
	}

	auto mv = CardinalSplineTo::create(200, arr, 0);
	auto next = CallFunc::create(this,callfunc_selector(EnemyAiofFree::MoveFree));
	auto seq = Sequence::create(mv, next, nullptr);
	m_target->runAction(seq);
}
