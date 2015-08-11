#include "Bullet.h"
#include "../Layer/UIController.h"
#include "../Layer/PhysicsLayer.h"
#include "../../GameCommon/GameData.h"
#include "../../GameCommon/EffectsManager.h"
#include "../../GameCommon/GameConfig.h"
#include "../Layer/MenuLayer.h"
#include "../Weapon/WeaponFactory.h"
#include "../Weapon/weapon.h"
#include "GameCommon/GameDefine.h"
#include "BulletFactory.h"


Bullet::Bullet()
{
	m_weapon = nullptr;
	m_WeaponDelegate = nullptr;
	m_FlySpeed = 500;
	m_OriginVelocity = Vec2(0,1);
}

Bullet* Bullet::create( const std::string& frameName )
{
	Bullet* bullet = new (std::nothrow) Bullet;
	if (bullet && bullet->initWithSpriteFrameName(frameName))
	{
		bullet->autorelease();
		bullet->initBody();
		bullet->initBullet();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void Bullet::FixAngleWithWeapon( Weapon* weapon )
{
	if (!weapon)
	{
		return;
	}
	m_weapon = weapon;

	//得到武器相对于世界的旋转角度，并让子弹旋转相应的角度
	Node* plane = weapon->getParent();
	float planeRotate = plane->getRotation();
	float wpRotate = weapon->getRotation();
	float sumRotate = planeRotate + wpRotate;
	this->setRotation(sumRotate);
	
	Vec2 vec = m_weapon->getBltOriginVelocity().forAngle( (90 -sumRotate) * Angle2pi );
	vec.normalize();
	_physicsBody->setVelocity(vec * m_weapon->getBltSpeed());
}

void Bullet::initBody()
{
	auto s = getContentSize();
	auto body = PhysicsBody::createBox(Size(s.width*0.5, s.height*0.6));
	body->setGravityEnable(false);
	setPhysicsBody(body);

	setBodyPro();
}

void Bullet::setBodyPro()
{
	if (!m_WeaponDelegate)
	{
		return;
	}
	//body->setVelocity(m_WeaponDelegate->getBltSpeed() * m_WeaponDelegate->getBltOriginVelocity());
	_physicsBody->setLinearDamping(m_WeaponDelegate->getBltLinearDamping());
	_physicsBody->setGravityEnable(m_WeaponDelegate->getBltGravityAble());
	_physicsBody->setMass(m_WeaponDelegate->getBltMass());
	_physicsBody->setCategoryBitmask(m_WeaponDelegate->getBltCategorybitmask());
	_physicsBody->setCollisionBitmask(m_WeaponDelegate->getBltCollisionbitmask());
	_physicsBody->setContactTestBitmask(m_WeaponDelegate->getBltContacttestbitmask());
}

void Bullet::setBitmask(const int& cate,const int& collision,const int& test )
{
	_physicsBody->setCategoryBitmask(cate);
	_physicsBody->setCollisionBitmask(collision);
	_physicsBody->setContactTestBitmask(test);
}

void Bullet::onEnter()
{
	PhysicsUnit::onEnter();
	ListeningBossDead();
	ListeningProtectRemove();

	setBodyPro();
}

void Bullet::InitEdgeLine()
{
	m_EdgeLineVec.push_back(0);
	m_EdgeLineVec.push_back(m_WinSize.width);
	m_EdgeLineVec.push_back(-50);
	m_EdgeLineVec.push_back(m_WinSize.height + 300);
}

void Bullet::resetHurt( int hurts )
{
	m_ContactPro.hurts = hurts;
}

void Bullet::ListeningStopWeapon()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Bullet::StopBullet),Msg_StopMainPlBullet, nullptr);
}

void Bullet::ListeningBossDead()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Bullet::BossDead),Msg_BossDead, nullptr);
}

void Bullet::ListeningProtectRemove()
{
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(Bullet::ProtectRemove),Msg_ProtectRemove, nullptr);
}

void Bullet::BossDead( Ref* obj )
{
	if (m_SelfPro.belongTo == UnitId::ePlaneBoss)
	{
		setDirectDeath();
	}
}

void Bullet::StopBullet( Ref* obj )
{
	setDirectDeath();
}

void Bullet::ProtectRemove( Ref* obj )
{
	if (m_SelfPro.belongTo <= UnitId::ePlaneBoss)
	{
		//敌方子弹，全部消失
		_physicsBody->setDynamic(false);
		stopAllActions();
		resetHurt(0);
		setOpacity(0);
		auto fadeOut = FadeOut::create(0.02f);
		auto fadeIn = FadeIn::create(0.02f);
		auto seq = Sequence::create(fadeIn,fadeOut,nullptr);
		auto rep = Repeat::create(seq, 5);
		auto func = CallFunc::create(this, callfunc_selector(PhysicsUnit::setDirectDeath));
		auto seq1 = Sequence::create(rep, func, nullptr);
		runAction(seq1);
	}
}

void Bullet::setWeaponDelegate( WeaponDelegate* wpDlgt )
{
	m_WeaponDelegate = wpDlgt;

	resetHurt(wpDlgt->getBltHurts());
	setScaleX(wpDlgt->getBltScaleX());
	setScaleY(wpDlgt->getBltScaleY());
	setBelongTo(wpDlgt->getBltBelongTo());
}

void Bullet::setBltFlySpeed( int speed )
{
	m_FlySpeed = speed;
	Vec2 v = m_OriginVelocity;
	v.normalize();
	_physicsBody->setVelocity(v * m_FlySpeed);
}

int Bullet::getBltFlySpeed()
{
	return m_FlySpeed;
}




