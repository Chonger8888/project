#include "EffectsManager.h"
#include "GameData.h"
#include "../GamePlaying/Layer/UIController.h"
#include "GamePlaying/Bullet/Bullet.h"
#include "GameConfig.h"
#include "GameUI/GameMainLayer.h"
#include "GamePlaying/BasePlane/PlaneManager.h"
#include "GamePlaying/Layer/PhysicsLayer.h"
#include "GamePlaying/SpecialBody.h"
#include "GamePlaying/Layer/RollingBg.h"
#include "GamePlaying/Layer/CloudLayer.h"
#include "../GamePlaying/RapidParticle/ParticleManager.h"

using namespace experimental;

void EffectsManager::displayEffectsHitTarget(const Vec2& displayPos )
{

}

void EffectsManager::displayScopeAmmo(const Vec2& displayPos )
{
	auto eff = ScopeEffect::create("weapon.png");
	eff->setPosition(displayPos);
	addChild(eff);

	ActionInterval* delay = DelayTime::create(1.1);
	CallFuncN* func = CallFuncN::create(this,callfuncN_selector(EffectsManager::removeScopeAmmo));
	ActionInterval* fadeOut = FadeOut::create(0.4);
	eff->runAction(Sequence::create(delay, fadeOut,func, nullptr));
}

void EffectsManager::removeScopeAmmo(Node* sender)
{
	PhysicsUnit* pu = (PhysicsUnit*)sender;
	if (pu)
	{
		pu->setDirectDeath();
	}
}

void EffectsManager::displayHorizontalLaser()
{
	
}

void EffectsManager::removeHorizontalLaser( Node* sender )
{
	PhysicsUnit* pu = (PhysicsUnit*)sender;
	if (pu)
	{
		pu->setDirectDeath();
	}
}

void EffectsManager::displaySlowdown( float duration, float timescale )
{
	Director::getInstance()->getScheduler()->setTimeScale(timescale);

	//由于时间被缩放，那么持续时间也应该乘以缩放比例
	ActionInterval* delay = DelayTime::create(duration*timescale);
	CallFunc* func = CallFunc::create(this,callfunc_selector(EffectsManager::resumeScheduler));

	runAction(CCSequence::create(delay,func, nullptr));
}

void EffectsManager::resumeScheduler()
{
	Director::getInstance()->getScheduler()->setTimeScale(1.0f);
}

void EffectsManager::displayAnimation(Node* parent, const char* plist, const char* animationName,bool isRepeatForever /*= false*/ )
{
	CCAnimationCache::sharedAnimationCache()->addAnimationsWithFile(plist);
	CCAnimation *animation = CCAnimationCache::sharedAnimationCache()->animationByName(animationName);
	animation->setRestoreOriginalFrame(true);

	CCAnimate *animate = CCAnimate::create(animation);
	//ActionInterval* action;
	//action = RepeatForever::create(animate);

	/*if (isRepeatForever)
	{
		action = RepeatForever::create(animate);
	}
	else
	{
		action = Repeat::create(animate,1);
	}*/

	parent->runAction(animate);
}

Animate* EffectsManager::getFramesAnimation(const std::string& qianzui, int nums, float dt,int start,int loop,bool iscache)
{
	Animation* animation = nullptr;
	if (iscache)
	{
		animation = AnimationCache::getInstance()->getAnimation(qianzui);
	}
	if (!animation)
	{
		//用CCArray存储图片对象
		Vector<SpriteFrame*> frame_array;

		//从缓存读取图片对象
		for (int i = start ; i < nums; ++i)
		{
			std::string pic = CCString::createWithFormat("%s%d.png",qianzui.c_str(),i)->getCString();
			CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pic.c_str());
			frame_array.pushBack(frame);
		}

		//生成一个CCAnimation对象
		animation = CCAnimation::createWithSpriteFrames(frame_array,dt);
		animation->setLoops(loop);

		AnimationCache::getInstance()->addAnimation(animation, qianzui);
	}

	//开始创建动画
	Animate* animate =  Animate::create(animation) ;
	return animate;
}

Animate* EffectsManager::getFramesAnimation( const std::string& qianzui, float dt, int loop /*= 1*/,bool iscache /*= true*/ )
{
	Animation* animation = nullptr;
	if (iscache)
	{
		animation = AnimationCache::getInstance()->getAnimation(qianzui);
	}
	if (!animation)
	{
		//用CCArray存储图片对象
		Vector<SpriteFrame*> frame_array;
		int i = 0;
		do 
		{
			std::string pic = CCString::createWithFormat("%s%d.png",qianzui.c_str(),i++)->getCString();
			CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pic.c_str());
			if (frame == nullptr)
			{
				break;
			}
			frame_array.pushBack(frame);
		} while (true);

		//生成一个CCAnimation对象
		animation = CCAnimation::createWithSpriteFrames(frame_array,dt);
		animation->setLoops(loop);

		AnimationCache::getInstance()->addAnimation(animation, qianzui);
	}

	//开始创建动画
	Animate* animate =  Animate::create(animation) ;
	return animate;
}

void EffectsManager::removeAnimation( Node* node )
{
	if (node)
	{
		node->removeFromParentAndCleanup(true);
	}
}

void EffectsManager::playLightAnimation( const Point& startPos, const Point& endPos )
{
	Sprite* node = Sprite::create("weapon.png");
	addChild(node,-1);

	node->setPosition(ccp(startPos.x,startPos.y));
	node->setAnchorPoint(ccp(0.5,0));

	//计算旋转角度
	Vec2 direc = endPos - startPos;
	float angle = direc.getAngle()*Pi2Angle;
	node->setRotation(-angle + 90);

	//计算缩放比例
	float dis = startPos.distance(endPos);
	//动画的长度
	float AnimationWidth = 1280;
	float bili = dis/AnimationWidth * 1.25;
	node->setScaleY(bili);
	node->setScaleX(0.5);

	ActionInterval* animate = getFramesAnimation("light_", 5, 0.01);
	ActionInterval* fadeOut = FadeOut::create(0.2);
	CallFuncN* func = CallFuncN::create(this,callfuncN_selector(EffectsManager::removeAnimation));

	node->runAction(Sequence::create(animate,fadeOut,func,nullptr));
}

void EffectsManager::playLaserAnimation1(Sprite* sp)
{
	ActionInterval* animate = getFramesAnimation("dazhao_", 48, 0.1);
	sp->runAction(animate);
}

void EffectsManager::playLaserAnimation2(Sprite* sp)
{
	ActionInterval* animate = getFramesAnimation("dazhao_", 48, 0.1, 17);
	sp->runAction(RepeatForever::create(animate));
}

void EffectsManager::displaySlowDownEffect(const Vec2& displayPos)
{
	
}

void EffectsManager::playExplosionAnimation( Sprite* sp )
{
	float timeScale = Director::getInstance()->getScheduler()->getTimeScale();
	ActionInterval* animate = getFramesAnimation("pengbao_", 6, 0.1*timeScale);
	CallFuncN* funcN = CallFuncN::create(this,callfuncN_selector(EffectsManager::removeScopeAmmo));
	ActionInterval* seq = Sequence::create(animate,funcN,nullptr);
	sp->runAction(seq);
}

void EffectsManager::displayDeathExplosion( const Point& pos )
{
	auto sp = Sprite::create("weapon.png");
	sp->setPosition(pos);
	sp->setScale(3.0f + rand_0_1() * 2.0f);
	sp->setRotation(rand_0_1() * 360.0f);
	UIController::getInstance()->getPhysicsLayer()->addChild(sp);

	auto animat = getFramesAnimation("PlaneDeadExpl_", 17, 1.0f/24);
	auto rm = RemoveSelf::create(true);
	auto seq = Sequence::create(animat, rm, nullptr);
	sp->runAction(seq);

}

void EffectsManager::playSpecialLightAnimation( const Point& startPos, const Point& endPos )
{
	Sprite* node = Sprite::create("weapon.png");
	addChild(node,-1);

	node->setPosition(ccp(startPos.x,startPos.y));
	node->setAnchorPoint(ccp(0.5,0));

	//计算旋转角度
	Vec2 direc = endPos - startPos;
	float angle = direc.getAngle()*Pi2Angle;
	node->setRotation(-angle + 90);

	//计算缩放比例
	float dis = startPos.distance(endPos);
	//动画的长度
	float AnimationHeight = 556;
	float bili = dis/AnimationHeight * 1.25;
	node->setScaleY(bili);
	node->setScaleX(0.5);

	ActionInterval* animate = getFramesAnimation("cutisd_", 9, 0.01);
	ActionInterval* fadeOut = FadeOut::create(0.2);
	CallFuncN* func = CallFuncN::create(this,callfuncN_selector(EffectsManager::removeAnimation));

	node->runAction(Sequence::create(animate,fadeOut,func,nullptr));
}

void EffectsManager::playElectricAnimation( Sprite* sp )
{
	ActionInterval* animate = getFramesAnimation("diangun_", 16, 0.05);

	sp->runAction(animate);
}

