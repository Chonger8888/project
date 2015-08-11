#include "GameMainLayer.h"
#include "../GameCommon/GameData.h"
#include "../GameCommon/GameDefine.h"
#include "../GameCommon/EffectsManager.h"
#include "../GamePlaying/BasePlane/BasePlane.h"
#include "../GameCommon/NewDataMgr.h"
#include "../GamePlaying/Layer/UIController.h"
#include "../GamePlaying/Layer/PhysicsLayer.h"
#include "../JniHelp/CppCallJava.h"
#include "../GameCommon/CommonRes.h"
#include "GameSceneMgr.h"
#include "../GamePlaying/Layer/RollingBg.h"


bool GameMainLayer::init()
{
	bool ret = false;
	if ( Layer::init() )
	{
		ret = true;
	}

	return ret;
}

void GameMainLayer::onEnter()
{
	Layer::onEnter();

	initInterface();
}

void GameMainLayer::initInterface( void )
{
	_mainLayerRootNode = CSLoader::createNode( Ccs_GameMainLayer_FilePath );

	if ( !_mainLayerRootNode )
	{
		return;
	}
	addChild( _mainLayerRootNode );

	auto loadingBarEnergy			= (LoadingBar*)GameMainLayer::seekNodeByName(_mainLayerRootNode, "LoadingBarEnergy" );
	loadingBarEnergy->setPercent( 100 );

	Button* BtnPause = (Button*)GameMainLayer::seekNodeByName(_mainLayerRootNode, "ButtonMenu" );
	BtnPause->addClickEventListener(CC_CALLBACK_1(GameMainLayer::callbackPause, this));

	Button* ButtonDaZhao = (Button*)GameMainLayer::seekNodeByName(_mainLayerRootNode, "ButtonDaZhao" );
	ButtonDaZhao->addClickEventListener(CC_CALLBACK_1(GameMainLayer::callbackDazhao, this));


	auto SpritePropIco = (Sprite*)GameMainLayer::seekNodeByName(_mainLayerRootNode, "SpritePropIco" );
	SpritePropIco->setTexture(__String::createWithFormat("roleImg/dazhao%d.png", NewDataMgr::getInstance()->mRoleConfig.roleIndex )->getCString() );

	

	// 卡牌设置
	for ( int i = 1; i<=3; i++ )
	{
		for( int j = 1; j<=5; j++ )
		{
			// 星星等级
			auto Lv5Star = (Node*)GameMainLayer::seekNodeByName(_mainLayerRootNode, __String::createWithFormat("LvStar%d_%d", i, j)->getCString() );

			if ( NewDataMgr::getInstance()->mRoleConfig.cardArr[i-1].cardStarLv < j )
				Lv5Star->setVisible( false );
			else
				Lv5Star->setVisible( true );
		}
		// 设置头像
		std::string cardHeadPath = __String::createWithFormat("%s%d.png", Img_CardHead_FilePath ,  NewDataMgr::getInstance()->mRoleConfig.cardArr[i-1].cardID)->getCString();
		auto imgPrand = (Sprite*)GameMainLayer::seekNodeByName(_mainLayerRootNode, __String::createWithFormat("imgPrand%d", i)->getCString());
		imgPrand->setTexture( cardHeadPath );
		imgPrand->setVisible( false );

	}
	updataGamedistance(0);

	updateUIGold( 0 );

	schedule(schedule_selector(GameMainLayer::HpDecrease),0.1); 

	

}



void GameMainLayer::updateEnergyBar( float num )
{
	// 更新能量条
	auto loadingBarEnergy			= (LoadingBar*)GameMainLayer::seekNodeByName(_mainLayerRootNode, "LoadingBarEnergy" );
	//_curMaxEnergy = num + loadingBarEnergy->getPercent();


	if ( 0 == num)
	{
		loadingBarEnergy->setPercent( num );
 		unschedule( CC_SCHEDULE_SELECTOR(GameMainLayer::callFuncRandAnim) );
// 		loadingBarEnergy->stopAllActions();
// 		loadingBarEnergy->loadTexture("ccsres/gamemainui/imgload.png");
// 		loadingBarEnergy->setPercent( 0 );

	}
	else
	{
		float curBarNum = loadingBarEnergy->getPercent();
		// 血条为0时  加血前先停了之前的动作 恢复正常颜色
		if ( curBarNum  == 0 )
		{
			loadingBarEnergy->stopAllActions();
			loadingBarEnergy->runAction( TintTo::create(0.1f, 255,255,255));
		}

		loadingBarEnergy->runAction( RepeatForever::create(Sequence::create(
			DelayTime::create( 0.02f ),
			CallFuncN::create( CC_CALLBACK_1(GameMainLayer::energyBarAction1, this) ),
			nullptr
			)));
	}

}

