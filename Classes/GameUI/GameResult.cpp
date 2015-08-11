#include "GameResult.h"
#include "GameSceneMgr.h"
#include "../GameCommon/GameData.h"
#include "../GameCommon/ActionRecord.h"
#include "../GameCommon/LabelNumAction.h"
#include "../GamePlaying/BasePlane/PlaneManager.h"
#include "GameCommon/GlobalSchedule.h"
#include "GameCommon/NewDataMgr.h"
#include "GameMainLayer.h"
#include "GameCommon/CommonRes.h"
#include "GameRoleScene.h"
#include "GamePlaying/Layer/RollingBg.h"

bool GameResult::init()
{
	if (!Layer::init())
	{
		return false;
	}

	_resultWidget = nullptr;
	_rollCurScore = 0;
	_countGetRewardNum = 0;
	_crossing = false;

	return true;
}

void GameResult::onEnter()
{
	Layer::onEnter();

	//分析统计数据
	GameData::getInstance()->AnalyseStaticsData();
	// 完成本局数据的任务处理
	NewDataMgr::getInstance()->mGameTaskSystem->setGameTaskProperty( GameData::getInstance()->getAllStatistics() );

	initInterface();
	changeResoultInterface( Interface_Tag_Resout );

	// 显示任务栏
	MainSceneTaskBar::showLayout( this ,false );
	GameInterfaceMgr::getInstance()->setParentNode( this );
} 

void GameResult::onExit()
{
	Layer::onExit();

}
void GameResult::onTouchEnded( Ref *sender, Widget::TouchEventType controlEvent )
{
	if (controlEvent == Widget::TouchEventType::ENDED) 
	{
		// 切换到抽奖界面
		if ( _curInterfaceTag == Interface_Tag_Resout )
		{
			changeResoultInterface( Interface_Tag_ReWard );
		}	
	}
}

void GameResult::showResultWidget( bool resoult )
{
	int goldnum = GameData::getInstance()->getGoldNum();
	NewDataMgr::getInstance()->addGold( goldnum );

	GameData::getInstance()->setisDisabledKey(true);

	// 发送记录点
	userAction::getInstance()->sendRecordingMsg( RECODING_MSG_ACTIONID_1005,
		nullptr,
		__String::createWithFormat("%d", GameData::getInstance()->getPlaneManager()->getCurLvl() )->getCString(), 
		__String::createWithFormat("%d", resoult )->getCString() );

	if ( !Director::getInstance()->getRunningScene()->getChildByName("GameResult") )
	{
		auto layout = GameResult::create();
		Director::getInstance()->getRunningScene()->addChild( layout, 10000, "GameResult" );
	}
	

}