//----------------------------------
//导弹
MissileForDis* MissileForDis::create( const std::string& frameName )
{
	MissileForDis* bullet = new (std::nothrow) MissileForDis;
	if (bullet && bullet->initWithSpriteFrameName(frameName))
	{
		bullet->autorelease();
		bullet->initBody();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

MissileForDis* MissileForDis::create()
{
	MissileForDis* bullet = new (std::nothrow) MissileForDis;
	if (bullet && bullet->init())
	{
		bullet->autorelease();
		bullet->initBody();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void MissileForDis::initBody()
{	
	auto body = PhysicsBody::createCircle(10);
	body->setGravityEnable(false);
	body->setRotationEnable(false);
	setPhysicsBody(body);
	setBitmask(
		SELF_AMMO_CATEGORYBITMASK,
		SELF_AMMO_COLLISIONBITMASK_P,
		SELF_AMMO_CONTACTTESTBITMASK);
	setInvincible(true);

	schedule(schedule_selector(MissileForDis::setBody), 0.1);
}

void MissileForDis::onEnter()
{
	Bullet::onEnter();
	NewAudio::getInstance()->playEff(Music_IceMissile);

	m_targetPos = Vec2(360, 1500);

	addParticle();

	schedule(schedule_selector(MissileForDis::FlyToTarget));
}

void MissileForDis::FlyToTarget( float dt )
{
	Vec2 curVec = _physicsBody->getVelocity();
	Vec2 myPos = convertToWorldSpaceAR(Vec2::ZERO);
	{
		//由于不断的setVelocity（）导致粒子产生抖动，故用插值函数减弱振动
		if (m_particle1)
		{
			Vec2 partcPos = m_particle1->convertToWorldSpaceAR(Vec2::ZERO);
			Vec2 lerpPos = partcPos.lerp(myPos,0.3);
			m_particle1->setPosition(lerpPos);
			if (m_particle2)
			{
				m_particle2->setPosition(lerpPos);
			}
		}
	}
	

	CheckIsOutScreen(0.0f);
	if (m_isOutScreen)
	{
		Dying();
		return;
	}
	
	auto enemyPlaneList = GameData::getInstance()->getEnemyPlaneVec();
	if (!enemyPlaneList.empty())
	{
		m_targetPos = GameData::getInstance()->getNearTargetByPos(myPos);
	}

	Vec2 newVec = m_targetPos - myPos;

	float distance = myPos.distance(m_targetPos);
	curVec.smooth(newVec,100,1 + distance); 
	curVec.normalize();
	_physicsBody->setVelocity(curVec * m_FlySpeed);

	//float bulletR = curVec.getAngle()*Pi2Angle;
	//this->setRotation(90-bulletR);		//因为加了粒子，所以不能旋转

	if (m_FlySpeed < 300)
	{
		m_FlySpeed += 5;
	}
}


void MissileForDis::addParticle()
{
	auto layer = UIController::getInstance()->getPhysicsLayer();
	auto mypos = convertToWorldSpaceAR(Vec2::ZERO);

	m_particle1 = EffectsManager::addParticle("particla/IceMissile/IceMissile_1.plist",layer,mypos,false);
	m_particle2 = EffectsManager::addParticle("particla/IceMissile/IceMissile_2.plist",layer,mypos,false);
}

void MissileForDis::Explosion()
{
	//碰撞爆炸
	auto layer = UIController::getInstance()->getPhysicsLayer();
	auto pos = convertToWorldSpaceAR(Vec2::ZERO);
	EffectsManager::addParticle("particla/IceMissleExpl/IceMissleExpl.plist",layer,pos);

	auto sp = Sprite::create("particla/IceMissleExpl/IceMissleExpl.png");
	sp->setPosition(pos);
	sp->setScale(0.1);
	layer->addChild(sp,-1);

	ActionInterval* scale = ScaleTo::create(0.3f, 2.0f);
	ActionInterval* fade = FadeOut::create(0.3f);
	CallFuncN* rm = CallFuncN::create(this,callfuncN_selector(MissileForDis::removeExpSp));
	ActionInterval* seq = Sequence::create(scale, fade, rm, nullptr);
	sp->runAction(seq);

	ActionInterval* rotate = RotateBy::create(0.3f, 1000);
	ActionInterval* rep = RepeatForever::create(rotate);
	sp->runAction(rep);

}

void MissileForDis::removeExpSp( Node* node )
{
	node->removeFromParentAndCleanup(true);
}

void MissileForDis::removeme()
{
	setDirectDeath();
}

void MissileForDis::Dying()
{
	unschedule(schedule_selector(MissileForDis::FlyToTarget));
	getPhysicsBody()->setVelocity(Vec2(0.01,0.01));

	setBitmask(
		SELF_AMMO_CATEGORYBITMASK,
		SELF_AMMO_COLLISIONBITMASK_P,
		SELF_AMMO_COLLISIONBITMASK_P);
	m_ContactPro.hurts = 0;
	
	ActionInterval* delay = DelayTime::create(0.25f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(MissileForDis::removeme));
	ActionInterval* seq = Sequence::create(delay, func, nullptr);
	runAction(seq);
}

void MissileForDis::InitEdgeLine()
{
	m_EdgeLineVec.push_back(-100);
	m_EdgeLineVec.push_back(m_WinSize.width + 100);
	m_EdgeLineVec.push_back(-100);
	m_EdgeLineVec.push_back(m_WinSize.height + 100);
}

void MissileForDis::DoSomethingBeforeDead()
{
	if (m_particle1)
	{
		m_particle1->removeFromParentAndCleanup(true);
		m_particle1 = nullptr;
	}
	if (m_particle2)
	{
		m_particle2->removeFromParentAndCleanup(true);
		m_particle2 = nullptr;
	}
}

void MissileForDis::OtherDealWhenContact( const ContactPro& rs )
{
	Dying();
	Explosion();
}

void MissileForDis::setBody( float t )
{
	auto body = PhysicsBody::createCircle(10);
	body->setGravityEnable(false);
	body->setRotationEnable(false);
	setPhysicsBody(body);
	setBitmask(
		SELF_AMMO_CATEGORYBITMASK,
		SELF_AMMO_COLLISIONBITMASK_P,
		SELF_AMMO_CONTACTTESTBITMASK);
	setInvincible(true);
}




//-----------------------------------------
MissileForBloodlimit::MissileForBloodlimit()
{
	
}

MissileForBloodlimit* MissileForBloodlimit::create( const std::string& frameName,const float& AngleOffet )
{
	MissileForBloodlimit* bullet = new (std::nothrow) MissileForBloodlimit;
	if (bullet && bullet->initWithSpriteFrameName(frameName))
	{
		bullet->autorelease();
		bullet->m_AngleOffset = AngleOffet;
		bullet->initBody();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void MissileForBloodlimit::onEnter()
{
	Bullet::onEnter();
	addParticle();

	//修正发射角度
	FixDirect();

	ActionInterval* delay = DelayTime::create(1.0);
	CallFunc* func = CallFunc::create(this,callfunc_selector(MissileForBloodlimit::Explosion));
	ActionInterval* seq = Sequence::create(delay,func,nullptr);
	runAction(seq);

	schedule(schedule_selector(MissileForBloodlimit::FlyToTarget));
}

void MissileForBloodlimit::FlyToTarget( float dt )
{
	Vec2 curVec = _physicsBody->getVelocity();
	auto target = GameData::getInstance()->getEnemyPlaneByMaxHp();

	if (target)
	{
		Vec2 bulletPos = getPosition();

		Vec2 targetPos = target->convertToWorldSpaceAR(Point::ZERO);

		//log("--target pos = %f, %f",targetPos.x,targetPos.y);
		Vec2 newVec = targetPos - bulletPos;

		//两个向量的夹角
		float jiajiao = newVec.getAngle(curVec)*Pi2Angle;
		float distance = bulletPos.distance(targetPos);
		curVec.smooth(newVec,100,1 + distance); 
	}

	curVec.normalize();
	float bulletR = curVec.getAngle()*Pi2Angle;
	this->setRotation(90-bulletR);
	_physicsBody->setVelocity(curVec * m_FlySpeed);
}

void MissileForBloodlimit::FixDirect()
{
	//取得目标
	auto target = GameData::getInstance()->getEnemyPlaneByMaxHp();

	Vec2 curVec = m_weapon->getBltOriginVelocity();

	if (target)
	{
		//锁定标示
		ContactPro rs = target->getContactPro();
		rs.isTarget = true;
		target->setContactPro(rs);

		Vec2 bulletPos = getPosition();
		Vec2 targetPos = target->getPosition();
		curVec = targetPos - bulletPos;
	}

	//标准化后乘以速率
	float bulletR = curVec.getAngle()*Pi2Angle;
	float sumAngle = 90 - bulletR + m_AngleOffset;
	this->setRotation(sumAngle);
	curVec = curVec.forAngle((90 + sumAngle) * Angle2pi);
	curVec.normalize();
	_physicsBody->setVelocity(curVec * m_FlySpeed);
}

void MissileForBloodlimit::OtherDealWhenContact( const ContactPro& rs )
{
	//如果碰撞的是被锁定的目标,那么导弹消亡
	//log("-- isTarget = %d ",rs.isTarget);
	if (rs.isTarget)
	{
		Explosion();
	}
}

void MissileForBloodlimit::addParticle()
{
	
}

void MissileForBloodlimit::Explosion()
{
	UIController::getInstance()->getEffectsLayer()->displayScopeAmmo(getPosition());

	setDirectDeath();
}






//----------------------------------
FallingBullet* FallingBullet::create( const std::string& frameName )
{
	FallingBullet* bullet = new (std::nothrow) FallingBullet;
	if (bullet && bullet->initWithSpriteFrameName(frameName))
	{
		bullet->autorelease();
		bullet->initBody();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void FallingBullet::onEnter()
{
	Bullet::onEnter();

	setBitmask(
		ENEMY_AMMO_CATEGORYBITMASK,
		ENEMY_AMMO_COLLISIONBITMASK,
		ENEMY_AMMO_CONTACTTESTBITMASK);
	_physicsBody->setLinearDamping(0.8);
	_physicsBody->setGravityEnable(false);
	_physicsBody->setVelocity(Vec2(0,0));
	_physicsBody->setMass(0.008);

	_physicsBody->applyForce(Vec2(rand_minus1_1()*0.3,-rand_0_1()),Vec2(0,0));

	schedule(schedule_selector(FallingBullet::newFalling),1.0);
}

void FallingBullet::newFalling(float dt)
{
	_physicsBody->applyForce(Vec2(rand_minus1_1()*0.3,-rand_0_1()*0.1),Vec2(0,0));
}

//------------------------------
CurveBullet* CurveBullet::create( const std::string& frameName)
{
	CurveBullet* bullet = new (std::nothrow) CurveBullet;
	if (bullet && bullet->initWithSpriteFrameName(frameName))
	{
		bullet->autorelease();
		bullet->initBody();
		bullet->initBullet();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void CurveBullet::onEnter()
{
	Bullet::onEnter();
	float wpRotate = m_weapon->getRotation() - 180;
	setBitmask(
		ENEMY_AMMO_CATEGORYBITMASK,
		ENEMY_AMMO_COLLISIONBITMASK,
		ENEMY_AMMO_CONTACTTESTBITMASK);
	_physicsBody->setMass(0.1);
	//schedule(schedule_selector(CurveBullet::newCurve));

	Point myPos = this->convertToWorldSpaceAR(Point::ZERO);
	
	float offset = rand_0_1() * 300;
	//贝塞尔曲线运动
	ccBezierConfig bzconfig;
	if (wpRotate < 0)	// 右
	{
		bzconfig.endPosition = Vec2(m_WinSize.width*0.5 - 100,-120);
		bzconfig.controlPoint_1 = Vec2(m_WinSize.width - 100 + offset,myPos.y*0.8);
		bzconfig.controlPoint_2 = Vec2(m_WinSize.width - 100 - offset,myPos.y*0.33);
	}
	else    // 左
	{
		bzconfig.endPosition = Vec2(m_WinSize.width*0.5 + 100,-120);
		bzconfig.controlPoint_1 = Vec2(100 - offset,myPos.y*0.8);
		bzconfig.controlPoint_2 = Vec2(100 + offset,myPos.y*0.33);
	}
	
	ActionInterval* bez = BezierTo::create(1.0f,bzconfig);
	runAction(bez);
}

void CurveBullet::newCurve( float dt )
{
	//得到当前飞行速度，格式化
	Vec2 curV = _physicsBody->getVelocity();
	//log("curv = %f, %f",curV.x,curV.y);
	curV.normalize();

	//让子弹旋转，使其朝向运动方向
	float r = curV.getAngle(Vec2(1,0)) * Pi2Angle - 90;
	this->setRotation(r);

	//找到速度向量的法线，朝着该方向给一个力(绕着一个点慢慢扩散)
	/*Vec2 faxian = curV.getPerp();
	faxian.normalize();
	_physicsBody->applyImpulse(faxian*5,Vec2(0,0));*/

}

void CurveBullet::initBullet()
{
	//属性初始化
	m_SelfPro.maxHP = 1;
	m_SelfPro.curHP = m_SelfPro.maxHP;

	m_ContactPro.hurts = - 1000;
}


//-------------------------- 曲线子弹带方向
CurveBullet2* CurveBullet2::create( const std::string& frameName,const BullDirection& dir )
{
	CurveBullet2* bullet = new (std::nothrow) CurveBullet2;
	if (bullet && bullet->initWithSpriteFrameName(frameName))
	{
		bullet->autorelease();
		bullet->initBody();
		bullet->initBullet();
		bullet->_dir = dir;
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void CurveBullet2::onEnter()
{
	Bullet::onEnter();

	setBitmask(
		ENEMY_AMMO_CATEGORYBITMASK,
		ENEMY_AMMO_COLLISIONBITMASK,
		ENEMY_AMMO_CONTACTTESTBITMASK);
	_physicsBody->setMass(0.1);
	//schedule(schedule_selector(CurveBullet::newCurve));
	Point myPos = this->convertToWorldSpaceAR(Point::ZERO);

	//贝塞尔曲线运动
	ccBezierConfig bzconfig;
	if ( Dirct_Right == _dir )	// 右
	{
		bzconfig.endPosition = Vec2( 0,200);
		bzconfig.controlPoint_1 = Vec2(m_WinSize.width + 100,myPos.y*0.5);
		bzconfig.controlPoint_2 = Vec2(m_WinSize.width - 200 ,myPos.y*0.5);
	}
	else    // 左
	{
		bzconfig.endPosition = Vec2(m_WinSize.width,200);
		bzconfig.controlPoint_1 = Vec2(-100,myPos.y*0.5);
		bzconfig.controlPoint_2 = Vec2(200,myPos.y*0.5);
	}

	ActionInterval* bez = BezierTo::create(1.0f,bzconfig);
	runAction(bez);
}

void CurveBullet2::newCurve( float dt )
{
	//得到当前飞行速度，格式化
	Vec2 curV = _physicsBody->getVelocity();
	//log("curv = %f, %f",curV.x,curV.y);
	curV.normalize();

	//让子弹旋转，使其朝向运动方向
	float r = curV.getAngle(Vec2(1,0)) * Pi2Angle - 90;
	this->setRotation(r);
}

void CurveBullet2::initBullet()
{
	//属性初始化
	m_SelfPro.maxHP = 1;
	m_SelfPro.curHP = m_SelfPro.maxHP;

	m_ContactPro.hurts = - 1000;
}



//-------------------------- 反弹子弹
ReboundBullect* ReboundBullect::create( const std::string& frameName )
{
	ReboundBullect* bullet = new (std::nothrow) ReboundBullect;
	if (bullet && bullet->initWithSpriteFrameName(frameName))
	{
		bullet->autorelease();
		bullet->initBody();
		bullet->initBullet();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void ReboundBullect::onEnter()
{
	Bullet::onEnter();

	ActionInterval* rot = RotateBy::create(1.0f,360 - cocos2d::random(0,1) * 720 );
	ActionInterval* rep = RepeatForever::create(rot);
	runAction(rep);

	//解析其他数据
	void* otherdata = m_weapon->getBltOtherData();
	if (otherdata != nullptr)
	{
		int times = (int)otherdata;
		m_MaxTimes = times;
	}
}

void ReboundBullect::initBody()
{
	//圆球
	auto body = PhysicsBody::createCircle( getContentSize().width*0.5, PhysicsMaterial(0.0f,1.0f,0.5f));
	body->setGravityEnable(true);
	body->setRotationEnable(true);
	body->setLinearDamping(0.4f);
	body->setMass(100);
	setPhysicsBody(body);
	setBitmask(
		ENEMY_AMMO2_CATEGORYBITMASK,
		ENEMY_AMMO2_COLLISIONBITMASK_P,
		ENEMY_AMMO2_CONTACTTESTBITMASK);
	//setInvincible(true);
}

void ReboundBullect::OtherDealWhenContact( const ContactPro& rs )
{
	// 达到撞击次数，销毁
	if ( m_MaxTimes <= _collisionTimes && rs.idx == UnitId::eBottomWall)
	{
		setDirectDeath();
	}

	// 第一次撞击底部进行统计
	if ( rs.idx == UnitId::eBottomWall )
	{
		_collisionTimes ++;
	}
}


//流弹
GrenadeBullect* GrenadeBullect::create( const std::string& frameName )
{
	auto bullet = new (std::nothrow) GrenadeBullect;
	if (bullet && bullet->initWithSpriteFrameName(frameName))
	{
		bullet->autorelease();
		bullet->initBody();
		bullet->initBullet();
		bullet->RandPos();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void GrenadeBullect::onEnter()
{
	Bullet::onEnter();
	
	Point myPos = this->convertToWorldSpaceAR(Point::ZERO);

	ccBezierConfig bzconfig;
	if ( cocos2d::random(0,1) )	// 右
	{
		m_EdgeLineVec.push_back(-500);
		bzconfig.endPosition = Vec2( myPos.x,m_EdgeLineVec.at(2) - 50);
		bzconfig.controlPoint_1 = Vec2(m_WinSize.width + 100 - myPos.x,myPos.y*0.8);
		bzconfig.controlPoint_2 = Vec2(m_WinSize.width - 200 - myPos.x ,myPos.y*0.4);
	}
	else    // 左
	{
		bzconfig.endPosition = Vec2(myPos.x,m_EdgeLineVec.at(2) - 50);
		bzconfig.controlPoint_1 = Vec2(-100 + myPos.x,myPos.y*0.8);
		bzconfig.controlPoint_2 = Vec2(200 + myPos.x ,myPos.y*0.4);
	}

	// 设置透明度为0 因为有拖尾
	setOpacity( 0 );

	ActionInterval* bez = BezierTo::create(3.0f,bzconfig);
	runAction( bez );

	auto s = getContentSize();

	auto sp = Sprite::create("particla/liudan/kulou.png");
	sp->setPosition(s.width*0.5, s.height*0.5);
	sp->setName("kulou");
	addChild(sp);

	//拖尾粒子
	EffectsManager::addParticle("particla/liudan/tail_1.plist", this, Vec2(s.width*0.5, s.height*0.1), false);
	EffectsManager::addParticle("particla/liudan/tail_2.plist", this, Vec2(s.width*0.5, s.height*0.5), false);
	EffectsManager::addParticle("particla/liudan/tail_3.plist", this, Vec2(s.width*0.5, s.height*0.8), false);

}

void GrenadeBullect::initBullet()
{
	//属性初始化
	m_SelfPro.maxHP = 1;
	m_SelfPro.curHP = m_SelfPro.maxHP;

	m_ContactPro.hurts = Ene_Blt_hurt;
	setInvincible(true);
}

void GrenadeBullect::RandPos()
{
	int x = cocos2d::random( 50, 700 );
	setPosition( Vec2(x, 1280) );
}

void GrenadeBullect::InitEdgeLine()
{
	m_EdgeLineVec.push_back(-200);
	m_EdgeLineVec.push_back(m_WinSize.width + 200);
	m_EdgeLineVec.push_back(-500);
	m_EdgeLineVec.push_back(m_WinSize.height);
}

void GrenadeBullect::OtherDealWhenContact( const ContactPro& rs )
{
	stopAllActions();
	_physicsBody->setCollisionBitmask(0);

	//UIController::getInstance()->getEffectsLayer()->PlaneCantTouchAble(2.0f);

	Node* node = getChildByName("kulou");
	if (node)
	{
		auto fadeout = FadeOut::create(0.5f);
		auto scale = ScaleTo::create(0.1f,10.0f);
		auto spw = Spawn::create(fadeout, scale, nullptr);
		auto func = CallFunc::create(this, callfunc_selector(PhysicsUnit::setDirectDeath));
		auto seq = Sequence::create(spw, func, nullptr);
		node->runAction(seq);
	}
}



//-------------------
HelixBullet* HelixBullet::create( const std::string& frameName )
{
	HelixBullet* bullet = new (std::nothrow) HelixBullet;
	if (bullet && bullet->initWithSpriteFrameName(frameName))
	{
		bullet->autorelease();
		bullet->initBody();
		bullet->initBullet();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void HelixBullet::onEnter()
{
	Bullet::onEnter();
	setBitmask(
		ENEMY_AMMO_CATEGORYBITMASK,
		ENEMY_AMMO_COLLISIONBITMASK,
		ENEMY_AMMO_CONTACTTESTBITMASK);
	_physicsBody->setLinearDamping(0.8);
	_physicsBody->setGravityEnable(false);
	//_physicsBody->setVelocity(Vec2(0,-10));
	_physicsBody->setMass(0.008);

	m_centerPos = getPosition();
	m_beginPos = m_centerPos;
	m_centerPos.y -= 50;
	schedule(schedule_selector(HelixBullet::Helix));

	{
		ActionInterval* movedown = MoveBy::create(2.0,ccp(0,-250));
		ActionInterval* repeat = RepeatForever::create(movedown);
		//runAction(repeat);
	}
	m_randNun = random(0,1) - 0.5;
	m_randNun /= 5;
}

void HelixBullet::Helix( float dt )
{
	//得到当前飞行速度，格式化
	//Vec2 curV = _physicsBody->getVelocity();

	////找到速度向量的法线，朝着该方向给一个力(绕着一个点慢慢扩散)
	//Vec2 faxian = curV.getPerp();
	//faxian.normalize();
	//_physicsBody->applyImpulse(faxian * 0.1,Vec2(0,0));
	auto nextPos = ccpRotateByAngle(m_beginPos, m_centerPos,0.12); //弧度
	setPosition(nextPos);
	m_beginPos = nextPos;
	m_centerPos.y -= 3.0;
}

void HelixBullet::initBullet()
{
	//属性初始化
	m_SelfPro.maxHP = 1;
	m_SelfPro.curHP = m_SelfPro.maxHP;

	m_ContactPro.hurts = - 1000;
}


//----------------------
HelixBulletEx* HelixBulletEx::create( const std::string& frameName )
{
	HelixBulletEx* bullet = new (std::nothrow) HelixBulletEx;
	if (bullet && bullet->initWithSpriteFrameName(frameName))
	{
		bullet->autorelease();
		bullet->initBody();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void HelixBulletEx::onEnter()
{
	HelixBullet::onEnter();

	float helixTime = 1.0f;
	{
		auto delay = DelayTime::create(helixTime);
		auto func = CallFunc::create(this,callfunc_selector(HelixBulletEx::StopHelix));
		auto seq = Sequence::create(delay, func, nullptr);
		runAction(seq);
	}
}

void HelixBulletEx::StopHelix()
{
	unschedule(schedule_selector(HelixBulletEx::Helix));
	MoveToPl();
}

void HelixBulletEx::MoveToPl()
{
	stopAllActions();
	_physicsBody->setLinearDamping(0.0);
	auto pl = GameData::getInstance()->getMainPlane();
	Vec2 pos;
	if (pl)
	{
		pos = pl->convertToWorldSpaceAR(Vec2::ZERO);
	}
	else
	{
		pos = Vec2(360,-10);
	}

	auto mypos = convertToWorldSpaceAR(Vec2::ZERO);
	auto newVec = pos - mypos;
	newVec.normalize();
	_physicsBody->setVelocity(newVec * 700);
}

void HelixBulletEx::initBody()
{
	Size size = getContentSize();

	auto body = PhysicsBody::createBox(size);
	body->setGravityEnable(false);
	body->setLinearDamping(0.8);
	body->setVelocity(Vec2(0,-10));
	body->setMass(0.008);
	setPhysicsBody(body);
	setBitmask(
		ENEMY_AMMO_CATEGORYBITMASK,
		ENEMY_AMMO_COLLISIONBITMASK,
		ENEMY_AMMO_CONTACTTESTBITMASK);
}



//-------------------------
FallingBulletEX* FallingBulletEX::create( const std::string& frameName )
{
	FallingBulletEX* bullet = new (std::nothrow) FallingBulletEX;
	if (bullet && bullet->initWithSpriteFrameName(frameName))
	{
		bullet->autorelease();
		bullet->initBody();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void FallingBulletEX::initBody()
{
	Bullet::initBody();

	setBitmask(
		ENEMY_AMMO_CATEGORYBITMASK,
		ENEMY_AMMO_COLLISIONBITMASK,
		ENEMY_AMMO_CONTACTTESTBITMASK);
	_physicsBody->setLinearDamping(0.5);
	_physicsBody->setMass(0.008);

}

void FallingBulletEX::onEnter()
{
	Bullet::onEnter();

	QuickShoot();
}

void FallingBulletEX::newFalling( float dt )
{
	_physicsBody->applyForce(Vec2(rand_minus1_1()*0.3,-rand_0_1()),Vec2(0,0));
}

void FallingBulletEX::QuickShoot()
{
	//_physicsBody->setVelocity(Vec2(0, -300));
	ActionInterval* delay = DelayTime::create(0.2f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(FallingBulletEX::shootCallback));
	ActionInterval* seq = Sequence::create(delay,func,nullptr);
	runAction(seq);
}

void FallingBulletEX::shootCallback()
{
	schedule(schedule_selector(FallingBullet::newFalling),0.1);
}



//---------------------------------
DartBullet* DartBullet::create( const std::string& frameName )
{
	DartBullet* db = new (std::nothrow) DartBullet;
	if (db && db->initWithSpriteFrameName(frameName))
	{
		db->autorelease();
		db->initBody();
		db->initBullet();
		return db;
	}
	else
	{
		CC_SAFE_DELETE(db);
		return nullptr;
	}
}

void DartBullet::onEnter()
{
	Bullet::onEnter();
	Size s = getContentSize();
	setInvincible(true); 
	PlayAction();
	addParticle();
	setInvincibleAtLine();//检查过，后面有删除处理

	NewAudio::getInstance()->playEff(Music_Feibiao);
}

void DartBullet::PlayAction()
{
	//自身旋转
	ActionInterval* rotate = RotateBy::create(1.0f,-2000);
	ActionInterval* rep = RepeatForever::create(rotate);
	runAction(rep);

	//移动到屏幕顶端
	Vec2 endPos = Vec2(m_WinSize.width* 0.5, m_WinSize.height - 100);
	ActionInterval* mv = MoveTo::create(2.0f,endPos);
	ActionInterval* ease = EaseExponentialOut::create(mv);
	runAction(ease);

	ActionInterval* delay = DelayTime::create(1.8f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(DartBullet::addForce));
	ActionInterval* seq = Sequence::create(delay,func,nullptr);
	runAction(seq);
}

void DartBullet::addParticle()
{
	auto size = getContentSize();
	auto pos = Vec2(size.width * 0.5, size.height *0.5);
	EffectsManager::addParticle("particla/Huixuanbiao/Huixuanbiao.plist",this,pos,false);
}

void DartBullet::addForce()
{
	Vec2 mypos = convertToWorldSpaceAR(Vec2::ZERO);
	Vec2 amiPos = Vec2(360,m_WinSize.height*0.15);
	auto mainpl = GameData::getInstance()->getMainPlane();
	if (mainpl)
	{
		amiPos = mainpl->convertToWorldSpaceAR(Vec2::ZERO);
	}

	Vec2 newVec = amiPos - mypos;
	newVec.normalize();
	_physicsBody->applyForce(newVec * 0.3);

	ActionInterval* delay = DelayTime::create(1.5f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(PhysicsUnit::setDirectDeath));
	ActionInterval* seq = Sequence::create(delay,func,nullptr);
	runAction(seq);
}

void DartBullet::initBullet()
{
	setBitmask(
		SELF_AMMO_CATEGORYBITMASK,
		SELF_AMMO_COLLISIONBITMASK_P,
		SELF_AMMO_CONTACTTESTBITMASK);
	_physicsBody->setGravityEnable(false);

	//属性初始化
	setInvincible(true);
	m_ContactPro.hurts = -1000;
}

//-----------------------------------
LaserBullet* LaserBullet::create(int lvl)
{
	LaserBullet* db = new (std::nothrow) LaserBullet;
	if (db && db->init())
	{
		db->m_level = lvl;
		db->autorelease();
		db->initBody();
		db->initBullet();
		return db;
	}
	else
	{
		CC_SAFE_DELETE(db);
		return nullptr;
	}
}

void LaserBullet::initBody()
{
	auto body = PhysicsBody::createBox(Size(25,2000),PHYSICSBODY_MATERIAL_DEFAULT, Vec2(0,1000));
	body->setGravityEnable(false);
	body->setMass(0.0001);
	setPhysicsBody(body);
	setBitmask(
		SELF_AMMO_CATEGORYBITMASK,
		SELF_AMMO_COLLISIONBITMASK_P,
		SELF_AMMO_CONTACTTESTBITMASK);

	schedule(schedule_selector(LaserBullet::setBody),0.1);
}

void LaserBullet::initBullet()
{
	setInvincible(true);
	setInvincibleAtLine(true);//检查过，后面有删除处理
}

void LaserBullet::onEnter()
{
	Bullet::onEnter();
	NewAudio::getInstance()->playEff(Music_Laser);
	ListeningStopWeapon();

	addShootPoint();
	addExtraLight();
}

void LaserBullet::onExit()
{
	Bullet::onExit();
}

void LaserBullet::removeLaser()
{
	setDirectDeath();
}

void LaserBullet::removeLaserO( Ref* obj )
{
	setDirectDeath();
}

void LaserBullet::setBody(float dt)
{
	auto body = PhysicsBody::createBox(Size(25,2000),PHYSICSBODY_MATERIAL_DEFAULT, Vec2(0,1000));
	body->setGravityEnable(false);
	body->setMass(0.0001);
	setPhysicsBody(body);
	setBitmask(
		SELF_AMMO_CATEGORYBITMASK,
		SELF_AMMO_COLLISIONBITMASK_P,
		SELF_AMMO_CONTACTTESTBITMASK);
}

void LaserBullet::addExtraLight()
{
	if (m_level == 3)
	{
		auto sp = Sprite::create("weapon.png");
		sp->setPosition(Vec2::ZERO);
		sp->setAnchorPoint(Vec2(0.5,0));
		addChild(sp);
		sp->setScaleX(0.01);
		sp->setScaleY(2.0);
		auto action = getCommonAction(0.8, 3.0);
		sp->runAction(action);

		auto layer = UIController::getInstance()->getEffectsLayer();
		auto animat = layer->getFramesAnimation("Laser3_", 8, 1.0/24, 0, REPEAT_FORVER, true);
		sp->runAction(animat);

		addExtraCircle();
	}
}

void LaserBullet::addExtraCircle()
{
	auto delay = DelayTime::create(0.1 + rand_0_1() * 0.1);
	auto next = CallFunc::create(this, callfunc_selector(LaserBullet::ExtraCircle));
	auto seq = Sequence::create(delay, next, nullptr);
	runAction(seq);
}

void LaserBullet::ExtraCircle()
{
	auto sp = Sprite::create("animation/Laser3/quan.png");
	addChild(sp);
	auto curScale = m_ShootSp->getScaleX();
	sp->setScale(0.25 * curScale);

	{
		auto mv = MoveBy::create(0.7 + rand_0_1() * 0.1,Vec2(0, 1500));
		auto ease = EaseSineIn::create(mv);
		auto rm = RemoveSelf::create(true);
		auto seq = Sequence::create(ease, rm, nullptr);
		sp->runAction(seq);
	}

	addExtraCircle();
}

void LaserBullet::addLaserCore()
{
	auto laerCore = Sprite::create("ammo/laser/laser.png");
	laerCore->setAnchorPoint(Vec2(0.5f,0));
	laerCore->setPosition(Vec2(0, m_ShootSp->getContentSize().height - 25) + getLaserCoreOffset());
	laerCore->setScaleX(0.05);
	laerCore->setScaleY(m_scaleY);
	addChild(laerCore,-1);

	auto action = getCommonAction(m_scaleX, m_scaleY);
	laerCore->runAction(action);
}

void LaserBullet::addShootPoint()
{
	//发射点
	m_ShootSp = Sprite::create("ammo/laser/ShootPos.png");
	m_ShootSp->setPosition(Vec2(0,-25) + getShootPointOffset());
	m_ShootSp->setAnchorPoint(Vec2(0.5f,0.5f));
	m_ShootSp->setScaleX(0.05f);
	m_ShootSp->setScaleY(m_scaleX);
	addChild(m_ShootSp);
	{
		auto action1 = getCommonAction(m_scaleX, m_scaleX);
		auto rm = CallFunc::create(this, callfunc_selector(LaserBullet::removeLaser));
		auto seq = Sequence::create(action1, rm, nullptr);
		m_ShootSp->runAction(seq);
	}
	
	{
		//激光芯横向轻微浮动
		auto mv = MoveBy::create(0.1, Vec2(2,0));
		auto mvBack = MoveBy::create(0.1, Vec2(-2,0));
		auto seq = Sequence::create(mv, mvBack, nullptr);
		auto rep = RepeatForever::create(seq);
		runAction(rep);
	}

	addLaserCore();
}

ActionInterval* LaserBullet::getCommonAction(float scaleX, float scaleY)
{
	//渐渐变宽
	auto delay = DelayTime::create(m_timeIn);
	auto scaleto = ScaleTo::create(m_TimeToScale, scaleX, scaleY);
	auto scaleback = ScaleTo::create(m_TimeToBack, 0.05, scaleY);
	auto fadeOut = FadeOut::create(m_TimeToBack);
	auto stay = DelayTime::create(m_dur);
	auto spwOut = Spawn::create(scaleback, fadeOut, nullptr);
	auto seq = Sequence::create(delay, scaleto, stay, spwOut, nullptr);
	return seq;
}

cocos2d::Vec2 LaserBullet::getLaserCoreOffset()
{
	return Vec2(0,0);
}

cocos2d::Vec2 LaserBullet::getShootPointOffset()
{
	return Vec2(0,0);
}

//-------------------------------
OutsideLaser::OutsideLaser()
{
	m_scaleX = 1.0f;
}

OutsideLaser* OutsideLaser::create(const Vec2& pos,int zorder)
{
	OutsideLaser* db = new (std::nothrow) OutsideLaser;
	if (db && db->init())
	{
		db->autorelease();
		db->m_pos = pos;
		db->m_zorder = zorder;
		db->initBody();
		db->initBullet();
		return db;
	}
	else
	{
		CC_SAFE_DELETE(db);
		return nullptr;
	}
}

void OutsideLaser::MoveCircle()
{
	{
		ActionInterval* moveBy = MoveBy::create(0.6f,Vec2(-m_pos.x * 2,0));
		ActionInterval* ease = EaseCircleActionInOut::create(moveBy);
		ActionInterval* back = ease->reverse();
		ActionInterval* seq = Sequence::create(ease,back,nullptr);
		ActionInterval* rep = RepeatForever::create(seq);
		this->runAction(rep);
	}
	
	//schedule(schedule_selector(OutsideLaser::refreshPos));
}

void OutsideLaser::onEnter()
{
	LaserBullet::onEnter();
	addDipan();
	MoveCircle();

	schedule(schedule_selector(LaserBullet::setBody),0.1f);
}

void OutsideLaser::addDipan()
{
	auto sp = Sprite::create("ammo/laser/dipan.png");
	sp->setPosition(0,0);
	sp->setScale(1.5f);
	sp->setOpacity(0);
	addChild(sp,-1);

	auto fadein = FadeIn::create(0.1);
	auto delay = DelayTime::create(m_timeIn + m_dur + m_TimeToScale - 0.1);
	auto fadeOut = FadeOut::create(m_TimeToBack);
	auto seq = Sequence::create(fadein, delay, fadeOut, nullptr);
	sp->runAction(seq);
}

cocos2d::Vec2 OutsideLaser::getLaserCoreOffset()
{
	return Vec2(0,-58);
}

cocos2d::Vec2 OutsideLaser::getShootPointOffset()
{
	return Vec2(0,15);
}



//-----------------------------
CatBullet* CatBullet::create( bool isleft, CatType ct )
{
	CatBullet* cat = new (std::nothrow) CatBullet;
	if (cat && cat->init())
	{
		cat->autorelease();
		cat->initBody();
		cat->initBullet();
		cat->m_catType = ct;
		cat->m_direct = 1;
		if (isleft)
		{
			cat->m_direct = -1;
		}
		return cat;
	}
	else
	{
		CC_SAFE_DELETE(cat);
		return nullptr;
	}
}

void CatBullet::onEnter()
{
	Bullet::onEnter();
	setInvincible(true);
	initCat();
	FirstJump();
	setScale(0.7);
	NewAudio::getInstance()->playEff(Music_Cat);
	CatLeave();
}

void CatBullet::NextPos()
{
	Vec2 mypos = convertToWorldSpaceAR(Vec2::ZERO);
	float dis = 150 + rand_0_1() * 50 + m_ExDis;

	auto targetPos = GameData::getInstance()->getNearTargetByPos(mypos);
	bool isIn = isTargetInScreen(targetPos);

	//是否有目标，有则朝向目标点跳跃，无则随机跳跃，计算出落脚点
	if (!isIn || m_timeToLeave)
	{
		m_JumpVec = Vec2(dis * m_direct, dis);
	}
	else
	{
		m_JumpVec = targetPos - mypos;
		m_JumpVec.normalize();
		m_JumpVec *= dis;
	}
	FixJumpPos();

	m_NextRunPos = mypos + m_JumpVec;
}

void CatBullet::TurnDirect()
{
	//横向转向
	if (m_JumpVec.x > 0) 
	{
		//转向
		m_catStay->setFlipX(true);
	}
	else
	{
		//不转向
		m_catStay->setFlipX(false);
	}

	//1象限
	if (m_JumpVec.y > 0 && m_JumpVec.x > 0)
	{
		m_catJump->setFlipY(false);
	}
	else if(m_JumpVec.y > 0 && m_JumpVec.x <= 0)
	{
		//2象限
		m_catJump->setFlipY(true);
	}
	else if(m_JumpVec.y <= 0 && m_JumpVec.x <= 0)
	{
		//3象限
		m_catJump->setFlipY(true);
	}
	else
	{
		m_catJump->setFlipY(false);
	}
	m_catJump->setFlipX(true);

	if (m_direct < 0)
	{
		m_direct = 1;
	}
	else
	{
		m_direct = -1;
	}
}

void CatBullet::AdjustCatAngle()
{
	float angle = m_JumpVec.getAngle() * Pi2Angle;

	m_catJump->setRotation(-angle);
}

void CatBullet::CatRun()
{
	NextPos();
	TurnDirect();
	AdjustCatAngle();
	JumpState();
	ActionInterval* move = MoveTo::create(0.15f + rand_0_1() * 0.03f,m_NextRunPos);
	CallFunc* stay = CallFunc::create(this,callfunc_selector(CatBullet::StayState));
	ActionInterval* delay = DelayTime::create(0.05f + rand_0_1() * 0.2f);
	CallFunc* next = CallFunc::create(this,callfunc_selector(CatBullet::CatRun));
	ActionInterval* seq = Sequence::create(move,stay,delay,next,nullptr);
	runAction(seq);
}

void CatBullet::initBullet()
{
	setBitmask(
		SELF_AMMO_CATEGORYBITMASK,
		SELF_AMMO_COLLISIONBITMASK_P,
		SELF_AMMO_CONTACTTESTBITMASK);
	_physicsBody->setGravityEnable(false);
}

void CatBullet::initBody()
{
	auto body = PhysicsBody::createBox(Size(100,50));
	body->setGravityEnable(false);
	body->setMass(0.0001);
	setPhysicsBody(body);
}

void CatBullet::addClawL()
{
	CatClaw* claw = CatClaw::create(m_JumpVec);
	claw->setPosition(m_contactPos);
	UIController::getInstance()->getPhysicsLayer()->addChild(claw);
}

void CatBullet::addClawR()
{
	CatClaw* claw = CatClaw::create(m_JumpVec);
	claw->setPosition(m_contactPos);
	UIController::getInstance()->getPhysicsLayer()->addChild(claw);
}

void CatBullet::OtherDealWhenContact( const ContactPro& rs )
{
	m_contactPos = rs.pos;
	if (rs.idx == UnitId::ePlaneBoss)
	{
		m_ExDis = 150.0f;
	}
	else
	{
		m_ExDis = 0.0f;
	}

	//addClawL();	//不能旋转

	//他妈的一定要延迟创建，爪子才能旋转
	ActionInterval* delay = DelayTime::create(0.05);
	CallFunc* func = CallFunc::create(this,callfunc_selector(CatBullet::addClawR));
	ActionInterval* seq = Sequence::create(delay,func,nullptr);
	runAction(seq);

}

void CatBullet::JumpState()
{
	m_catJump->setVisible(true);
	m_catStay->setVisible(false);
}

void CatBullet::StayState()
{
	m_catJump->setVisible(false);
	m_catStay->setVisible(true);
}

void CatBullet::initCat()
{
	std::string Stayres;
	std::string Jumpres;
	switch (m_catType)
	{
	case CatBullet::cat:
		{
			Stayres = "Effect/cat/catStay_b.png";
			Jumpres = "Effect/cat/catJump_b.png";
			if (m_direct < 0)
			{
				Stayres = "Effect/cat/catStay_w.png";
				Jumpres = "Effect/cat/catJump_w.png";
			}
		}
		break;
	case CatBullet::wolf:
		{
			Stayres = "Effect/cat/wolfStay_b.png";
			Jumpres = "Effect/cat/wolfJump_b.png";
			if (m_direct < 0)
			{
				Stayres = "Effect/cat/wolfStay_w.png";
				Jumpres = "Effect/cat/wolfJump_w.png";
			}
		}
		break;
	case CatBullet::tiger:
		{
			Stayres = "Effect/cat/tigerStay_b.png";
			Jumpres = "Effect/cat/tigerJump_b.png";
			if (m_direct < 0)
			{
				Stayres = "Effect/cat/tigerStay_w.png";
				Jumpres = "Effect/cat/tigerJump_w.png";
			}
		}
		break;
	default:
		break;
	}
	
	auto s = getContentSize();
	m_catStay = Sprite::create(Stayres);
	m_catStay->setPosition(s.width * 0.5,s.height*0.5);
	addChild(m_catStay);

	m_catJump = Sprite::create(Jumpres);
	m_catJump->setPosition(s.width * 0.5,s.height*0.5);
	addChild(m_catJump);
}

bool CatBullet::isTargetInScreen( const Vec2& pos )
{
	if (pos.x > m_WinSize.width - 50 || pos.x < 50  )
	{
		return false;
	}
	if (pos.y > m_WinSize.height - 50 || pos.y < 50)
	{
		return false;
	}
	return true;
}

void CatBullet::FirstJump()
{
	Vec2 mypos = convertToWorldSpaceAR(Vec2::ZERO);
	float dis = 150 + rand_0_1() * 50 + m_ExDis;

	m_JumpVec = Vec2(dis * m_direct, dis);

	m_NextRunPos = mypos + m_JumpVec;
	TurnDirect();
	AdjustCatAngle();
	JumpState();

	ActionInterval* move = MoveTo::create(0.2f + rand_0_1() * 0.03f,m_NextRunPos);
	ActionInterval* ease = EaseExponentialOut::create(move);
	CallFunc* stay = CallFunc::create(this,callfunc_selector(CatBullet::StayState));
	ActionInterval* delay = DelayTime::create(0.05f + rand_0_1() * 0.2f);
	CallFunc* next = CallFunc::create(this,callfunc_selector(CatBullet::CatRun));
	ActionInterval* seq = Sequence::create(ease,stay,delay,next,nullptr);
	runAction(seq);
}

void CatBullet::CatLeave()
{
	ActionInterval* delay = DelayTime::create(5.0f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(CatBullet::LeaveCB));
	ActionInterval* seq = Sequence::create(delay, func, nullptr);
	runAction(seq);
}

void CatBullet::LeaveCB()
{
	m_timeToLeave = true;
}

void CatBullet::FixJumpPos()
{
	auto Rpos = Vec2(0,0);	//旋转基点
	float angle = m_JumpVec.getAngle();
	if (angle > PI/4 && angle < PI/2)
	{
		m_JumpVec.rotate(Rpos,PI/4 - angle );
	}
	else if(angle >= PI/2 && angle < PI * 3/4)
	{
		m_JumpVec.rotate(Rpos,PI * 3/4 - angle );
	}
	else if(angle >= PI * 5/4 && angle < PI * 6/4)
	{
		m_JumpVec.rotate(Rpos,PI * 5/4 - angle );
	}
	else if(angle >= PI * 6/4 && angle < PI * 7/4)
	{
		m_JumpVec.rotate(Rpos,PI * 7/4 - angle );
	}

}

//----------------------
CatClaw* CatClaw::create( const Vec2& direc )
{
	CatClaw* claw = new (std::nothrow) CatClaw;
	if (claw && claw->initWithFile("Effect/cat/claw.png"))
	{
		claw->m_MvDirect = direc;
		claw->autorelease();
		claw->initBody();
		return claw;
	}
	else
	{
		CC_SAFE_DELETE(claw);
		return nullptr;
	}
}

void CatClaw::initBody()
{
	auto body = PhysicsBody::createBox(getContentSize());
	body->setGravityEnable(false);
	body->setMass(0.001);
	setPhysicsBody(body);
	setBitmask(
		SELF_AMMO_CATEGORYBITMASK,
		SELF_AMMO_COLLISIONBITMASK_P,
		SELF_AMMO_CONTACTTESTBITMASK);
	setInvincible(true);
}

void CatClaw::onEnter()
{
	Bullet::onEnter();
	
	float r = m_MvDirect.getAngle() * Pi2Angle;
	setRotation(-r);	//他妈的竟然无法旋转

	//runAction(RotateBy::create(0.01,-r));

	setScale(3);
	Catch();
}

void CatClaw::Catch()
{
	float mvDis = 100 + rand_0_1() * 20;
	m_MvDirect.normalize();
	m_MvDirect *= mvDis;

	ActionInterval* scale = ScaleTo::create(0.1f,1.0f,1.0f);
	ActionInterval* move = MoveBy::create(0.25f,m_MvDirect);
	ActionInterval* fadein = FadeOut::create(0.25f);
	ActionInterval* spaw = Spawn::create(scale,move,fadein,nullptr);

	CallFunc* func = CallFunc::create(this,callfunc_selector(PhysicsUnit::setDirectDeath));
	ActionInterval* seq = Sequence::create(spaw,func,nullptr);

	runAction(seq);
}


//----------------------------------------
BossDart* BossDart::create(const std::string& frameName)
{
	BossDart* claw = new (std::nothrow) BossDart;
	if (claw && claw->initWithSpriteFrameName(frameName))
	{
		claw->autorelease();
		claw->initBody();
		claw->initBullet();
		return claw;
	}
	else
	{
		CC_SAFE_DELETE(claw);
		return nullptr;
	}
}

void BossDart::initBody()
{
	//圆球
	auto body = PhysicsBody::createBox( getContentSize());
	body->setGravityEnable(true);
	body->setRotationEnable(false);
	setPhysicsBody(body);
	setBitmask(
		ENEMY_AMMO_CATEGORYBITMASK,
		ENEMY_AMMO_COLLISIONBITMASK,
		ENEMY_AMMO_CONTACTTESTBITMASK);

	addParticle();
}

void BossDart::initBullet()
{
	m_SelfPro.curHP = 1;
	m_SelfPro.maxHP= 1;
	m_ContactPro.hurts = Ene_Blt_hurt;
}

void BossDart::addParticle()
{
	auto pos = Vec2(getContentSize().width*0.5,getContentSize().height*0.5);
	EffectsManager::addParticle("particla/Boss1ammo/Boss1ammo.plist",this,pos,false);
}


//---------------------------
FlowerBullet* FlowerBullet::create()
{
	FlowerBullet* flower = new (std::nothrow) FlowerBullet;
	if (flower && flower->init())
	{
		flower->initBody();
		flower->initFLower();
		flower->autorelease();
		return flower;
	}
	else
	{
		CC_SAFE_DELETE(flower);
		return nullptr;
	}
}

void FlowerBullet::initBody()
{
	auto body = PhysicsBody::createCircle(m_radius);;
	body->setGravityEnable(false);
	setPhysicsBody(body);
	setBitmask(
		SELF_AMMO_CATEGORYBITMASK,
		SELF_AMMO_COLLISIONBITMASK_P,
		SELF_AMMO_CONTACTTESTBITMASK);
	setInvincible(true);

	m_ContactPro.hurts = -700;
}

void FlowerBullet::onEnter()
{
	Bullet::onEnter();
	schedule(schedule_selector(FlowerBullet::setBody),0.1f);
	start();

	NewAudio::getInstance()->playEff(Music_Flower);
}

void FlowerBullet::setBody(float dt)
{
	auto body = PhysicsBody::createCircle(m_radius);;
	body->setGravityEnable(false);
	setPhysicsBody(body);
	setBitmask(
		SELF_AMMO_CATEGORYBITMASK,
		SELF_AMMO_COLLISIONBITMASK_P,
		SELF_AMMO_CONTACTTESTBITMASK);
}

void FlowerBullet::initFLower()
{
	m_Flower = Sprite::create("particla/Petals/flower.png");
	addChild(m_Flower,-1);

	m_eff1 = Sprite::create("particla/Petals/flowerEff1.png");
	addChild(m_eff1,-1);
	m_eff1->setVisible(false);

	m_eff2 = Sprite::create("particla/Petals/flowerEff2.png");
	addChild(m_eff2,-1);
	m_eff2->setVisible(false);
}

void FlowerBullet::start()
{
	Vec2 pos = convertToWorldSpaceAR(Vec2::ZERO);

	ActionInterval* mv = MoveTo::create(1.0f,Vec2(pos.x, m_WinSize.height * 0.8f));
	runAction(mv);

	ActionInterval* delay = DelayTime::create(0.5f);
	CallFunc* next = CallFunc::create(this,callfunc_selector(FlowerBullet::FLowerAction));
	ActionInterval* seq = Sequence::create(delay, next, nullptr);
	runAction(seq);

	DropFlower();
}

void FlowerBullet::FLowerAction()
{
	{
		ActionInterval* rotate = RotateBy::create(2.0f,-1000);
		ActionInterval* ease = EaseCubicActionIn::create(rotate);
		m_Flower->runAction(ease);
	}

	{
		ActionInterval* scale = ScaleBy::create(2.0f,4.5);
		m_Flower->runAction(scale);
	}

	{
		ActionInterval* delay = DelayTime::create(2.0f);
		CallFunc* next = CallFunc::create(this,callfunc_selector(FlowerBullet::Eff1Action));
		ActionInterval* seq = Sequence::create(delay, next, nullptr);
		m_Flower->runAction(seq);
	}
	
	{
		ActionInterval* delay = DelayTime::create(1.8f);
		ActionInterval* fade = FadeOut::create(0.8f);
		ActionInterval* seq = Sequence::create(delay,fade,nullptr);
		m_Flower->runAction(seq);
	}
}

void FlowerBullet::Eff1Action()
{
	m_eff1->setVisible(true);
	m_eff1->setScale(5.0);

	//永远旋转 + 渐现
	{
		ActionInterval* rotate = RotateBy::create(1.0f,-1000);
		ActionInterval* rep = RepeatForever::create(rotate);
		m_eff1->runAction(rep);

		ActionInterval* fadein = FadeIn::create(0.5f);
		m_eff1->runAction(fadein);
	}

	//持续
	ActionInterval* delay = DelayTime::create(0.8f);

	//放大 + 出现下个特效
	{
		ActionInterval* scale = ScaleTo::create(0.05f,5.0);
		CallFunc* next = CallFunc::create(this,callfunc_selector(FlowerBullet::Eff2Action));
		ActionInterval* spw = Spawn::create(scale, next, nullptr);
		ActionInterval* seq = Sequence::create(delay, spw, nullptr);
		m_eff1->runAction(seq);
	}

	//爆炸
	{
		ActionInterval* delay = DelayTime::create(0.8);
		CallFunc* next = CallFunc::create(this,callfunc_selector(FlowerBullet::Explosion));
		ActionInterval* seq = Sequence::create(delay, next, nullptr);
		runAction(seq);
		//Explosion();
	}
}

void FlowerBullet::Eff2Action()
{
	m_eff2->setVisible(true);
	m_eff2->setScale(0.1);
	//EFF1渐隐
	{
		ActionInterval* delay = DelayTime::create(0.4f);
		ActionInterval* fadeout = FadeOut::create(0.2f);
		ActionInterval* seq = Sequence::create(delay,fadeout, nullptr);
		m_eff1->runAction(seq);
	}

	// EFF2渐现 + EFF2放大 + EFF2旋转
	{
		ActionInterval* fadein = FadeIn::create(0.01);
		ActionInterval* rotate = RotateBy::create(0.8f,-100);
		ActionInterval* scale = ScaleTo::create(0.01f, 5.0);
		ActionInterval* spw = Spawn::create(fadein, rotate, scale, nullptr);
		
		m_eff2->runAction(spw);
	}

	//EFF2渐隐
	{
		ActionInterval* scale = ScaleTo::create(0.01f,5.0);
		ActionInterval* fadeout = FadeOut::create(0.7);
		ActionInterval* spw = Spawn::create(scale, fadeout, nullptr);
		m_eff2->runAction(fadeout);
	}

	
}

void FlowerBullet::Explosion()
{
	unschedule(schedule_selector(FlowerBullet::setBody));
	
	auto body = PhysicsBody::createCircle(150);
	body->setGravityEnable(false);
	setPhysicsBody(body);
	setBitmask(
		SELF_AMMO_CATEGORYBITMASK,
		SELF_AMMO_COLLISIONBITMASK_P,
		SELF_AMMO_CONTACTTESTBITMASK);
	setInvincible(true);
	{
		//爆炸粒子
		Vec2 pos = convertToWorldSpaceAR(Vec2::ZERO);
		auto layer = UIController::getInstance()->getPhysicsLayer();
		EffectsManager::addParticle("particla/Petals/Petals3.plist", layer, pos, true, 5.0f);
	}
	{
		ActionInterval* delay = DelayTime::create(1.0f);
		CallFunc* rm = CallFunc::create(this,callfunc_selector(PhysicsUnit::setDirectDeath));
		ActionInterval* seq = Sequence::create(delay, rm, nullptr);
		runAction(seq);
		runAction(ScaleBy::create(0.2f,1.3));
	}
}

void FlowerBullet::DropFlower()
{
	EffectsManager::addParticle("particla/Petals/Petals1.plist",this,Vec2(0,0),false);
	EffectsManager::addParticle("particla/Petals/Petals2.plist",this,Vec2(0,0),false);
}

//--------------------------------
FireDragon* FireDragon::create(Flaming::FireType ft)
{
	FireDragon* fd = new (std::nothrow) FireDragon;
	if (fd && fd->init())
	{
		fd->m_fireType = ft;
		fd->addArmature();
		fd->initBody();
		fd->autorelease();
		return fd;
	}
	else
	{
		CC_SAFE_DELETE(fd);
		return nullptr;
	}
}

void FireDragon::initBody()
{
	auto body = PhysicsBody::createBox(getContentSize());
	body->setGravityEnable(false);
	setPhysicsBody(body);
	setBitmask(
		SELF_AMMO_CATEGORYBITMASK,
		SELF_AMMO_COLLISIONBITMASK_P,
		SELF_AMMO_CONTACTTESTBITMASK);
	setInvincible(true);

	m_ContactPro.hurts = -700;
}

void FireDragon::onEnter()
{
	Bullet::onEnter();
	MoveUp();

	NewAudio::getInstance()->playEff(Music_Dragon);
}

void FireDragon::MoveUp()
{
	auto pos = convertToWorldSpaceAR(Vec2::ZERO);

	{
		ActionInterval* mv = MoveTo::create(6,Vec2(pos.x, getEdgeLine(3) + 50));
		ActionInterval* ease = CCEaseSineIn::create(mv);
		runAction(ease);
	}

	ShootFire();
}

void FireDragon::ShootFire()
{
	auto s = getContentSize();

	auto fire = Flaming::create(m_fireType);
	fire->setPosition(s.width * 0.5, s.height);

	auto pro = fire->getContactPro();
	pro.hurts = m_ContactPro.hurts;
	fire->setContactPro(pro);

	addChild(fire);
}

void FireDragon::addArmature()
{
	std::string name;
	
	switch (m_fireType)
	{
	case Flaming::eSmall:
		name = "xiaolong";
		break;
	case Flaming::eMiddle:
		name = "zhonglong";
		break;
	case Flaming::eBig:
		name = "julong";
		break;
	default:
		break;
	}

	auto Armat = Armature::create(name);
	auto s = Armat->getContentSize();
	Armat->setAnchorPoint(Vec2(0.5,0.5));
	Armat->setPosition(s.width*0.5,s.height*0.5);
	addChild(Armat);
	setContentSize(s);
	Armat->getAnimation()->playWithIndex(0);
}



//------------------------
Flaming* Flaming::create(FireType ft)
{
	Flaming* fire = new (std::nothrow) Flaming;
	if (fire && fire->init())
	{
		fire->autorelease();
		fire->m_fireType = ft;
		fire->initBody();
		return fire;
	}
	else
	{
		CC_SAFE_DELETE(fire);
		return nullptr;
	}
}

void Flaming::initBody()
{
	setInvincible(true);
	Size sizeOffset = Size(0,0);
	switch (m_fireType)
	{
	case Flaming::eSmall:
		{

		}
		break;
	case Flaming::eMiddle:
		{
			sizeOffset = Size(20,100);
		}
		break;
	case Flaming::eBig:
		{
			sizeOffset = Size(20,200);
		}
		break;
	default:
		break;
	}

	Vec2 vec[3]=
	{
		Vec2(0,0),
		Vec2(-100 - sizeOffset.width, 250 + sizeOffset.height),
		Vec2(100 + sizeOffset.width, 250 + sizeOffset.height)
	};
	m_FireSize = Size(100 + sizeOffset.width, 250 + sizeOffset.height);

	auto body = PhysicsBody::createPolygon(vec,3,PHYSICSBODY_MATERIAL_DEFAULT,Vec2(0,0));
	body->setGravityEnable(false);
	setPhysicsBody(body);
	_physicsBody->setCategoryBitmask(SELF_AMMO_CATEGORYBITMASK);
	_physicsBody->setCollisionBitmask(SELF_AMMO_COLLISIONBITMASK_P);
	_physicsBody->setContactTestBitmask(SELF_AMMO_CONTACTTESTBITMASK);
	
}

void Flaming::setBody( float dt )
{
	initBody();
}

void Flaming::onEnter()
{
	PhysicsUnit::onEnter();
	schedule(schedule_selector(Flaming::setBody), 0.1);

	std::string particleFile = "particla/FireDragon/FireDragon.plist";
	switch (m_fireType)
	{
	case Flaming::eSmall:
		break;
	case Flaming::eMiddle:
		{
			particleFile = "particla/FireDragon/FireDragonMid.plist";
		}
		break;
	case Flaming::eBig:
		{
			particleFile = "particla/FireDragon/FireDragonBig.plist";
		}
		break;
	default:
		break;
	}

	m_particle = EffectsManager::addParticle(particleFile,this,Vec2(0,0),false);
	if (m_particle)
	{
		m_particle->setPositionType(ParticleSystem::PositionType::GROUPED);
	}

	if (m_fireType == eBig)
	{
		{
			auto next = CallFunc::create(this,callfunc_selector(Flaming::DelayShowSparks));
			auto delay = DelayTime::create(0.7);
			auto seq = Sequence::create(delay, next, nullptr);
			runAction(seq);
		}
		/*{
			auto next = CallFunc::create(this,callfunc_selector(Flaming::DelayShowFireCircle));
			auto delay = DelayTime::create(0.7);
			auto seq = Sequence::create(delay, next, nullptr);
			runAction(seq);
		}*/
	}
}

void Flaming::Sparks()
{
	//出现位置的范围
	float y = m_FireSize.height * random(0.5f, 1.0f);
	float rate = y/m_FireSize.height;
	float x = rate * m_FireSize.width * 1.2 * rand_minus1_1();
	Vec2 pos = Vec2(x,y);
	
	auto sp = Sprite::create("weapon.png");
	sp->setPosition(pos);
	addChild(sp);
	sp->setRotation(360 * rand_0_1());
	sp->setScale(0.5 + rand_0_1() * 0.3);

	auto layer = UIController::getInstance()->getEffectsLayer();
	auto animat = layer->getFramesAnimation("FireExpl_", 3, 1.0/24);
	auto mv = RemoveSelf::create(true);
	auto seq = Sequence::create(animat, mv, nullptr);
	sp->runAction(seq);
}

void Flaming::FlamCircle()
{
	auto sp = Sprite::create("animation/FireSurround/FireCircle.png");
	sp->setPosition(Vec2(-5,0));
	addChild(sp);
	sp->setScale(1.4);

	//由下到上运动，慢慢变大，删除
	float t = 1.0 + rand_0_1() * 0.2;
	auto mv = MoveBy::create(t, Vec2(0, 150 + rand_0_1() * 20));
	auto ease = EaseSineIn::create(mv);
	auto scale = ScaleTo::create(t, 3.5f + rand_0_1(), 1.0f);
	auto spa = Spawn::create(mv, scale, nullptr);
	auto rm = RemoveSelf::create(true);
	auto fadeout = FadeOut::create(0.5);
	auto seq = Sequence::create(spa, fadeout, rm, nullptr);
	sp->runAction(seq);
}

void Flaming::DelayShowSparks()
{
	auto next = CallFunc::create(this,callfunc_selector(Flaming::Sparks));
	auto interval = DelayTime::create(0.03 + rand_0_1() * 0.02);
	auto seq = Sequence::create(next, interval, nullptr);
	auto rep = RepeatForever::create(seq);

	runAction(rep);
}

void Flaming::DelayShowFireCircle()
{
	auto func = CallFunc::create(this,callfunc_selector(Flaming::FlamCircle));
	auto next = CallFunc::create(this,callfunc_selector(Flaming::DelayShowFireCircle));
	auto interval = DelayTime::create(0.3 + rand_0_1() * 0.1);
	auto seq = Sequence::create(func, interval, next, nullptr);

	runAction(seq);
}

//---------------------------
FireWorks* FireWorks::create(const std::string& frameName)
{
	FireWorks* fire = new (std::nothrow) FireWorks;
	if (fire && fire->initWithSpriteFrameName(frameName))
	{
		fire->autorelease();
		fire->initBody();
		return fire;
	}
	else
	{
		CC_SAFE_DELETE(fire);
		return nullptr;
	}
}

void FireWorks::initBody()
{
	auto body = PhysicsBody::createBox(getContentSize());
	body->setGravityEnable(true);
	body->setLinearDamping(0.8f);
	setPhysicsBody(body);
	setBitmask(
		ENEMY_AMMO_CATEGORYBITMASK,
		ENEMY_AMMO_COLLISIONBITMASK,
		ENEMY_AMMO_CONTACTTESTBITMASK);
}

void FireWorks::onEnter()
{
	Bullet::onEnter();

	ActionInterval* delay = DelayTime::create(0.5f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(FireWorks::resetLinearDamping));
	ActionInterval* seq = Sequence::create(delay, func, nullptr);
	runAction(seq);
}

void FireWorks::resetLinearDamping()
{
	_physicsBody->setLinearDamping(0.0f);
}

//------------------------
EneRebounceBullet* EneRebounceBullet::create( const std::string& frameName )
{
	EneRebounceBullet* rb = new (std::nothrow) EneRebounceBullet;
	if (rb && rb->initWithSpriteFrameName(frameName))
	{
		rb->initBody();
		rb->autorelease();
		return rb;
	}
	else
	{
		CC_SAFE_DELETE(rb);
		return nullptr;
	}
}

void EneRebounceBullet::initBody()
{
	PhysicsMaterial pm(0.0,1.0,0.5);
	auto body = PhysicsBody::createCircle(getContentSize().width*0.5,pm);
	body->setGravityEnable(true);
	body->setLinearDamping(0.8f);
	setPhysicsBody(body);
	setBitmask(
		ENEMY_AMMO2_CATEGORYBITMASK,	
		ENEMY_AMMO2_COLLISIONBITMASK,	
		ENEMY_AMMO2_CONTACTTESTBITMASK);	
}

void EneRebounceBullet::OtherDealWhenContact( const ContactPro& rs )
{
	// 第一次撞击底部进行统计
	if ( rs.idx == UnitId::eBottomWall )
	{
		setDirectDeath();
	}
}

//-----------------------------
BulletToTarget* BulletToTarget::create(const std::string& frameName, bool isSelf /*= true*/)
{
	BulletToTarget* rb = new (std::nothrow) BulletToTarget;
	if (rb && rb->initWithSpriteFrameName(frameName))
	{
		rb->m_isSelf = isSelf;
		rb->initBody();
		rb->autorelease();
		return rb;
	}
	else
	{
		CC_SAFE_DELETE(rb);
		return nullptr;
	}
}

void BulletToTarget::initBody()
{
	auto body = PhysicsBody::createBox(getContentSize());
	body->setGravityEnable(false);
	setPhysicsBody(body);
	setBitmask(
		SELF_AMMO_CATEGORYBITMASK,
		SELF_AMMO_COLLISIONBITMASK_P,
		SELF_AMMO_CONTACTTESTBITMASK);
	m_ContactPro.hurts = Ene_Blt_hurt * 10;
}

void BulletToTarget::FixAngleOfSelf()
{
	auto PlVec = GameData::getInstance()->getEnemyPlaneVec();
	if (PlVec.empty())
	{
		return;
	}
	else
	{
		float wpRot = m_weapon->getRotation();
		if (wpRot < 0)
		{
			//寻找左边的目标
			FindAmiOfSelf(-1);
		}
		else
		{
			//寻找右边的目标
			FindAmiOfSelf(1);
		}
	}
	
}

void BulletToTarget::onEnter()
{
	Bullet::onEnter();
	if (m_isSelf)
	{
		FixAngleOfSelf();
	}
	else
	{
		FixAngleToMainplane();
	}
}

void BulletToTarget::FindAmiOfSelf( int direct )
{
	bool isFound = false;
	Vec2 amiPos = Vec2(360 + direct * 360, 640);
	float tempdis = 100000;
	auto mypos = convertToWorldSpaceAR(Vec2::ZERO);
	auto PlVec = GameData::getInstance()->getEnemyPlaneVec();
	for (auto it = PlVec.begin(); it != PlVec.end(); it++)
	{
		PhysicsUnit* pu = (*it);
		auto pos = pu->convertToWorldSpaceAR(Vec2::ZERO);
		float dis = pos.distance(mypos);
		if (direct < 0)
		{
			if (pos.x > mypos.x)
			{
				continue;
			}
		}
		else
		{
			if (pos.x < mypos.x)
			{
				continue;
			}
		}
		if (dis < tempdis)
		{
			isFound = true;
			amiPos = pos;
			tempdis = dis;
		}
	}
	if (isFound)
	{
		auto myPos = convertToWorldSpaceAR(Vec2::ZERO);
		auto newVec = amiPos - myPos;
		float angle = newVec.getAngle() * Pi2Angle;

		setRotation(90-angle);
		newVec.normalize();
		_physicsBody->setVelocity(newVec * m_FlySpeed);
	}
}

void BulletToTarget::FixAngleToMainplane()
{
	auto mainPl = GameData::getInstance()->getMainPlane();
	if (!mainPl)
	{
		return;
	}
	auto mainplPos = mainPl->convertToWorldSpaceAR(Vec2::ZERO);
	auto myPos = convertToWorldSpaceAR(Vec2::ZERO);

	auto newVec = mainplPos - myPos;
	newVec.normalize();
	_physicsBody->setVelocity(newVec * m_FlySpeed);
}

//--------------------------------------------
BulletSurroundMainPl* BulletSurroundMainPl::create(int lvl)
{
	BulletSurroundMainPl* bullet = new (std::nothrow) BulletSurroundMainPl;
	if (bullet && bullet->init())
	{
		bullet->autorelease();
		bullet->initBody();
		bullet->m_Level = lvl;
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void BulletSurroundMainPl::onEnter()
{
	Bullet::onEnter();

	ListeningStopWeapon();

	m_curBodyRadius = 10.0f;
	float DoublePI = 2 * PI;

	switch (m_Level)
	{
	case 1:
		{
			m_radius = 200;
			addSurroundBullet(0,false);
			addSurroundBullet(DoublePI * 1/3,false);
			addSurroundBullet(DoublePI * 2/3,false);
		}
		break;
	case 2:
		{
			m_radius = 250;
			addSurroundBullet(0,false);
			addSurroundBullet(DoublePI * 1/4,false);
			addSurroundBullet(DoublePI * 2/4,false);
			addSurroundBullet(DoublePI * 3/4,false);
		}
		break;
	case 3:
		{
			m_radius = 300;
			addSurroundBullet(0);
			addSurroundBullet(DoublePI * 1/5);
			addSurroundBullet(DoublePI * 2/5);
			addSurroundBullet(DoublePI * 3/5);
			addSurroundBullet(DoublePI * 4/5);
		}
		break;

	default:
		break;
	}

	ActionInterval* rotate = RotateBy::create(1.0f + rand_0_1()*0.2,600);
	ActionInterval* rep = RepeatForever::create(rotate);
	runAction(rep);

	//一段时间后删除
	ActionInterval* delay = DelayTime::create(m_dur);
	CallFunc* func = CallFunc::create(this,callfunc_selector(PhysicsUnit::setDirectDeath));
	ActionInterval* seq = Sequence::create(delay, func,nullptr);
	runAction(seq);
}

void BulletSurroundMainPl::addSurroundBullet( float angle,bool isflyout /*= true*/ )
{
	//根据旋转角度参数，计算子弹的位置
	Vec2 originVec = Vec2(m_radius,0);
	originVec.rotate(Vec2(0,0),angle);

	std::string file = String::createWithFormat("ammo/surround/surround_%d.png",m_Level)->getCString();

	SurroundBullet* bullet = SurroundBullet::create(file,m_dur,isflyout);
	bullet->setPosition(Vec2(0,0));
	bullet->setOffset(originVec);
	bullet->setScale(0.2);
	addChild(bullet);

	ActionInterval* mv = MoveTo::create(1.5f,originVec);
	bullet->runAction(mv);

	ActionInterval* scale = ScaleTo::create(1.0f,1.0);
	bullet->runAction(scale);
}

void BulletSurroundMainPl::setBody(float dt)
{
	if (m_curBodyRadius < m_radius*1.1)
	{
		m_curBodyRadius += 180*dt;
	}
	auto body = PhysicsBody::createCircle(m_curBodyRadius);
	body->setGravityEnable(false);
	body->setRotationEnable(false);
	setPhysicsBody(body);
	setBitmask(
		SELF_AMMO_CATEGORYBITMASK,
		SELF_AMMO_COLLISIONBITMASK_P,
		SELF_AMMO_CONTACTTESTBITMASK);
}

void BulletSurroundMainPl::initBody()
{
	setInvincible(true);
	setBody(0.0);
	schedule(schedule_selector(BulletSurroundMainPl::setBody), 0.1);
}

float BulletSurroundMainPl::getDur()
{
	return m_dur;
}

void BulletSurroundMainPl::removeMeO( Ref* obj )
{
	setDirectDeath();
}


//-----------------------------------------
ThreePetalsBullet* ThreePetalsBullet::create(const std::string& file)
{
	ThreePetalsBullet* bullet = new (std::nothrow) ThreePetalsBullet;
	if (bullet && bullet->initWithFile(file))
	{
		bullet->initBody();
		bullet->autorelease();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void ThreePetalsBullet::initBody()
{
	auto body = PhysicsBody::createCircle(getContentSize().width*0.7);
	body->setGravityEnable(false);
	setPhysicsBody(body);
	setBitmask(
		SELF_AMMO_CATEGORYBITMASK,
		SELF_AMMO_COLLISIONBITMASK_P,
		SELF_AMMO_CONTACTTESTBITMASK);
	setInvincible(true);
}

void ThreePetalsBullet::onEnter()
{
	Bullet::onEnter();

	auto myPos = convertToWorldSpaceAR(Vec2::ZERO);

	//向上移动到屏幕外
	ActionInterval* mv = MoveTo::create(3.0f,Vec2(myPos.x, m_WinSize.height * 1.5));
	ActionInterval* ease = CCEaseSineIn::create(mv);
	runAction(ease);

	ActionInterval* rotate = RotateBy::create(3.0f, 1080);
	runAction(rotate);

	addParticle();
}

void ThreePetalsBullet::addParticle()
{
	auto pos = Vec2(getContentSize().width*0.5, getContentSize().height*0.5);
	EffectsManager::addParticle("particla/Petals/Lvl_1/flower_lvl_1_1.plist",this,pos,false);
	EffectsManager::addParticle("particla/Petals/Lvl_1/flower_lvl_1_2.plist",this,pos,false);
}

void ThreePetalsBullet::DoSomethingBeforeDead()
{
	//log("");
}


//--------------------------------------------
DartBulletRebounce* DartBulletRebounce::create( const std::string& frameName, int direct )
{
	DartBulletRebounce* bullet = new (std::nothrow) DartBulletRebounce;
	if (bullet && bullet->initWithSpriteFrameName(frameName))
	{
		bullet->autorelease();
		bullet->initBody();
		bullet->m_direct = direct;
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void DartBulletRebounce::onEnter()
{
	Bullet::onEnter();
	setInvincible(true); 
	PlayAction();
	fixAngle();
}

void DartBulletRebounce::initBody()
{
	PhysicsBody* body = PhysicsBody::createCircle(getContentSize().width*0.5);
	body->setGravityEnable(false);
	body->setVelocity(Vec2(0,10));
	setPhysicsBody(body);

	setBitmask(
		HOODLE_CATEGORYBITMASK,
		HOODLE_COLLISIONBITMASK_P,
		HOODLE_CONTACTTESTBITMASK);
	
	m_ContactPro.hurts = -1000;
}

void DartBulletRebounce::PlayAction()
{
	//自身旋转
	ActionInterval* rotate = RotateBy::create(1.0f,-2000);
	ActionInterval* rep = RepeatForever::create(rotate);
	runAction(rep);
}

void DartBulletRebounce::fixAngle()
{
	auto curV = _physicsBody->getVelocity();

	//旋转一个角度
	float r = (m_direct * 30 + 90) * Angle2pi;
	curV = curV.forAngle(r);
  	curV.normalize();
	
	_physicsBody->setVelocity(curV * m_FlySpeed);
}

void DartBulletRebounce::OtherDealWhenContact( const ContactPro& rs )
{
	if (rs.idx == UnitId::eLeftWall || rs.idx == UnitId::eRightWall || rs.idx == UnitId::eUpWall)
	{
		setBitmask(
			SELF_AMMO_CATEGORYBITMASK,
			SELF_AMMO_COLLISIONBITMASK_P,
			SELF_AMMO_CONTACTTESTBITMASK);
	}
}

void DartBulletRebounce::InitEdgeLine()
{
	m_EdgeLineVec.push_back(-500);
	m_EdgeLineVec.push_back(m_WinSize.width + 500);
	m_EdgeLineVec.push_back(-100);
	m_EdgeLineVec.push_back(m_WinSize.height + 300);
}

//-------------------------------------------------------
DartBulletSmall* DartBulletSmall::create( const std::string& framename)
{
	DartBulletSmall* bullet = new (std::nothrow) DartBulletSmall;
	if (bullet && bullet->initWithSpriteFrameName(framename))
	{
		bullet->autorelease();
		bullet->initBody();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void DartBulletSmall::initBody()
{
	PhysicsBody* body = PhysicsBody::createCircle(getContentSize().width*0.5);
	body->setGravityEnable(false);
	setPhysicsBody(body);

	setBitmask(
		SELF_AMMO_CATEGORYBITMASK,
		SELF_AMMO_COLLISIONBITMASK_P,
		SELF_AMMO_CONTACTTESTBITMASK);
	setInvincible(true);
}

void DartBulletSmall::onEnter()
{
	Bullet::onEnter();

	ActionInterval* rotate = RotateBy::create(0.5f,-1080);
	ActionInterval* rep = RepeatForever::create(rotate);
	runAction(rep);
}

//----------------------------
DartBulletTrace* DartBulletTrace::create( const std::string& framename )
{
	DartBulletTrace* bullet = new (std::nothrow) DartBulletTrace;
	if (bullet && bullet->initWithSpriteFrameName(framename))
	{
		bullet->initBody();
		bullet->autorelease();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void DartBulletTrace::initBody()
{
	PhysicsBody* body = PhysicsBody::createCircle(getContentSize().width*0.5);
	body->setGravityEnable(false);
	setPhysicsBody(body);

	setBitmask(
		SELF_AMMO_CATEGORYBITMASK,
		SELF_AMMO_COLLISIONBITMASK_P,
		SELF_AMMO_CONTACTTESTBITMASK);
	setInvincible(true);
}

void DartBulletTrace::onEnter()
{
	Bullet::onEnter();
	setScale(3.0f);
	schedule(schedule_selector(DartBulletTrace::UpdatePos));
	schedule(schedule_selector(DartBulletTrace::Hurt),0.1);

	{
		ActionInterval* delay = DelayTime::create(10.0f);
		CallFunc* func = CallFunc::create(this,callfunc_selector(PhysicsUnit::setDirectDeath));
		ActionInterval* seq = Sequence::create(delay, func, nullptr);
		runAction(seq);
	}

	{
		ActionInterval* rotate = RotateBy::create(1.5f,-1080);
		ActionInterval* rep = RepeatForever::create(rotate);
		runAction(rep);
	}
	{
		auto delay = DelayTime::create(8.0f);
		auto fadeout = FadeOut::create(2.0f);
		auto seq = Sequence::create(delay, fadeout, nullptr);
		runAction(seq);
	}
	addParticle();
}

void DartBulletTrace::UpdatePos( float dt )
{
	//是否有敌机
	auto enemyList = GameData::getInstance()->getEnemyPlaneVec();
	if (enemyList.empty())
	{
		//log("no energy");
	}
	else
	{
		//找到血量最大的目标
		PhysicsUnit* pl = GameData::getInstance()->getEnemyPlaneByMaxHp();
		Point myPos = this->convertToWorldSpaceAR(Point::ZERO);
		Point AimPos;
		if (pl)
		{
			AimPos = pl->convertToWorldSpaceAR(Point::ZERO);
		}
		else
		{
			AimPos = myPos;
		}

		AimPos.x = AimPos.x + rand_minus1_1() * 100;
		AimPos.y = AimPos.y + rand_minus1_1() * 100;

		Point movePos = MoveStep(AimPos,myPos,6);

		setPosition(movePos);
	}

	//不能超出屏幕
	fixPos();
}

void DartBulletTrace::fixPos()
{
	Point myPos = convertToWorldSpaceAR(Point::ZERO);

	if (myPos.x >= m_WinSize.width - 50)
	{
		myPos.x = m_WinSize.width - 50;
	}
	else if(myPos.x <= 50)
	{
		myPos.x = 50;
	}
	if (myPos.y >= m_WinSize.height - 100)
	{
		myPos.y = m_WinSize.height - 100;
	}
	else if(myPos.y <= 50)
	{
		myPos.y = 50;
	}
	setPosition(myPos);
}

void DartBulletTrace::Hurt( float dt )
{
	initBody();
}

void DartBulletTrace::addParticle()
{
	auto size = getContentSize();
	auto pos = Vec2(size.width * 0.5, size.height *0.5);
	EffectsManager::addParticle("particla/Huixuanbiao/Huixuanbiao.plist",this,pos,false);
	auto partc = EffectsManager::addParticle("particla/Huixuanbiao/Huixuanbiao.plist",this,pos,false);
	if (partc)
	{
		partc->setPositionType(ParticleSystem::PositionType::GROUPED);
	}
}

//-------------------------------------
SurroundBullet* SurroundBullet::create(const std::string& file, float dur, bool isflyout /*= false*/)
{
	SurroundBullet* bullet = new (std::nothrow) SurroundBullet;
	if (bullet && bullet->initWithFile(file))
	{
		bullet->setDur(dur);
		bullet->autorelease();
		bullet->m_isFlyOut = isflyout;
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void SurroundBullet::onEnter()
{
	Sprite::onEnter();
	//addParticle();
	FixRotate();

	if (!m_isFlyOut)
	{
		//渐隐
		ActionInterval* delay = DelayTime::create(m_dur - 2.0f);
		ActionInterval* fadeout = FadeOut::create(2.0f);
		CallFunc* func = CallFunc::create(this,callfunc_selector(SurroundBullet::removeMe));
		ActionInterval* seq = Sequence::create(delay, fadeout, func, nullptr);
		runAction(seq);
	}
	else
	{
		//飞出去结束
		auto delay = DelayTime::create(m_dur - 0.5);
		auto func = CallFunc::create(this, callfunc_selector(SurroundBullet::FlyOut));
		auto seq = Sequence::create(delay, func, nullptr);
		runAction(seq);
	}
}

void SurroundBullet::onExit()
{
	Sprite::onExit();

	
}

void SurroundBullet::FlyOut()
{
	auto parent = getParent();
	auto parentPos = parent->convertToWorldSpaceAR(Vec2::ZERO);
	auto mypos = convertToWorldSpaceAR(Vec2::ZERO);

	Vec2 newVec = mypos - parentPos;
	newVec.rotate(Vec2(0,0),-90 * Angle2pi);
	newVec.normalize();
	newVec *= 1000;

	PhysicsUnit* circle = (PhysicsUnit*)parent;

	//替换掉原来的
	FlyOutBullet* bullet = FlyOutBullet::create();
	bullet->setPosition(mypos);
	bullet->resetHurt(circle->getContactPro().hurts);
	bullet->setBelongTo(UnitId::eRedCard);
	bullet->getPhysicsBody()->setVelocity(newVec);
	UIController::getInstance()->getPhysicsLayer()->addChild(bullet);

	auto vec = newVec;
	auto angle = vec.getAngle() * Pi2Angle;
	bullet->setRotation(-angle + 180);

	removeMe();
}

void SurroundBullet::addParticle()
{
	auto s = getContentSize();
	EffectsManager::addParticle("particla/Boss1ammo/SurroundSingle.plist",this,Vec2(s.width*0.5,s.height*0.5),false);
}

void SurroundBullet::FixRotate()
{
	auto vec = m_offset;
	vec.rotate(Vec2(0,0),90);
	auto angle = vec.getAngle() * Pi2Angle;
	setRotation(-angle + 28);
}

void SurroundBullet::setOffset( const Vec2& pos )
{
	m_offset = pos;
}

void SurroundBullet::removeMe()
{
	removeFromParentAndCleanup(true);
}

void SurroundBullet::setDur( float t )
{
	m_dur = t;
}

//-------------------------------
FlyOutBullet* FlyOutBullet::create()
{
	FlyOutBullet* bullet = new (std::nothrow) FlyOutBullet;
	if (bullet && bullet->initWithFile("ammo/surround/surround_3_1.png"))
	{
		bullet->initBody();
		bullet->autorelease();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void FlyOutBullet::initBody()
{
	auto body = PhysicsBody::createBox(getContentSize());
	body->setGravityEnable(false);
	body->setRotationEnable(false);
	setPhysicsBody(body);
	setBitmask(
		SELF_AMMO_CATEGORYBITMASK,
		SELF_AMMO_COLLISIONBITMASK_P,
		SELF_AMMO_CONTACTTESTBITMASK);
	m_ContactPro.hurts = Ene_Blt_hurt * 10;
	setInvincible(true);
}

//---------------------------------------------------------------------
AirazorBullet* AirazorBullet::create( const std::string& file, float r, bool isPierce )
{
	AirazorBullet* bullet = new (std::nothrow) AirazorBullet;
	if (bullet && bullet->initWithFile(file))
	{
		bullet->m_isPierce = isPierce;
		bullet->m_rotate = r;
		bullet->autorelease();
		bullet->initBody();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void AirazorBullet::onEnter()
{
	Bullet::onEnter();
	appear();
}

void AirazorBullet::initBody()
{
	auto body = PhysicsBody::createCircle(getContentSize().width*0.5);
	body->setGravityEnable(false);
	body->setMass(0.00001);
	setPhysicsBody(body);
	m_SelfPro.curHP = 1;
	m_SelfPro.maxHP = 1;
	m_ContactPro.hurts = Ene_Blt_hurt * 10;
	if (m_isPierce)
	{
		setInvincible(true);
		setBitmask(
			SELF_AMMO_CATEGORYBITMASK,
			SELF_AMMO_COLLISIONBITMASK_P,
			SELF_AMMO_CONTACTTESTBITMASK);
	}
	else
	{
		setBitmask(
			SELF_AMMO_CATEGORYBITMASK,
			SELF_AMMO_COLLISIONBITMASK,
			SELF_AMMO_CONTACTTESTBITMASK);
	}
}

void AirazorBullet::appear()
{
	float radius = 100;
	Vec2 originVec = Vec2(0, -radius);
	originVec.rotate(Vec2::ZERO, m_rotate * Angle2pi );

	setOpacity(0);
	ActionInterval* mv = MoveBy::create(0.5f, originVec);
	ActionInterval* fadein = FadeIn::create(0.5f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(AirazorBullet::FixAngle));
	ActionInterval* seq = Sequence::create(mv,func, nullptr);
	runAction(seq);
	runAction(fadein);
}

void AirazorBullet::FixAngle()
{
	Vec2 targetPos;
	if(GameData::getInstance()->getEnemyPlaneVec().empty())
	{
		targetPos = Vec2(m_WinSize.width*0.5, m_WinSize.height);
	}
	else
	{
		auto pl = GameData::getInstance()->getEnemyPlaneByMaxHp();//得到血量最大的敌机目标
		if (!pl)
		{
			targetPos = Vec2(m_WinSize.width*0.5, m_WinSize.height);
		}
		else
		{
			targetPos = pl->convertToWorldSpaceAR(Vec2::ZERO);
		}
		if (targetPos.y < 0 || targetPos.x < 0 || targetPos.x > m_WinSize.width)
		{
			targetPos = Vec2(m_WinSize.width*0.5, m_WinSize.height);
		}
	}
	
	auto mypos = convertToWorldSpaceAR(Vec2::ZERO);
	auto newVec = targetPos - mypos;
	m_flyDirect = newVec;
	float angle = newVec.getAngle() * Pi2Angle - 90;

	if (angle > 180)
	{
		angle = angle - 360;
	}
	ActionInterval* rt = RotateBy::create(0.5f,-angle);
	CallFunc* func = CallFunc::create(this,callfunc_selector(AirazorBullet::FlyOut));
	ActionInterval* seq = Sequence::create(rt, func, nullptr);
	runAction(seq);
}

void AirazorBullet::FlyOut()
{
	m_flyDirect.normalize();
	_physicsBody->setVelocity(m_flyDirect * 1500);
	if (m_isPierce)
	{
		AddTail();
	}
}

void AirazorBullet::AddTail()
{
	auto s = getContentSize();
	EffectsManager::addParticle("particla/SwordTail/Sword1.plist",this,Vec2(s.width*0.5,0),false);
	EffectsManager::addParticle("particla/SwordTail/Sword2.plist",this,Vec2(s.width*0.5,0),false);
}

void AirazorBullet::DoSomethingBeforeDead()
{
	
}

void AirazorBullet::InitEdgeLine()
{
	m_EdgeLineVec.push_back(-500);
	m_EdgeLineVec.push_back(m_WinSize.width + 500);
	m_EdgeLineVec.push_back(-500);
	m_EdgeLineVec.push_back(m_WinSize.height + 500);
}


//---------------------------------------
DrawLighting::DrawLighting()
{

}

DrawLighting* DrawLighting::create( const Vec2& startPos,const Vec2& endPos )
{
	DrawLighting* lighting = new (std::nothrow) DrawLighting;
	if (lighting && lighting->init())
	{
		lighting->autorelease();
		lighting->m_startPos = startPos;
		lighting->m_endPos = endPos;
		return lighting;
	}
	else
	{
		CC_SAFE_DELETE(lighting);
		return nullptr;
	}
}

void DrawLighting::onEnter()
{
	Node::onEnter();
	setPosition(m_startPos);
	RefreshPicture();
}

void DrawLighting::RefreshPicture()
{
	int num1 = random(1,6);
	std::string file = CCString::createWithFormat("ammo/lighting/longlighting_%d.png",num1)->getCString();
	m_Sp = Sprite::create(file);
	m_Sp->setAnchorPoint(Vec2(0.5,0));
	addChild(m_Sp);

	auto newVec = m_endPos - m_startPos;
	float angle = newVec.getAngle() * Pi2Angle;
	setRotation(-angle + 90);

	//根据距离缩放
	auto s = m_Sp->getContentSize();
	auto dis = m_startPos.distance(m_endPos);
	float ratio = dis/s.height;
	if (ratio <= 1.0)
	{
		Rect rect = Rect(0,0, s.width, s.height * ratio);
		m_Sp->setTextureRect(rect);
	}
	else
	{
		setScaleY(ratio);
	}
	

	ActionInterval* delay = DelayTime::create(1.0f/24);
	CallFunc* func = CallFunc::create(this,callfunc_selector(DrawLighting::removeMe));
	ActionInterval* seq = Sequence::create(delay, func, nullptr);
	runAction(seq);
}

void DrawLighting::removeMe()
{
	removeFromParentAndCleanup(true);
}


//-=------------------------
NewLightingLayer::NewLightingLayer()
{
	auto s = Director::getInstance()->getWinSize();

	m_EdgeL = 50.0f;
	m_EdgeR = s.width - 50.0f;
	m_EdgeD = 50.0f;
	m_EdgeU = s.height - 50.0f;
}

NewLightingLayer* NewLightingLayer::create( int num, int hurt )
{
	NewLightingLayer* layer = new (std::nothrow) NewLightingLayer;
	if (layer && layer->init())
	{
		layer->m_lightingNums = num;
		layer->setHurt(hurt);
		layer->autorelease();
		return layer;
	}
	else
	{
		CC_SAFE_DELETE(layer);
		return nullptr;
	}
}

void NewLightingLayer::onEnter()
{
	Layer::onEnter();
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(NewLightingLayer::removeMeO),
		Msg_StopMainPlBullet,nullptr);
	StartLighting();
}

void NewLightingLayer::onExit()
{
	Layer::onExit();
	NotificationCenter::getInstance()->removeAllObservers(this);
}

void NewLightingLayer::FindAllTargets()
{
	m_targetList.clear();
	m_posList.clear();

	//先传入主飞机的坐标
	auto mainpl = GameData::getInstance()->getMainPlane();
	auto curpos = Vec2(0,0);

	if (mainpl)
	{
		curpos = mainpl->convertToWorldSpaceAR(Vec2::ZERO);
		m_posList.push_back(curpos);
	}
	else
	{
		//删除
		removeFromParentAndCleanup(true);
		return;
	}

	auto plVec = GameData::getInstance()->getEnemyPlaneVec();

	//先判断是否有BOSS
	for (auto it = plVec.begin(); it != plVec.end(); it++)
	{
		auto pl = (*it);
		auto plidx = pl->getSelfPro().idx;
		if (plidx == UnitId::ePlaneBoss )
		{
			curpos = pl->convertToWorldSpaceAR(Vec2::ZERO);
			bool isInView = isInScreen(curpos);//是否进入视野
			if (!isInView)
			{
				break;
			}
			curpos.y -= 60;
			m_posList.push_back(curpos);
			m_targetList.push_back(pl);
			break;
		}
	}

	//找普通目标
	int findTimes =  plVec.size() - m_posList.size() + 1;
	for (int i = 0; i < findTimes; i++)
	{
		if (m_posList.size() > m_lightingNums )
		{
			break;
		}
		bool isfind = FindTarget(curpos);	//没找到目标，那么不在继续，否则继续寻找
		if (!isfind)
		{
			break;;
		}
	}

	//所有点之间闪电
	DrawLight();
}

bool NewLightingLayer::FindTarget( Vec2& startPos )
{
	auto plVec = GameData::getInstance()->getEnemyPlaneVec();
	PhysicsUnit* TargetPl  = nullptr;
	Vec2 targetPos = Vec2(0,0);

	//遍历普通敌机
	float dis = 100000.0f;
	bool isSuc = false;
	for (auto it = plVec.begin(); it != plVec.end(); it++)
	{
		auto pl = (*it);
		if (pl)
		{
			bool isfound = isFound(pl);		//是否已经是被找到的目标
			if (isfound)
			{
				continue;
			}
			Vec2 pos = pl->convertToWorldSpaceAR(Vec2::ZERO);
			bool isInView = isInScreen(pos);//是否进入视野
			if (!isInView)
			{
				continue;
			}
			float TempDis = pos.distance(startPos);
			if (TempDis < dis)
			{
				isSuc = true;
				dis = TempDis;
				targetPos = pos;
				TargetPl = pl;
			}
		}
	}

	//记录找到的敌机
	if(isSuc)
	{
		startPos = targetPos;
		m_posList.push_back(targetPos);
		m_targetList.push_back(TargetPl);
	}

	return isSuc;
}

bool NewLightingLayer::isFound( PhysicsUnit* target )
{
	bool b = false;
	for (int i = 0; i < m_targetList.size(); i++)
	{
		auto pu = m_targetList.at(i);
		if (pu == target)
		{
			b = true;
			break;
		}
	}

	return b;
}

void NewLightingLayer::DrawLight()
{
	if (m_posList.empty())
	{
		return;
	}
	for (int i = 0; i < m_posList.size() - 1; i++)
	{
		Vec2 startPos = m_posList.at(i);
		Vec2 endPos = m_posList.at(i+1);
		DrawLighting* light = DrawLighting::create(startPos, endPos);
		addChild(light);
	}
	for (int i = 1; i < m_posList.size(); i++)
	{
		Vec2 pos = m_posList.at(i);
		LightingAttackPoint* attack = LightingAttackPoint::create();
		attack->setPosition(pos);
		attack->setBelongTo(UnitId::eYellowCard);	//设置为属于黄球
		attack->resetHurt(m_hurt);
		addChild(attack);
	}
}

void NewLightingLayer::StartLighting( float dur )
{
	{
		ActionInterval* interval = DelayTime::create(1.0f/24);
		CallFunc* func = CallFunc::create(this,callfunc_selector(NewLightingLayer::FindAllTargets));
		ActionInterval* seq = Sequence::create(func, interval, nullptr);
		ActionInterval* rep = RepeatForever::create(seq);
		runAction(rep);
	}
	if(dur > 0)
	{
		ActionInterval* delay = DelayTime::create(dur);
		CallFunc* func = CallFunc::create(this,callfunc_selector(NewLightingLayer::removeMe));
		ActionInterval* seq = Sequence::create(delay,func,nullptr);
		runAction(seq);
	}
}

void NewLightingLayer::removeMe()
{
	removeFromParentAndCleanup(true);
}

void NewLightingLayer::removeMeO(Ref* obj)
{
	removeMe();
}

bool NewLightingLayer::isInScreen( const Vec2& pos )
{
	bool b = false; 
	if (pos.x > m_EdgeL && pos.x < m_EdgeR &&
		pos.y > m_EdgeD && pos.y < m_EdgeU)
	{
		b = true;
	}

	return b;
}

void NewLightingLayer::setHurt( int hurt )
{
	m_hurt = hurt;
}

//-------------------------------
LightingAttackPoint* LightingAttackPoint::create()
{
	LightingAttackPoint* point = new (std::nothrow) LightingAttackPoint;
	if (point && point->init())
	{
		point->initBody();
		point->autorelease();
		return point;
	}
	else
	{
		CC_SAFE_DELETE(point);
		return nullptr;
	}
}

void LightingAttackPoint::initBody()
{
	auto body = PhysicsBody::createCircle(20);
	body->setGravityEnable(false);
	setPhysicsBody(body);
	m_SelfPro.curHP = 1;
	m_SelfPro.maxHP = 1;
	setUnitId(UnitId::eBullet_38);

	setBitmask(
		SELF_AMMO_CATEGORYBITMASK,
		SELF_AMMO_COLLISIONBITMASK_P,
		SELF_AMMO_CONTACTTESTBITMASK);
}

void LightingAttackPoint::onEnter()
{
	Bullet::onEnter();

	ActionInterval* delay = DelayTime::create(0.5f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(PhysicsUnit::setDirectDeath));
	ActionInterval* seq = Sequence::create(delay,func,nullptr);
	runAction(seq);
}

//---------------------------------
ExplosionBullet* ExplosionBullet::create( float angle)
{
	ExplosionBullet* bullet = new (std::nothrow) ExplosionBullet;
	if (bullet && bullet->initWithFile("animation/ExplosionBullet/FlyState.png"))
	{
		bullet->initBody();
		bullet->autorelease();
		bullet->m_angle = angle;
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void ExplosionBullet::initBody()
{
	PhysicsBody* body = PhysicsBody::createBox(getContentSize());
	body->setLinearDamping(5.0);
	body->setGravityEnable(false);
	setPhysicsBody(body);

	setBitmask(ENEMY_AMMO_CATEGORYBITMASK,
			0,
			ENEMY_AMMO_CONTACTTESTBITMASK);
	setInvincible(true);
	setBelongTo(UnitId::ePlaneBoss);

	m_ContactPro.hurts = 0;
}

void ExplosionBullet::onEnter()
{
	Bullet::onEnter();
	FixBulletAngle();
	ExplAction();
}

void ExplosionBullet::Explosion()
{
	setVisible(false);

	NewAudio::getInstance()->playEffWithPar(Music_Boss1Shoot1,false,0.3f,nullptr);
	auto s = getContentSize();

	for (int i = 0; i < 8; i++)
	{
		Weapon* wp = Weapon::create(this);
		wp->setBullstType(BulletType::Enemy_default);
		wp->setBltResTag(ResTag::Bullet_BossFireAmmo);
		wp->setBltBelongTo(UnitId::ePlaneBoss);
		wp->setShootTimes(3);
		wp->setBltHurts(Ene_Blt_hurt);
		wp->setBltSpeed(500);
		wp->setBltScaleX(0.65);
		wp->setBltScaleY(0.65);
		wp->setBltLinearDamping(0.2);
		wp->setRotation(i * 360.0f/8 );
		wp->setOffset(Vec2(s.width*0.5, s.height*0.5));
		wp->StartShoot();
		addChild(wp);
		wp->runAction(RotateBy::create(0.45,20));
	}

	//烟花
	/*for (int i = 0; i < 8; i++)
	{
		Weapon* wp = Weapon::create(m_target);
		wp->setBullstType(BulletType::Enemy_BossDart);
		wp->ChangePro(Ene_Blt_hurt,10.f,0.3f);	
		wp->setBltSpeed(500);
		wp->setRotation(i * 360.0f/8);
		wp->setScale(0.1f);
		wp->setOffset(Vec2(s.width*0.5, s.height*0.5));
		wp->StartShoot();
		addChild(wp);
	}*/

	//N秒后删除自己
	ActionInterval* delay = DelayTime::create(10.5f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(PhysicsUnit::setDirectDeath));
	ActionInterval* seq = Sequence::create(delay, func, nullptr);
	runAction(seq);
}

void ExplosionBullet::FixBulletAngle()
{
	auto originVec = Vec2(0,-1);
	originVec.rotate(Vec2(0,0), -m_angle * DEG_RAD);
	_physicsBody->setVelocity(originVec * 2000);
	setRotation(180 + m_angle);
}

void ExplosionBullet::ExplAction()
{
	setScaleX(0.25);

	auto layer = UIController::getInstance()->getEffectsLayer();

	auto scale = ScaleTo::create(0.5f,1.0f);
	auto animat1 = layer->getFramesAnimation("ExplBult_1_", 11, 1.0/60);
	auto animat2 = layer->getFramesAnimation("ExplBult_2_", 6, 1.0/60);
	auto func = CallFunc::create(this, callfunc_selector(ExplosionBullet::Explosion));
	auto seq = Sequence::create(scale, animat1, animat2, func, nullptr);
	runAction(seq);
}

//---------------------------------
DartOfBoss3* DartOfBoss3::create()
{
	DartOfBoss3* dart = new (std::nothrow) DartOfBoss3;
	auto file = GameData::getInstance()->getBulletFactory()->getResByTag(ResTag::Bullet_BossBigDart);
	if (dart && dart->initWithSpriteFrameName(file))
	{
		dart->initBody();
		dart->autorelease();
		return dart;
	}
	else
	{
		CC_SAFE_DELETE(dart);
		return nullptr;
	}
}

void DartOfBoss3::initBody()
{
	setInvincible(true);

	PhysicsBody* body = PhysicsBody::createCircle(getContentSize().width * 0.5);
	body->setGravityEnable(false);
	body->setCategoryBitmask(ENEMY_AMMO_CATEGORYBITMASK);
	body->setCollisionBitmask(0);
	body->setContactTestBitmask(ENEMY_AMMO_CONTACTTESTBITMASK);
	setPhysicsBody(body);

	schedule(schedule_selector(DartOfBoss3::setBody),0.1);

}

void DartOfBoss3::onEnter()
{
	PhysicsUnit::onEnter();
	addParticle();
}

void DartOfBoss3::setBody(float dt)
{
	PhysicsBody* body = PhysicsBody::createCircle(getContentSize().width * 0.5);
	body->setGravityEnable(false);
	body->setCategoryBitmask(ENEMY_AMMO_CATEGORYBITMASK);
	body->setCollisionBitmask(0);
	body->setContactTestBitmask(ENEMY_AMMO_CONTACTTESTBITMASK);
	setPhysicsBody(body);
}

void DartOfBoss3::addParticle()
{
	auto s = getContentSize();
	auto partc = EffectsManager::addParticle("particla/Huixuanbiao/Huixuanbiao_1.plist",this,Vec2(s.width * 0.5, s.height*0.5),false);
	if (partc)
	{
		partc->setScale(2.5);
	}
}

void DartOfBoss3::InitEdgeLine()
{
	m_EdgeLineVec.push_back(-300);
	m_EdgeLineVec.push_back(m_WinSize.width + 300);
	m_EdgeLineVec.push_back(-50);
	m_EdgeLineVec.push_back(m_WinSize.height + 300);
}

//----------------------------------
Bullet1OfBoss4* Bullet1OfBoss4::create(ResTag res, float stayTime)
{
	Bullet1OfBoss4* bullet = new (std::nothrow) Bullet1OfBoss4;
	auto file = GameData::getInstance()->getBulletFactory()->getResByTag(res);
	if (bullet && bullet->initWithSpriteFrameName(file))
	{
		bullet->m_StayTime = stayTime;
		bullet->initBody();
		bullet->autorelease();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void Bullet1OfBoss4::FloatUpDown()
{
	setOpacity(0);
	auto fadein = FadeIn::create(0.5 + rand_0_1() * 0.2);
	auto mvUp = MoveBy::create(1.5f + rand_0_1() * 0.5, Vec2(0,50));
	auto ease = EaseInOut::create(mvUp, 0.8);
	auto mvDown = ease->reverse();
	auto seq = Sequence::create(fadein, ease, mvDown, nullptr);
	auto rep = RepeatForever::create(seq);
	runAction(rep);
}

void Bullet1OfBoss4::initBody()
{
	PhysicsBody* body = PhysicsBody::createCircle(getContentSize().width * 0.5);
	body->setGravityEnable(false);
	body->setLinearDamping(0.3);
	body->setMass(0.1);
	setPhysicsBody(body);
	setBitmask(ENEMY_AMMO_CATEGORYBITMASK, 0, ENEMY_AMMO_CONTACTTESTBITMASK);
	m_ContactPro.hurts = Ene_Blt_hurt;
}

void Bullet1OfBoss4::onEnter()
{
	Bullet::onEnter();
	FloatUpDown();

	BreathAction(this);

	ActionInterval* delay = DelayTime::create(m_StayTime /*+ rand_0_1()*/);
	CallFunc* func = CallFunc::create(this, callfunc_selector(Bullet1OfBoss4::Start));
	//CallFunc* func = CallFunc::create(this, callfunc_selector(Bullet1OfBoss4::MoveToPl));
	ActionInterval* seq = Sequence::create(delay, func, nullptr);
	runAction(seq);
}

void Bullet1OfBoss4::Helix( float dt )
{
	//得到当前飞行速度，格式化
	Vec2 curV = _physicsBody->getVelocity();

	//找到速度向量的法线，朝着该方向给一个力(绕着一个点慢慢扩散)
	Vec2 faxian = curV.getPerp();
	faxian.normalize();
	_physicsBody->applyImpulse(faxian * 5.0f,Vec2(0,0));
}

void Bullet1OfBoss4::StopHelix()
{
	unschedule(schedule_selector(Bullet1OfBoss4::Helix));
	MoveToPl();
}

void Bullet1OfBoss4::MoveToPl()
{
	stopAllActions();
	_physicsBody->setLinearDamping(0.0);
	auto pl = GameData::getInstance()->getMainPlane();
	Vec2 pos = Vec2(360,-10);
	if (pl)
	{
		pos = pl->convertToWorldSpaceAR(Vec2::ZERO);
	}
	
	auto mypos = convertToWorldSpaceAR(Vec2::ZERO);
	auto newVec = pos - mypos;
	newVec.normalize();
	_physicsBody->setVelocity(newVec * 800);
}

void Bullet1OfBoss4::Start()
{ 
	_physicsBody->setVelocity(Vec2(0,-100));

	auto mv = MoveBy::create(1.5f, Vec2(0, - 200));
	runAction(mv);

	schedule(schedule_selector(Bullet1OfBoss4::Helix));

	ActionInterval* delay = DelayTime::create(1.5f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(Bullet1OfBoss4::StopHelix));
	ActionInterval* seq = Sequence::create(delay,func, nullptr);
	runAction(seq);
}

void Bullet1OfBoss4::BreathAction( Node* node )
{
	float curScale = node->getScale();

	auto scaleX = ScaleTo::create(0.15,0.9 * curScale,curScale);
	auto scaleY = ScaleTo::create(0.15,curScale,curScale * 0.9);
	auto easeX = EaseSineInOut::create(scaleX);
	auto easeY = EaseSineInOut::create(scaleY);

	auto seq = Sequence::create(easeX, easeY, nullptr);
	auto rep = RepeatForever::create(seq);
	node->runAction(rep);

	auto rot = RotateBy::create(1.0f,45);
	auto rep1 = RepeatForever::create(rot);
	node->runAction(rep1);
}

//------------------------------------
Bullet2OfBoss4* Bullet2OfBoss4::create()
{
	Bullet2OfBoss4* bullet = new (std::nothrow) Bullet2OfBoss4;
	auto file = GameData::getInstance()->getBulletFactory()->getResByTag(ResTag::Bullet_BossShuiPaoSmall);
	if (bullet && bullet->initWithSpriteFrameName(file))
	{
		bullet->initBody();
		bullet->autorelease();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void Bullet2OfBoss4::initBody()
{
	PhysicsBody* body = PhysicsBody::createCircle(getContentSize().width * 0.5);
	body->setGravityEnable(false);
	body->setMass(0.1);
	setPhysicsBody(body);
	setBitmask(ENEMY_AMMO_CATEGORYBITMASK, 0, ENEMY_AMMO_CONTACTTESTBITMASK);
	m_ContactPro.hurts = Ene_Blt_hurt;
}

void Bullet2OfBoss4::onEnter()
{
	Bullet::onEnter();

	setScale(0.5f);
	runAction(ScaleTo::create(1.0f,1.0f));

	MoveBezier();
}

void Bullet2OfBoss4::MoveBezier()
{
	auto pos = convertToWorldSpaceAR(Vec2::ZERO);
	auto endPos = Vec2(m_WinSize.width*0.5, -100);
	auto mainPl = GameData::getInstance()->getMainPlane();
	if (mainPl)
	{
		endPos = mainPl->convertToWorldSpaceAR(Vec2::ZERO);
	}

	//先判断当前刷的速度方向，确定贝塞尔曲线
	auto v = _physicsBody->getVelocity();
	_physicsBody->setVelocity(Vec2(0,1));

	ccBezierConfig config;
	if (v.x < 0)
	{
		config.controlPoint_1 = pos;
		config.controlPoint_2 = Vec2(pos.x - 400, pos.y + 250);
		config.endPosition = Vec2(endPos.x + 150, -100);
	}
	else
	{
		config.controlPoint_1 = pos;
		config.controlPoint_2 = Vec2(pos.x + 400, pos.y + 250);
		config.endPosition = Vec2(endPos.x - 150, -100);
	}
	
	auto mv = BezierTo::create(2.0f, config);
	auto func = CallFunc::create(this, callfunc_selector(Bullet2OfBoss4::StartFloat));
	auto seq = Sequence::create(mv, func, nullptr);
	runAction(mv);
}

void Bullet2OfBoss4::StartFloat()
{
	schedule(schedule_selector(Bullet2OfBoss4::newFalling),0.1);
}

void Bullet2OfBoss4::newFalling(float dt)
{
	_physicsBody->applyForce(Vec2(rand_minus1_1()*6.0, -rand_0_1()* 3),Vec2(0,0));
}

//------------------------------------
Bullet3OfBoss4* Bullet3OfBoss4::create()
{
	Bullet3OfBoss4* bullet = new (std::nothrow) Bullet3OfBoss4;
	auto file = GameData::getInstance()->getBulletFactory()->getResByTag(ResTag::Bullet_BossShuiPaoBig);
	if (bullet && bullet->initWithSpriteFrameName(file))
	{
		bullet->initBody();
		bullet->autorelease();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void Bullet3OfBoss4::initBody()
{
	PhysicsBody* body = PhysicsBody::createCircle(getContentSize().width * 0.5);
	body->setGravityEnable(false);
	body->setLinearDamping(0.3);
	body->setMass(0.1);
	body->setVelocity(Vec2(0,400));
	setPhysicsBody(body);
	setInvincible(true);
	setBitmask(ENEMY_AMMO_CATEGORYBITMASK, 0, ENEMY_AMMO_CONTACTTESTBITMASK);
}

void Bullet3OfBoss4::onEnter()
{
	Bullet::onEnter();
	
	setScale(0.8);
	Bullet1OfBoss4::BreathAction(this);

	setPosition(Vec2(m_WinSize.width*0.5 + rand_minus1_1() * 250, -200));

	auto delay = DelayTime::create(5.0f);
	auto func = CallFunc::create(this, callfunc_selector(Bullet3OfBoss4::expolsion));
	auto delay1 = DelayTime::create(1.5f);
	auto rm = CallFunc::create(this, callfunc_selector(PhysicsUnit::setDirectDeath));
	auto seq = Sequence::create(delay, func, delay1, rm, nullptr);
	runAction(seq);

	/*auto fadeout = FadeOut::create(7.0);
	runAction(fadeout);

	auto scale = ScaleTo::create(7.0, 2.0);
	runAction(scale);*/
}

void Bullet3OfBoss4::addWp(Node* node, int hurt)
{
	node->setVisible(false);
	auto s = node->getContentSize();
	float randScale = 0.2 * rand_minus1_1();
	for (int i = 0; i < 15; ++i)
	{
		Weapon* wp = Weapon::create(node);
		wp->setBullstType(BulletType::Enemy_default);
		wp->setBltResTag(ResTag::Bullet_BossShuiPaoSmall);
		wp->setBltGravityAble(true);
		wp->setDelayShoot(rand_0_1() * 0.05);
		wp->setBltSpeed(100 + rand_0_1() * 100);
		randScale = 0.2 * rand_minus1_1();
		wp->setBltScaleX(0.8f + randScale);
		wp->setBltScaleY(0.8f + randScale);
		wp->setBltMass(0.01);
		wp->setPosition(Vec2(s.width*0.5, 80));
		wp->setShootTimes(1);
		wp->setBltHurts(hurt);
		wp->StartShoot();
		node->addChild(wp);
		wp->setRotation(360 * rand_0_1());
	}
}

void Bullet3OfBoss4::expolsion()
{
	int hurt = getContactPro().hurts;
	addWp(this, hurt);
}

//------------------------------------
Bullet4OfBoss5* Bullet4OfBoss5::create(float dur)
{
	Bullet4OfBoss5* bullet = new (std::nothrow) Bullet4OfBoss5;
	auto file = GameData::getInstance()->getBulletFactory()->getResByTag(ResTag::Bullet_BossShuiPaoBig);
	if (bullet && bullet->initWithSpriteFrameName(file))
	{
		bullet->m_dur = dur;
		bullet->initBody();
		bullet->autorelease();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void Bullet4OfBoss5::initBody()
{
	PhysicsBody* body = PhysicsBody::createCircle(getContentSize().width * 0.5);
	body->setGravityEnable(false);
	setPhysicsBody(body);
	setInvincible(true);
	setBitmask(ENEMY_AMMO_CATEGORYBITMASK, 0, ENEMY_AMMO_CONTACTTESTBITMASK);
	m_ContactPro.hurts = Ene_Blt_hurt;

}

void Bullet4OfBoss5::onEnter()
{
	Bullet::onEnter();
	setOpacity(0);
	auto fadein = FadeIn::create(1.5 + rand_0_1() * 0.2);
	runAction(fadein);

	setScale(0.8);
	Bullet1OfBoss4::BreathAction(this);

	float t = 5.0;

	auto delay = DelayTime::create(t - 2.0 + m_dur);
	auto func = CallFunc::create(this, callfunc_selector(Bullet3OfBoss4::expolsion));
	auto delay1 = DelayTime::create(1.5f);
	auto rm = CallFunc::create(this, callfunc_selector(PhysicsUnit::setDirectDeath));
	auto seq = Sequence::create(delay, func, delay1, rm, nullptr);
	runAction(seq);

	/*auto fadeout = FadeOut::create(t);
	runAction(fadeout);

	auto scale = ScaleTo::create(t, 2.0);
	runAction(scale);*/
}

void Bullet4OfBoss5::expolsion()
{
	int hurt = getContactPro().hurts;
	Bullet3OfBoss4::addWp(this, hurt);
}

//--------------------------------
BulletMeteor* BulletMeteor::create(const std::string& file, float v)
{
	BulletMeteor* bullet = new (std::nothrow) BulletMeteor;
	if (bullet && bullet->initWithFile(file))
	{
		bullet->m_velocity = v;
		bullet->initBody();
		bullet->autorelease();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void BulletMeteor::initBody()
{
	Vec2 v = Vec2(-1,-1);
	PhysicsBody* body = PhysicsBody::createCircle(20);
	body->setGravityEnable(false);
	body->setVelocity(v * m_velocity * random(0.9f,1.0f));
	setPhysicsBody(body);
	setInvincible(true);
	setBitmask(SELF_SCOPE_CATEGORYBITMASK, 0, SELF_SCOPE_CONTACTTESTBITMASK);

	float angle = v.getAngle() * RAD_DEG;
	setRotation(-angle);
	m_ContactPro.hurts = -1000;
}

void BulletMeteor::addParticle()
{
	EffectsManager::addParticle("particla/Meteor/tuowei.plist",this, Vec2(0,0), false);

	/*int randNum = random(0,1);
	if (randNum)
	{
		EffectsManager::addParticle("particla/Meteor/lizituowei.plist",this, Vec2(0,0), false);
	}
	else
	{
		EffectsManager::addParticle("particla/Meteor/tuowei.plist",this, Vec2(0,0), false);
	}*/
}

void BulletMeteor::onEnter()
{
	Bullet::onEnter();

	addParticle();
	//randRemove();
}

void BulletMeteor::randRemove()
{
	float randNum = rand_0_1();
	if (randNum < 0.5f)
	{
		auto delay = DelayTime::create( 0.3f + rand_0_1() * 0.3f);
		auto func = CallFunc::create(this, callfunc_selector(PhysicsUnit::setDirectDeath));
		auto seq = Sequence::create(delay, func, nullptr);
		runAction(seq);
	}
}

void BulletMeteor::explosion()
{
	auto layer = UIController::getInstance()->getPhysicsLayer();
	auto pos = convertToWorldSpaceAR(Vec2::ZERO);
	EffectsManager::addParticle("particla/Meteor/baokaihuoxing.plist",layer,pos);
}

void BulletMeteor::InitEdgeLine()
{
	m_EdgeLineVec.push_back(-300);
	m_EdgeLineVec.push_back(m_WinSize.width + 60);
	m_EdgeLineVec.push_back(-300);
	m_EdgeLineVec.push_back(m_WinSize.height);
}

void BulletMeteor::DoSomethingBeforeDead()
{
	//auto pos = convertToWorldSpaceAR(Vec2::ZERO);
	//
	//if (pos.x > 50 && pos.x < m_WinSize.width - 50 &&
	//	pos.y > 100)
	//{
	//	explosion();
	//}
}


//-------------------------------
CrossBullet* CrossBullet::create( const std::string& file, Weapon* weapon )
{
	CrossBullet* bullet = new (std::nothrow) CrossBullet;
	if (bullet && bullet->initWithSpriteFrameName(file))
	{
		bullet->m_weapon = weapon;
		bullet->initBody();
		bullet->autorelease();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void CrossBullet::initBody()
{
	PhysicsBody* body = PhysicsBody::createBox(getContentSize());
	body->setGravityEnable(false);
	setPhysicsBody(body);
	setInvincible(true);
	setBitmask(SELF_SCOPE_CATEGORYBITMASK, 0, SELF_SCOPE_CONTACTTESTBITMASK);
}

void CrossBullet::onEnter()
{
	Bullet::onEnter();
	setAnchorPoint(Vec2(0.5,0));
	{
		float angle = m_weapon->getRotation();
		auto rot = RotateTo::create(0.1, angle);
		auto back = RotateTo::create(0.1, -angle);
		auto seq = Sequence::create(rot, back, nullptr);
		auto rep = RepeatForever::create(seq);
		runAction(rep);
	}
	{
		float t = 0.1 + rand_0_1() * 0.2;
		auto tint = TintTo::create(t, ccc3(255,200,0));
		auto tintBack = TintTo::create(t, 255,255,255);
		auto seq = Sequence::create(tint, tintBack, nullptr);
		auto rep = RepeatForever::create(seq);
		runAction(rep);
	}

	auto s = getContentSize();

	schedule(schedule_selector(CrossBullet::cross));
}

void CrossBullet::cross(float dt)
{
	float rot = getRotation() - 90;
	Vec2 v = _physicsBody->getVelocity();
	v.normalize();
	v = v.forAngle(-rot * DEG_RAD);
	_physicsBody->setVelocity(v * m_FlySpeed);
}

//------------------------------------------
void BulletConvolution::onEnter()
{
	Bullet::onEnter();

	auto delay = DelayTime::create(m_ConvolutionDur);
	auto func = CallFunc::create(this, callfunc_selector(BulletConvolution::BulletReverse));
	auto seq = Sequence::create(delay, func, nullptr);
	auto rep = Repeat::create(seq,m_ConvolutionTimes);
	runAction(rep);
}

void BulletConvolution::BulletReverse()
{
	auto curV = _physicsBody->getVelocity();
	auto reverseV = -curV;
	_physicsBody->setVelocity(reverseV);

	float curRot = getRotation();
	setRotation(curRot + 180);
}