void GameMainLayer::energyBarAction1( Node* node )
{
	auto loadingBarEnergy = (LoadingBar*)node;

	if ( loadingBarEnergy )
	{
		int curLoadNum = loadingBarEnergy->getPercent();

		// 能量条进度系数
		curLoadNum += 2;
//		if( curLoadNum <= _curMaxEnergy )
		{
			loadingBarEnergy->setPercent( curLoadNum );
		}
	//	else
		{
			loadingBarEnergy->stopAllActions();
			_decayIng = false;
			decayEnergyBar();
		}

		if ( 100 <= curLoadNum )
		{
			loadingBarEnergy->loadTexture("ccsres/gamemainui/imgEnergybarmax.png");

			schedule(CC_SCHEDULE_SELECTOR(GameMainLayer::callFuncRandAnim), CCRANDOM_0_1()*5*0.1 );

		}
	}
}

void GameMainLayer::decayEnergyBar( void )
{
	// 衰减中 防止多次调用处理
	if ( _decayIng )
		return;

	_decayIng = true;
	auto loadingBarEnergy			= (LoadingBar*)GameMainLayer::seekNodeByName(_mainLayerRootNode, "LoadingBarEnergy" );
	if ( 0 < loadingBarEnergy->getPercent() )
	{
		// 进度条调整间隔
		float intervalTimg = 0.02;

//		setdecayConfig( intervalTimg, decayTime );

		// 更新能量条
		{
			loadingBarEnergy->runAction( RepeatForever::create(Sequence::create(
				DelayTime::create( intervalTimg ),
				CallFuncN::create( CC_CALLBACK_1(GameMainLayer::energyBarAction2, this) ),
				nullptr
				)));
		}
	}
}

void GameMainLayer::energyBarAction2( Node* node )
{
	auto loadingBarEnergy = (LoadingBar*)node;

	if ( loadingBarEnergy )
	{
		float curLoadNum = loadingBarEnergy->getPercent();

		// 能量条进度系数
		curLoadNum -= 100/(_decayCountT/_decayInterval);
		if( 0 < curLoadNum )
		{
			loadingBarEnergy->setPercent( curLoadNum );
		}
		else
		{
			loadingBarEnergy->stopAllActions();
			loadingBarEnergy->loadTexture("ccsres/gamemainui/imgload.png");
			loadingBarEnergy->setPercent( 0 );

			_decayIng = false;
			// 完成衰减回调接口

			playLifeBarWarning();

		}
	}
}

float GameMainLayer::getDecayCountT( int type  )
{
	// 
// 	switch ( type )
// 	{
// 	case 1:
// 		return 50.0f* (curPercent/100);
// 		break;
// 
// 	case 2:
// 		return 60.0f* (curPercent/100);
// 		break;
// 
// 	case 3:
// 		return 75.0f* (curPercent/100);
// 		break;
// 
// 	default:
// 		break;
// 	}

	return 15.0f;
}

void GameMainLayer::setdecayConfig( float interval, float t )
{
	_decayInterval = interval;
	_decayCountT = t;
}

bool GameMainLayer::getDecayState( void )
{
	return _decayIng;
}

void GameMainLayer::updateUIGold( int goldnum )
{
	int curScore = GameData::getInstance()->getGoldNum();

	// 更新成绩
	auto atlasLabelScore = (TextAtlas*)GameMainLayer::seekNodeByName(_mainLayerRootNode, "AtlasLabelGold" );
	atlasLabelScore->setString( __String::createWithFormat("%d", curScore)->getCString() );
}


void GameMainLayer::updateUIScore( int score )
{
	// 更新成绩
	auto atlasLabelScore = (TextAtlas*)GameMainLayer::seekNodeByName(_mainLayerRootNode, "AtlasLabelScore" );
	atlasLabelScore->setString( __String::createWithFormat("%d", score)->getCString() );
}

Node* GameMainLayer::getCollectNode( void )
{
	return GameMainLayer::seekNodeByName(_mainLayerRootNode, "SpriteCollectIco" );
}

Node* GameMainLayer::createEnergyFullAnim( Vec2 ps )
{
	Vector<SpriteFrame*> frame_array;

	for (int i = 0 ; i <=4; ++i)
	{
		std::string pic = CCString::createWithFormat("%s%d.png","Electric_",i)->getCString();
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pic.c_str());

		frame_array.pushBack(frame);
	}
	auto loadingBarEnergy			= (LoadingBar*)GameMainLayer::seekNodeByName(_mainLayerRootNode, "LoadingBarEnergy" );

	auto p_animation = Animation::createWithSpriteFrames(frame_array,0.1f);
	p_animation->setLoops(1);

	auto animate =  Animate::create(p_animation) ;

	Vec2 pos = loadingBarEnergy->convertToWorldSpace( loadingBarEnergy->getPosition());

	auto upgradeAnimNode = Sprite::create("weapon.png");
	upgradeAnimNode->setPosition( Vec2(pos.x,pos.y+CCRANDOM_0_1()*600) );// 在Y值高高为600内随机位置
	addChild( upgradeAnimNode );

	upgradeAnimNode->runAction(Sequence::create( 
		animate, 
		CallFuncN::create(CC_CALLBACK_1(GameMainLayer::removeEnergyAnim, this)),
		nullptr ));

	return nullptr;
}