void GameResult::initInterface( void )
{
	_resultWidget = CSLoader::createNode( Ccs_ResoutLayer_FilePath );

	if ( !_resultWidget )
	{
		return;
	}
	addChild( _resultWidget );

	do 
	{
		// 添加触摸事件 
		Widget* panelWidget = (Widget*)GameMainLayer::seekNodeByName( _resultWidget, "Panel_1_2");
		panelWidget->addTouchEventListener(CC_CALLBACK_2(GameResult::onTouchEnded, this));

		auto tempData = GameData::getInstance()->getAllStatistics();

		// 返回
		auto ButtonBack = (Button*)GameMainLayer::seekNodeByName(_resultWidget, "ButtonBack" );
		ButtonBack->setVisible( false );
		ButtonBack->setTouchEnabled( false );
		setEventForNode( ButtonBack, Tag_Back );
		//setEventForNode( ButtonBack, Tag_Back );

		auto TextRewardTips = (Text*)GameMainLayer::seekNodeByName( _resultWidget,  "TextRewardTips" ); 
		TextRewardTips->setString( "点击卡牌领取奖励" );

		// 每张卡牌得分
		auto AtlasLabelCardReward1 = (TextAtlas*)GameMainLayer::seekNodeByName( _resultWidget,  "AtlasLabelCardReward1" ); 
		auto AtlasLabelCardReward2 = (TextAtlas*)GameMainLayer::seekNodeByName( _resultWidget,  "AtlasLabelCardReward2" ); 
		auto AtlasLabelCardReward3 = (TextAtlas*)GameMainLayer::seekNodeByName( _resultWidget,  "AtlasLabelCardReward3" ); 

		AtlasLabelCardReward1->setString( __String::createWithFormat("%d", tempData[GamePrKey_RedScore]		)->getCString() );
		AtlasLabelCardReward2->setString( __String::createWithFormat("%d", tempData[GamePrKey_BlueScore]		)->getCString() );
		AtlasLabelCardReward3->setString( __String::createWithFormat("%d", tempData[GamePrKey_YellowScore]		)->getCString() );

		// 每张卡牌获得金币
		float cardGoldConvCoe = 0.023f;
		int goldCoe = tempData[GamePrKey_RedScore] * cardGoldConvCoe/20;
		auto AtlasLabelCardGold1 = (TextAtlas*)GameMainLayer::seekNodeByName( _resultWidget,  "AtlasLabelCardGold1" ); 
		AtlasLabelCardGold1->setString( "0" );
		AtlasLabelCardGold1->setTag( tempData[GamePrKey_RedScore]* cardGoldConvCoe );
		AtlasLabelCardGold1->runAction( Sequence::create( 
			DelayTime::create(1.0f), 
			CCCallFuncND::create(this, callfuncND_selector(GameResult::getLabelNumAction), (void*)goldCoe),
			nullptr) );

		goldCoe = tempData[GamePrKey_BlueScore] * cardGoldConvCoe / 20;
		auto AtlasLabelCardGold2 = (TextAtlas*)GameMainLayer::seekNodeByName( _resultWidget,  "AtlasLabelCardGold2" ); 
		AtlasLabelCardGold2->setString( "0" );
		AtlasLabelCardGold2->setTag( tempData[GamePrKey_BlueScore]* cardGoldConvCoe);
		AtlasLabelCardGold2->runAction( Sequence::create( 
			DelayTime::create(1.5f), 
			CCCallFuncND::create( this, callfuncND_selector(GameResult::getLabelNumAction),(void*) goldCoe ),
			nullptr) );

		goldCoe = tempData[GamePrKey_YellowScore] * cardGoldConvCoe / 20;
		auto AtlasLabelCardGold3 = (TextAtlas*)GameMainLayer::seekNodeByName(_resultWidget, "AtlasLabelCardGold3");
		AtlasLabelCardGold3->setString( "0" );
		AtlasLabelCardGold3->setTag( tempData[GamePrKey_YellowScore]* cardGoldConvCoe);
		AtlasLabelCardGold3->runAction( Sequence::create( 
			DelayTime::create(2.0f), 
			CCCallFuncND::create( this, callfuncND_selector(GameResult::getLabelNumAction),(void*) goldCoe ),
			nullptr) );


		// 成绩
		auto AtlasLabelRewardScores = (TextAtlas*)GameMainLayer::seekNodeByName( _resultWidget,  "AtlasLabelRewardScores" ); 
		AtlasLabelRewardScores->setString( "0" );
		AtlasLabelRewardScores->setTag( GameData::getInstance()->getScoreNum()  );
		AtlasLabelRewardScores->runAction( Sequence::create( 
			DelayTime::create(0.5f), 
			CCCallFuncND::create( this, callfuncND_selector(GameResult::getLabelNumAction),(void*) (GameData::getInstance()->getScoreNum()/20) ),
			nullptr) );


		// 金币
		auto AtlasLabelRewardGold = (TextAtlas*)GameMainLayer::seekNodeByName( _resultWidget,  "AtlasLabelRewardGold" ); 
		AtlasLabelRewardGold->setString( "0" );
		AtlasLabelRewardGold->setTag( GameData::getInstance()->getGoldNum()  );
		AtlasLabelRewardGold->runAction( Sequence::create( 
			DelayTime::create(0.5f), 
			CCCallFuncND::create( this, callfuncND_selector(GameResult::getLabelNumAction),(void*) (GameData::getInstance()->getGoldNum()/20) ),
			nullptr) );

		// 幸运加成
		int luckCoc = NewDataMgr::getInstance()->getRoleLuckWithId( NewDataMgr::getInstance()->mRoleConfig.roleIndex );
		auto TextRewordJiaChen = (Text*)GameMainLayer::seekNodeByName( _resultWidget,  "TextRewordJiaChen" ); 
		TextRewordJiaChen->setString( __String::createWithFormat("+%d%", luckCoc )->getCString() );

		//地图进度
		float mapPer = UIController::getInstance()->getGameBgMgr()->getCurBgId();
		auto LoadingBarGame = (LoadingBar*)GameMainLayer::seekNodeByName( _resultWidget,  "LoadingBarGame" ); 
		LoadingBarGame->setPercent( (mapPer/3)*100 );

		// 卡牌设置
		for ( int i = 1; i<=3; i++ )
		{
			for( int j = 1; j<=5; j++ )
			{
				// 星星等级
				auto Lv5Star = (Node*)GameMainLayer::seekNodeByName(_resultWidget, __String::createWithFormat("LvStar%d_%d", i, j)->getCString() );

				if ( NewDataMgr::getInstance()->mRoleConfig.cardArr[i-1].cardStarLv < j )
					Lv5Star->setVisible( false );
				else
					Lv5Star->setVisible( true );
			}
			// 设置头像
			std::string cardHeadPath = __String::createWithFormat("%s%d.png", Img_CardHead_FilePath ,  NewDataMgr::getInstance()->mRoleConfig.cardArr[i-1].cardID)->getCString();
			auto ButtonCard = (Button*)GameMainLayer::seekNodeByName(_resultWidget, __String::createWithFormat("ButtonCard%d", i)->getCString());
			ButtonCard->loadTextures( cardHeadPath, cardHeadPath, cardHeadPath );

			// 等级框
			auto StarLvFream = (Sprite*)GameMainLayer::seekNodeByName(_resultWidget, __String::createWithFormat("StarLvFream_%d", i)->getCString());

			std::string tempName = __String::createWithFormat( "ccsRes/cardOptionLayer/starLv_%d.png", NewDataMgr::getInstance()->mRoleConfig.cardArr[i-1].cardStarLv)->getCString();
			StarLvFream->setTexture( tempName );
		}

		// 历史新高
		auto SpriteNewResult = (Sprite*)GameMainLayer::seekNodeByName(_resultWidget,"SpriteNewResult");

		if ( NewDataMgr::getInstance()->getHistoryHighResult() >= GameData::getInstance()->getScoreNum() )
		{
			SpriteNewResult->setVisible( false );
		}
		else
		{
			SpriteNewResult->setScale( 1.8f );
			SpriteNewResult->setOpacity( 0 );
			SpriteNewResult->runAction( Sequence::create(
				DelayTime::create(1.0f),
				CCSpawn::create(
				CCEaseSineIn::create(ScaleTo::create(0.2f, 1.0f)),
				CCFadeIn::create((0.2f)),
				nullptr),nullptr) );
		}


		// 成绩光
		auto SpriteResultGuang = (Sprite*)GameMainLayer::seekNodeByName(_resultWidget,"SpriteResultGuang");
		SpriteResultGuang->runAction(Sequence::create(
			Sequence::create(ScaleTo::create(0.2f, 3.4f, 3.5f),ScaleTo::create(0.1, 3.4f, 3.0f),ScaleTo::create(0.1, 3.4f, 3.5f),ScaleTo::create(0.1, 3.4f, 3.2f),ScaleTo::create(0.1, 3.4f, 3.5f),nullptr),
			DelayTime::create(1.5f),
			CCSpawn::create(ScaleTo::create(0.2, 3.4f, 30), CCFadeOut::create((0.2)), nullptr),
			nullptr));

		initRewardInterface();

		// boss评级
		auto PerfectNum = (TextAtlas*)GameMainLayer::seekNodeByName( _resultWidget,  "PerfectNum" ); 
		PerfectNum->setString( __String::createWithFormat(".%d", tempData[GamePrKey_SSGradeNum] )->getCString() );
		auto CoolNum	= (TextAtlas*)GameMainLayer::seekNodeByName( _resultWidget,  "CoolNum" ); 
		CoolNum->setString( __String::createWithFormat(".%d", tempData[GamePrKey_SGradeNum] )->getCString() );
		auto GoodNum	= (TextAtlas*)GameMainLayer::seekNodeByName( _resultWidget,  "GoodNum" ); 
		GoodNum->setString( __String::createWithFormat(".%d", tempData[GamePrKey_BGradeNum] )->getCString() );
		auto GreatNum	= (TextAtlas*)GameMainLayer::seekNodeByName( _resultWidget,  "GreatNum" ); 
		GreatNum->setString( __String::createWithFormat(".%d", tempData[GamePrKey_AGradeNum] )->getCString() );


	} while (0);
}

