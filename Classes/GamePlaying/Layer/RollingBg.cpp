#include "RollingBg.h"
#include "UIController.h"
#include "../../GameCommon/GameData.h"
#include "../../GameCommon/EffectsManager.h"



bool GameBg::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_size = Director::getInstance()->getWinSize();
	m_curSpeed = GameData::getInstance()->getUpdateSpeed();

	m_BgTexture1 = Director::getInstance()->getTextureCache()->addImage("Bg/bg1.jpg");
	m_BgTexture2 = Director::getInstance()->getTextureCache()->addImage("Bg/bg2.jpg");
	m_BgTexture3 = Director::getInstance()->getTextureCache()->addImage("Bg/bg3.jpg");
	m_curBgId = 1;
	return true;
}

void GameBg::onEnter()
{
	Layer::onEnter();
	addBgAndRolling();
	scheduleUpdate();
	schedule(schedule_selector(GameBg::refreshSpeed));
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(GameBg::Shake),Msg_Shake,nullptr);
}

void GameBg::onExit()
{
	Layer::onExit();
	NotificationCenter::getInstance()->removeAllObservers(this);
}

float GameBg::SpeedRate()
{
	return 0.0f;
}

bool GameBg::isFlip()
{
	return false;
}

void GameBg::addBgAndRolling()
{
	m_size = Director::sharedDirector()->getWinSize();
	m_Bg1 = Sprite::createWithTexture(m_BgTexture1);
	m_Bg1->setAnchorPoint(ccp(0.5,1));
	m_Bg1->setPosition(m_size.width*0.5, m_size.height*0.5);
	addChild(m_Bg1);
	m_Bg1->setScaleX(2.0f);

	m_Bg2 = Sprite::createWithTexture(m_BgTexture1);
	m_Bg2->setPosition(m_size.width*0.5, m_Bg1->getContentSize().height*1.5);
	m_Bg2->setFlipY(isFlip());
	m_Bg2->setAnchorPoint(ccp(0.5,1));
	addChild(m_Bg2);
	m_Bg2->setScaleX(2.0f);
/*
	CallFunc* func = CallFunc::create(this,callfunc_selector(GameBg::addQipaoRep));
	ActionInterval* delay = DelayTime::create(3.0f + rand_0_1());
	ActionInterval* seq = Sequence::create(func,delay,nullptr);
	ActionInterval* rep = RepeatForever::create(seq);
	runAction(rep);*/
}

void GameBg::update( float delta )
{
	bool isPause = GameData::getInstance()->isGamePause();
	if (isPause)
	{
		return;
	}
	auto bg1Pos = m_Bg1->getPosition();
	auto bg2Pos = m_Bg2->getPosition();

	float offset = m_curSpeed + SpeedRate();

	bg1Pos.y -= offset;
	bg2Pos.y -= offset;

	m_Bg1->setPosition(bg1Pos.x,bg1Pos.y);
	m_Bg2->setPosition(bg2Pos.x,bg2Pos.y);

	if (bg1Pos.y <= 0.0f)
	{
		m_Bg1->setPosition(m_size.width*0.5, bg2Pos.y + m_Bg1->getContentSize().height);
	}
	if (bg2Pos.y <= 0.0f)
	{
		m_Bg2->setFlipY(isFlip());
		m_Bg2->setPosition(m_size.width*0.5, bg1Pos.y + m_Bg2->getContentSize().height);
	}

}

void GameBg::Shake( Ref* obj )
{
	UIController::getInstance()->getEffectsLayer()->ScreenShake(this);
}

void GameBg::refreshSpeed( float dt )
{
	SpeedUpEase();
}

void GameBg::SwitchBg( int id )
{
	if (m_curBgId == id)
	{
		//已经是该背景
		MapSwitchTips* tips = MapSwitchTips::create(m_curBgId,5.0f);
		UIController::getInstance()->getEffectsLayer()->addChild(tips,1000);
		return;
	}
	m_curBgId = id;
	switch (m_curBgId)
	{
	case 1:
		{
			m_Bg1->setTexture(m_BgTexture1);
			m_Bg2->setTexture(m_BgTexture1);
		}
		break;
	case 2:
		{
			m_Bg1->setTexture(m_BgTexture2);
			m_Bg2->setTexture(m_BgTexture2);
		}
		break;
	case 3:
		{
			m_Bg1->setTexture(m_BgTexture3);
			m_Bg2->setTexture(m_BgTexture3);
		}
		break;

	default:
		break;
	}

	MapSwitchTips* tips = MapSwitchTips::create(m_curBgId,5.0, 5.0);
	UIController::getInstance()->getEffectsLayer()->addChild(tips,1000);
}


//---------------------

float RollingBg::SpeedRate()
{
	return 0.0f;
}

bool RollingBg::isFlip()
{
	return false;
}

void RollingBg::SpeedUpEase()
{
	float newSpeed = GameData::getInstance()->getUpdateSpeed();
	m_curSpeed = newSpeed;
	/*if (m_curSpeed < newSpeed)
	{
	m_curSpeed += 0.1f;
	}
	else
	{
	m_curSpeed -= 0.3f;
	}*/
}

void RollingBg::onEnter()
{
	GameBg::onEnter();
	//auto cloudLayer = RollingClouds::create();
	//addChild(cloudLayer);
}


//----------------------
float RollingClouds::SpeedRate()
{
	return 40.0f;
}

bool RollingClouds::isFlip()
{
	return false;
}

void RollingClouds::SpeedUpEase()
{
	float newSpeed = GameData::getInstance()->getUpdateSpeed();
	if (m_curSpeed < newSpeed)
	{
		m_curSpeed += 0.2f;
	}
	else
	{
		m_curSpeed -= 0.2f;
	}
}