void EffectsManager::displayEletricCircle( Sprite* node )
{
	
}

void EffectsManager::playBounceWallBeginAnimation( Sprite* sp )
{
	ActionInterval* animate = getFramesAnimation("qianshe_", 6, 0.05);

	sp->runAction(animate);
}

void EffectsManager::playBounceWallRepeatAnimation( Sprite* sp )
{
	ActionInterval* animate = getFramesAnimation("houshe_", 12, 0.05,0,100);

	sp->runAction(animate);
}

void EffectsManager::playRecyclePadAnimation( Sprite* sp )
{
	ActionInterval* animate = getFramesAnimation("mmdhb_", 13, 0.05, 0, 1000);

	sp->runAction(animate);
}

void EffectsManager::displayBurningExplosion( const Vec2& pos )
{
}

void EffectsManager::playFireBirdAnimation( Sprite* sp )
{
	ActionInterval* animate = getFramesAnimation("huoniao_", 7, 0.05, 0, 1000,true);

	sp->runAction(animate);
}

void EffectsManager::playWindEnemyContactAnimation( Node* node )
{
	Sprite* sp = AnimationSprite(node);

	ActionInterval* animate = getFramesAnimation("huangfeng_", 11, 0.05, 0, 1);
	CallFuncN* removeFunc = CallFuncN::create(this,callfuncN_selector(EffectsManager::removeAnimation));
	ActionInterval* seq = Sequence::create(animate,removeFunc,nullptr);
	sp->runAction(seq);
}

Sprite* EffectsManager::AnimationSprite( Node* node )
{
	Sprite* sp = Sprite::create("weapon.png");
	sp->setPosition(ccp(node->getContentSize().width*0.5,node->getContentSize().height*0.5));
	node->addChild(sp);

	return sp;
}

void EffectsManager::playTornadoAnimation( Node* node )
{
	float timeScale = Director::getInstance()->getScheduler()->getTimeScale();

	Sprite* sp = AnimationSprite(node);
	//ActionInterval* animate = getFramesAnimation("longjf_", 8, 0.05, 0, 500000);
	ActionInterval* animate = getFramesAnimation("longjuanf_", 5, 0.05 * timeScale, 0, 500000,true);

	sp->runAction(animate);
}

void EffectsManager::playBurningAnimation( Node* node )
{
	Sprite* sp = AnimationSprite(node);

	ActionInterval* animate = getFramesAnimation("zhoushao_", 7, 0.1, 1, 500000);
	sp->runAction(animate);
}

void EffectsManager::playFireBallNormalAnimation(Node* node)
{
	ActionInterval* animate = getFramesAnimation("huoqiuyiji_", 4, 0.1, 1, 500000);
	node->runAction(animate);
}

void EffectsManager::playFireBallEnergyFullAnimation( Node* node )
{
	ActionInterval* animate = getFramesAnimation("huoqiuerji_", 7, 0.05, 1, 500000);
	node->runAction(animate);
}

void EffectsManager::playElectricBallNormalAnimation( Node* node )
{
	ActionInterval* animate = getFramesAnimation("dqyiji_", 10, 0.05, 1, 500000);
	node->runAction(animate);
}

void EffectsManager::playElectricBallEnergyFullAnimation( Node* node )
{
	ActionInterval* animate = getFramesAnimation("dqeji_", 10, 0.05, 1, 500000);
	node->runAction(animate);
}

void EffectsManager::playElectricContactAnimation( Node* node )
{
	Sprite* sp = AnimationSprite(node);

	ActionInterval* animate = getFramesAnimation("landian_", 10, 0.05, 0, 1);
	CallFuncN* removeFunc = CallFuncN::create(this,callfuncN_selector(EffectsManager::removeAnimation));
	ActionInterval* seq = Sequence::create(animate,removeFunc,nullptr);
	sp->runAction(seq);

}

Animate* EffectsManager::getMainPl1Animation_N( )
{
	Animate* animate = getFramesAnimation("Lead_wind_", 8, 1.0f/8, 0, 50000000);
	return animate;
}

Animate* EffectsManager::getMainPl1Animation_S( )
{
	Animate* animate = getFramesAnimation("leadwind_max_", 8, 0.1, 0, 50000000);
	return animate;
}

Animate* EffectsManager::getMainPl2Animation_N( )
{
	Animate* animate = getFramesAnimation("lead_firepu_", 8, 0.1, 0, 50000000);
	return animate;
}

Animate* EffectsManager::getMainPl2Animation_S( )
{
	Animate* animate = getFramesAnimation("Lead_Maxele_", 8, 0.1, 0, 50000000);
	return animate;
}

Animate* EffectsManager::getMainPl3Animation_N( )
{
	Animate* animate = getFramesAnimation("Lead_puele_", 8, 0.1, 0, 50000000);
	return animate;
}

Animate* EffectsManager::getMainPl3Animation_S( )
{
	Animate* animate = getFramesAnimation("lead_firemax_", 8, 0.1, 0, 50000000);
	return animate;
}

ActionInterval* EffectsManager::playHoodleAnimation()
{
	ActionInterval* animate = getFramesAnimation("Deformation_", 10, 0.1);
	return animate;
}

void EffectsManager::displayQuxianLaserAnimation( Node* node )
{
	ActionInterval* animate = getFramesAnimation("liuguang_", 6, 0.1,0,1000000,true);
	node->runAction(animate);
}

void EffectsManager::ScreenShake(Node* node,float time)
{
	//每次抖动的时间
	float eaceTime = 0.03f + rand_0_1() * 0.02f;

	//总共抖动次数
	int nums = time / (eaceTime * 2);
	if (time < 0.0f)
	{
		nums = cocos2d::random(7,10);
	}

	//移动
	ActionInterval* move = MoveBy::create(eaceTime,
		Vec2(250 * (cocos2d::random(1,2) - 1.5) + rand_minus1_1() * 20,
		0));
	ActionInterval* moveBack = move->reverse();
	ActionInterval* seq1 = Sequence::create(move,moveBack,nullptr);
	ActionInterval* rep1 = Repeat::create(seq1,nums);

	node->runAction(rep1);
}

void EffectsManager::NodeShake( Node* node, float time /*= -0.1f*/ )
{
	//每次抖动的时间
	float eaceTime = 0.04f;

	//总共抖动次数
	int nums = time / (eaceTime * 2);
	if (time < 0.0f)
	{
		nums = cocos2d::random(7,10);
	}
	//移动
	ActionInterval* move = MoveBy::create(eaceTime,
		Vec2(20 + rand_0_1() * 10,0 ));
	ActionInterval* moveBack = move->reverse();
	ActionInterval* seq1 = Sequence::create(move,moveBack,nullptr);
	ActionInterval* rep1 = Repeat::create(seq1,nums);

	node->runAction(rep1);
}

ParticleSystemQuad* EffectsManager::addParticle( const std::string& file, Node* parent,const Vec2& pos, bool autoRM, float t)
{
	//log("+++ start particle = %s",file.c_str());
	auto emitter = ParticleManager::getInstance()->getParticle(file);
	//log("--- end particle = %s",file.c_str());
	emitter->setAutoRemoveOnFinish(true);
	emitter->setPosition(pos);
	parent->addChild(emitter, 10);

	//auto node = ParticleBatchNode::createWithTexture(emitter->getTexture());//貌似次batchnode没有达到降低渲染效果；应该在外部创建一个，而不是每次创建一个batchnode
	//node->setPosition(pos);
	//node->addChild(emitter, 100);
	//parent->addChild(node, 10);

	if (autoRM)
	{
		if (t <= 0.0f)
		{
			t = emitter->getLife() + emitter->getLifeVar();
		}
		//释放
		ActionInterval* delay = DelayTime::create(t + 0.2f);
		CallFuncN* funcN = CallFuncN::create(nullptr,callfuncN_selector(EffectsManager::removeAnimation));
		ActionInterval* seq = Sequence::create(delay, funcN, nullptr);
		emitter->runAction(seq);
	}
	
	return emitter;
}

void EffectsManager::displayAniOfWhiteCat(Node* node)
{
	Sprite* sp = Sprite::create("weapon.png");
	node->addChild(sp);
	sp->setFlippedX(true);
	sp->setTag(20150325);
	sp->setPosition(Vec2(-30,0));
	ActionInterval* animate = getFramesAnimation("WhiteCat_", 4, 0.1,0,1000000,true);
	sp->runAction(animate);
}

void EffectsManager::playActionOfMainPlDead( BasePlane* node, ActionInterval* func )
{
	if (!node)
	{
		return;
	}

	//主角死亡效果:同时进行移动、缩小、旋转、变暗动作
	auto pos = node->convertToWorldSpaceAR(Vec2::ZERO);
	float offsetX = (random(1,2)-1.5f) * 400;
	auto pos1 = Vec2(pos.x + offsetX, pos.y + 150);

	ccBezierConfig config;
	config.controlPoint_1 = Vec2(pos1);
	config.controlPoint_2 = Vec2(pos.x + offsetX * 1.5, pos1.y - 500);
	config.endPosition = Vec2(pos.x + offsetX * 2, pos1.y - 1000);
	ActionInterval* bzb = BezierTo::create(2.0f,config);
	ActionInterval* move = MoveBy::create(2.0f,Vec2(rand_minus1_1() * 200,rand_minus1_1() *100 ));
	ActionInterval* scale = ScaleTo::create(2.0f,0.3);
	ActionInterval* rotate = RotateBy::create(2.0f,720);
	ActionInterval* tint = TintTo::create(2.0f,100,50,50);
	ActionInterval* spaw = Spawn::create(bzb,scale,rotate,tint,nullptr);
	ActionInterval* seq = Sequence::create(spaw,func,nullptr);
	node->runAction(seq);
}