void GameResult::initRewardInterface( void )
{
	// 抽奖
	auto NodeReward1 = (Node*)GameMainLayer::seekNodeByName( _resultWidget, "NodeReward1" );
	auto NodeReward2 = (Node*)GameMainLayer::seekNodeByName( _resultWidget, "NodeReward2" );
	auto NodeReward3 = (Node*)GameMainLayer::seekNodeByName( _resultWidget, "NodeReward3" );


	_rewardCard[0] = Sprite::create("roleImg/jiesuan17.png");
	NodeReward1->addChild( _rewardCard[0], 2,  "rewardGoods1" );
	_rewardCard[0]->setTag( 1 );
	_rewardCard[0]->setPosition( Vec2(0, 1400) );
	_rewardCard[0]->setCascadeOpacityEnabled(true);

	_rewardCard[1] = Sprite::create("roleImg/jiesuan17.png");
	NodeReward2->addChild( _rewardCard[1], 2,  "rewardGoods2"  );
	_rewardCard[1]->setTag( 1 );
	_rewardCard[1]->setPosition( Vec2(0, 1400) );
	_rewardCard[1]->setCascadeOpacityEnabled(true);

	_rewardCard[2] = Sprite::create("roleImg/jiesuan17.png");
	NodeReward3->addChild( _rewardCard[2], 2,  "rewardGoods3"  );
	_rewardCard[2]->setTag( 1 );
	_rewardCard[2]->setPosition( Vec2(0, 1400) );
	_rewardCard[2]->setCascadeOpacityEnabled(true);

	auto listener1 = EventListenerTouchOneByOne::create();
	listener1->setSwallowTouches(true);

	std::shared_ptr<bool> firstClick(new bool(true));

	listener1->onTouchBegan = [=](Touch* touch, Event* event){

		if ( Interface_Tag_Resout == _curInterfaceTag )
		{
			changeResoultInterface( Interface_Tag_ReWard );
			return true;
		}

		Vec2 locationInNode1 = _rewardCard[0]->convertToNodeSpace(touch->getLocation());
		Vec2 locationInNode2 = _rewardCard[1]->convertToNodeSpace(touch->getLocation());
		Vec2 locationInNode3 = _rewardCard[2]->convertToNodeSpace(touch->getLocation());
		Size s = _rewardCard[0]->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		auto action = Sequence::create(
			CCOrbitCamera::create(0.2,1, 0, 0, 90, 0, 0)
			,CCCallFuncN::create(this , callfuncN_selector(GameResult::callFuncReversal) ) 
			,CCOrbitCamera::create(0.2,1, 0, -90, -90, 0, 0)
			, DelayTime::create(0.3f)
			, CCCallFuncN::create(this, callfuncN_selector(GameResult::callFuncFlashAction))
			, DelayTime::create(1.5f)
			, CCSpawn::create(CCMoveBy::create(0.5f, Vec2(0, 300)), CCFadeOut::create(0.5f), nullptr)
			, DelayTime::create(1.0f)
			, CCCallFuncN::create(this, callfuncN_selector(GameResult::callFuncRewardFinish))
			,NULL);

		if ( _crossing )
			return true;

// 		_rewardCard[0]->stopAllActions();
// 		_rewardCard[1]->stopAllActions();
// 		_rewardCard[2]->stopAllActions();

		if (rect.containsPoint(locationInNode1) && 1 == _rewardCard[0]->getTag() && isBuyReward() )
		{
			_crossing = true;
			_countGetRewardNum += 1;
			_rewardCard[0]->setTag(0);
			_rewardCard[0]->runAction( action );
			updateRewardInterface();
			return true;
		}
		else if(rect.containsPoint(locationInNode2)&& 1 == _rewardCard[1]->getTag()&& isBuyReward())
		{
			_crossing = true;
			_countGetRewardNum += 1;
			_rewardCard[1]->setTag(0);
			_rewardCard[1]->runAction( action );
			updateRewardInterface();
			return true;
		}
		else if(rect.containsPoint(locationInNode3)&& 1 == _rewardCard[2]->getTag()&& isBuyReward())
		{
			_crossing = true;
			_countGetRewardNum += 1;
			_rewardCard[2]->setTag(0);
			_rewardCard[2]->runAction( action );
			updateRewardInterface();
			return true;
		}

		return true;
	};
	updateRewardInterface();

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, _rewardCard[0]);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1->clone(), _rewardCard[1]);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1->clone(), _rewardCard[2]);
}