void GameMainLayer::removeEnergyAnim( Node* node )
{
	node->removeFromParentAndCleanup(true);
}

void GameMainLayer::callFuncRandAnim( float t )
{
	if ( !_decayIng )
	{
		// 创建闪电
		createEnergyFullAnim(Vec2(0,0));

		// 随机一个间隔进行创建闪电
		schedule(CC_SCHEDULE_SELECTOR(GameMainLayer::callFuncRandAnim), CCRANDOM_0_1()*5*0.1 );
	}
	else
	{
		unschedule( CC_SCHEDULE_SELECTOR(GameMainLayer::callFuncRandAnim) );
	}
}

void GameMainLayer::decayLifeBar( void )
{
	auto loadingBarEnergy			= (LoadingBar*)GameMainLayer::seekNodeByName(_mainLayerRootNode, "LoadingBarEnergy" );

	loadingBarEnergy->setPercent( 100 );
	decayEnergyBar();
	//schedule(CC_SCHEDULE_SELECTOR(GameMainLayer::callbackDecayLIfe), 0.5f );
}

void GameMainLayer::callbackDecayLIfe( float t )
{
	auto loadingBarEnergy			= (LoadingBar*)GameMainLayer::seekNodeByName(_mainLayerRootNode, "LoadingBarEnergy" );

	float curEnergy = loadingBarEnergy->getPercent();

	curEnergy -= 1;
	loadingBarEnergy->setPercent( curEnergy );

}

void GameMainLayer::playLifeBarWarning( void )
{
	auto loadingBarEnergy			= (Sprite*)GameMainLayer::seekNodeByName(_mainLayerRootNode, "imgloadbg" );

	loadingBarEnergy->runAction( RepeatForever::create(
		Sequence::create(TintTo::create(1.0f, 255, 75,75),TintTo::create(1.0f, 255, 255,255),nullptr)));
}

void GameMainLayer::HpDecrease( float t )
{
	if (m_CurHp > 0)
	{
		m_CurHp -= 0.1f;

		//当前血量百分比
		float p = m_CurHp/m_HpLimit * 100;

		auto hpBar = (LoadingBar*)GameMainLayer::seekNodeByName(_mainLayerRootNode, "LoadingBarEnergy" );
		hpBar->setPercent(p);

		//如果此时血量为0，那么闪烁
		if (m_CurHp <= 0.0f)
		{
			m_CurHp = 0.0f;
			auto barbg = (Sprite*)GameMainLayer::seekNodeByName(_mainLayerRootNode, "imgloadbg" );
			//闪烁
			ActionInterval* tint = TintTo::create(0.1f,255,0,0);
			ActionInterval* back = TintTo::create(0.1f,255,255,255);
			ActionInterval* seq = Sequence::create(tint,back,nullptr);
			ActionInterval* rep = RepeatForever::create(seq);
			barbg->runAction(rep);
			rep->setTag(20150328);
		}
	}
}