void EffectsManager::playEffGetRewards( int tp )
{
	auto pl = GameData::getInstance()->getMainPlane();
	if (!pl)
	{
		return;
	}
	std::string file1;
	std::string file2;
	switch (tp)
	{
	case 1:
		{
			//红球
			file1 = "Effect/getRewards/red1.png";
			file2 = "Effect/getRewards/red2.png";
		}
		break;
	case 2:
		{
			//黄球
			file1 = "Effect/getRewards/yellow1.png";
			file2 = "Effect/getRewards/yellow2.png";
		}
		break;
	case 3:
		{
			//蓝球
			file1 = "Effect/getRewards/blue1.png";
			file2 = "Effect/getRewards/blue2.png";
		}
		break;
	default:
		break;
	}

	auto sp1 = Sprite::create(file1);
	auto sp2 = Sprite::create(file2);

	//同时出现，不同时刻消失（2前1后）
	{
		ActionInterval* scale = ScaleTo::create(0.1f,5);
		ActionInterval* scaleBack = ScaleTo::create(0.5,0.2);
		ActionInterval* fadeout = FadeOut::create(0.5);
		ActionInterval* spw= Spawn::create(scaleBack, fadeout, nullptr);
		CallFuncN* func = CallFuncN::create(this,callfuncN_selector(EffectsManager::removeAnimation));
		ActionInterval* seq = Sequence::create(scale, spw, func, nullptr);
		sp1->runAction(seq);

		ActionInterval* rotate = RotateBy::create(0.5f,30);
		ActionInterval* rep = RepeatForever::create(rotate);
		sp1->runAction(rep);
	}
	{
		ActionInterval* scale = ScaleTo::create(0.1f,3);
		ActionInterval* scaleBack = ScaleTo::create(0.3,0.2);
		ActionInterval* fadeout = FadeOut::create(0.3);
		ActionInterval* spw= Spawn::create(scaleBack, fadeout, nullptr);
		CallFuncN* func = CallFuncN::create(this,callfuncN_selector(EffectsManager::removeAnimation));
		ActionInterval* seq = Sequence::create(scale, spw, func, nullptr);
		sp2->runAction(seq);
	}

	auto s = pl->getContentSize();
	sp1->setPosition(s.width*0.5, s.height*0.8);
	pl->addChild(sp1,1);
	sp1->setOpacity(200);

	sp2->setPosition(s.width*0.5, s.height*0.8);
	pl->addChild(sp2,-2);


}

void EffectsManager::displayLightingEff( Node* parent )
{
	if (parent)
	{
		auto s = parent->getContentSize();

		auto node = Sprite::create("weapon.png");
		node->setPosition(s.width*0.5,s.height*0.5);
		parent->addChild(node);
		node->setRotation(360 * rand_0_1());

		ActionInterval* animate = getFramesAnimation("lightingEff_", 3, 1.0/72, 0, 1,true);
		CallFuncN* func = CallFuncN::create(this,callfuncN_selector(EffectsManager::removeAnimation));

		node->runAction(Sequence::create(animate,func,nullptr));
	}
}

void EffectsManager::PlaneCantTouchAble( float t )
{
	auto pl = GameData::getInstance()->getMainPlane();
	if (pl)
	{
		pl->setTouchAble(false);
		auto delay = DelayTime::create(t);
		auto func = CallFunc::create(this, callfunc_selector(EffectsManager::PlaneResumeTouch));
		auto seq = Sequence::create(delay, func, nullptr);
		runAction(seq);
	}
}

void EffectsManager::PlaneResumeTouch()
{
	auto pl = GameData::getInstance()->getMainPlane();
	if (pl)
	{
		pl->setTouchAble(true);
	}
}


//--------------
ScopeEffect* ScopeEffect::create(const char* file, const Size& size)
{
	ScopeEffect* eu = new (std::nothrow) ScopeEffect;
	if (eu && eu->initWithFile(file))
	{
		eu->m_expSize = size;
		eu->initSelfPro();
		eu->initBody();
		eu->autorelease();
		return eu;
	}
	else
	{
		CC_SAFE_DELETE(eu);
		return nullptr;
	}
}

void ScopeEffect::initBody()
{
	auto body = PhysicsBody::createBox(m_expSize);
	body->setGravityEnable(false);
	body->setDynamic(false);
	body->setRotationEnable(false);
	body->setMass(10000);
	body->setCategoryBitmask(SELF_EXPLOSION_CATEGORYBITMASK);
	body->setCollisionBitmask(0);
	body->setContactTestBitmask(SELF_EXPLOSION_CONTACTTESTBITMASK);
	setPhysicsBody(body);
	setInvincibleAtLine();		//检查过，后面有删除处理
}

void ScopeEffect::initSelfPro()
{
	m_ContactPro.hurts = -1000;
	m_ContactPro.effectsid = 0;

	m_SelfPro.maxHP = 1000000;
	m_SelfPro.curHP = m_SelfPro.maxHP;
	m_SelfPro.isInvincible = 0;
	m_SelfPro.score = 0;
	setUnitId(UnitId::eScopeExplosion);
}

void ScopeEffect::FuncAfterEnter()
{
	ActionInterval* delay = DelayTime::create(1.0f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(PhysicsUnit::setDirectDeath));
	ActionInterval* seq = Sequence::create(delay, func, nullptr);
	runAction(seq);
}

//----------------------------------
BloodDecreaseTTF* BloodDecreaseTTF::create( Node* targert, int bloodNum )
{
	BloodDecreaseTTF* ttf = new (std::nothrow) BloodDecreaseTTF;
	if (ttf && ttf->init())
	{
		ttf->autorelease();
		ttf->addTTF(targert,bloodNum);
		return ttf;
	}
	else
	{
		CC_SAFE_DELETE(ttf);
		return nullptr;
	}
}

void BloodDecreaseTTF::addTTF( Node* targert,const int& num )
{
	std::string str = String::createWithFormat("%d",num)->getCString();
	Label* label = Label::create(str.c_str(),"",30);
	label->setPosition(0,0);
	addChild(label);
	label->setColor(ccc3(254,254,65));

	//执行动作
	ActionInterval* move;
	int randnum = cocos2d::random(0,1);
	if (randnum)
	{
		move = MoveBy::create(2.0f,ccp(50,50));
	}
	else
	{
		move = MoveBy::create(2.0f,ccp(-50,50));
	}
	CallFunc* func = CallFunc::create(this,callfunc_selector(BloodDecreaseTTF::removeTTF));
	ActionInterval* seq = Sequence::create(move, func, nullptr);

	runAction(seq);
}

void BloodDecreaseTTF::removeTTF()
{
	this->removeFromParentAndCleanup(true);
}

ActionInterval* ShanshuoAction(const float& time )
{
	ActionInterval* fadeOut = TintTo::create(time,255,255,255);
	ActionInterval* fadein = TintTo::create(time,255,0,0);
	ActionInterval* seq = Sequence::create(fadein,fadeOut,fadein,fadeOut,nullptr);
	return seq;
}

//----------------------------------
BigLighting* BigLighting::create()
{
	BigLighting* light = new (std::nothrow) BigLighting;
	if (light && light->init())
	{
		light->autorelease();
		light->initSelfPro();
		return light;
	}
	else
	{
		CC_SAFE_DELETE(light);
		return nullptr;
	}
}

void BigLighting::initBody()
{
	auto body = PhysicsBody::createBox(Size(m_WinSize.width, m_WinSize.height*0.5));
	body->setGravityEnable(false);
	body->setDynamic(false);
	body->setRotationEnable(false);
	body->setMass(10000);
	body->setCategoryBitmask(SELF_EXPLOSION_CATEGORYBITMASK);
	body->setCollisionBitmask(SELF_EXPLOSION_COLLISIONBITMASK);
	body->setContactTestBitmask(SELF_EXPLOSION_CONTACTTESTBITMASK);
	setPhysicsBody(body);
	setInvincibleAtLine();//检查过，后面有删除处理
}

void BigLighting::initSelfPro()
{
	m_ContactPro.hurts = -70000;

	m_SelfPro.isInvincible = 1;
}

void BigLighting::FuncAfterEnter()
{
	ActionInterval* delay = DelayTime::create(0.1);
	CallFunc* func = CallFunc::create(this, callfunc_selector(BigLighting::removeCallback));
	ActionInterval* seq = Sequence::create(delay, func, nullptr);
	runAction(seq);
}