void GameResult::playRewardAction( void )
{
	_rewardCard[0]->runAction(Sequence::create( DelayTime::create(0.5f),CCMoveTo::create(0.2f, Vec2(0, 0)), nullptr)  );
	_rewardCard[1]->runAction(Sequence::create( DelayTime::create(0.8f),CCMoveTo::create(0.2f, Vec2(0, 0)), nullptr)  );
	_rewardCard[2]->runAction(Sequence::create( DelayTime::create(1.1f),CCMoveTo::create(0.2f, Vec2(0, 0)), nullptr)  );

	// 提示
	auto TextRewardTips = (Text*)GameMainLayer::seekNodeByName( _resultWidget,  "TextRewardTips" ); 
	TextRewardTips->runAction( CCRepeatForever::create(  
		CCSequence::create(
		CCFadeOut::create(0.5f), 
		CCFadeIn::create(0.5f),
		nullptr) ) );
}

void GameResult::updateRewardInterface( void )
{
	auto SpriteRewardPrice1 = (Sprite*)GameMainLayer::seekNodeByName( _resultWidget, "SpriteRewardPrice1" );
	auto SpriteRewardPrice2 = (Sprite*)GameMainLayer::seekNodeByName( _resultWidget, "SpriteRewardPrice2" );
	auto SpriteRewardPrice3 = (Sprite*)GameMainLayer::seekNodeByName( _resultWidget, "SpriteRewardPrice3" );
	SpriteRewardPrice1->setVisible( false );
	SpriteRewardPrice2->setVisible( false );
	SpriteRewardPrice3->setVisible( false );

	if ( 1 == _rewardCard[0]->getTag() && _countGetRewardNum )
	{
		SpriteRewardPrice1->setVisible( true );
	}

	if ( 1 == _rewardCard[1]->getTag() && _countGetRewardNum )
	{
		SpriteRewardPrice2->setVisible( true );
	}

	if ( 1 == _rewardCard[2]->getTag() && _countGetRewardNum )
	{
		SpriteRewardPrice3->setVisible( true );
	}

	// 三张翻完
	if ( 3 <= _countGetRewardNum )
	{
		auto TextRewardTips = (Text*)GameMainLayer::seekNodeByName( _resultWidget,  "TextRewardTips" ); 
		TextRewardTips->setVisible( false );
	}

}