void GameMainLayer::AddHp( float num )
{
	//每次被撞前，先判断HP是否为0：
	//@ 如果为0，那么此次撞击造成伤害的话，如果有道具，则加上HP；否则主飞机死亡；
	//@ 如果还有HP，那么正常计算HP的加减，HP值减到0，飞机死亡
	if (m_isDead)
	{
		return;
	}
	if (!m_isCanBeHurt)
	{
		return;		//受到伤害期间不再受伤害
	}
	auto mianpl = GameData::getInstance()->getMainPlane();

	if (num < 0)
	{
		mianpl->WeaponDowngrade(); //武器降级

		//受到伤害时，飞机闪烁，屏幕闪烁
		CppCallJava::getInstance()->vibrate(350);
		UIController::getInstance()->getPhysicsLayer()->BeHurtWarning();
		NotificationCenter::getInstance()->postNotification(Msg_Shake,nullptr);
		WudiSometime(2.0f);
		mianpl->TintWhenHurt(2.0f);
	}
	bool isDead = false;
	m_CurHp += num;

	if (m_CurHp <= 0)	//HP是否为0
	{
		if (num < 0 )	//是否有伤害值
		{
			isDead = true;
		}
	}

	if (m_CurHp > m_HpLimit)
	{
		m_CurHp = m_HpLimit;
	}
	else if(m_CurHp < 0.0f)
	{
		m_CurHp = 0.0;
	}

	float p = m_CurHp/m_HpLimit * 100;
	auto hpBar = (LoadingBar*)GameMainLayer::seekNodeByName(_mainLayerRootNode, "LoadingBarEnergy" );
	hpBar->setPercent(p);

	auto barbg = (Sprite*)GameMainLayer::seekNodeByName( _mainLayerRootNode,"imgloadbg" );
	barbg->stopActionByTag(20150328);
	barbg->runAction( TintTo::create(0.1f, 255,255,255));

	//飞机死亡操作
	if (isDead)
	{
		int curRole = NewDataMgr::getInstance()->mRoleConfig.roleIndex;
		switch (curRole)
		{
		case 1:
			NewAudio::getInstance()->playEff(Music_MainPlDeadVoice1);
			break;
		case 2:
			NewAudio::getInstance()->playEff(Music_MainPlDeadVoice2);
			break;
		case 3:
			NewAudio::getInstance()->playEff(Music_MainPlDeadVoice3);
			break;

		default:
			break;
		}

		if (mianpl)
		{
			mianpl->setShootAble(false);
			mianpl->clearWeapon();
			mianpl->setTouchAble(false);
			mianpl->setShootMusicAble(false);
			mianpl->setCantContact();
		}

		NotificationCenter::getInstance()->postNotification(Msg_StopMainPlBullet,nullptr);
		
		CallFunc* func = CallFunc::create(this,callfunc_selector(GameMainLayer::GameFailed));

		//直接被打死,弹出游戏失败界面
		UIController::getInstance()->getEffectsLayer()->playActionOfMainPlDead(mianpl,(ActionInterval*)func);
	}
}

void GameMainLayer::GameFailed()
{
	GameData::getInstance()->getMainPlane()->setDirectDeath();
	GameData::getInstance()->GameOver(false);
}

void GameMainLayer::showBuyFuHuoWidget( void )
{
	BuyFuHuo::showLayout( this );

	DelayPuse(this);
// 
// 	auto widget = CommonWidget::create();
// 	widget->setContent( NewDataMgr::getInstance()->getXmlStringWithKey("goods_fuhuo_des").c_str(),
// 		NewDataMgr::getInstance()->getXmlStringWithKey("goods_fuhuo_buy").c_str());
// 	widget->addBtnExitClickEventListener( CC_CALLBACK_0(GameMainLayer::buyFuhuoCancel, this) );
// 	widget->addBtn1ClickEventListener( CC_CALLBACK_0(GameMainLayer::BuyFuHuoConfirm, this) );
// 	addChild( widget,15000 );

}

void GameMainLayer::showSettingWidget( void )
{
	SettingLayer::showLayout( this );
}

void GameMainLayer::buyFuhuoBtnEvent( CommonWidget::EventKey key )
{
	switch (key)
	{
	case CommonWidget::EventKey::EventKey_Exit:
		break;
	case CommonWidget::EventKey::EventKey_Btn1:
		break;
	case CommonWidget::EventKey::EventKey_Btn2:
		break;
	default:
		break;
	}
}

void GameMainLayer::buyFuhuoCancel( void )
{
	GameData::getInstance()->GameResume();

	// 取消购买 失败
	UIController::getInstance()->ShowResultLayer(false);
}

void GameMainLayer::BuyFuHuoConfirm( void )
{
	GameData::getInstance()->GameResume();

	//游戏状态
	GameData::getInstance()->setisGamePlaying(true);

	//死亡状态
	UIController::getInstance()->getMainUILayer()->SetRealDead(false);

	//血量回满
	UIController::getInstance()->getMainUILayer()->AddHp(10000);
	UIController::getInstance()->getPhysicsLayer()->addPlaneWithProtect();
}

void GameMainLayer::callbackPause( Ref* ref )
{
	UIController::getInstance()->getPhysicsLayer()->doPause();
}

void GameMainLayer::callbackDazhao( Ref* ref )
{
	// 大招道具处理
	if ( 0 >= NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey(BuyKey_Dazhao)->number )
	{
		GameMainLayer::DelayPuse(this);

		auto widget = CommonWidget::create();
		widget->setContent(__String::createWithFormat("%s%d%s","花费", NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey(BuyKey_Dazhao)->Gold * 2, "购买大招道具?")->getCString() );
		widget->addBtn1ClickEventListener( CC_CALLBACK_0(GameMainLayer::callFuncWidgetBuyDz, this) );
		widget->addBtnExitClickEventListener( CC_CALLBACK_0(GameMainLayer::callFuncWidgetBuyDzExit, this) );
		addChild( widget );

		return;
	}
	setDaZhaoState( false );

	NewDataMgr::getInstance()->mGameBuySystem->addGoodsNumWithKey(BuyKey_Dazhao, -1);

	int curRole = NewDataMgr::getInstance()->mRoleConfig.roleIndex;

	SpecialSkillTips* tips = SpecialSkillTips::create(curRole);
	addChild(tips);
	
	NewDataMgr::getInstance()->mGameBuySystem->addGoodsNumWithKey(BuyKey_Dazhao, -1);
}