void BigLighting::removeCallback()
{
	setDirectDeath();
}





//--------------------------
void LightingLayer::onEnter()
{
	Layer::onEnter();
	m_fadeIntime = 1.0f;
}

void LightingLayer::addEffect()
{
	Size ws = Director::getInstance()->getWinSize();

	int randNum = cocos2d::random(1,3);

	auto file = __String::createWithFormat("Effect/Lighting/lighting%d.png",randNum)->getCString();

	Sprite* sp = Sprite::create(file);
	sp->setPosition(ccp(ws.width * 0.5, ws.height * 1.05));
	sp->setAnchorPoint(ccp(0.5,1.0));

	float rotate = 0;
	switch (randNum)
	{
	case 1:
		rotate = 20;
		break;
	case 2:
		rotate = 0;
		break;
	case 3:
		rotate = -20;
		break;
	default:
		break;
	}
	sp->setRotation(rotate + rand_minus1_1() * 15);
	sp->setScale(1.5);
	addChild(sp);

	ActionInterval* fadeIn = FadeOut::create(m_fadeIntime);
	CallFuncN* func = CallFuncN::create(this,callfuncN_selector(LightingLayer::effectCallback));
	ActionInterval* seq = Sequence::create(fadeIn, func, nullptr);
	sp->runAction(seq);
}

void LightingLayer::effectCallback(Node* node)
{
	node->removeFromParentAndCleanup(true);
}

void LightingLayer::addLight()
{
	Size ws = Director::getInstance()->getWinSize();
	BigLighting* light = BigLighting::create();
	light->setPosition(ws.width*0.5,ws.height*0.75);
	addChild(light);

	addEffect();
}

void LightingLayer::Lighting(const int& times )
{
	ActionInterval* delay = DelayTime::create(0.5);
	ActionInterval* delay2 = DelayTime::create(m_fadeIntime);
	CallFunc* func = CallFunc::create(this,callfunc_selector(LightingLayer::addLight));
	ActionInterval* seq = Sequence::create(func,delay,nullptr);
	ActionInterval* repeat = Repeat::create(seq,times);
	CallFunc* removefunc = CallFunc::create(this,callfunc_selector(LightingLayer::lightCallback));
	ActionInterval* seq1 = Sequence::create(repeat,delay,delay2,removefunc,nullptr);
	runAction(seq1);

	
}

void LightingLayer::lightCallback()
{
	removeFromParentAndCleanup(true);
}

//陨石
FallingStone* FallingStone::create()
{
	FallingStone* fs = new (std::nothrow) FallingStone;
	if (fs && fs->initWithFile("Effect/fallingStone/stone.png"))
	{
		fs->initSelfPro();
		fs->initBody();
		fs->autorelease();
		return fs;
	}
	else
	{
		CC_SAFE_DELETE(fs);
		return nullptr;
	}
}

void FallingStone::initBody()
{
	auto body = PhysicsBody::createBox(Size(getContentSize().width * 0.9, getContentSize().height*0.9));
	body->setGravityEnable(false);
	body->setRotationEnable(false);
	body->setMass(10000);
	body->setCategoryBitmask(ENEMY_AMMO_CATEGORYBITMASK);
	body->setCollisionBitmask(SELF_AMMO_COLLISIONBITMASK_P);
	body->setContactTestBitmask(ENEMY_AMMO_CONTACTTESTBITMASK);
	setPhysicsBody(body);
}

void FallingStone::initSelfPro()
{
	m_ContactPro.hurts = FallSton_hurt;
	m_SelfPro.isInvincible = 1;
}

void FallingStone::FuncAfterEnter()
{
	setScale(1.3);

	warning();

	//auto fire = EffectsManager::addParticle("particla/FallingStone/FallingStone.plist",this,
	//	Vec2(getContentSize().width*0.5,getContentSize().height*0.5),false);
	//if (fire)
	//{
	//	//fire->setPositionType(ParticleSystem::PositionType::GROUPED);
	//	//fire->setScale(3.0f);
	//}
	EffectsManager::addParticle("particla/FallingStone/par_FallingStone_tail.plist",this,
		Vec2(getContentSize().width*0.5,getContentSize().height*0.5),false);
}

void FallingStone::warning()
{
	auto laser = Sprite::create("Effect/fallingStone/warn.png");
	laser->setPosition(Vec2(getContentSize().width*0.5,0));
	laser->setScaleY(100);
	laser->setScaleX(0.15);
	laser->setOpacity(0);
	addChild(laser);

	//渐变 + 缩放
	ActionInterval* fadeIn = FadeIn::create(0.1f);
	ActionInterval* fadeout = FadeOut::create(0.1f);
	ActionInterval* seq = Sequence::create(fadeIn, fadeout, nullptr);
	ActionInterval* rep = Repeat::create(seq,10);
	ActionInterval* ease = EaseExponentialIn::create(rep);
	//ActionInterval* ease = EaseSineIn::create(rep);
	laser->runAction(ease);

	ActionInterval* delay1 = DelayTime::create(2.0f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(FallingStone::falling));
	ActionInterval* seq2 = Sequence::create(delay1,func,nullptr);
	laser->runAction(seq2);
}

void FallingStone::falling()
{
	_physicsBody->setVelocity(Vec2(0,-1700));

	//拖尾
	auto tail = Sprite::create("Effect/fallingStone/stonetail.png");
	tail->setPosition(getContentSize().width * 0.3,-50);
	tail->setAnchorPoint(Vec2(0.5,0));
	tail->setScaleY(m_WinSize.height*0.5/tail->getContentSize().height * 2);
	addChild(tail);

	ActionInterval* fadein = FadeOut::create(1.0f);
	tail->runAction(fadein);
}

void FallingStone::InitEdgeLine()
{
	m_EdgeLineVec.push_back(-60);
	m_EdgeLineVec.push_back(m_WinSize.width + 60);
	m_EdgeLineVec.push_back(-1280);
	m_EdgeLineVec.push_back(m_WinSize.height);
}



//--------------------------
void ExplosionSprite::onEnter()
{
	Node::onEnter();
	displayParticle();
	setRotation(rand_minus1_1() * 20);
	runAction(RepeatForever::create(RotateBy::create(1.0f + rand_0_1() * 0.3f,rand_minus1_1() * 200)));
}

void ExplosionSprite::RemovePartical( Node* node )
{
	node->removeFromParentAndCleanup(true);
}

void ExplosionSprite::displayParticle()
{
	//按顺序播放
	PartEff1();

	ActionInterval* intervval2 = DelayTime::create(0.5f);
	CallFunc* eff2 = CallFunc::create(this,callfunc_selector(ExplosionSprite::PartEff2));
	ActionInterval* seq2 = Sequence::create(intervval2,eff2,nullptr);
	runAction(seq2);

	ActionInterval* intervval3 = DelayTime::create(1.3f);
	CallFunc* eff3 = CallFunc::create(this,callfunc_selector(ExplosionSprite::PartEff3));
	ActionInterval* seq3 = Sequence::create(intervval3,eff3,nullptr);
	runAction(seq3);

	ActionInterval* delayRemove = DelayTime::create(10.0f);
	CallFuncN* funcN = CallFuncN::create(this,callfuncN_selector(ExplosionSprite::RemovePartical));
	ActionInterval* seq = Sequence::create(delayRemove,funcN,nullptr);
	runAction(seq);
}

void ExplosionSprite::PartEff1()
{
	EffectsManager::addParticle("particla/BossDeadExpl/1/test0.plist",this,Vec2(0,0), false);
}

void ExplosionSprite::PartEff2()
{
	EffectsManager::addParticle("particla/BossDeadExpl/2/test1.plist",this,Vec2(0,0),false);
	EffectsManager::addParticle("particla/BossDeadExpl/2/test2.plist",this,Vec2(0,0),false);
	EffectsManager::addParticle("particla/BossDeadExpl/2/test3.plist",this,Vec2(0,0),false);
}

void ExplosionSprite::PartEff3()
{
	shake();
	clearAllEnemy();
	EffectsManager::addParticle("particla/BossDeadExpl/3/test4.plist",this,Vec2(0,0),false);
}

void ExplosionSprite::shake()
{
	NotificationCenter::getInstance()->postNotification(Msg_Shake,nullptr);
}

void ExplosionSprite::clearAllEnemy()
{
	auto s = Director::getInstance()->getWinSize();
	auto layer = UIController::getInstance()->getEffectsLayer();
	ScopeEffect* eff = ScopeEffect::create("weapon.png", s);
	eff->setPosition(s.width*0.5,s.height*0.5);
	layer->addChild(eff);
}


//----------------------------
void WarningOfBoss::onEnter()
{
	Layer::onEnter();

	RollSp(true);
	RollSp(false);
	MidWenzi();
	ScreenRed();

	ActionInterval* delay = DelayTime::create(2.0f);
	CallFunc* func = CallFunc::create(this,callfunc_selector(WarningOfBoss::removeWarning));
	ActionInterval* seq = Sequence::create(delay, func, nullptr);
	runAction(seq);
}