void GameResult::setEventForNode( Button* node, ButtonTag tag )
{
	do 
	{
		CC_BREAK_IF( node == nullptr );

		node->setTag( tag );
		node->addClickEventListener(CC_CALLBACK_1(GameResult::callFuncButtonEvent, this));

	} while (0);
}

void GameResult::callFuncButtonEvent( Ref* ref )
{
	Button* btn = (Button*)ref;
	int nTag = btn->getTag();

	switch ( nTag )
	{
		// 再来一局
	case Tag_Again:
		{
			GameSceneMgr::getInstance()->changeScene( Scene_Type_RoleInterface );
		}
		break;

		// 返回到角色选择主界面
	case Tag_Back:
		{
			Director::getInstance()->resume();

			GameSceneMgr::getInstance()->changeScene( Scene_Type_RoleInterface );
		}
		break;
	default:
		break;
	}
}

void GameResult::changeResoultInterface( InterfaceTag tag )
{
	auto size = Director::getInstance()->getVisibleSize();
	float centreX  = size.width/2;

	auto NodeResout = (Node*)GameMainLayer::seekNodeByName(_resultWidget, "NodeResout" );
	auto PanelReward = (Node*)GameMainLayer::seekNodeByName(_resultWidget, "PanelReward" );

	_curInterfaceTag = tag;
	switch ( tag )
	{
	case Interface_Tag_Resout:
		NodeResout->setPosition( Vec2(centreX, NodeResout->getPosition().y) );
		PanelReward->setPosition( Vec2(-centreX, PanelReward->getPosition().y) );
		break;

	case Interface_Tag_ReWard:
		playRewardAction();
		NodeResout->setPosition( Vec2(-centreX, NodeResout->getPosition().y) );
		PanelReward->setPosition( Vec2(centreX, PanelReward->getPosition().y) );
		break;

	default:
		break;
	}
}