Node * GameMainLayer::seekNodeByName(Node* widget, const std::string &name ) 
{ 
	if(!widget || widget->getName() == name) 
		return widget; 

	auto vecChildren = &widget->getChildren(); 
	std::vector<decltype(vecChildren)>vec; 
	vec.push_back(vecChildren); 
	size_t index = 0; 
	do  
	{ 
		vecChildren = vec[index]; 
		for(auto node : *vecChildren) 
		{ 
			if(node->getName() == name) 
			{ 
				return node; 
			} 
			vec.push_back(&node->getChildren()); 
		} 
		++index; 
	} while (index != vec.size()); 

	return nullptr; 
}

void GameMainLayer::GamePause()
{
	GameData::getInstance()->GamePause();
}

void GameMainLayer::DelayPuse(Node* node)
{
	ActionInterval* delay = DelayTime::create(0.15f);
	CallFunc* func = CallFunc::create(nullptr,callfunc_selector(GameMainLayer::GamePause));
	ActionInterval* seq = Sequence::create(delay, func, nullptr);
	node->runAction(seq);
}

void GameMainLayer::WudiSometime(float dur)
{
	m_isCanBeHurt = false;
	auto node = getChildByTag(4251206);
	if (node)
	{
		node->removeFromParentAndCleanup(true);
	}
	node = Node::create();
	addChild(node,0,4251206);

	ActionInterval* delay = DelayTime::create(dur);
	CallFuncN* funN = CallFuncN::create(this,callfuncN_selector(GameMainLayer::WudiOver));
	ActionInterval* seq = Sequence::create(delay, funN, nullptr);
	node->runAction(seq);
}

void GameMainLayer::WudiOver(Node* node)
{
	m_isCanBeHurt = true;
	node->removeFromParentAndCleanup(true);
}

void GameMainLayer::SetRealDead(bool b)
{
	m_isDead = b;
}

void GameMainLayer::playPassSucceed( void )
{
	auto ws = Director::getInstance()->getWinSize();

	auto nodePassSuccee = Node::create();
	nodePassSuccee->setPosition( Vec2(ws.width*0.55, ws.height * 0.82) );
	addChild(nodePassSuccee,1000);

	auto spriteYuan = Sprite::create( "Effect/passSuccee/zhandou4.png" );
	auto spriteZhen = Sprite::create( "Effect/passSuccee/zhandou3.png" );
	auto spriteChen = Sprite::create( "Effect/passSuccee/zhandou1.png" );
	auto spriteGong = Sprite::create( "Effect/passSuccee/zhandou2.png" );

	auto spriteTopGuang = Sprite::create( "Effect/passSuccee/guang2.png" );
	auto spriteDownGuang = Sprite::create( "Effect/passSuccee/guang2.png" );

	spriteYuan->setPosition( Vec2(-250, 0) );spriteYuan->setOpacity( 0 );spriteYuan->setScale( 1.4f );
	nodePassSuccee->addChild( spriteYuan, 0, "yuanNode" );
	spriteZhen->setPosition( Vec2(-100, 0) );spriteZhen->setOpacity( 0 );spriteZhen->setScale( 1.4f );
	nodePassSuccee->addChild( spriteZhen , 0, "zhenNode");
	spriteChen->setPosition( Vec2(50, 0) );spriteChen->setOpacity( 0 );spriteChen->setScale( 1.4f );
	nodePassSuccee->addChild( spriteChen  , 0, "chenNode");
	spriteGong->setPosition( Vec2(200, 0) );spriteGong->setOpacity( 0 );spriteGong->setScale( 1.4f );
	nodePassSuccee->addChild( spriteGong  , 0, "gongNode");



	float fadeInT = 0.2f;
	float delayT = 0.1f;


	spriteYuan->runAction( Sequence::create( CCDelayTime::create(delayT*1)	
		,CCSpawn::create(
		CCCallFuncN::create(this, callfuncN_selector(GameMainLayer::callFuncGuang1)),CCFadeIn::create(fadeInT), CCScaleTo::create( fadeInT ,1.0f ), nullptr)
		,nullptr ) );


	spriteZhen->runAction( Sequence::create( CCDelayTime::create(delayT*2)	
		,CCSpawn::create(
		CCCallFuncN::create(this, callfuncN_selector(GameMainLayer::callFuncGuang1)),CCFadeIn::create(fadeInT), CCScaleTo::create( fadeInT ,1.0f ), nullptr)
		,nullptr ) );


	spriteChen->runAction( Sequence::create( CCDelayTime::create(delayT*3)	
		,CCSpawn::create(
		CCCallFuncN::create(this, callfuncN_selector(GameMainLayer::callFuncGuang1)),CCFadeIn::create(fadeInT), CCScaleTo::create( fadeInT ,1.0f ), nullptr)
		,nullptr ) );


	spriteGong->runAction( Sequence::create( CCDelayTime::create(delayT*4)	
		,CCSpawn::create(
		CCCallFuncN::create(this, callfuncN_selector(GameMainLayer::callFuncGuang1)),CCFadeIn::create(fadeInT), CCScaleTo::create( fadeInT ,1.0f ), nullptr)
		,nullptr ) );


	spriteTopGuang->setPosition( Vec2(-300, 55 ) );spriteTopGuang->setOpacity( 0 );spriteTopGuang->setScaleX( 1.5f );
	nodePassSuccee->addChild( spriteTopGuang );

	spriteDownGuang->setPosition(Vec2(250, -55 )  );spriteDownGuang->setOpacity( 0 );spriteDownGuang->setScaleX( 1.5f );
	nodePassSuccee->addChild( spriteDownGuang );

	spriteTopGuang->runAction( Sequence::create(
		CCFadeIn::create(delayT*6)
		,CCMoveTo::create(0.5, spriteTopGuang->getPosition() + Vec2(600, 0) )
		,CCFadeOut::create(delayT*2)
		,nullptr ) );

	spriteDownGuang->runAction( Sequence::create(
		CCFadeIn::create(delayT*6)
		,CCMoveTo::create(0.5, spriteDownGuang->getPosition() + Vec2(-600, 0) )
		,CCFadeOut::create(delayT*2)
		,DelayTime::create( 0.5f)
		,CCCallFuncN::create(this, callfuncN_selector(GameMainLayer::callFuncNodeMove))
		,nullptr ) );

}