void WarningOfBoss::RollSp( bool isUp )
{
	auto size = Director::getInstance()->getWinSize();
	Vec2 startPos;
	if (isUp)
	{
		startPos = Vec2(size.width - 25,size.height);
	}
	else
	{
		startPos = Vec2(25,0);
	}
	std::string file = "Effect/BossIn/biankuang.png";
	auto StartSp = Sprite::create(file);
	StartSp->setPosition(startPos);
	addChild(StartSp);

	auto mySize = StartSp->getContentSize();
	for (int i = 1; i < 40; i++)
	{
		auto sp = Sprite::create(file);
		if (isUp)
		{
			sp->setPosition(mySize.width*0.5,mySize.height*0.5 - (mySize.height*i) );
		}
		else
		{
			sp->setPosition(mySize.width*0.5,mySize.height*0.5 + (mySize.height*i) );
		}
		StartSp->addChild(sp);
	}
	
	ActionInterval* mv;
	if (isUp)
	{
		mv = MoveBy::create(2.0f,Vec2(0,500));
	}
	else
	{
		mv = MoveBy::create(2.0f,Vec2(0,-500));
	}
	StartSp->runAction(mv);
}

void WarningOfBoss::MidWenzi()
{
	auto ws = Director::getInstance()->getWinSize();
	auto sp = Sprite::create("Effect/BossIn/wenzi.png");
	sp->setPosition(ws.width*0.5,ws.height*0.65);
	addChild(sp);

	ActionInterval* fadein = FadeIn::create(0.5);
	ActionInterval* fadeout = FadeOut::create(0.5);
	ActionInterval* seq = Sequence::create(fadein, fadeout, nullptr);
	ActionInterval* rep = RepeatForever::create(seq);
	sp->runAction(rep);
}

void WarningOfBoss::ScreenRed()
{
	auto layer = LayerColor::create(ccc4(255,0,0,20));
	addChild(layer,-1);
}

void WarningOfBoss::removeWarning()
{
	removeFromParentAndCleanup(true);
}



//----------------------------
void RunEllipticAction(Node* node,const EllipticFunc::EllipticConfig& ec, float time,bool isleft)
{
	if (node)
	{
		float a = ec.aLength;
		float c = ec.cLength;
		float x = EllipticFunc::tuoyuanXat(a, c, time,isleft);//调用之前的坐标计算函数来计算出坐标值
		float y = EllipticFunc::tuoyuanYat(a, c, time);
		node->setPosition(Vec2(x, y) + ec.conterPos);
	}
}

cocos2d::Point MoveStep( const Point& Amipos, const Point& myPos,const float& maxPix )
{
	//方向
	Vec2 direc = Amipos - myPos;
	float dis = myPos.distance(Amipos);

	//这步处理：距离越远，单位时间移动的距离越远，反之单位时间移动的距离越近（单位时间移动距离范围0~maxPix-1）
	float move = maxPix - maxPix/(dis/maxPix + 1);		
	move = move * Director::getInstance()->getScheduler()->getTimeScale();

	direc.normalize();
	direc = direc * move;		
	Point newPos = myPos + direc;
	return newPos;
}

void MeteorLayer::onEnter()
{
	Layer::onEnter();

	m_num = 0;

	StartMeteor();

	auto bgLayer = UIController::getInstance()->getGameBgMgr();
	UIController::getInstance()->getEffectsLayer()->ScreenShake(bgLayer, 6.0f);

	//持续几秒
	auto delay = DelayTime::create(6.0f);
	auto rm = CallFunc::create(this,callfunc_selector(MeteorLayer::RemoveMe));
	auto seq = Sequence::create(delay, rm, nullptr);
	runAction(seq);
}

void MeteorLayer::RemoveMe()
{
	UIController::getInstance()->getMainUILayer()->startDaZhaoSD();
	removeFromParentAndCleanup(true);
}

void MeteorLayer::StartMeteor()
{
	auto ws = Director::getInstance()->getWinSize();

	int randNum = random(1,5);
	int v = 800 + randNum * 100;
	int randScale = 2.0f + randNum * 0.5;

	BulletMeteor* bullet = BulletMeteor::create("weapon.png",v);
	bullet->setPosition(ws.width * 1.2, ws.height * random(0.1,1.5) );
	bullet->setScale(randScale);
	UIController::getInstance()->getPhysicsLayer()->addChild(bullet);

	auto delay = DelayTime::create(0.03 + rand_0_1() * 0.03);
	auto next = CallFunc::create(this, callfunc_selector(MeteorLayer::StartMeteor));
	auto seq = Sequence::create(delay, next, nullptr);
	runAction(seq);
}


//--------------------------
void TalismanLayer::onEnter()
{
	Node::onEnter();
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(TalismanLayer::StopTalisman),Msg_StopMainPlBullet, nullptr);

	setScale(1.2);
	setCascadeOpacityEnabled(true);

	m_ws = Director::getInstance()->getWinSize();
	m_TextId = 0;

	auto delay = DelayTime::create(m_dur - 1.5);
	auto func = CallFunc::create(this, callfunc_selector(TalismanLayer::StopText));
	auto fadeout = FadeOut::create(1.5f);
	auto rm = CallFunc::create(this,callfunc_selector(TalismanLayer::RemoveMe));
	auto seq = Sequence::create(delay, func, fadeout, rm, nullptr);
	runAction(seq);

	RotateSurround();
	//MidText();

	auto s = Director::getInstance()->getWinSize();
	AttackContinuous* attack = AttackContinuous::createBox(Size(3000,3000), m_dur, -100,
		SELF_SCOPE_CATEGORYBITMASK, 0, SELF_SCOPE_CONTACTTESTBITMASK);
	addChild(attack);
}

void TalismanLayer::onExit()
{
	Node::onExit();
	NotificationCenter::getInstance()->removeAllObservers(this);
}

void TalismanLayer::MidText()
{
	if (m_isStop)
	{
		return;
	}
	if (m_TextId > 5)
	{
		m_TextId = 0;
	}
	std::string file = String::createWithFormat("animation/Talisman/Text%d.png",m_TextId)->getCString();
	Sprite* sp = Sprite::create(file);
	sp->setPosition(m_ws.width * 0.5, m_ws.height*0.5);
	addChild(sp);
	sp->setScale(5.0f);
	sp->setTag(15062519);

	{
		auto scaleTo = ScaleTo::create(0.05f,2.4f);
		auto ease = EaseSineOut::create(scaleTo);
		auto rot = RotateBy::create(0.05f,rand_minus1_1() * 45);
		auto spw = Spawn::create(ease, rot, nullptr);

		//缩放
		auto scale = ScaleTo::create(0.03f,2.2f);
		auto scaleBack = ScaleTo::create(0.03f,2.4f);
		auto seq = Sequence::create(scale, scaleBack, nullptr);
		auto rep = Repeat::create(seq, 5);
		auto fadeOut = FadeOut::create(0.5f);
		auto delay = DelayTime::create(0.2f);
		auto next = CallFunc::create(this,callfunc_selector(TalismanLayer::MidText));
		auto seq1 = Sequence::create(delay, next, nullptr);
		auto spw1 = Spawn::create(fadeOut, seq1, nullptr);
		auto rm = RemoveSelf::create(true);
		auto seq2 = Sequence::create(spw, rep, spw1, rm, nullptr);
		sp->runAction(seq2);
	}

	m_TextId++;
}

void TalismanLayer::RemoveMe()
{
	UIController::getInstance()->getMainUILayer()->startDaZhaoSD();
	removeFromParentAndCleanup(true);
}

void TalismanLayer::StopText()
{
	m_isStop = true;
	auto children = getChildren();
	for (int i = 0; i < children.size(); i++)
	{
		Node* node = children.at(i);
		int tag = node->getTag();
		if (tag == 15062519)
		{
			node->stopAllActions();
			node->setScale(2.4f);
			auto fadeout = FadeOut::create(0.3);
			auto rm = RemoveSelf::create(true);
			auto seq = Sequence::create(fadeout, rm, nullptr);
			node->runAction(seq);
		}
	}
}

Node* TalismanLayer::createHalfEff()
{
	auto node = Node::create();
	
	auto moon = Sprite::create("animation/Talisman/talisman1.png");
	auto laser = Sprite::create("animation/Talisman/talisman2.png");
	auto star = Sprite::create("animation/Talisman/talisman3.png");

	//其中两个使用混合模式，让其与第三个融为一体
	BlendFunc blend;
	blend.src = GL_SRC_COLOR;
	blend.dst = GL_ONE;
	moon->setBlendFunc(blend);
	laser->setBlendFunc(blend);
	
	moon->setAnchorPoint(Vec2(0.9, 0.1));
	//moon->setScale(1.5f);
	laser->setAnchorPoint(Vec2(0,0.5));
	laser->setScale(2.0f);

	node->addChild(star);
	node->addChild(laser);
	node->addChild(moon);

	auto rot = RotateBy::create(1.0f, 360);
	auto rep = RepeatForever::create(rot);
	star->runAction(rep);

	return node;
}