void GameResult::callFuncReversal( Node* node )
{
	auto cardSprite = (Sprite*)node;

	if( cardSprite )
	{
		if ( 1 /*== cardSprite->getTag()*/ )
		{
			//cardSprite->setTag(0);
			cardSprite->setScaleX( -1 );
			cardSprite->setTexture("roleImg/jiesuan16.png");

			auto cardFlash = Sprite::create("roleImg/cardFreamK.png");
			cardFlash->setOpacity(0);
			cardFlash->setPosition(Vec2(node->getContentSize().width / 2 , node->getContentSize().height / 2 ));
			node->addChild(cardFlash, 1, "flash");

			int randGoodsID = random((int)RewardKey_7, (int)RewardKey_13);
			auto goodsNode = RewardGoodsNode::create( (RewardGoodsKey)randGoodsID );
			goodsNode->setScale( 1.45f );
			goodsNode->setPosition( Vec2(node->getContentSize().width/2 + 3, node->getContentSize().height/2-33) );
			goodsNode->setCascadeOpacityEnabled(true);
			node->addChild(goodsNode, 2, "goods");
			cardSprite->setTag( randGoodsID );

		}
		// 反复翻牌 test
		else
		{
			cardSprite->setTag( 1 );
			cardSprite->setTexture("roleImg/jiesuan17.png");
			cardSprite->removeAllChildrenWithCleanup(true);
		}
	}
}

void GameResult::callFuncFlashAction(Node* node)
{
	auto cardFlash = (Sprite*)node->getChildByName("flash");

	cardFlash->runAction(Sequence::create(
		CCFadeIn::create( 0.2f ),
		CCFadeOut::create( 0.5f),
		nullptr));
}

void GameResult::callFuncRewardFinish( Node* node )
{
	if ( _countGetRewardNum )
	{
		auto TextRewardTips = (Text*)GameMainLayer::seekNodeByName( _resultWidget,  "TextRewardTips" ); 
		TextRewardTips->setString( "花费5000金币再抽一张？" );

		// 返回
		auto ButtonBack = (Button*)GameMainLayer::seekNodeByName(_resultWidget, "ButtonBack" );
		ButtonBack->setVisible( true );
		ButtonBack->setTouchEnabled( true );

		int goodsTag = node->getTag();
		NewDataMgr::getInstance()->goodsRewardWithKey( (RewardGoodsKey)goodsTag  );
	}

	// 翻完
	_crossing = false;
}

void GameResult::getLabelNumAction( Node* node, void* data )
{

	auto action = CCRepeatForever::create( Sequence::create(	
		DelayTime::create(0.01f),
		CCCallFuncND::create(this, callfuncND_selector(GameResult::callFuncLabelNumAction) ,data ),
		nullptr));

	node->runAction( action );
}

void GameResult::callFuncLabelNumAction( Node* node, void* data )
{
	auto lableNum = (TextAtlas*)node;
	
	int curNum = atoi( lableNum->getString().c_str() );
	int tagerNum = lableNum->getTag();

	int goleCoe = (intptr_t)data;
	curNum += goleCoe;

	if (curNum>=tagerNum)
	{
		curNum = tagerNum;
		lableNum->stopAllActions();
	}

	lableNum->setString( __String::createWithFormat("%d", curNum)->getCString() );

}

bool GameResult::isBuyReward()
{
	if ( 0 == _countGetRewardNum )
	{
		return true;
	}

	if ( 5000 > NewDataMgr::getInstance()->getGold() )
	{
		auto widget = CommonWidget::create();
		widget->setContent("金币不足");
		addChild( widget );
		return false;
	}
	else
	{
		NewDataMgr::getInstance()->addGold( -5000 );
		NotificationCenter::getInstance()->postNotification(Msg_ChangeGemGold,nullptr);

		return true;

	}

	
}