void GameMainLayer::callFuncGuang1( Node* node )
{
	auto spriteYuan = Sprite::create( "Effect/passSuccee/glow_0.png" );
	spriteYuan->setPosition( node->getContentSize()/2);
	node->addChild( spriteYuan );

	spriteYuan->setScale( 1.5f);
	spriteYuan->runAction( Sequence::create(
		CCSpawn::create(CCFadeOut::create(0.3f), CCScaleTo::create( 0.3f ,0.7f ), nullptr)
		,CCScaleTo::create( 0.01f ,1.5f )
		,DelayTime::create(0.3f)
		,CCFadeIn::create(0.01f)
		,DelayTime::create(0.1f)
		,CCFadeOut::create(0.02f)
		,nullptr ) );
}

void GameMainLayer::callFuncNodeMove( Node* node )
{
	auto nodePassSuccee = (Node*)GameMainLayer::seekNodeByName( _mainLayerRootNode, "NodePassSuccee" );
	nodePassSuccee->runAction( Spawn::create( MoveTo::create( 0.8f,  nodePassSuccee->getPosition() + Vec2(600, 0)), FadeOut::create( 0.7f ), nullptr));
}

void GameMainLayer::setDaZhaoState( bool state )
{
	Button* ButtonDaZhao = (Button*)GameMainLayer::seekNodeByName(_mainLayerRootNode, "ButtonDaZhao" );

	if ( !state || 0 == NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey(BuyKey_Dazhao)->number)
	{
		ButtonDaZhao->setTouchEnabled( false );
		//ButtonDaZhao->setColor( Color3B(74,74,74) );
	}
	else
	{
		ButtonDaZhao->setTouchEnabled( true );
		//ButtonDaZhao->setColor( Color3B(255,255,255) );
	}
}

void GameMainLayer::updataGamedistance( int km )
{
	auto SpriteKmIco = (Sprite*)GameMainLayer::seekNodeByName(_mainLayerRootNode, "SpriteKmIco" );
	// 更新成绩
	auto AtlasLabelDistance = (TextAtlas*)GameMainLayer::seekNodeByName(_mainLayerRootNode, "AtlasLabelDistance" );
	AtlasLabelDistance->setString( StringUtils::format("%d", km) );

	// 偏移KM 
	SpriteKmIco->setPosition( Vec2(AtlasLabelDistance->getContentSize().width,SpriteKmIco->getPosition().y) );
}