void TalismanLayer::RotateSurround()
{
	auto s = Director::getInstance()->getWinSize();
	auto node = Node::create();
	addChild(node);

	auto LeftNode = createHalfEff();
	auto RightNode = createHalfEff();
	LeftNode->setPosition(-110,-0);
	RightNode->setPosition(110,0);
	LeftNode->setScale(1.5);
	RightNode->setScale(1.5);
	node->addChild(LeftNode);
	node->addChild(RightNode);

	LeftNode->setRotation(180);

	auto rot = RotateBy::create(1.0f, 500);
	auto rep = RepeatForever::create(rot);
	node->runAction(rep);
}

void TalismanLayer::StopTalisman( Ref* obj )
{
	RemoveMe();
}

//---------------------------------------
AttackContinuous* AttackContinuous::createCircle(float radius, float dur, int hurts, 
												 int cateBit, int collisionBit, int contactBit)
{
	AttackContinuous* bullet = new (std::nothrow) AttackContinuous;
	if (bullet && bullet->init())
	{
		bullet->m_hurts = hurts;
		bullet->m_cateBit = cateBit;
		bullet->m_collosionBit = collisionBit;
		bullet->m_contactBit = contactBit;
		bullet->initCircle(radius);
		bullet->RemoveWhenTimeout(dur);
		bullet->autorelease();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

AttackContinuous* AttackContinuous::createBox( const Size& size, float dur, int hurts, 
											  int cateBit, int collisionBit, int contactBit)
{
	AttackContinuous* bullet = new (std::nothrow) AttackContinuous;
	if (bullet && bullet->init())
	{
		bullet->m_hurts = hurts;
		bullet->m_cateBit = cateBit;
		bullet->m_collosionBit = collisionBit;
		bullet->m_contactBit = contactBit;
		bullet->initBox(size);
		bullet->RemoveWhenTimeout(dur);
		bullet->autorelease();
		return bullet;
	}
	else
	{
		CC_SAFE_DELETE(bullet);
		return nullptr;
	}
}

void AttackContinuous::UpdateBox(float dt)
{
	auto body = PhysicsBody::createBox(m_Size);
	body->setGravityEnable(false);
	body->setRotationEnable(false);
	body->setMass(100000);
	body->setCategoryBitmask(m_cateBit);
	body->setCollisionBitmask(m_collosionBit);
	body->setContactTestBitmask(m_contactBit);

	setPhysicsBody(body);
	setInvincible(true);
	m_ContactPro.hurts = m_hurts;
}

void AttackContinuous::UpdateCircle(float dt)
{
	auto body = PhysicsBody::createCircle(m_radius);
	body->setGravityEnable(false);
	body->setRotationEnable(false);
	body->setMass(100000);
	body->setCategoryBitmask(m_cateBit);
	body->setCollisionBitmask(m_collosionBit);
	body->setContactTestBitmask(m_contactBit);

	setPhysicsBody(body);
	setInvincible(true);
	m_ContactPro.hurts = m_hurts;
}

void AttackContinuous::initBox(const Size& size)
{
	m_Size = size;
	schedule(schedule_selector(AttackContinuous::UpdateBox),0.1);
}

void AttackContinuous::initCircle(float radius)
{
	m_radius = radius;
	schedule(schedule_selector(AttackContinuous::UpdateCircle),0.1);
}

void AttackContinuous::RemoveWhenTimeout( float dur )
{
	auto delay = DelayTime::create(dur);
	auto func = CallFunc::create(this,callfunc_selector(PhysicsUnit::setDirectDeath));
	auto seq = Sequence::create(delay, func, nullptr);
	runAction(seq);
}

//----------------------------------------
SpecialSkillTips* SpecialSkillTips::create( int lvl )
{
	SpecialSkillTips* tips = new (std::nothrow) SpecialSkillTips;
	if (tips && tips->init())
	{
		tips->m_lvl = lvl;
		tips->autorelease();
		return tips;
	}
	else
	{
		CC_SAFE_DELETE(tips);
		return nullptr;
	}
}

void SpecialSkillTips::addClippingNode()
{
	auto ws = Director::getInstance()->getWinSize();

	Layout* clip = Layout::create();
	clip->setPosition(Vec2(0, ws.height*0.5 - 120));
	clip->setSize(Size(ws.width, ws.height));
	clip->setClippingEnabled(true);
	addChild(clip,1);

	//内容
	auto content = getClippingContent();
	clip->addChild(content);

	addTips();
}

void SpecialSkillTips::onEnter()
{
	Layer::onEnter();

	PlaneProtect::ClearScreen();

	UIController::getInstance()->getPhysicsLayer()->ToBlack(10.0f);
	GameData::getInstance()->GameSlowDown(1.0f,false);

	m_ws = Director::getInstance()->getWinSize();

	addClippingNode();

	scheduleUpdate();
}

Node* SpecialSkillTips::getClippingContent()
{
	std::string amrtName;
	switch (m_lvl)
	{
	case 1:
		amrtName = "shijiu";
		break;
	case 2:
		amrtName = "shuanglong";
		break;
	case 3:
		amrtName = "huolong";
		break;

	default:
		break;
	}
	auto armat = Armature::create(amrtName);
	armat->getAnimation()->playWithIndex(0);
	armat->setScale(1.2);
	armat->setPosition(Vec2(m_ws.width * 1.5,-m_ws.height* 0.05));

	auto mv = MoveBy::create(0.05f, Vec2(-m_ws.width*0.8, 0));
	auto ease = EaseSineIn::create(mv);
	auto stay = MoveBy::create(1.0f, Vec2(-m_ws.width*0.2, 0));
	auto leave = MoveBy::create(0.1f, Vec2(-m_ws.width*0.6, 0));
	auto func = CallFunc::create(this,callfunc_selector(SpecialSkillTips::removeMe));
	auto seq = Sequence::create(ease, stay, leave, func, nullptr);
	armat->runAction(seq);

	return armat;
}

void SpecialSkillTips::addTips()
{
	auto ws = Director::getInstance()->getWinSize();

	m_bg1 = Sprite::create("Image/SpecialSkillTips/Bg.png");
	m_bg1->setPosition(ws.width*0.5, ws.height*0.5);
	m_bg1->setScale(4);
	addChild(m_bg1,-1);

	m_bg2 = Sprite::create("Image/SpecialSkillTips/Bg.png");
	m_bg2->setPosition(-ws.width*0.5, ws.height*0.5);
	m_bg2->setScale(4);
	addChild(m_bg2,-1);
	m_bg2->setFlippedX(true);

	{
		/*auto frameUp = Sprite::create("Image/SpecialSkillTips/FrameUp.png");
		frameUp->setPosition(ws.width*0.5, ws.height*0.58);
		frameUp->setScale(4);
		addChild(frameUp,1);*/
	}
	{
		auto frameDown = Sprite::create("Image/SpecialSkillTips/FrameDown.png");
		frameDown->setPosition(ws.width*0.5, ws.height*0.42);
		frameDown->setScale(4);
		addChild(frameDown,1);
	}
	{
		auto text = Sprite::create("Image/SpecialSkillTips/text.png");
		text->setPosition(-ws.width*0.3, ws.height*0.45);
		addChild(text,2);
		
		auto mv = MoveBy::create(0.1f, Vec2(m_ws.width*0.6, 0));
		auto back = mv->reverse();
		auto ease = EaseSineIn::create(mv);
		auto delay = DelayTime::create(1.0f);
		auto seq = Sequence::create(mv, delay, back, nullptr);
		text->runAction(seq);
	}
}

void SpecialSkillTips::update( float delta )
{
	auto pos1 = m_bg1->convertToWorldSpaceAR(Vec2::ZERO);
	auto pos2 = m_bg2->convertToWorldSpaceAR(Vec2::ZERO);

	pos1.x += 30;
	pos2.x += 30;

	auto edge1 = m_ws.width * 1.5;
	auto edge2 = -m_ws.width * 0.5;

	if (pos1.x >= edge1)
	{
		pos1.x = edge2;
	}
	if (pos2.x >= edge1)
	{
		pos2.x = edge2;
	}

	m_bg1->setPosition(pos1);
	m_bg2->setPosition(pos2);
}

void SpecialSkillTips::removeMe()
{
	//删除后释放大招
	switch (m_lvl)
	{
	case 1:
		{
			auto pl = GameData::getInstance()->getMainPlane();
			if (!pl)
			{
				break;
			}
			TalismanLayer* Talisman = TalismanLayer::create();
			Talisman->setduration(6);
			pl->addChild(Talisman,-1);
		}
		break;
	case 2:
		{
			Bow* arrow = Bow::create();
			arrow->setPosition(360,150);
			UIController::getInstance()->getPhysicsLayer()->addChild(arrow);
		}
		break;
	case 3:
		{
			MeteorLayer* layer = MeteorLayer::create();
			UIController::getInstance()->getPhysicsLayer()->addChild(layer);
		}
		break;

	default:
		break;
	}

	this->removeFromParentAndCleanup(true);
}

//--------------------------------
CompleteGame* CompleteGame::create( int lvl )
{
	CompleteGame* layer = new (std::nothrow) CompleteGame;
	if (layer && layer->init())
	{
		layer->m_lvl = lvl;
		layer->autorelease();
		return layer;
	}
	else
	{
		CC_SAFE_DELETE(layer);
		return nullptr;
	}
}

void CompleteGame::onEnter()
{
	Layer::onEnter();
	m_ws = Director::getInstance()->getWinSize();

	addBg();
	addClippingNode();	//主角形象
	UIController::getInstance()->getMainUILayer()->playPassSucceed();	//远征成功

	auto delay = DelayTime::create(3.0f);
	auto func = CallFunc::create(this,callfunc_selector(CompleteGame::TurnToResultLayer));
	auto seq = Sequence::create(delay, func, nullptr);
	runAction(seq);
}

void CompleteGame::addClippingNode()
{
	//加个黑色底层
	auto bg = LayerColor::create(ccc4(0,0,0,200));
	addChild(bg);

	Layout* clip = Layout::create();
	clip->setPosition(Vec2(0, m_ws.height*0.5 - 120));
	clip->setSize(Size(m_ws.width, m_ws.height));
	clip->setClippingEnabled(true);
	addChild(clip,1);

	//内容
	auto content = getClippingContent();
	clip->addChild(content);
}

Node* CompleteGame::getClippingContent()
{
	std::string amrtName;
	float scale = 1.0;
	Vec2 offset = Vec2(100,0);
	switch (m_lvl)
	{
	case 1:
		amrtName = "shijiu";
		offset.y = 100;
		offset.x = 40;
		break;
	case 2:
		amrtName = "shuanglong";
		break;
	case 3:
		amrtName = "huolong";
		scale = 1.2;
		offset.y = 100;
		break;

	default:
		break;
	}
	auto armat = Armature::create(amrtName);
	armat->getAnimation()->playWithIndex(0);
	armat->setScale(scale);
	armat->setPosition(Vec2(m_ws.width * 0.65,-m_ws.height*0.1) + offset);
	return armat;
}

void CompleteGame::TurnToResultLayer()
{
	UIController::getInstance()->ShowResultLayer(true);
	
	removeFromParentAndCleanup(true);
}

void CompleteGame::addBg()
{
	auto sp = Sprite::create("Effect/passSuccee/bg.png");
	sp->setPosition(m_ws.width*0.5, m_ws.height*0.5);
	sp->setScale(2.0);
	addChild(sp);
}

//----------------------------------
MapSwitchTips* MapSwitchTips::create( int lvl, float restTime,float delay )
{
	MapSwitchTips* maptips = new (std::nothrow) MapSwitchTips;
	if (maptips && maptips->init())
	{
		maptips->setRestTime(restTime);
		maptips->m_delay = delay;
		maptips->m_lvl = lvl;
		maptips->autorelease();
		return maptips;
	}
	else
	{
		CC_SAFE_DELETE(maptips);
		return nullptr;
	}
}

bool MapSwitchTips::init()
{
	if (!Layer::init())
	{
		return false;
	}

	return true;
}

void MapSwitchTips::onEnter()
{
	Layer::onEnter();
	
	m_ws = Director::getInstance()->getWinSize();
	setPosition(Vec2(0, m_ws.height*0.15));

	auto delay = DelayTime::create(m_delay);
	auto func = CallFunc::create(this,callfunc_selector(MapSwitchTips::addTips));
	auto seq = Sequence::create(delay, func, nullptr);
	runAction(seq);
}

void MapSwitchTips::addTips()
{
	std::string bgName = String::createWithFormat("Bg/SwitchBg/SimpleBg%d.png",m_lvl)->getCString();
	std::string TextName = String::createWithFormat("Bg/SwitchBg/BgText%d.png",m_lvl)->getCString();

	//背景
	auto bg = Sprite::create(bgName);
	bg->setPosition(m_ws.width*1.5, m_ws.height*0.5);
	bg->setScale(1.1);
	addChild(bg,-100);

	//文字图片
	auto text = Sprite::create(TextName);
	text->setPosition(m_ws.width*1.5, m_ws.height*0.46);
	addChild(text);

	//上下框
	auto upFrame = Sprite::create("Bg/SwitchBg/Bgframe.png");
	upFrame->setFlippedY(true);
	upFrame->setPosition(-m_ws.width*0.5, m_ws.height*0.6);
	addChild(upFrame);

	auto BottomFrame = Sprite::create("Bg/SwitchBg/Bgframe.png");
	BottomFrame->setPosition(m_ws.width*1.5, m_ws.height*0.4);
	addChild(BottomFrame);

	//小光
	auto upSamllLight = Sprite::create("Bg/SwitchBg/light1.png");
	upSamllLight->setPosition(-m_ws.width*0.5, m_ws.height*0.5 + 100);
	addChild(upSamllLight);

	auto BottomSamllLight = Sprite::create("Bg/SwitchBg/light1.png");
	BottomSamllLight->setPosition(m_ws.width*1.5, m_ws.height*0.5 - 105);
	addChild(BottomSamllLight);

	auto lightSize = upSamllLight->getContentSize();

	//高亮
	auto upHighLight = Sprite::create("Bg/SwitchBg/highLight.png");
	upHighLight->setPosition(lightSize.width*0.5, lightSize.height*0.5);
	upHighLight->setOpacity(0);
	upHighLight->setScale(0.2);
	upSamllLight->addChild(upHighLight,1);

	auto BottomHighLight = Sprite::create("Bg/SwitchBg/highLight.png");
	BottomHighLight->setPosition(lightSize.width*0.5, lightSize.height*0.5);
	BottomHighLight->setOpacity(0);
	BottomHighLight->setScale(0.2);
	BottomSamllLight->addChild(BottomHighLight,1);

	//各自动作
	{
		auto mvIn = MoveTo::create(0.5, Vec2(m_ws.width*0.5, m_ws.height*0.5));
		auto easeIn = EaseSineIn::create(mvIn);
		auto stay = DelayTime::create(2.5);
		auto mvOut = MoveTo::create(0.5, Vec2(-m_ws.width*0.5, m_ws.height*0.5));
		auto easeOut = EaseSineIn::create(mvOut);
		auto seq = Sequence::create(easeIn, stay, easeOut, nullptr);
		bg->runAction(seq);
	}
	{
		auto delay = DelayTime::create(0.2f);
		auto mvIn = MoveTo::create(0.5, Vec2(m_ws.width*0.57, m_ws.height*0.46));
		auto easeIn = EaseSineIn::create(mvIn);
		auto stay = DelayTime::create(2.0);
		auto mvOut = MoveTo::create(0.5, Vec2(-m_ws.width*0.5, m_ws.height*0.46));
		auto easeOut = EaseSineIn::create(mvOut);
		auto seq = Sequence::create(delay, easeIn, stay, easeOut, nullptr);
		text->runAction(seq);
	}
	{
		auto pos = BottomFrame->getPosition();

		auto delay = DelayTime::create(0.4f);
		auto mvIn = MoveTo::create(0.1, Vec2(m_ws.width*0.5, pos.y));
		auto easeIn = EaseSineIn::create(mvIn);
		auto mvSlowly = DelayTime::create(2.0);
		auto mvOut = MoveTo::create(0.5, Vec2(-m_ws.width*0.5, pos.y));
		auto easeOut = EaseSineIn::create(mvOut);
		auto seq = Sequence::create(delay, easeIn, mvSlowly, easeOut, nullptr);
		BottomFrame->runAction(seq);
	}
	{
		auto pos = upFrame->getPosition();

		auto delay = DelayTime::create(0.4f);
		auto mvIn = MoveTo::create(0.1, Vec2(m_ws.width*0.5, pos.y));
		auto easeIn = EaseSineIn::create(mvIn);
		auto mvSlowly = DelayTime::create(2.0);
		auto mvOut = MoveTo::create(0.5, Vec2(m_ws.width*1.5, pos.y));
		auto easeOut = EaseSineIn::create(mvOut);
		auto seq = Sequence::create(delay, easeIn, mvSlowly, easeOut, nullptr);
		upFrame->runAction(seq);
	}
	{
		auto pos = upSamllLight->getPosition();

		auto delay = DelayTime::create(1.0);
		auto mvIn = MoveTo::create(0.2, Vec2(m_ws.width*0.45, pos.y));
		auto easeIn = EaseSineIn::create(mvIn);
		auto mvSlowly =  MoveBy::create(0.5,Vec2(m_ws.width*0.1, 0));
		auto mvOut = MoveTo::create(0.1, Vec2(m_ws.width*1.5, pos.y));
		auto easeOut = EaseSineIn::create(mvOut);
		auto seq = Sequence::create(delay, easeIn, mvSlowly, easeOut, nullptr);
		upSamllLight->runAction(seq);
	}
	{
		auto pos = BottomSamllLight->getPosition();

		auto delay = DelayTime::create(1.0);
		auto mvIn = MoveTo::create(0.2, Vec2(m_ws.width*0.55, pos.y));
		auto easeIn = EaseSineIn::create(mvIn);
		auto mvSlowly =  MoveBy::create(0.5,Vec2(-m_ws.width*0.1, 0));
		auto mvOut = MoveTo::create(0.1, Vec2(-m_ws.width*0.5, pos.y));
		auto easeOut = EaseSineIn::create(mvOut);
		auto seq = Sequence::create(delay, easeIn, mvSlowly, easeOut, nullptr);
		BottomSamllLight->runAction(seq);
	}
	{
		auto delay = DelayTime::create(0.8);
		auto fadeIn = FadeIn::create(0.3);
		auto stay = DelayTime::create(1.0);
		auto fadeout = FadeOut::create(0.3);
		auto seq = Sequence::create(delay, fadeIn, stay, fadeout, nullptr);
		upHighLight->runAction(seq);

		auto scale = ScaleTo::create(0.25,1.5, 1.3);
		auto scaleBack = ScaleTo::create(0.25,0.2, 0.1);
		auto seq1 = Sequence::create(DelayTime::create(1.2), scale, scaleBack, nullptr);
		upHighLight->runAction(seq1);
	}
	{
		auto delay = DelayTime::create(0.8);
		auto fadeIn = FadeIn::create(0.3);
		auto stay = DelayTime::create(1.0);
		auto fadeout = FadeOut::create(0.3);
		auto seq = Sequence::create(delay, fadeIn, stay, fadeout, nullptr);
		BottomHighLight->runAction(seq);

		auto scale = ScaleTo::create(0.25,1.5, 1.3);
		auto scaleBack = ScaleTo::create(0.25,0.2, 0.1);
		auto seq1 = Sequence::create(DelayTime::create(1.2), scale, scaleBack, nullptr);
		BottomHighLight->runAction(seq1);
	}
	{
		auto delay = DelayTime::create(3.0);
		auto rm = CallFunc::create(this, callfunc_selector(MapSwitchTips::removeSelf));
		auto seq = Sequence::create(delay, rm, nullptr);
		runAction(seq);

	}
}

void MapSwitchTips::setRestTime( float t )
{
	//怪物延迟出现
	GameData::getInstance()->getPlaneManager()->Rest(t);
}

void MapSwitchTips::removeSelf()
{
	GameData::getInstance()->addProp();	//在提示结束后，开始使用道具
	removeFromParentAndCleanup(true);
}

//-----------------------------
FlyDistance* FlyDistance::create()
{
	FlyDistance* dis = new (std::nothrow) FlyDistance;
	if (dis && dis->init())
	{
		dis->autorelease();
		return dis;
	}
	else
	{
		CC_SAFE_DELETE(dis);
		return nullptr;
	}
}

void FlyDistance::onEnter()
{
	Node::onEnter();
	m_dis = 0;
	schedule(schedule_selector(FlyDistance::refreshDis),0.1);
}

void FlyDistance::refreshDis(float dt)
{
	bool isSpeedUp = GameData::getInstance()->getisSpeedUp();
	if (isSpeedUp)
	{
		m_dis += 20;
	}
	else
	{
		m_dis += 10;
	}
	
	//更新显示
	auto data = GameData::getInstance()->getAllStatistics();
	data[GamePrKey_totalKm] = m_dis;
	//GameData::getInstance()->setAllStatistics(data);
	UIController::getInstance()->getMainUILayer()->updataGamedistance(m_dis);
}

//----------------------
Bow* Bow::create()
{
	Bow* bow = new (std::nothrow) Bow;
	if (bow && bow->initWithFile("animation/bow/bow.png"))
	{
		bow->autorelease();
		return bow;
	}
	else
	{
		CC_SAFE_DELETE(bow);
		return nullptr;
	}
}

void Bow::onEnter()
{
	PhysicsUnit::onEnter();

	setInvincible(true);
	setOpacity(0);
	auto fadeIn = FadeIn::create(0.5f);
	auto func = CallFunc::create(this,callfunc_selector(Bow::Start));
	auto seq = Sequence::create(fadeIn, func, nullptr);
	runAction(seq);
}

void Bow::InitNode()
{
	Start();
}

void Bow::Start()
{
	auto s = getContentSize();

	auto BowAnimateNode = Sprite::create("weapon.png");
	BowAnimateNode->setPosition(s.width*0.5, s.height*0.5);
	addChild(BowAnimateNode);

	auto ArrowAnimateNode = Sprite::create("weapon.png");
	ArrowAnimateNode->setPosition(s.width*0.5, -200);
	addChild(ArrowAnimateNode);
	ArrowAnimateNode->setAnchorPoint(Vec2(0,0.5));
	ArrowAnimateNode->setRotation(-90);
	ArrowAnimateNode->setScale(1.5f);

	if (m_shootTimes >= 6)
	{
		{
			auto delay = DelayTime::create(2.3f);
			auto fadeOut = FadeOut::create(1.0f);
			auto seq = Sequence::create(delay, fadeOut, nullptr);
			runAction(seq);
		}

		{
			auto delay = DelayTime::create(2.5f);
			auto shine = BowShine();
			auto rm = CallFunc::create(this, callfunc_selector(Bow::removeSelf));
			auto seq = Sequence::create(delay, shine, rm, nullptr);
			BowAnimateNode->runAction(seq);
		}
		return;
	}
	//先发光，后聚气
	auto shine = BowShine();
	BowAnimateNode->runAction(shine);
	EffectsManager::addParticle("particla/arrow/gongjuqi.plist",this,Vec2(s.width*0.5,s.height*0.5 + 100),false);

	auto delay = DelayTime::create(0.5f);
	auto juqi = JuqiAnimat();
	auto seq = Sequence::create(delay, juqi, nullptr);
	ArrowAnimateNode->runAction(seq);

	auto wait = DelayTime::create(juqi->getDuration() * 0.9);
	auto shoot = CallFunc::create(this, callfunc_selector(Bow::ShootOut));
	auto seq1 = Sequence::create(wait, shoot, nullptr);
	ArrowAnimateNode->runAction(seq1);

	{
		auto wait1 = DelayTime::create(1.5f);
		auto next = CallFunc::create(this, callfunc_selector(Bow::Start));
		auto seq3 = Sequence::create(wait1, next, nullptr);
		runAction(seq3);
	}

	m_shootTimes++;
}

void Bow::removeSelf()
{
	UIController::getInstance()->getMainUILayer()->startDaZhaoSD();
	UIController::getInstance()->getMainUILayer()->setDaZhaoState(true);
	setDirectDeath();
}

Animate* Bow::JuqiAnimat()
{
	float interval = 1.2f/24;
	auto animat = EffectsManager:: getFramesAnimation("arrowAnimat_", 36, interval, 0, 1, false);
	return animat;
}

Animate* Bow::BowShine()
{
	float interval = 0.8/24;
	auto bowAnimat = EffectsManager::getFramesAnimation("bow_",15,1.0f/24);
	return bowAnimat;
}


void Bow::Hurt()
{
	NotificationCenter::getInstance()->postNotification(Msg_Shake,nullptr);
	setBody();
}

void Bow::setBody()
{
	PhysicsBody* body = PhysicsBody::createBox(Size(m_WinSize.width,m_WinSize.height*5));
	body->setGravityEnable(false);
	body->setCategoryBitmask(SELF_SCOPE_CATEGORYBITMASK);
	body->setCollisionBitmask(0);
	body->setContactTestBitmask(SELF_SCOPE_CONTACTTESTBITMASK);
	setPhysicsBody(body);
	m_ContactPro.hurts = Bow_hurt;
}

void Bow::ShootOut()
{
	auto s = getContentSize();

	auto arrow = Node::create();
	arrow->setPosition(s.width * 0.5, s.height*0.5 + 100);
	addChild(arrow);

	{
		auto mv = MoveBy::create(0.65, Vec2(0,m_WinSize.height * 1.2));
		auto ease = EaseSineIn::create(mv);
		auto delay = DelayTime::create(1.0f);
		auto rm = RemoveSelf::create(true);
		auto seq = Sequence::create(ease, delay, rm, nullptr);
		arrow->runAction(seq);
	}

	{
		auto delay = DelayTime::create(0.25);
		auto hurt = CallFunc::create(this, callfunc_selector(Bow::Hurt));
		auto seq = Sequence::create(delay, hurt, nullptr);
		runAction(seq);
	}

	//加上各种粒子
	EffectsManager::addParticle("particla/arrow/bullet.plist",arrow,Vec2::ZERO,false);	//子弹头
	EffectsManager::addParticle("particla/arrow/tail_circle.plist",arrow,Vec2::ZERO,false);
	EffectsManager::addParticle("particla/arrow/tail_lizi.plist",arrow,Vec2::ZERO,false);
	EffectsManager::addParticle("particla/arrow/tail_quan1.plist",arrow,Vec2::ZERO,false);
	EffectsManager::addParticle("particla/arrow/tail_smoke.plist",arrow,Vec2::ZERO,false);
	EffectsManager::addParticle("particla/arrow/tail_smoke1.plist",arrow,Vec2::ZERO,false);
}