void GameMainLayer::playDaZhaoCD( void )
{
	Button* ButtonDaZhao = (Button*)GameMainLayer::seekNodeByName(_mainLayerRootNode, "ButtonDaZhao" );

	auto *s=CCSprite::create("ccsRes/WujinModeLayer/goodsbg.png");s->setOpacity( 140 );
	CCProgressTimer *pt=CCProgressTimer::create(s);
	pt->setReverseProgress(true);
	pt->setPosition( ButtonDaZhao->getPosition() );
	pt->setType(cocos2d::CCProgressTimerType(kCCProgressTimerTypeRadial));
	ButtonDaZhao->getParent()->addChild( pt );
	auto to1 = Sequence::createWithTwoActions(ProgressFromTo::create(0, 0, 0), ProgressFromTo::create(18, 100, 0));
	pt->runAction( Sequence::create(
		to1, 
		CCCallFuncN::create( this, callfuncN_selector(GameMainLayer::callFuncDaZhaoCD) ),
		nullptr));
}

void GameMainLayer::callFuncDaZhaoCD( Node* node )
{
	node->removeFromParentAndCleanup( true );

	setDaZhaoState( true );
}

void GameMainLayer::startDaZhaoSD( void )
{
	playDaZhaoCD();
}

void GameMainLayer::callFuncWidgetBuyDz( void )
{
	int goodsprice = NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey(BuyKey_Dazhao)->Gold *2;

	if ( NewDataMgr::getInstance()->getGold() < goodsprice )
	{
		auto widget = CommonWidget::create();
		widget->setContent("金币不足!");
		widget->addBtn1ClickEventListener( CC_CALLBACK_0(GameMainLayer::callWidgetGoldTips, this) );
		widget->addBtnExitClickEventListener( CC_CALLBACK_0(GameMainLayer::callWidgetGoldTips, this) );
		addChild( widget );
		return;
	}
	// 减金币
	NewDataMgr::getInstance()->addGold( -goodsprice );

	setDaZhaoState( false );
	GameData::getInstance()->GameResume();

	NewDataMgr::getInstance()->mGameBuySystem->addGoodsNumWithKey(BuyKey_Dazhao, -1);

	int curRole = NewDataMgr::getInstance()->mRoleConfig.roleIndex;

	SpecialSkillTips* tips = SpecialSkillTips::create(curRole);
	addChild(tips);

	NewDataMgr::getInstance()->mGameBuySystem->addGoodsNumWithKey(BuyKey_Dazhao, -1);
}

void GameMainLayer::callFuncWidgetBuyDzExit( void )
{
	GameData::getInstance()->GameResume();
}

void GameMainLayer::callWidgetGoldTips( void )
{
	GameData::getInstance()->GameResume();
}




// 复活

void BuyFuHuo::showLayout( Node* node )
{
	if ( node && !node->getChildByName("BuyFuHuo") )
	{
		auto layout = BuyFuHuo::create();
		node->addChild( layout, 1, "BuyFuHuo" );
	}
}

void BuyFuHuo::onEnter()
{
	Layer::onEnter();

	initInterface();
}

void BuyFuHuo::initInterface()
{
	_fuHuoLayout = CSLoader::createNode( Ccs_FuHuoLayer_FilePath );

	if ( !_fuHuoLayout )
	{
		return;
	}

	addChild( _fuHuoLayout );

	do 
	{
		auto ButtonFuHuo = (Button*)GameMainLayer::seekNodeByName(_fuHuoLayout, "ButtonFuHuo" );
		setEventForNode( ButtonFuHuo, Tag_BuyFuhuo );


		auto Buttonclose = (Button*)GameMainLayer::seekNodeByName(_fuHuoLayout, "Buttonclose" );
		setEventForNode( Buttonclose, Tag_Close );

		//地图进度
		float mapPer = UIController::getInstance()->getGameBgMgr()->getCurBgId();
		auto LoadingBarGame = (LoadingBar*)GameMainLayer::seekNodeByName(_fuHuoLayout, "LoadingBarYuanzhen");
		LoadingBarGame->setPercent(((mapPer / 3) * 100) );

		auto AtlasLabelYuanZhenPer = (TextAtlas*)GameMainLayer::seekNodeByName(_fuHuoLayout, "AtlasLabelYuanZhenPer");
		AtlasLabelYuanZhenPer->setString(StringUtils::format("%d/", int((mapPer / 3) * 100)));

	}while(0);

}

void BuyFuHuo::setEventForNode( Button* node, ButtonTag tag )
{
	do 
	{
		CC_BREAK_IF( node == nullptr );

		node->setTag( tag );
		node->addClickEventListener(CC_CALLBACK_1(BuyFuHuo::callFuncButtonEvent, this));


	} while (0);
}

void BuyFuHuo::callFuncButtonEvent( Ref* ref )
{
	Button* btn = (Button*)ref;
	int nTag = btn->getTag();


	switch ( nTag )
	{

	case Tag_BuyFuhuo:
		{
			removeFromParentAndCleanup( true );
			BuyFuHuoConfirm();
		}
		break;	

	case Tag_Close:
		{
			removeFromParentAndCleanup( true );
			buyFuhuoCancel();
		}
		break;	

		default:
			break;
	}
}

void BuyFuHuo::buyFuhuoCancel( void )
{
	GameData::getInstance()->GameResume();

	// 取消购买 失败
	UIController::getInstance()->ShowResultLayer(false);
}

void BuyFuHuo::BuyFuHuoConfirm( void )
{
	GameData::getInstance()->GameResume();

	//游戏状态
	GameData::getInstance()->setisGamePlaying(true);

	//死亡状态
	UIController::getInstance()->getMainUILayer()->SetRealDead(false);

	//血量回满
	UIController::getInstance()->getMainUILayer()->AddHp(10000);
	UIController::getInstance()->getPhysicsLayer()->addPlaneWithProtect();

}

void BuyFuHuo::callFuncAction1( Node* node )
{
	auto AtlasLabelFHTime = (TextAtlas*)node;

	int time = atoi( AtlasLabelFHTime->getString().c_str() );

	time -= 1;
	if ( -1 == time )
	{
		AtlasLabelFHTime->stopAllActions();
		buyFuhuoCancel();
	}
	else
	{
		AtlasLabelFHTime->setString( __String::createWithFormat("%d", time )->getCString() );
	}

}

// 设置

void SettingLayer::showLayout( Node* node )
{
	if ( node && !node->getChildByName("SettingLayer") )
	{
		auto layout = SettingLayer::create();
		node->addChild( layout, 1, "SettingLayer" );
	}
}

void SettingLayer::onEnter()
{
	Layer::onEnter();
	initInterface();
}

void SettingLayer::initInterface()
{
	_settingayout = CSLoader::createNode( Ccs_SettingLayer_FilePath );

	if ( !_settingayout )
	{
		return;
	}

	addChild( _settingayout );

	do 
	{
		auto ButtonExitGame = (Button*)GameMainLayer::seekNodeByName(_settingayout, "ButtonExitGame" );
		setEventForNode( ButtonExitGame, Tag_ExitGame );

		auto ButtonContinue = (Button*)GameMainLayer::seekNodeByName(_settingayout, "ButtonContinue" );
		setEventForNode( ButtonContinue, Tag_ContinueGame );

		// 音效
		auto CheckBoxMuice = (CheckBox*)GameMainLayer::seekNodeByName(_settingayout, "CheckBoxMuice" );
		CC_BREAK_IF( CheckBoxMuice == nullptr );
		CheckBoxMuice->addEventListener(CC_CALLBACK_2(SettingLayer::selectedMusicEvent, this));
		CheckBoxMuice->setSelected( !NewAudio::getInstance()->getMusicState() );

		// 音乐
		auto CheckBoxSound = (CheckBox*)GameMainLayer::seekNodeByName(_settingayout, "CheckBoxSound" );
		CC_BREAK_IF( CheckBoxSound == nullptr );
		CheckBoxSound->addEventListener(CC_CALLBACK_2(SettingLayer::selectedSoundEvent, this));
		CheckBoxSound->setSelected( !NewAudio::getInstance()->getSoundEffState() );


	}while(0);
}

void SettingLayer::setEventForNode( Button* node, ButtonTag tag )
{
	do 
	{
		CC_BREAK_IF( node == nullptr );

		node->setTag( tag );
		node->addClickEventListener(CC_CALLBACK_1(SettingLayer::callFuncButtonEvent, this));


	} while (0);
}

void SettingLayer::callFuncButtonEvent( Ref* ref )
{
	Button* btn = (Button*)ref;
	int nTag = btn->getTag();


	switch ( nTag )
	{
	case Tag_ExitGame:
		{
            EnemyPlaneController::destroyInstance();
			GameData::getInstance()->GameResume();
			GameSceneMgr::getInstance()->changeScene( Scene_Type_RoleInterface );
		}
		break;	

	case Tag_ContinueGame:
		{
			removeFromParentAndCleanup( true );
			GameData::getInstance()->GameResume();
		}
		break;	
	default:
		break;
	}
}

void SettingLayer::selectedMusicEvent( Ref* pSender,CheckBox::EventType type )
{
	switch (type)
	{
	case CheckBox::EventType::UNSELECTED:
		NewAudio::getInstance()->setMusicState(false);
		break;

	case CheckBox::EventType::SELECTED:
		NewAudio::getInstance()->setMusicState(true);
		break;

	default:
		break;
	}
}

void SettingLayer::selectedSoundEvent( Ref* pSender,CheckBox::EventType type )
{
	switch (type)
	{
	case CheckBox::EventType::UNSELECTED:
		NewAudio::getInstance()->setSoundEffState(false);
		break;

	case CheckBox::EventType::SELECTED:
		NewAudio::getInstance()->setSoundEffState(true);
		break;

	default:
		break;
	}
}
