#include "GameRoleScene.h"
#include "GameSceneMgr.h"
#include "CommonWidget.h"
#include "../GameCommon/CircleStageList.h"
#include "GameMainLayer.h"
#include "../GameCommon/NewAudio.h"
#include "../GameCommon/CommonRes.h"
#include "GameCommon/GameDefine.h"
#include "JniHelp/CppCallJava.h"
#include <algorithm>


void GameInterfaceMgr::addInterfaceToList( InterfaceTag tag , bool isShow/* = true*/, void* data/* = nullptr*/  )
{
	for ( std::list<InterfaceTag>::iterator item = _interfaceVector.begin(); item != _interfaceVector.end(); item++ )
	{
		if (tag == *item)
		{
			return;
		}
	}

	_interfaceVector.push_back( tag );
	if (isShow)
	{
		_data = data;
		showNextInterface();
	}
}

void GameInterfaceMgr::showNextInterface( void )
{
	if ( _interfaceVector.empty() )
	{
		return;
	}

 	std::list<InterfaceTag>::iterator item = _interfaceVector.end();

	InterfaceTag tag = *(--item);
	//_interfaceVector.erase(item);

	switch (tag)
	{
	case GameInterfaceMgr::InterfaceTag_RoleOption:
		RoleOptionlayer::showLayout( _node, tag  );
		NotificationCenter::getInstance()->postNotification(Msg_HideButtomBar,nullptr);
		break;

	case GameInterfaceMgr::InterfaceTag_CardOption:
		CardOptionLayout::showLayout( _node, tag );
		NotificationCenter::getInstance()->postNotification(Msg_ShowButtomBar,nullptr);
		break;

	case GameInterfaceMgr::InterfaceTag_CompoundCard:
		CardCompoundLayer::showLayout( _node, tag );
		NotificationCenter::getInstance()->postNotification(Msg_ShowButtomBar,nullptr);
		break;

	case GameInterfaceMgr::InterfaceTag_Reward:
		GameRewardLayer::showLayout( _node, tag );
		NotificationCenter::getInstance()->postNotification(Msg_ShowButtomBar,nullptr);
		break;

	case GameInterfaceMgr::InterfaceTag_WujinMode:
		WuJinModeLayer::showLayout( _node, tag );
		NotificationCenter::getInstance()->postNotification(Msg_ShowButtomBar,nullptr);
		break;

	case GameInterfaceMgr::InterfaceTag_ShopBuy:
		ShopBuyLayer::showLayout( _node, tag, (int)_data );
		NotificationCenter::getInstance()->postNotification(Msg_ShowButtomBar,nullptr);
		break;

	case GameInterfaceMgr::InterfaceTag_QuickStone:
		QuickStoneLayer::showLayout(_node, tag);
		NotificationCenter::getInstance()->postNotification(Msg_ShowButtomBar, nullptr);
		break;

	default:
		break;
	}
}

void GameInterfaceMgr::setParentNode( Node* node )
{
	if ( node )
	{
		_node = node;
	}
}

Node* GameInterfaceMgr::getParentNode( void )
{
	if ( _node )
	{
		return _node ;
	}
	return nullptr ;
}

void GameInterfaceMgr::closeCurInterface( void )
{
	std::list<InterfaceTag>::iterator item = _interfaceVector.end();

	InterfaceTag tag = *(--item);
	_interfaceVector.erase(item);

	auto node = _node->getChildByTag( tag );
	if ( node )	
	{
		node->removeFromParentAndCleanup(true);
	}

	

	// 回到主界面时主界面动画
	if ( _interfaceVector.empty() )
	{
		NotificationCenter::getInstance()->postNotification(Msg_PlayMainSceneAction,nullptr);
		return;
	}
	else
	{
		item = _interfaceVector.end();
		// 显示下边菜单栏
		if ( InterfaceTag_RoleOption == *(--item))
		{
			NotificationCenter::getInstance()->postNotification(Msg_HideButtomBar,nullptr);
		}

		showNextInterface();
	}
}

void GameInterfaceMgr::closeAllInterface( void )
{
	for ( std::list<InterfaceTag>::iterator item = _interfaceVector.begin(); item != _interfaceVector.end(); item++ )
	{
		auto node = _node->getChildByTag( *item );
		if ( node )	
		{
			node->removeFromParentAndCleanup(true);
		}
	}
	if ( !_interfaceVector.empty() )
		NotificationCenter::getInstance()->postNotification(Msg_PlayMainSceneAction,nullptr);

	_interfaceVector.clear();
}


/*******************************************
* 主界面
*******************************************/
GameRoleScene::GameRoleScene()
	:_roleRootNode(nullptr)
	,_lastPageIndex( 0 )
	,_scrollTag( false )
	,_winSize(Size(0,0))
{

}
GameRoleScene::~GameRoleScene()
{


}

Scene* GameRoleScene::createScene()
{
	auto scene = Scene::create();

	auto layer = GameRoleScene::create();
	scene->addChild(layer);

	return scene;
}

bool GameRoleScene::init()
{
	if ( !Layer::init() )
	{
		return false;
	}

	return true;
}

void GameRoleScene::playPassSucceed( void )
{
	auto nodePassSuccee = (Node*)GameMainLayer::seekNodeByName( _roleRootNode, "Node_5" );
	nodePassSuccee->removeAllChildrenWithCleanup(true);
	nodePassSuccee->setPosition( Vec2(360, 640) );
	nodePassSuccee->setOpacity( 255 );

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
		CCCallFuncN::create(this, callfuncN_selector(GameRoleScene::callFuncGuang1)),CCFadeIn::create(fadeInT), CCScaleTo::create( fadeInT ,1.0f ))
		,nullptr ) );


	spriteZhen->runAction( Sequence::create( CCDelayTime::create(delayT*2)	
		,CCSpawn::create(
		CCCallFuncN::create(this, callfuncN_selector(GameRoleScene::callFuncGuang1)),CCFadeIn::create(fadeInT), CCScaleTo::create( fadeInT ,1.0f ))
		,nullptr ) );


	spriteChen->runAction( Sequence::create( CCDelayTime::create(delayT*3)	
		,CCSpawn::create(
		CCCallFuncN::create(this, callfuncN_selector(GameRoleScene::callFuncGuang1)),CCFadeIn::create(fadeInT), CCScaleTo::create( fadeInT ,1.0f ))
		,nullptr ) );


	spriteGong->runAction( Sequence::create( CCDelayTime::create(delayT*4)	
		,CCSpawn::create(
		CCCallFuncN::create(this, callfuncN_selector(GameRoleScene::callFuncGuang1)),CCFadeIn::create(fadeInT), CCScaleTo::create( fadeInT ,1.0f ))
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
		,CCCallFuncN::create(this, callfuncN_selector(GameRoleScene::callFuncNodeMove))
		,nullptr ) );

}


void GameRoleScene::callFuncGuang1( Node* node )
{
	auto spriteYuan = Sprite::create( "Effect/passSuccee/glow_0.png" );
	spriteYuan->setPosition( node->getContentSize()/2);
	node->addChild( spriteYuan );

	spriteYuan->setScale( 1.5f);
	spriteYuan->runAction( Sequence::create(
		CCSpawn::create(CCFadeOut::create(0.3f), CCScaleTo::create( 0.3f ,0.7f ))
		,CCScaleTo::create( 0.01f ,1.5f )
		,DelayTime::create(0.3f)
		,CCFadeIn::create(0.01f)
		,DelayTime::create(0.1f)
		,CCFadeOut::create(0.02f)
		,nullptr ) );
}

void GameRoleScene::callFuncNodeMove( Node* node )
{
	auto nodePassSuccee = (Node*)GameMainLayer::seekNodeByName( _roleRootNode, "Node_5" );
	nodePassSuccee->runAction( Spawn::create( MoveTo::create( 0.8f,  nodePassSuccee->getPosition() + Vec2(600, 0)), FadeOut::create( 0.7f ), nullptr));
}

void GameRoleScene::onEnter()
{
	Layer::onEnter();

	// 添加改变角色消息处理
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(GameRoleScene::playMainSceneAction), Msg_PlayMainSceneAction, nullptr);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(GameRoleScene::updataCardHead), Msg_ChangeCard, nullptr);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(GameRoleScene::updataChangeRole), Msg_ChangeRole, nullptr);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(GameRoleScene::showPaySuccee), Msg_BuySucceeEff, nullptr);

	initInterface();
	
	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("animation/Role1/shijiu.ExportJson", this, schedule_selector(GameRoleScene::LoadArmatureCallback));
	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("animation/Role2/shuanglong.ExportJson", this, schedule_selector(GameRoleScene::LoadArmatureCallback));
	ArmatureDataManager::getInstance()->addArmatureFileInfoAsync("animation/Role3/huolong.ExportJson", this, schedule_selector(GameRoleScene::LoadArmatureCallback));

	MainSceneTaskBar::showLayout( this );
	GameInterfaceMgr::getInstance()->setParentNode( this );

	NewAudio::getInstance()->playBgMusicForce(Music_Bg);

	_winSize = Director::getInstance()->getVisibleOrigin();


}

void GameRoleScene::onExit()
{
	Layer::onExit();

	// 移除观察者消息

	NotificationCenter::getInstance()->removeAllObservers(this);
}

void GameRoleScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(GameRoleScene::onKeyReleased,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


void GameRoleScene::initInterface( void )
{
	_roleRootNode = CSLoader::createNode( Ccs_MainLayer_FilePath );
	Size visibleSize = Director::getInstance()->getVisibleSize();

	if ( !_roleRootNode )
	{
		return;
	}

	addChild( _roleRootNode );
	do 
	{

		Button* ButtonWJMode = (Button*)GameMainLayer::seekNodeByName(_roleRootNode, "ButtonWJMode" );
		setEventForNode( ButtonWJMode, Tag_wjMode );

		Button* ButtonGKMode = (Button*)GameMainLayer::seekNodeByName(_roleRootNode, "ButtonGKMode" );
		setEventForNode( ButtonGKMode, Tag_gkMode );

		Button* ButtonTZMode = (Button*)GameMainLayer::seekNodeByName(_roleRootNode, "ButtonTZMode" );
		setEventForNode( ButtonTZMode, Tag_tzMode );
		
		Button* ButtonCard1 = (Button*)GameMainLayer::seekNodeByName(_roleRootNode, "ButtonCard1" );
		setEventForNode( ButtonCard1, Tag_Card );

		Button* ButtonCard2 = (Button*)GameMainLayer::seekNodeByName(_roleRootNode, "ButtonCard2" );
		setEventForNode( ButtonCard2, Tag_Card );

		Button* ButtonCard3 = (Button*)GameMainLayer::seekNodeByName(_roleRootNode, "ButtonCard3" );
		setEventForNode( ButtonCard3, Tag_Card );

		Button* ButtonSignIn = (Button*)GameMainLayer::seekNodeByName(_roleRootNode, "ButtonSignIn" );
		setEventForNode( ButtonSignIn, Tag_SignIn );

		Button* ButtonOptionRole = (Button*)GameMainLayer::seekNodeByName(_roleRootNode, "ButtonOptionRole" );
		setEventForNode( ButtonOptionRole, Tag_RoleOption );
		
		//updataChangeRole(nullptr);
		updataCardHead( nullptr );

		NotificationCenter::getInstance()->postNotification(Msg_PlayMainSceneAction,nullptr);

	} while (0);

}

void GameRoleScene::setEventForNode( Button* node, ButtonTag tag )
{
	do 
	{
		CC_BREAK_IF( node == nullptr );
		node->setTag( tag );
		node->addClickEventListener(CC_CALLBACK_1(GameRoleScene::callFuncButtonEvent, this));

	} while (0);

}

void GameRoleScene::callFuncButtonEvent( Ref* ref )
{
	Button* btn = (Button*)ref;
	int nTag = btn->getTag();


	switch ( nTag )
	{
	case Tag_wjMode:
		GameInterfaceMgr::getInstance()->addInterfaceToList( GameInterfaceMgr::InterfaceTag_WujinMode );
		//WuJinModeLayer::showLayout( this );
		break;
	case Tag_gkMode:
		GameSceneMgr::getInstance()->changeScene( Scene_Type_PlayILoading );
		break;	
	case Tag_tzMode:
		GameSceneMgr::getInstance()->changeScene( Scene_Type_PlayILoading );
		break;		

	case Tag_Card:
		GameInterfaceMgr::getInstance()->addInterfaceToList( GameInterfaceMgr::InterfaceTag_CardOption );
		break;		

	case Tag_SignIn:
		playPassSucceed();
		break;		

	case Tag_RoleOption:
		GameInterfaceMgr::getInstance()->addInterfaceToList( GameInterfaceMgr::InterfaceTag_RoleOption );
		break;	
			
	default:
		break;
	}
}


void GameRoleScene::onKeyReleased( EventKeyboard::KeyCode keyCode,Event * pEvent )
{
	// 防止重复show出widget
	if ( getChildByName( "exitWidget" ) ) return;
	
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE
		|| keyCode == EventKeyboard::KeyCode::KEY_ESCAPE )
	{
		auto widget = CommonWidget::create();
		widget->setContent( "是否确定退出游戏？" );
		widget->addBtn1ClickEventListener( CC_CALLBACK_0(GameRoleScene::callFuncExitGame, this) );
		addChild( widget, 15000, "exitWidget" );
	}
}

void GameRoleScene::callFuncExitGame( void )
{
	AudioEngine::end();
	Director::getInstance()->end();
}

void GameRoleScene::playUpgradeAnim( void )
{
	Vector<SpriteFrame*> frame_array;

	for (int i = 0 ; i <=9; ++i)
	{
		std::string pic = CCString::createWithFormat("%s%d.png","Upgrade_",i)->getCString();
		CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pic.c_str());

		frame_array.pushBack(frame);
	}

	auto p_animation = Animation::createWithSpriteFrames(frame_array,0.1f);
	p_animation->setLoops(1);

	auto animate =  Animate::create(p_animation) ;
	auto pageVGameRole = (PageView*)GameMainLayer::seekNodeByName(_roleRootNode, "PageViewRole" );

	auto upgradeAnimNode = Sprite::create("weapon.png");
	upgradeAnimNode->setScale( 2.8f );
	upgradeAnimNode->setPosition(Vec2(170, 380));
	addChild( upgradeAnimNode );

	upgradeAnimNode->runAction(Sequence::create( 
		animate, 
		CallFuncN::create(CC_CALLBACK_1(GameRoleScene::removeUpgradeAnim, this)),
		nullptr ));
}

void GameRoleScene::removeUpgradeAnim( Node* node )
{
	auto animSprite = (Sprite*)node;
	animSprite->removeFromParentAndCleanup(true);
}

void GameRoleScene::buySucceedAction( void )
{
	this;
	auto vsize = Director::getInstance()->getVisibleSize();
	// 特效节点
	auto node = Node::create();
	node->setPosition(Vec2(vsize.width/2,vsize.height/4*3));
	/*Director::getInstance()->getRunningScene()->*/addChild( node , 1001);

	// 购买成功图片动作
	auto spriteBuy = Sprite::create("particla/BuySucceed/gmcg.png");
	node->addChild(spriteBuy,-1);
	spriteBuy->runAction( Sequence::create (
		ScaleTo::create(0.15f, 1.5f ),
		ScaleTo::create(0.20f, 1.8f ),
		Spawn::create(  FadeOut::create(0.5f),ScaleTo::create(0.5f, 2.5f ), nullptr),
		CallFuncN::create( this, callfuncN_selector(GameRoleScene::callFuncbuySucceed)),
		nullptr));

	// 礼花绽放
	auto parcile2 = ParticleSystemQuad::create("particla/BuySucceed/test2.plist");
	node->addChild(parcile2,-1);
	auto parcile3 = ParticleSystemQuad::create("particla/BuySucceed/test3.plist");
	node->addChild(parcile3,-1);
	auto parcile4 = ParticleSystemQuad::create("particla/BuySucceed/test4.plist");
	node->addChild(parcile4,-1);
}

void GameRoleScene::callFuncbuySucceed( Node* node )
{
	//node->removeFromParentAndCleanup( true );
}


void GameRoleScene::updataCardHead( Ref* sender )
{
	for ( int i = 1; i<=3; i++ )
	{
		// 头像按钮
		std::string cardHeadPath = __String::createWithFormat("%s%d.png", Img_CardHead_FilePath, NewDataMgr::getInstance()->mRoleConfig.cardArr[i-1].cardID )->getCString();
		auto ButtonCard = (Button*)GameMainLayer::seekNodeByName(_roleRootNode, __String::createWithFormat("ButtonCard%d", i)->getCString() );
		ButtonCard->loadTextures( cardHeadPath, cardHeadPath, cardHeadPath );


		// 等级框
		auto StarLvFream = (Sprite*)GameMainLayer::seekNodeByName(_roleRootNode, __String::createWithFormat("StarLvFream_%d", i)->getCString() );
		StarLvFream->setTexture( __String::createWithFormat( "ccsRes/cardOptionLayer/starLv_%d.png", NewDataMgr::getInstance()->mRoleConfig.cardArr[i-1].cardStarLv)->getCString() );

		for( int j = 1; j<=5; j++ )
		{
			// 星星等级
			auto Lv5Star = (Node*)GameMainLayer::seekNodeByName(_roleRootNode, __String::createWithFormat("LvStar%d_%d", i, j)->getCString() );

			if ( NewDataMgr::getInstance()->mRoleConfig.cardArr[i-1].cardStarLv < j )
				Lv5Star->setVisible( false );
			else
				Lv5Star->setVisible( true );

		}
	}

	// 更改按钮头像
	auto BtnRoleHead = (Sprite*)GameMainLayer::seekNodeByName(_roleRootNode,"BtnRoleHead" );
	BtnRoleHead->setTexture(__String::createWithFormat("%s%d.png", Img_BtnRoleHead_FilePath, NewDataMgr::getInstance()->mRoleConfig.roleIndex)->getCString() );

}

void GameRoleScene::updataChangeRole( Ref* sender )
{

// 	auto NodeRole = (Node*)GameMainLayer::seekNodeByName(_roleRootNode, "NodeRole");
// 
// 	auto spriteRole = (Sprite*)GameMainLayer::seekNodeByName(_roleRootNode, "spriteRole");
// 	if ( !spriteRole )
// 	{
// 		spriteRole = Sprite::create();
// 		spriteRole->setFlipX( true );
// 		NodeRole->addChild( spriteRole, 0, "spriteRole" );
// 	}
// 	spriteRole->setTexture( __String::createWithFormat("roleImg/roleImg%d.png", NewRoleDataMgr::getInstance()->mRoleConfig.roleIndex )->getCString() );
	
	LoadArmatureCallback();

	updataCardHead( nullptr );
}

void GameRoleScene::showPaySuccee( Ref* sender )
{
	buySucceedAction();
}

void GameRoleScene::LoadArmatureCallback( float dt )
{
	auto NodeRole = (Node*)GameMainLayer::seekNodeByName(_roleRootNode, "NodeRole");
	if ( dt >= 1 && NodeRole)
	{
		Armature * armature = (Armature *)NodeRole->getParent()->getChildByName("boss");

		if ( !armature )
		{			

			if ( 1 == NewDataMgr::getInstance()->mRoleConfig.roleIndex )
			{
				armature = Armature::create("shijiu");
				armature->setScaleX( -1 );
				armature->setPosition( NodeRole->getPosition()+ Vec2(-70, -80)  );

			}
			else if ( 2 == NewDataMgr::getInstance()->mRoleConfig.roleIndex )
			{
				armature = Armature::create("shuanglong");
				armature->setScaleX( -1 );
				armature->setPosition( NodeRole->getPosition()+ Vec2(0, -50) );
			}
			else if ( 3 == NewDataMgr::getInstance()->mRoleConfig.roleIndex )
			{
				armature = Armature::create("huolong");
				armature->setScaleY(1.6f);
				armature->setScaleX(-1.6f);
				armature->setPosition( NodeRole->getPosition()+ Vec2(-220, 250) );
			}
			armature->setName( "boss" );
			armature->setAnchorPoint(Vec2(0.5,0.5));
			armature->getAnimation()->playWithIndex(0);
			NodeRole->getParent()->addChild(armature, -1 );
		}
	}else if ( 0 == dt && NodeRole )
	{
		if ( !NodeRole->getParent()->getChildByName("boss") )
			return;

		Armature * armature = (Armature *)NodeRole->getParent()->getChildByName("boss");
		armature->removeFromParentAndCleanup( true );
		

		if ( 1 == NewDataMgr::getInstance()->mRoleConfig.roleIndex )
		{
			armature = Armature::create("shijiu");
			armature->setScaleX( -1 );
			armature->setPosition( NodeRole->getPosition() + Vec2(-70, -80) );

		}
		else if ( 2 == NewDataMgr::getInstance()->mRoleConfig.roleIndex )
		{
			armature = Armature::create("shuanglong");
			armature->setScaleX( -1 );
			armature->setPosition( NodeRole->getPosition()+ Vec2(0, -50)  );
		}
		else if ( 3 == NewDataMgr::getInstance()->mRoleConfig.roleIndex )
		{
			armature = Armature::create("huolong");
			armature->setScaleY(1.6f);
			armature->setScaleX(-1.6f);
			armature->setPosition( NodeRole->getPosition() + Vec2(-220, 250) );
		}

		armature->setName( "boss" );
		armature->setAnchorPoint(Vec2(0.5,0.5));
		armature->getAnimation()->playWithIndex(0);
		NodeRole->getParent()->addChild(armature, -1);

	}
}

void GameRoleScene::playMainSceneAction( Ref* sender )
{
	Button* ButtonWJMode = (Button*)GameMainLayer::seekNodeByName(_roleRootNode, "ButtonWJMode" );
	ButtonWJMode->setScaleY(0.2f);

	Button* ButtonGKMode = (Button*)GameMainLayer::seekNodeByName(_roleRootNode, "ButtonGKMode" );
	ButtonGKMode->setScaleY(0.2f);

	Button* ButtonTZMode = (Button*)GameMainLayer::seekNodeByName(_roleRootNode, "ButtonTZMode" );
	ButtonTZMode->setScaleY(0.2f);

	
	ButtonWJMode->runAction( Sequence::create(
		CCSpawn::create(ScaleTo::create(0.1, 1.0f, 1.2f),CCMoveBy::create(0.1, Vec2(0, 40)),nullptr)
		,CCSpawn::create(ScaleTo::create(0.08, 1.0f, 1.0f),CCMoveBy::create(0.08, Vec2(0, -40)),nullptr)
		,nullptr) );
	ButtonGKMode->runAction( Sequence::create(
		CCSpawn::create(ScaleTo::create(0.1, 1.0f, 1.2f),CCMoveBy::create(0.1, Vec2(0, 40)),nullptr)
		,CCSpawn::create(ScaleTo::create(0.08, 1.0f, 1.0f),CCMoveBy::create(0.08, Vec2(0, -40)),nullptr)
		,nullptr) );
	ButtonTZMode->runAction( Sequence::create(
		CCSpawn::create(ScaleTo::create(0.1, 1.0f, 1.2f),CCMoveBy::create(0.1, Vec2(0, 40)),nullptr)
		,CCSpawn::create(ScaleTo::create(0.08, 1.0f, 1.0f),CCMoveBy::create(0.08, Vec2(0, -40)),nullptr)
		,nullptr) );

	Button* ButtonCard1 = (Button*)GameMainLayer::seekNodeByName(_roleRootNode, "ButtonCard1" );
	ButtonCard1->setOpacity( 0 );
	ButtonCard1->runAction( Sequence::create(
		CCMoveBy::create(0.01f, Vec2(-500,0))
		,Spawn::create(CCFadeIn::create(0.01f),CCEaseSineOut::create(CCMoveBy::create(0.2f, Vec2(500, 0))),nullptr) 
		,nullptr));

	Button* ButtonCard2 = (Button*)GameMainLayer::seekNodeByName(_roleRootNode, "ButtonCard2" );
	ButtonCard2->setOpacity( 0 );
	ButtonCard2->runAction( Sequence::create(
		CCMoveBy::create(0.01f, Vec2(-500,0))
		,Spawn::create(CCFadeIn::create(0.01f),CCEaseSineOut::create(CCMoveBy::create(0.25f,Vec2(500, 0))),nullptr) 
		,nullptr));

	Button* ButtonCard3 = (Button*)GameMainLayer::seekNodeByName(_roleRootNode, "ButtonCard3" );
	ButtonCard3->setOpacity( 0 );
	ButtonCard3->runAction( Sequence::create(
		CCMoveBy::create(0.01f, Vec2(-500,0))
		,Spawn::create(CCFadeIn::create(0.01f),CCEaseSineOut::create(CCMoveBy::create(0.3f,  Vec2(500, 0))),nullptr) 
		,nullptr));

	Button* ButtonSignIn = (Button*)GameMainLayer::seekNodeByName(_roleRootNode, "ButtonSignIn" );
	

	Button* ButtonOptionRole = (Button*)GameMainLayer::seekNodeByName(_roleRootNode, "ButtonOptionRole" );
	ButtonOptionRole->setOpacity( 0 );
	ButtonOptionRole->runAction( Sequence::create(
		CCMoveBy::create(0.01f, Vec2(-500,0))
		,Spawn::create(CCFadeIn::create(0.01f),CCEaseSineOut::create(CCMoveBy::create(0.4f, Vec2(500, 0))),nullptr) 
		,nullptr));

}

/*****************************************
* 角色选择
******************************************/
void RoleOptionlayer::showLayout( Node* node, int tag )
{
	if ( node && !node->getChildByName("RoleOptionlayer") )
	{
		auto layout = RoleOptionlayer::create();
		layout->setTag( tag );
		node->addChild( layout, LocalZOrde_500, "RoleOptionlayer" );

		NotificationCenter::getInstance()->postNotification(Msg_HideButtomBar,nullptr);

	}
}

void RoleOptionlayer::onEnter()
{
	Layer::onEnter();

	initInterface();

}

void RoleOptionlayer::initInterface()
{
	_roleOptionlayout = CSLoader::createNode( Ccs_roleOptionLayer_FilePath );

	if ( !_roleOptionlayout )
	{
		return;
	}

	addChild( _roleOptionlayout );

	do 
	{
		// 返回
		auto ButtonBack = (Button*)GameMainLayer::seekNodeByName(_roleOptionlayout, "ButtonBack" );
		setEventForNode( ButtonBack, Tag_Back );

		// 升级关闭
		auto Buttonclose = (Button*)GameMainLayer::seekNodeByName(_roleOptionlayout, "Buttonclose" );
		setEventForNode( Buttonclose, Tag_UpgradeClose );

		// 解锁
		auto ButtonUnlock = (Button*)GameMainLayer::seekNodeByName(_roleOptionlayout, "ButtonUnlock" );
		setEventForNode( ButtonUnlock, Tag_Unlock );

		// 选择角色1
		auto ButtonOptionRole1 = (Button*)GameMainLayer::seekNodeByName(_roleOptionlayout, "ButtonOptionRole1" );
		setEventForNode( ButtonOptionRole1, Tag_Role1 );

		// 选择角色2
		auto ButtonOptionRole2 = (Button*)GameMainLayer::seekNodeByName(_roleOptionlayout, "ButtonOptionRole2" );
		setEventForNode( ButtonOptionRole2, Tag_Role2 );

		// 选择角色3
		auto ButtonOptionRole3 = (Button*)GameMainLayer::seekNodeByName(_roleOptionlayout, "ButtonOptionRole3" );
		setEventForNode( ButtonOptionRole3, Tag_Role3 );

		// 角色升级
		auto ButtonUpgrade = (Button*)GameMainLayer::seekNodeByName(_roleOptionlayout, "ButtonUpgrade" );
		setEventForNode( ButtonUpgrade, Tag_Upgrade );

		// 石头1
		auto ButtonSt1 = (Button*)GameMainLayer::seekNodeByName(_roleOptionlayout, "ButtonSt1" );
		setEventForNode( ButtonSt1, Tag_Pebble1 );

		// 石头2
		auto ButtonSt2 = (Button*)GameMainLayer::seekNodeByName(_roleOptionlayout, "ButtonSt2" );
		setEventForNode( ButtonSt2, Tag_Pebble2 );

		// 石头3
		auto ButtonSt3 = (Button*)GameMainLayer::seekNodeByName(_roleOptionlayout, "ButtonSt3" );
		setEventForNode( ButtonSt3, Tag_Pebble3 );
	
		// 吸收水晶
		auto ButtonRoleCompound = (Button*)GameMainLayer::seekNodeByName(_roleOptionlayout, "ButtonRoleCompound" );
		setEventForNode( ButtonRoleCompound, Tag_RoleStartCompound );

		// 10连抽
		auto CheckBoxUpgrade = (CheckBox*)GameMainLayer::seekNodeByName(_roleOptionlayout, "CheckBoxUpgrade" );
		CC_BREAK_IF( CheckBoxUpgrade == nullptr );
		CheckBoxUpgrade->addEventListener(CC_CALLBACK_2(RoleOptionlayer::selected10ContinuonsEvent, this));

		_roleUpgradeInterface = (Button*)GameMainLayer::seekNodeByName(_roleOptionlayout, "PanelRoleSj" );

		_curOptionRoleIndex = NewDataMgr::getInstance()->getRoleIndex();
		updataRoleLock();
		changeOptionCard();

	} while (0);
}

void RoleOptionlayer::setEventForNode( Button* node, ButtonTag tag )
{
	do 
	{
		CC_BREAK_IF( node == nullptr );

		node->setTag( tag );
		node->addClickEventListener(CC_CALLBACK_1(RoleOptionlayer::callFuncButtonEvent, this));
			

	} while (0);

}

void RoleOptionlayer::callFuncButtonEvent( Ref* ref )
{
	Button* btn = (Button*)ref;
	int nTag = btn->getTag();

	switch ( nTag )
	{
	case Tag_Back:
		{
			NewDataMgr::getInstance()->setRoleIndex( _curOptionRoleIndex );
			NotificationCenter::getInstance()->postNotification(Msg_ChangeRole,nullptr);
			//removeFromParentAndCleanup(true);
			GameInterfaceMgr::getInstance()->closeCurInterface();
			NotificationCenter::getInstance()->postNotification(Msg_ShowButtomBar,nullptr);

		}
		break;

	case Tag_Unlock:
		{
			NewDataMgr::getInstance()->setRoleLockWithIndex( _curOptionRoleIndex, true );
			NotificationCenter::getInstance()->postNotification(Msg_BuySucceeEff,nullptr);

			updataRoleLock();
			changeOptionCard();
		}
		break;
	
	case Tag_Role1:
		_curOptionRoleIndex = 1;
		updataRoleLock();
		changeOptionCard();
		break;

	case Tag_Role2:
		_curOptionRoleIndex = 2;
		updataRoleLock();
		changeOptionCard();
		break;

	case Tag_Role3:
		_curOptionRoleIndex = 3;
		updataRoleLock();
		changeOptionCard();
		break;

	case Tag_UpgradeClose:
		{
			auto LoadingBarExp = (LoadingBar*)GameMainLayer::seekNodeByName(_roleOptionlayout, "LoadingBarExp" );
			LoadingBarExp->stopAllActions();
			NewDataMgr::getInstance()->addRoleCurExpWithId( _curOptionRoleIndex,_curHaveExp - _consumeExp);
			_curHaveExp = 0;
			_consumeExp = 0;

			changeOptionCard();
			hideRoleUpgrade();
		}
		break;

	case Tag_Upgrade:
		changeOptionRock(1);
		updataRoleUpgrade();
		showRoleUpgrade();
		break;

	case Tag_Pebble1:
		changeOptionRock(1);
		break;

	case Tag_Pebble2:
		changeOptionRock(2);
		break;

	case Tag_Pebble3:
		changeOptionRock(3);
		break;

	case Tag_RoleStartCompound:
		btn->setEnabled( false );
		btn->setBright( false );
		startUpgradePre();
		break;

	default:
		break;
	}
}

void RoleOptionlayer::selected10ContinuonsEvent( Ref* pSender,CheckBox::EventType type )
{
	auto AtLabelGoldPebble = (TextAtlas*)GameMainLayer::seekNodeByName(_roleOptionlayout, StringUtils::format("AtLabelGoldPebble%d",_curOptionRockIndex) );
	auto pebbledata = NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( getOptionPebbleKey() );

	switch (type)
	{
	case CheckBox::EventType::UNSELECTED:
		{
			if ( 0 >= pebbledata->number )
			{
				AtLabelGoldPebble->setString( StringUtils::format("%d", pebbledata->Gold));
			}
		}
		break;

	case CheckBox::EventType::SELECTED:
		if ( 0 >= pebbledata->number )
		{
			AtLabelGoldPebble->setString( StringUtils::format("%d", pebbledata->Gold * 10));
		}
		break;

	default:
		break;
	}
}

void RoleOptionlayer::changeOptionCard( void )
{
	// 当前选择的角色index
	int curOptionIndex =_curOptionRoleIndex;
	NewDataMgr::getInstance()->setRoleIndex(curOptionIndex);

	// 角色选择框及动作
	auto ButtonOptionRole = (Button*)GameMainLayer::seekNodeByName(_roleOptionlayout, __String::createWithFormat("ButtonOptionRole%d", curOptionIndex)->getCString() );
	auto OptionFrame = ( Button* )GameMainLayer::seekNodeByName(_roleOptionlayout, "OptionFrame" );
	OptionFrame->setPosition( ButtonOptionRole->getPosition() + Vec2(0, 3) );
	OptionFrame->stopAllActions();
	OptionFrame->runAction( RepeatForever::create(Sequence::create(
		CCFadeOut::create(0.01)
		,CCScaleTo::create(0.01, 0.9)
		,CCSpawn::create( CCFadeIn::create(0.5), CCScaleTo::create(0.5f, 1.0f), nullptr)
		,DelayTime::create(0.5f)
		,nullptr)));

	// 更换角色
	auto NodeRole = (Node*)GameMainLayer::seekNodeByName(_roleOptionlayout, "NodeRole");
	const char* RoleSpritePath = __String::createWithFormat("roleImg/roleImg%d.png", curOptionIndex )->getCString();
	auto spriteRole = (Sprite*)GameMainLayer::seekNodeByName(_roleOptionlayout, "RoleSprite" );
	if ( !spriteRole )
	{
		spriteRole = Sprite::create();
		spriteRole->setName( "RoleSprite" );
		spriteRole->setFlipX( true );
		NodeRole->addChild( spriteRole, 0 );

		
	}
	spriteRole->setTexture( RoleSpritePath );

	if ( 1 == curOptionIndex )
	{
		NodeRole->setPosition( Vec2(160, 645) );
	}
	else if ( 2 ==  curOptionIndex )
	{
		NodeRole->setPosition( Vec2(221, 580) );
	}
	else if ( 3 ==  curOptionIndex )
	{
		NodeRole->setPosition( Vec2(30, 810) );
	}
	auto offsetVec = Vec2(-150, 100);
	NodeRole->setPosition( NodeRole->getPosition() + offsetVec );
 	NodeRole->setOpacity( 0 );
 	NodeRole->runAction( CCSpawn::create(CCFadeIn::create(0.3f),CCMoveTo::create(0.3f, NodeRole->getPosition() - offsetVec ),nullptr) );

	// 自动解锁
	if ( 3 != curOptionIndex && NewDataMgr::getInstance()->getRoleGradeWithId( curOptionIndex ) >= NewDataMgr::getInstance()->getRoleAutoLockLvWithId(curOptionIndex) )
	{
		NewDataMgr::getInstance()->setRoleLockWithIndex( curOptionIndex + 1, true );
	}

	// 更换角色演示图
	auto RoleDemo = (Sprite*)GameMainLayer::seekNodeByName(_roleOptionlayout, "RoleDemo" );
	RoleDemo->setTexture( __String::createWithFormat("roleImg/roleDemo%d.png", curOptionIndex )->getCString() );
	// 攻击力
	auto TextAtkNum = (Text*)GameMainLayer::seekNodeByName(_roleOptionlayout, "TextAtkNum" );
	TextAtkNum->setString( __String::createWithFormat("%d", (int)NewDataMgr::getInstance()->getRoleAtkWithId( curOptionIndex ) )->getCString() );
	// 防御力
	auto TextFangyuNum = (Text*)GameMainLayer::seekNodeByName(_roleOptionlayout, "TextFangyuNum" );
	TextFangyuNum->setString( __String::createWithFormat("%d", (int)NewDataMgr::getInstance()->getRoleDefenseWithId( curOptionIndex ))->getCString() );
	// 幸运值
	auto TextLunkNum = (Text*)GameMainLayer::seekNodeByName(_roleOptionlayout, "TextLunkNum" );
	TextLunkNum->setString( __String::createWithFormat("%d", (int)NewDataMgr::getInstance()->getRoleLuckWithId( curOptionIndex ) )->getCString() );
	// 等级
	auto AtlasLabelRoleGrade = (TextAtlas*)GameMainLayer::seekNodeByName(_roleOptionlayout, "AtlasLabelRoleGrade" );
	AtlasLabelRoleGrade->setString( __String::createWithFormat("%d", (int)NewDataMgr::getInstance()->getRoleGradeWithId( curOptionIndex ) )->getCString() );
	// 进度条比
	float curExpPercent = NewDataMgr::getInstance()->getRoleCurExpPercentWithId( _curOptionRoleIndex );
	auto LoadingBarRoleExp = (LoadingBar*)GameMainLayer::seekNodeByName(_roleOptionlayout, "LoadingBarRoleExp" );
	LoadingBarRoleExp->setPercent( curExpPercent );
	// 名称
	std::string roleNameStr = NewDataMgr::getInstance()->getXmlStringWithKey( __String::createWithFormat( "rolename%d",  curOptionIndex )->getCString() );
	auto RoleName = (Text*)GameMainLayer::seekNodeByName(_roleOptionlayout, "RoleName" );
	RoleName->setString( roleNameStr );

	// 说明
	auto RoleExplain = (Text*)GameMainLayer::seekNodeByName(_roleOptionlayout, "RoleExplain" );
	RoleExplain->setString( NewDataMgr::getInstance()->getXmlStringWithKey( 
		__String::createWithFormat( "rolename%d_des",  curOptionIndex )->getCString() ) );

	// 解锁价格
	auto AtlasLabelGemNum = (TextAtlas*)GameMainLayer::seekNodeByName(_roleOptionlayout,"AtlasLabelGemNum");
	AtlasLabelGemNum->setString( __String::createWithFormat( "%d", NewDataMgr::getInstance()->getRolePriceWithId( curOptionIndex ) )->getCString() );

	// 自动解锁提示
	auto RoleUnlockTips = (Text*)GameMainLayer::seekNodeByName(_roleOptionlayout, "RoleUnlockTips" );
	RoleUnlockTips->setString( __String::createWithFormat("%s%d%s", "当前角色", NewDataMgr::getInstance()->getRoleAutoLockLvWithId( curOptionIndex ), "级自动解锁下一角色")->getCString() );
	RoleUnlockTips->setVisible( false );

	// 显示结果按钮
	auto showUnlock = (Sprite*)GameMainLayer::seekNodeByName(_roleOptionlayout, "showUnlock" );
	auto ButtonUnlock = (Button*)GameMainLayer::seekNodeByName(_roleOptionlayout, "ButtonUnlock" );
	auto PanelUpgrade = (Node*)GameMainLayer::seekNodeByName(_roleOptionlayout, "PanelUpgrade" );
	// 已解锁 
	if ( NewDataMgr::getInstance()->getRoleUnlockState( curOptionIndex ) )
	{
		ButtonUnlock->setTouchEnabled( false );
		showUnlock->setVisible( false );
		PanelUpgrade->setVisible( true );

		if ( 3 != curOptionIndex && !NewDataMgr::getInstance()->getRoleUnlockState( curOptionIndex+1 ) )
			RoleUnlockTips->setVisible( true );
	}
	// 未解锁 
	else
	{
		ButtonUnlock->setTouchEnabled( true );
		showUnlock->setVisible( true );
		PanelUpgrade->setVisible( false );
	}
}

void RoleOptionlayer::updataRoleLock( void )
{
	auto Role2Lock = (Sprite*)GameMainLayer::seekNodeByName(_roleOptionlayout, "Role2Lock" );
	auto Role3Lock = (Sprite*)GameMainLayer::seekNodeByName(_roleOptionlayout, "Role3Lock" );
	Role2Lock->setVisible( true );
	Role3Lock->setVisible( true );

	if ( NewDataMgr::getInstance()->getRoleInfoWithIndex( 2 )->isLock )
		Role2Lock->setVisible( false );

	if ( NewDataMgr::getInstance()->getRoleInfoWithIndex( 3 )->isLock )
		Role3Lock->setVisible( false );


}

void RoleOptionlayer::changeOptionRock( int index )
{
	_curOptionRockIndex = index;

	auto pebbledata = NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( getOptionPebbleKey() );
	auto btn = (Button*)GameMainLayer::seekNodeByName(_roleOptionlayout, __String::createWithFormat("ButtonSt%d", _curOptionRockIndex)->getCString() );

	auto PropOptionFram = (Sprite*)GameMainLayer::seekNodeByName(_roleOptionlayout, "PropOptionFram" );
	PropOptionFram->setPosition( btn->getPosition() + Vec2(-2,5) );

	auto TextUpgradeTips = (Text*)GameMainLayer::seekNodeByName(_roleOptionlayout, "TextUpgradeTips" );
	TextUpgradeTips->setString( NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( getOptionPebbleKey() )->GoodsDes );

	auto TextSJTips2 = (Text*)GameMainLayer::seekNodeByName(_roleOptionlayout, "TextSJTips2" );
	TextSJTips2->setString( __String::createWithFormat("%s%d%s", 
		"水晶", 
		(pebbledata->number == 0 )?10 : pebbledata->number, 
		"连吃")->getCString() );

// 
// 	TextUpgradeTips->setString(__String::createWithFormat(
// 		"能量水晶可以为你的女神提供升级经验+%d", 
// 		NewDataMgr::getInstance()->getPebbleExpWithKey( getOptionPebbleKey()) )->getCString() );
}

void RoleOptionlayer::showRoleUpgrade( void )
{
	auto s = Director::getInstance()->getVisibleSize();
	_roleUpgradeInterface->setPosition( Vec2( s.width/2, _roleUpgradeInterface->getPosition().y) );

	std::string filePath = __String::createWithFormat("%s%d.png", Img_RoleHead_FilePath, _curOptionRoleIndex)->getCString();
	auto ButtonRole = (Button*)GameMainLayer::seekNodeByName(_roleOptionlayout, "ButtonRole" );
	ButtonRole->loadTextures(filePath, filePath, filePath);

	// 按钮可用
	auto ButtonRoleCompound = (Button*)GameMainLayer::seekNodeByName(_roleOptionlayout, "ButtonRoleCompound" );

	if ( NewDataMgr::getInstance()->isFullGradeRoleWithId(_curOptionRoleIndex) )
	{
		ButtonRoleCompound->setEnabled( false );
		ButtonRoleCompound->setBright( false );

	}
	else
	{
		ButtonRoleCompound->setEnabled( true );
		ButtonRoleCompound->setBright( true );
	}
}

void RoleOptionlayer::hideRoleUpgrade( void )
{
	auto s = Director::getInstance()->getVisibleSize();
	_roleUpgradeInterface->setPosition( Vec2( -s.width/2, _roleUpgradeInterface->getPosition().y) );
}

void RoleOptionlayer::updataRoleUpgrade( void )
{
	auto pebbleData = NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( getOptionPebbleKey() );
	// 等级比

	auto AtlasLabelGradePercent = (TextAtlas*)GameMainLayer::seekNodeByName(_roleOptionlayout, "AtlasLabelGradePercent" );
	AtlasLabelGradePercent->setString( __String::createWithFormat("%d/%d", 
								NewDataMgr::getInstance()->getRoleGradeWithId(_curOptionRoleIndex ), 
								RoleGrade_Max)->getCString());

	
	// 经验比
	float curExpPercent = NewDataMgr::getInstance()->getRoleCurExpPercentWithId( _curOptionRoleIndex );
	// 经验比
	auto AtlasLabelExpData = (TextAtlas*)GameMainLayer::seekNodeByName(_roleOptionlayout, "AtlasLabelExpData" );
	//AtlasLabelExpData->setString( __String::createWithFormat("%.2f/", curExpPercent )->getCString());
	AtlasLabelExpData->setString( StringUtils::format("%d/%d", 
		(int)NewDataMgr::getInstance()->getRoleCurExpWithId(_curOptionRoleIndex), 
		(int)NewDataMgr::getInstance()->getRoleExpWithId(_curOptionRoleIndex, NewDataMgr::getInstance()->getRoleGradeWithId(_curOptionRoleIndex)) ));

	// 进度条比
	auto LoadingBarExp = (LoadingBar*)GameMainLayer::seekNodeByName(_roleOptionlayout, "LoadingBarExp" );
	LoadingBarExp->setPercent( curExpPercent );

	// 水晶金币显示
	auto AtLabelGoldPebble1 = (TextAtlas*)GameMainLayer::seekNodeByName(_roleOptionlayout, "AtLabelGoldPebble1" );
	auto AtLabelGoldPebble2 = (TextAtlas*)GameMainLayer::seekNodeByName(_roleOptionlayout, "AtLabelGoldPebble2" );
	auto AtLabelGoldPebble3 = (TextAtlas*)GameMainLayer::seekNodeByName(_roleOptionlayout, "AtLabelGoldPebble3" );
	AtLabelGoldPebble1->getParent()->setVisible( true );
	AtLabelGoldPebble2->getParent()->setVisible( true );
	AtLabelGoldPebble3->getParent()->setVisible( true );

	AtLabelGoldPebble1->setString( __String::createWithFormat("%d", 
								NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( BuyListKey::BuyKey_Pebble1 )->Gold)->getCString() );
	AtLabelGoldPebble2->setString( __String::createWithFormat("%d", 
								NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( BuyListKey::BuyKey_Pebble2 )->Gold)->getCString() );
	AtLabelGoldPebble3->setString( __String::createWithFormat("%d", 
								NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( BuyListKey::BuyKey_Pebble3 )->Gold)->getCString() );

	if ( NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( BuyListKey::BuyKey_Pebble1 )->number )	
		AtLabelGoldPebble1->getParent()->setVisible( false );

	if ( NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( BuyListKey::BuyKey_Pebble2 )->number )	
		AtLabelGoldPebble2->getParent()->setVisible( false );

	if ( NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( BuyListKey::BuyKey_Pebble3 )->number )	
		AtLabelGoldPebble3->getParent()->setVisible( false );

	// 水晶数量显示 
	auto AtlasLabelPebbleNum_1 = (TextAtlas*)GameMainLayer::seekNodeByName(_roleOptionlayout, "AtlasLabelPebbleNum_1" );
	auto AtlasLabelPebbleNum_2 = (TextAtlas*)GameMainLayer::seekNodeByName(_roleOptionlayout, "AtlasLabelPebbleNum_2" );
	auto AtlasLabelPebbleNum_3 = (TextAtlas*)GameMainLayer::seekNodeByName(_roleOptionlayout, "AtlasLabelPebbleNum_3" );

	AtlasLabelPebbleNum_1->setString( __String::createWithFormat("%d", 
		NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( BuyListKey::BuyKey_Pebble1 )->number)->getCString() );
	AtlasLabelPebbleNum_2->setString( __String::createWithFormat("%d", 
		NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( BuyListKey::BuyKey_Pebble2 )->number)->getCString() );
	AtlasLabelPebbleNum_3->setString( __String::createWithFormat("%d", 
		NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( BuyListKey::BuyKey_Pebble3 )->number)->getCString() );

	auto TextSJTips2 = (Text*)GameMainLayer::seekNodeByName(_roleOptionlayout, "TextSJTips2" );
	TextSJTips2->setString( __String::createWithFormat("%s%d%s", "水晶", pebbleData->number, "连吃")->getCString() );

	if ( 0 >= pebbleData->number )	
	{
		// 数量为0时 10连吃
		TextSJTips2->setString( __String::createWithFormat("%s%d%s", "水晶", 10, "连吃")->getCString() );

		auto CheckBoxUpgrade = (CheckBox*)GameMainLayer::seekNodeByName(_roleOptionlayout, "CheckBoxUpgrade" );
		CheckBoxUpgrade->setSelectedState( true );

		auto AtLabelGoldPebble = (TextAtlas*)GameMainLayer::seekNodeByName(_roleOptionlayout, 
			__String::createWithFormat("AtLabelGoldPebble%d", _curOptionRockIndex )->getCString() );
		AtLabelGoldPebble->setString(__String::createWithFormat("%d", pebbleData->Gold*10 )->getCString() );
	}
}

void RoleOptionlayer::callFuncUpgrade( Node* node )
{

	// 每升1级效果
	if ( NewDataMgr::getInstance()->mRoleUpgradeIng )
	{
		NewDataMgr::getInstance()->mRoleUpgradeIng = false;
		
		auto PanelRoleUpgrade = GameMainLayer::seekNodeByName(_roleOptionlayout, "PanelRoleUpgrade" );
		PanelRoleUpgrade->setOpacity( 0 );
		PanelRoleUpgrade->runAction( Sequence::create(
			CCSpawn::create(CCFadeIn::create(0.08 ),CCScaleTo::create(0.08, 1.1f), nullptr),
			CCSpawn::create(CCFadeOut::create(0.3), CCScaleTo::create(0.3, 1.0f), nullptr),
			CCCallFunc::create(this, callfunc_selector(RoleOptionlayer::callfunPlayUpgradeAction)),
			nullptr));
		return;
	}

 	int curGrade = NewDataMgr::getInstance()->getRoleGradeWithId( _curOptionRoleIndex );

	if ( RoleGrade_Max >= curGrade && !NewDataMgr::getInstance()->isFullGradeRoleWithId(_curOptionRoleIndex) )
	{
		// 升级速度
		int singleExp = _curHaveExp * 0.01;

		_consumeExp += singleExp;
		NewDataMgr::getInstance()->addRoleCurExpWithId( _curOptionRoleIndex, singleExp );

		if ( _consumeExp >= _curHaveExp )
		{
			NewDataMgr::getInstance()->setRoleGoadeWithIndex(_curOptionRoleIndex);
			NewDataMgr::getInstance()->setRoleExpWithIndex(_curOptionRoleIndex);

			// 升级结束
			node->stopAllActions();
			auto ButtonRoleCompound = (Button*)GameMainLayer::seekNodeByName(_roleOptionlayout, "ButtonRoleCompound" );
			ButtonRoleCompound->setEnabled( true );
			ButtonRoleCompound->setBright( true );
			return;
		}
	}
	
	// 满级
	if ( NewDataMgr::getInstance()->isFullGradeRoleWithId(_curOptionRoleIndex) )
	{
		node->stopAllActions();

		auto ButtonRoleCompound = (Button*)GameMainLayer::seekNodeByName(_roleOptionlayout, "ButtonRoleCompound" );
		ButtonRoleCompound->setEnabled( false );
		ButtonRoleCompound->setBright( false );
	}

	curGrade = NewDataMgr::getInstance()->getRoleGradeWithId( _curOptionRoleIndex );
	float curExpPercent = NewDataMgr::getInstance()->getRoleCurExpPercentWithId( _curOptionRoleIndex );
	// 经验比
	auto AtlasLabelExpData = (TextAtlas*)GameMainLayer::seekNodeByName(_roleOptionlayout, "AtlasLabelExpData" );
	//AtlasLabelExpData->setString( __String::createWithFormat("%.2f/", curExpPercent )->getCString());
	AtlasLabelExpData->setString( StringUtils::format("%d/%d", 
		(int)NewDataMgr::getInstance()->getRoleCurExpWithId(_curOptionRoleIndex), 
		(int)NewDataMgr::getInstance()->getRoleExpWithId(_curOptionRoleIndex, (RoleGrade_Max <= curGrade)?RoleGrade_Max:curGrade) ));
	// 进度条比
	auto LoadingBarExp = (LoadingBar*)GameMainLayer::seekNodeByName(_roleOptionlayout, "LoadingBarExp" );
	LoadingBarExp->setPercent( curExpPercent );

	// 等级比
	auto AtlasLabelGradePercent = (TextAtlas*)GameMainLayer::seekNodeByName(_roleOptionlayout, "AtlasLabelGradePercent" );
	AtlasLabelGradePercent->setString( __String::createWithFormat("%d/%d", 
		NewDataMgr::getInstance()->getRoleGradeWithId(_curOptionRoleIndex ), 
		RoleGrade_Max)->getCString());
}

void RoleOptionlayer::palyRoleUpgrade( void )
{
	auto LoadingBarExp = (LoadingBar*)GameMainLayer::seekNodeByName(_roleOptionlayout, "LoadingBarExp" );
	auto action = CCRepeatForever::create( CCSequence::create( DelayTime::create(0.01f), CCCallFuncN::create(this, callfuncN_selector(RoleOptionlayer::callFuncUpgrade)),nullptr ));
	LoadingBarExp->runAction( action );

}

void RoleOptionlayer::startUpgradePre( void )
{
	auto CheckBoxUpgrade = (CheckBox*)GameMainLayer::seekNodeByName(_roleOptionlayout, "CheckBoxUpgrade" );
	auto pebbleData = NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( getOptionPebbleKey() );
	if ( 0 >= pebbleData->number )	
	{
		int buyNum = ( CheckBoxUpgrade->getSelectedState() )?10:1;
		if ( NewDataMgr::getInstance()->mGameBuySystem->buyGoodsWithKey( getOptionPebbleKey(), buyNum ))
		{
			// 多连抽
			float pebbleExp = pebbleData->data;
			NewDataMgr::getInstance()->mGameBuySystem->addGoodsNumWithKey( getOptionPebbleKey(), -buyNum );
			NotificationCenter::getInstance()->postNotification(Msg_ChangeGemGold,nullptr);

			_curHaveExp = pebbleExp * buyNum;
			_consumeExp = 0;

			palyRoleUpgrade();

			return;
		}
// 		else
// 		{
// 			auto widget = CommonWidget::create();
// 			widget->setContent("金币不足,请前往商店购买");
// 			addChild( widget );
// 			return;
// 		}
	}



	float pebbleExp = pebbleData->data;
	int num = ( CheckBoxUpgrade->getSelectedState() )?pebbleData->number : 1;
	
	NewDataMgr::getInstance()->mGameBuySystem->addGoodsNumWithKey( getOptionPebbleKey(), -num );

	_curHaveExp = pebbleExp * num;
	_consumeExp = 0;

	updataRoleUpgrade();

	palyRoleUpgrade();

}

BuyListKey RoleOptionlayer::getOptionPebbleKey( void )
{
	switch ( _curOptionRockIndex )
	{
	case 1:
		return BuyListKey::BuyKey_Pebble1;
		break;
	case 2:
		return BuyListKey::BuyKey_Pebble2;
		break;
	case 3:
		return BuyListKey::BuyKey_Pebble3;
		break;
	default:
		break;
	}
}

void RoleOptionlayer::callfunPlayUpgradeAction( void )
{
//	 NewDataMgr::getInstance()->mRoleUpgradeIng = false;
}

/*******************************************
* 主界面顶部和底部框
*******************************************/
void MainSceneTaskBar::showLayout( Node* node, bool parm/* = true */ )
{
	if ( node && !node->getChildByName("MainSceneTaskBar"))
	{
		auto layout = MainSceneTaskBar::create();
		layout->_isShowButtomBar = parm;
		node->addChild( layout, LocalZOrde_1000, "MainSceneTaskBar" );
	}
}

MainSceneTaskBar::MainSceneTaskBar ()
	:_mainSceneTaskBar(nullptr)
	,_lifeReplyTime(0)
{

}

MainSceneTaskBar::~MainSceneTaskBar ()
{
	NotificationCenter::getInstance()->removeObserver( this, Msg_ShowButtomBar );
	NotificationCenter::getInstance()->removeObserver( this, Msg_HideButtomBar );
	NotificationCenter::getInstance()->removeObserver( this, Msg_ChangeGemGold );
}

void MainSceneTaskBar::onEnter()
{
	Layer::onEnter();

	initInterface();



	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainSceneTaskBar::MsgProShowButtomBar), Msg_ShowButtomBar, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainSceneTaskBar::MsgProHideButtomBar), Msg_HideButtomBar, nullptr);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MainSceneTaskBar::MsgProChangeGemGold), Msg_ChangeGemGold, nullptr);
}

void MainSceneTaskBar::setEventForNode( Button* node, ButtonTag tag )
{
	do 
	{
		CC_BREAK_IF( node == nullptr );

		node->setTag( tag );
		node->addClickEventListener(CC_CALLBACK_1(MainSceneTaskBar::callFuncButtonEvent, this));

	} while (0);
}

void MainSceneTaskBar::callFuncButtonEvent( Ref* ref )
{
	Button* btn = (Button*)ref;
	int nTag = btn->getTag();


	switch ( nTag )
	{
	case Tag_HzOption:
		{
			static int m = 1;m++;
			int hp = NewDataMgr::getInstance()->getRoleHpWithId( 1, m );
			int hp2 = NewDataMgr::getInstance()->getRoleHpWithId( 2, m );
			int hp3 = NewDataMgr::getInstance()->getRoleHpWithId( 3, m );

			CCLOG("--------test hp %d, %d, %d", hp, hp2, hp3);
		}

		break;	

	case Tag_HCOption:
		GameInterfaceMgr::getInstance()->addInterfaceToList( GameInterfaceMgr::InterfaceTag_CompoundCard );
		break;	

	case Tag_LsOption:
		GameInterfaceMgr::getInstance()->addInterfaceToList(GameInterfaceMgr::InterfaceTag_QuickStone);
		break;

	case Tag_MainInOption:
		GameInterfaceMgr::getInstance()->closeAllInterface();
		break;	

	case Tag_JzOption:
		GameInterfaceMgr::getInstance()->addInterfaceToList( GameInterfaceMgr::InterfaceTag_Reward );
		break;	

	case Tag_BuygGem:
		GameInterfaceMgr::getInstance()->addInterfaceToList( GameInterfaceMgr::InterfaceTag_ShopBuy,  true, (void*)ShopBuyLayer::Tag_LabelGem);
		break;
	case Tag_BuyGole:
		GameInterfaceMgr::getInstance()->addInterfaceToList( GameInterfaceMgr::InterfaceTag_ShopBuy,  true, (void*)ShopBuyLayer::Tag_LabelGold);
		break;
	case Tag_BuyWing:
		GameInterfaceMgr::getInstance()->addInterfaceToList( GameInterfaceMgr::InterfaceTag_ShopBuy,  true, (void*)ShopBuyLayer::Tag_LabelWing);
		break;
		
	default:
		break;
	}
}

void MainSceneTaskBar::initInterface()
{
	_mainSceneTaskBar = CSLoader::createNode( Ccs_mainSceneTaskBar_FilePath );

	if ( !_mainSceneTaskBar )
	{
		return;
	}

	addChild( _mainSceneTaskBar );

	do 
	{
		auto ButtonHH = (Button*)GameMainLayer::seekNodeByName(_mainSceneTaskBar, "ButtonHH" );
		setEventForNode( ButtonHH, Tag_HzOption );

		auto ButtonHC = (Button*)GameMainLayer::seekNodeByName(_mainSceneTaskBar, "ButtonHC" );
		setEventForNode( ButtonHC, Tag_HCOption );

		auto ButtonLS = (Button*)GameMainLayer::seekNodeByName(_mainSceneTaskBar, "ButtonLS" );
		setEventForNode( ButtonLS, Tag_LsOption );

		auto ButtonMainInterface = (Button*)GameMainLayer::seekNodeByName(_mainSceneTaskBar, "ButtonMainInterface" );
		setEventForNode( ButtonMainInterface, Tag_MainInOption );

		auto ButtonJC = (Button*)GameMainLayer::seekNodeByName(_mainSceneTaskBar, "ButtonJC" );
		setEventForNode( ButtonJC, Tag_JzOption );


		auto ButtonBuyGold = (Button*)GameMainLayer::seekNodeByName(_mainSceneTaskBar, "ButtonBuyGold" );
		setEventForNode( ButtonBuyGold, Tag_BuyGole );

		auto ButtonBuyGem = (Button*)GameMainLayer::seekNodeByName(_mainSceneTaskBar, "ButtonBuyGem" );
		setEventForNode( ButtonBuyGem, Tag_BuygGem );

		auto ButtonBuyWing = (Button*)GameMainLayer::seekNodeByName(_mainSceneTaskBar, "ButtonBuyWing" );
		setEventForNode( ButtonBuyWing, Tag_BuyWing );
		
		// 音效
		auto CheckBoxMusic = (CheckBox*)GameMainLayer::seekNodeByName(_mainSceneTaskBar, "CheckBoxMusic" );
		CC_BREAK_IF( CheckBoxMusic == nullptr );
		CheckBoxMusic->addEventListener(CC_CALLBACK_2(MainSceneTaskBar::selectedSoundEvent, this));
		CheckBoxMusic->setSelected( !NewAudio::getInstance()->getSoundEffState() );

		// 音乐
		auto CheckBoxSoundEff = (CheckBox*)GameMainLayer::seekNodeByName(_mainSceneTaskBar, "CheckBoxSoundEff" );
		CC_BREAK_IF( CheckBoxSoundEff == nullptr );
		CheckBoxSoundEff->addEventListener(CC_CALLBACK_2(MainSceneTaskBar::selectedMusicEvent, this));
		CheckBoxSoundEff->setSelected( !NewAudio::getInstance()->getMusicState() );


		auto Lifeaddguang	= (Sprite*)GameMainLayer::seekNodeByName(_mainSceneTaskBar, "Lifeaddguang" );
		auto Gemaddguang	= (Sprite*)GameMainLayer::seekNodeByName(_mainSceneTaskBar, "Gemaddguang" );
		auto Goldaddguang	= (Sprite*)GameMainLayer::seekNodeByName(_mainSceneTaskBar, "Goldaddguang" );
		Lifeaddguang->runAction( RepeatForever::create( Sequence::create(CCScaleTo::create(0.01, 0.3f), CCDelayTime::create(1.0f), Spawn::create(CCScaleTo::create(1.5, 0.6f), CCFadeIn::create(1.5),  nullptr),CCFadeOut::create(0.4f),nullptr)));
		Gemaddguang->runAction( RepeatForever::create( Sequence::create(CCScaleTo::create(0.01, 0.3f),  CCDelayTime::create(1.0f),Spawn::create(CCScaleTo::create(1.5, 0.6f), CCFadeIn::create(1.5),  nullptr),CCFadeOut::create(0.4f),nullptr)));
		Goldaddguang->runAction( RepeatForever::create( Sequence::create(CCScaleTo::create(0.01, 0.3f),  CCDelayTime::create(1.0f),Spawn::create(CCScaleTo::create(1.5, 0.6f), CCFadeIn::create(1.5), nullptr),CCFadeOut::create(0.4f),nullptr)));


		if ( !_isShowButtomBar )
		{
			MsgProHideButtomBar( nullptr );
		}
		
		updataShowData();
		verifyLifeTime();

		std::string curtime = CppCallJava::getInstance()->getSystemTime();

	} while (0);
}

void MainSceneTaskBar::selectedMusicEvent( Ref* pSender,CheckBox::EventType type )
{
	switch (type)
	{
	case CheckBox::EventType::UNSELECTED:
		NewAudio::getInstance()->setMusicState( true );
		break;

	case CheckBox::EventType::SELECTED:
		NewAudio::getInstance()->setMusicState( false );
		break;

	default:
		break;
	}
}

void MainSceneTaskBar::selectedSoundEvent( Ref* pSender,CheckBox::EventType type )
{
	switch (type)
	{
	case CheckBox::EventType::UNSELECTED:
		NewAudio::getInstance()->setSoundEffState( true );
		break;

	case CheckBox::EventType::SELECTED:
		NewAudio::getInstance()->setSoundEffState( false );
		break;

	default:
		break;
	}
}

void MainSceneTaskBar::MsgProShowButtomBar( Ref* sender )
{
	auto bottomBar = (Layer*)GameMainLayer::seekNodeByName(_mainSceneTaskBar,"bottomBar");
	bottomBar->setPosition( Vec2(0,0) );
}

void MainSceneTaskBar::MsgProHideButtomBar( Ref* sender )
{
	auto bottomBar = (Layer*)GameMainLayer::seekNodeByName(_mainSceneTaskBar,"bottomBar");
	bottomBar->setPosition( Vec2(0,-300) );
}

void MainSceneTaskBar::updataShowData( void )
{
	// 金币数
	auto AtlasLabelGoldNum = (TextAtlas*)GameMainLayer::seekNodeByName(_mainSceneTaskBar,"AtlasLabelGoldNum");
	AtlasLabelGoldNum->setString( __String::createWithFormat( "%d", NewDataMgr::getInstance()->mUserData.Gold )->getCString() );

	// 宝石数
	auto AtlasLabelGemnum = (TextAtlas*)GameMainLayer::seekNodeByName(_mainSceneTaskBar,"AtlasLabelGemnum");
	AtlasLabelGemnum->setString( __String::createWithFormat( "%d", NewDataMgr::getInstance()->mUserData.Gem )->getCString() );

	// 生命数
	auto AtlasLabellifeNum = (TextAtlas*)GameMainLayer::seekNodeByName(_mainSceneTaskBar,"AtlasLabellifeNum");
	AtlasLabellifeNum->setString( __String::createWithFormat( "/%d",NewDataMgr::getInstance()->mUserData.LifeNum )->getCString() );

	// 倒计时
	auto LifeReplyItem = (Sprite*)GameMainLayer::seekNodeByName(_mainSceneTaskBar,"LifeReplyItem");
	if ( GameLife_MaxLifeNum <= NewDataMgr::getInstance()->mUserData.LifeNum )
		LifeReplyItem->setVisible( false );
	else
		LifeReplyItem->setVisible( true );

}

void MainSceneTaskBar::MsgProConsumeLife( Ref* sender )
{
	NewDataMgr::getInstance()->addUserLife( -1 );
	updataShowData();

	if ( NewDataMgr::getInstance()->mUserData.LifeNum < GameLife_MaxLifeNum && 0 == NewDataMgr::getInstance()->getHistoryLifeTime() )
	{
		NewDataMgr::getInstance()->setHistoryLifeTime( NewDataMgr::getInstance()->convertTimeStrToNumber(CppCallJava::getInstance()->getSystemTime() ) );
		verifyLifeTime();
	}
}

void MainSceneTaskBar::MsgProChangeGemGold( Ref* sender )
{
	updataShowData();
}

void MainSceneTaskBar::verifyLifeTime( void )
{
	double historyTime = NewDataMgr::getInstance()->getHistoryLifeTime();
	int lifenum = NewDataMgr::getInstance()->mUserData.LifeNum;
	if ( 0 == historyTime && lifenum >= GameLife_MaxLifeNum )
	{
		return;
	}

	double curTime = NewDataMgr::getInstance()->convertTimeStrToNumber(CppCallJava::getInstance()->getSystemTime() );

	_lifeReplyTime =  curTime - historyTime;
	CCLOG("MainSceneTaskBar::verifyLifeTime _lifeReplyTime:%d", _lifeReplyTime);

	// 可回复生命数
	int replyLifeNum = _lifeReplyTime / GameLife_ReplyLife_Time;
	replyLifeNum = ( replyLifeNum + lifenum > GameLife_MaxLifeNum ) ? (GameLife_MaxLifeNum-lifenum):replyLifeNum;

	NewDataMgr::getInstance()->addUserLife( replyLifeNum );
	updataShowData();

	// 小于最小回复生命数
	if ( NewDataMgr::getInstance()->mUserData.LifeNum < GameLife_MaxLifeNum )
	{
		// 回复后剩余时间
		_lifeReplyTime = GameLife_ReplyLife_Time - _lifeReplyTime%GameLife_ReplyLife_Time;

		runLifeTime();
	}
	// 达到最小回复生命数
	else
	{
		NewDataMgr::getInstance()->setHistoryLifeTime( 0 );
	}

	
}

void MainSceneTaskBar::runLifeTime( void )
{
	auto AtlasLabellifeTime = (TextAtlas*)GameMainLayer::seekNodeByName(_mainSceneTaskBar,"AtlasLabellifeTime");
	AtlasLabellifeTime->runAction( RepeatForever::create(CCSequence::create(  CCDelayTime::create(1.0f), CCCallFunc::create(this, callfunc_selector(MainSceneTaskBar::callbackLifeTime)), nullptr )));
}

void MainSceneTaskBar::callbackLifeTime()
{
	auto AtlasLabellifeTime = (TextAtlas*)GameMainLayer::seekNodeByName(_mainSceneTaskBar,"AtlasLabellifeTime");

	if ( 0 >= _lifeReplyTime )
	{
		AtlasLabellifeTime->stopAllActions();
		replyLifePro();
		return;
	}

	std::string lifeTime;

	_lifeReplyTime -= 1;
	int min = _lifeReplyTime/60;
	int sec = _lifeReplyTime%60;

	std::string minStr , secStr;
	if (min/10) 
		minStr = __String::createWithFormat("%d",min )->getCString();
	else
		minStr = __String::createWithFormat("0%d",min )->getCString();

	if (sec/10) 
		secStr = __String::createWithFormat(".%d",sec )->getCString();
	else
		secStr = __String::createWithFormat(".0%d",sec )->getCString();

	minStr.append( secStr );
	AtlasLabellifeTime->setString( minStr );

}

void MainSceneTaskBar::replyLifePro( void )
{
	NewDataMgr::getInstance()->addUserLife( 1 );
	updataShowData();
	int lifenum = NewDataMgr::getInstance()->mUserData.LifeNum;


	if ( lifenum < GameLife_MaxLifeNum )
	{
		NewDataMgr::getInstance()->setHistoryLifeTime( NewDataMgr::getInstance()->getHistoryLifeTime() + GameLife_ReplyLife_Time );
		_lifeReplyTime = GameLife_ReplyLife_Time;
		runLifeTime();
	}
	else
	{
		NewDataMgr::getInstance()->setHistoryLifeTime( 0 );
	}
}



/*******************************************
* 卡牌选择界面
*******************************************/
void CardOptionLayout::showLayout( Node* node, int tag )
{
	if ( node && !node->getChildByName("CardOptionLayout") )
	{
		auto layout = CardOptionLayout::create();
		layout->setTag( tag );
		node->addChild( layout, LocalZOrde_500, "CardOptionLayout" );
	}
}

void CardOptionLayout::onEnter()
{
	Layer::onEnter();


	initInterface();

	auto pLayer = (Layer*)GameMainLayer::seekNodeByName(_cardOptionlayout, "PanelTouch" );

	_touchLayer = Layer::create();
	_touchLayer->setContentSize( Director::getInstance()->getVisibleSize() );
	pLayer->addChild(_touchLayer );

	OptionCardTouchListener = EventListenerTouchOneByOne::create();
	OptionCardTouchListener->onTouchBegan = CC_CALLBACK_2(CardOptionLayout::onTouchBegan,this);
	OptionCardTouchListener->onTouchMoved = CC_CALLBACK_2(CardOptionLayout::onTouchMoved,this);
	OptionCardTouchListener->onTouchEnded = CC_CALLBACK_2(CardOptionLayout::onTouchEnded,this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(OptionCardTouchListener,_touchLayer);

	OptionCardTouchListener->setEnabled( false );


}

void CardOptionLayout::initInterface()
{
	_cardOptionlayout = CSLoader::createNode( Ccs_cardOptionLayer_FilePath );

	if ( !_cardOptionlayout )
	{
		return;
	}
	addChild( _cardOptionlayout );

	do 
	{
		// 返回
		auto ButtonBack = (Button*)GameMainLayer::seekNodeByName(_cardOptionlayout, "ButtonBack" );
		setEventForNode( ButtonBack, Tag_Back );

		// 卡牌排序
		auto ButtonSort = (Button*)GameMainLayer::seekNodeByName(_cardOptionlayout, "ButtonSort");
		setEventForNode(ButtonSort, Tag_CardSort);

		// 卡牌1
		auto ButtonCard1 = (Button*)GameMainLayer::seekNodeByName(_cardOptionlayout, "ButtonCard1" );
		ButtonCard1->setCascadeColorEnabled( true );
		setEventForNode( ButtonCard1, Tag_Card1 );

		// 卡牌2
		auto ButtonCard2 = (Button*)GameMainLayer::seekNodeByName(_cardOptionlayout, "ButtonCard2" );
		ButtonCard2->setCascadeColorEnabled( true );
		setEventForNode( ButtonCard2, Tag_Card2 );

		// 卡牌3
		auto ButtonCard3 = (Button*)GameMainLayer::seekNodeByName(_cardOptionlayout, "ButtonCard3" );
		ButtonCard3->setCascadeColorEnabled( true );
		setEventForNode( ButtonCard3, Tag_Card3 );

		// 关闭卡牌选择
		auto ButtonClose = (Button*)GameMainLayer::seekNodeByName(_cardOptionlayout, "ButtonClose" );
		setEventForNode( ButtonClose, Tag_CloseOption );

		auto ButtonCardStar1 = (Button*)GameMainLayer::seekNodeByName(_cardOptionlayout,"ButtonCardStar1");
		setEventForNode( ButtonCardStar1, Tag_CardStarLv1 );

		auto ButtonCardStar2 = (Button*)GameMainLayer::seekNodeByName(_cardOptionlayout,"ButtonCardStar2");
		setEventForNode( ButtonCardStar2, Tag_CardStarLv2 );

		auto ButtonCardStar3 = (Button*)GameMainLayer::seekNodeByName(_cardOptionlayout,"ButtonCardStar3");
		setEventForNode( ButtonCardStar3, Tag_CardStarLv3 );

		auto ButtonCardStar4 = (Button*)GameMainLayer::seekNodeByName(_cardOptionlayout,"ButtonCardStar4");
		setEventForNode( ButtonCardStar4, Tag_CardStarLv4 );

		auto ButtonCardStar5 = (Button*)GameMainLayer::seekNodeByName(_cardOptionlayout,"ButtonCardStar5");
		setEventForNode( ButtonCardStar5, Tag_CardStarLv5 );

		addCardToList();


		// 		_curSelectCardID = NewRoleDataMgr::getInstance()->mRoleConfig.cardArr[_curSelectCardType-1].cardID ;
		// 		_curSelectCardStarLv = NewRoleDataMgr::getInstance()->mRoleConfig.cardArr[_curSelectCardType-1].cardStarLv ;
		updataChangeCard();


	} while (0);
}

void CardOptionLayout::setEventForNode( Button* node, ButtonTag tag )
{
	do 
	{
		CC_BREAK_IF( node == nullptr );
		node->setTag( tag );
		node->addClickEventListener(CC_CALLBACK_1(CardOptionLayout::callFuncButtonEvent, this));

	} while (0);
}

bool CardOptionLayout::onTouchBegan( Touch *touch, Event *unused_event )
{
	hideConfirmOption();

	return true;
}

void CardOptionLayout::callFuncButtonEvent( Ref* ref )
{
	Button* btn = (Button*)ref;
	int nTag = btn->getTag();


	switch ( nTag )
	{
	case Tag_Back:
		{
			NotificationCenter::getInstance()->postNotification(Msg_ChangeCard,nullptr);

			GameInterfaceMgr::getInstance()->showNextInterface();
			//removeFromParentAndCleanup(true);
			GameInterfaceMgr::getInstance()->closeCurInterface();
		}
		break;

	case Tag_CardSort:
		{
			sortCard();
		}
		break;

	case Tag_Card1:	
		{
			runButtonAction(btn);
			updataCardTypeState(nTag);
			updataCardListState(nTag);
		}
		break;

	case Tag_Card2:		
		{
			runButtonAction(btn);
			updataCardTypeState(nTag);
			updataCardListState(nTag);
		}
		break;

	case Tag_Card3:		
		{
			runButtonAction(btn);
			updataCardTypeState(nTag);
			updataCardListState(nTag);
		}
		break;

	case Tag_CloseOption:
		{
			hideConfirmOption();

			// 设置卡牌选择
			// 			NewRoleDataMgr::getInstance()->mRoleConfig.cardArr[_curSelectCardType-1].cardID = _curSelectCardID;
			// 			NewRoleDataMgr::getInstance()->mRoleConfig.cardArr[_curSelectCardType-1].cardStarLv = _curSelectCardStarLv;
		}
		break;

	case Tag_CardStarLv1:
		_curSelectCardStarLv = 1;
		updataChangeCard();
		hideConfirmOption();
		break;
	case Tag_CardStarLv2:
		_curSelectCardStarLv = 2;
		updataChangeCard();
		hideConfirmOption();
		break;
	case Tag_CardStarLv3:
		_curSelectCardStarLv = 3;
		updataChangeCard();
		hideConfirmOption();
		break;
	case Tag_CardStarLv4:
		_curSelectCardStarLv = 4;
		updataChangeCard();
		hideConfirmOption();
		break;
	case Tag_CardStarLv5:
		_curSelectCardStarLv = 5;
		updataChangeCard();
		hideConfirmOption();
		break;

	default:
		break;
	}
}

void CardOptionLayout::callFuncCardOptionEvent( Ref* ref )
{
	Button* btn = (Button*)ref;
	int nTag = btn->getTag();

	_curSelectCardID = nTag+1;
	// 显示卡牌星级选择框
	showConfirmOption();

	updataStarLvInterface( nTag );
}


void CardOptionLayout::addCardToList()
{
	auto ScrollViewCardOption = (cocos2d::ui::ScrollView*)GameMainLayer::seekNodeByName(_cardOptionlayout, "ScrollViewCardOption" );
	ScrollViewCardOption->removeAllChildrenWithCleanup(true);

	Vec2 firstPoint(55, 345);
	Vec2 offsetPoint(127, 124);
	Vec2 valPoint(0,0);

	for ( int i = 0, j = 0; i< GameData_CardNum; i++)
	{
		int maxStarLv = NewDataMgr::getInstance()->getCardMaxStarLvWithId( i );
		if ( maxStarLv )
		{
			valPoint = Vec2(offsetPoint.x*(j%5),   (j/5)*offsetPoint.y);j++;

			// 列表按钮
			std::string buttonFilePath =  __String::createWithFormat("%s%d.png", Img_CardHead_FilePath,i+1)->getCString();
			auto buttonItem = Button::create( buttonFilePath, buttonFilePath, buttonFilePath );
			buttonItem->setPosition( Vec2(firstPoint.x + valPoint.x, firstPoint.y - valPoint.y) );
			buttonItem->setScale( 0.95 );
			buttonItem->setTag( i );
			buttonItem->setCascadeColorEnabled( true );
			buttonItem->addClickEventListener(CC_CALLBACK_1(CardOptionLayout::callFuncCardOptionEvent, this));
			ScrollViewCardOption->addChild( buttonItem );

			// 按钮框
			auto buttonFram = Sprite::create(__String::createWithFormat("%s%d.png", Img_CardHeadK_FilePath, maxStarLv)->getCString());
			buttonFram->setScale( 1.05f );
			buttonFram->setPosition( Vec2(49,44) );
			buttonItem->addChild( buttonFram );

			// 类型
			auto cardType = Sprite::create(__String::createWithFormat("%s%d.png", Img_CardType_FilePath, (i / 3) + 1)->getCString());
			cardType->setScale(0.7f);
			cardType->setPosition(Vec2(5, 64));
			buttonItem->addChild(cardType);

			_cardOptionList.push_back( buttonItem );
		}
	}

}

Button* CardOptionLayout::getCardItem( void )
{
	return nullptr;
}

void CardOptionLayout::updataCardTypeState( int cardtype )
{
	_curSelectCardType = cardtype;
	_isOption = (_lastSelectCardType == _curSelectCardType) ? !_isOption : true;;
	_lastSelectCardType = _curSelectCardType;

	for ( int i = 1; i<= 3; i++ )
	{
		auto ButtonCard = (Button*)GameMainLayer::seekNodeByName(_cardOptionlayout, __String::createWithFormat("ButtonCard%d", i)->getCString() );

		if (i != cardtype && _isOption)
			ButtonCard->runAction(CCTintTo::create(0.1f, Color3B(74, 74, 74)));//setColor( Color3B(74,74,74) );
		else
			ButtonCard->runAction(CCTintTo::create(0.1f, Color3B(255, 255, 255)));//setColor( Color3B(255,255,255) );
	}
}

void CardOptionLayout::updataCardListState( int optionid )
{
	for (std::vector<Button*>::iterator item = _cardOptionList.begin(); item!= _cardOptionList.end(); item++ )
	{
		int tempid = ((*item)->getTag()/3)+1;

		if ( tempid != optionid && _isOption)
		{
			(*item)->runAction(CCTintTo::create(0.1f, Color3B(74, 74, 74)));//setColor( Color3B(74,74,74) );
			(*item)->setTouchEnabled(false);
		}
		else
		{
			(*item)->runAction(CCTintTo::create(0.1f, Color3B(255, 255, 255)));//setColor( Color3B(255,255,255) );
			(*item)->setTouchEnabled( true );
		}

	}

}

void CardOptionLayout::updataStarLvInterface( int id )
{
	if ( !_isOption )
	{
		updataCardTypeState((id / 3) + 1);
	}

	updataCardListState( (id/3)+1 );
	for ( int i = 1; i<= 5; i++ )
	{
		std::string cardHeadPath = __String::createWithFormat("%s%d.png", Img_CardHead_FilePath , _curSelectCardID)->getCString();
		auto ButtonCardStar = (Button*)GameMainLayer::seekNodeByName(_cardOptionlayout,__String::createWithFormat("ButtonCardStar%d", i)->getCString() );
		ButtonCardStar->loadTextures( cardHeadPath, cardHeadPath, cardHeadPath );

		int cardNum = NewDataMgr::getInstance()->getCardNumWithStarLv( id, i );
		if ( cardNum )
		{
			ButtonCardStar->setVisible( true );
		}
		else
		{
			ButtonCardStar->setVisible( false );
		}

		// 类型
		std::string filePath = StringUtils::format("%s%d.png", Img_CardType_FilePath, (id/3)+1);
		auto CardType = (Sprite*)GameMainLayer::seekNodeByName(_cardOptionlayout, StringUtils::format( "StarLvCardType_%d", i) );
		CardType->setTexture( filePath );

		// 攻击力
		auto AtlasLabelAtkCard = (TextAtlas*)GameMainLayer::seekNodeByName(_cardOptionlayout, __String::createWithFormat( "AtlasLabelATKStar%d", i)->getCString() );
		AtlasLabelAtkCard->setString( __String::createWithFormat("%d%s", (int)NewDataMgr::getInstance()->getCardAtk(_curSelectCardID-1, i ), "%")->getCString() );
	}



}

void CardOptionLayout::showConfirmOption( void )
{
	OptionCardTouchListener->setEnabled( true );
	//OptionCardTouchListener->setSwallowTouches(true);

	auto PanelOptionCard = (Layer*)GameMainLayer::seekNodeByName(_cardOptionlayout, "PanelOptionCard" );
	PanelOptionCard->setPosition(  Vec2( 360, PanelOptionCard->getPosition().y) );

	OptionCardTouchListener->setSwallowTouches(true);

	//getEventDispatcher()->addEventListenerWithSceneGraphPriority(OptionCardTouchListener,_touchLayer);
}

void CardOptionLayout::hideConfirmOption( void )
{
	OptionCardTouchListener->setEnabled( false );

	auto PanelOptionCard = (Layer*)GameMainLayer::seekNodeByName(_cardOptionlayout, "PanelOptionCard" );
	PanelOptionCard->setPosition(  Vec2( 360+720, PanelOptionCard->getPosition().y) );

	updataCardTypeState( _curSelectCardType );
	updataCardListState( _curSelectCardID );

}

void CardOptionLayout::runButtonAction( Node* node )
{
	auto action = CCSequence::create(CCEaseSineOut::create(CCScaleTo::create(0.1f, 1.15f)), CCEaseSineIn::create(CCScaleTo::create(0.05f, 1.0f)), nullptr);
	node->runAction( action );
}

void CardOptionLayout::updataChangeCard( void )
{
	// 设置卡牌选择
	if ( _curSelectCardType )
	{
		NewDataMgr::getInstance()->setCardOptionWithIndex(_curSelectCardType, _curSelectCardID, _curSelectCardStarLv );
	}

	for ( int i = 1; i<=3; i++ )
	{		
		int cardId = NewDataMgr::getInstance()->mRoleConfig.cardArr[i - 1].cardID;
		
		// 卡牌
		std::string filePath = StringUtils::format("%s%d.png", Img_Card_FilePath, cardId);
		auto ButtonCard = (Button*)GameMainLayer::seekNodeByName(_cardOptionlayout, __String::createWithFormat( "ButtonCard%d", i)->getCString() );
		ButtonCard->loadTextures( filePath, filePath, filePath );

		// 灵石
		filePath = StringUtils::format("%s%d.png", Img_QuickStonLittle_FinePath, NewDataMgr::getInstance()->getCardConfigWithId(cardId-1)->quickStoneId);
		auto quickStone = (Sprite*)GameMainLayer::seekNodeByName(_cardOptionlayout, __String::createWithFormat("quickStone%d", i)->getCString());
		quickStone->setTexture(filePath);

		for( int j = 1; j<=5; j++ )
		{
			// 星星等级
			auto LvStar = (Node*)GameMainLayer::seekNodeByName(_cardOptionlayout, __String::createWithFormat( "StarLv%d_%d", i, j)->getCString() );

			int starLv = NewDataMgr::getInstance()->mRoleConfig.cardArr[i-1].cardStarLv;
			if ( starLv < j )
				LvStar->setVisible( false );
			else
				LvStar->setVisible( true );

		}

		// 攻击力
		auto AtlasLabelAtkCardType = (TextAtlas*)GameMainLayer::seekNodeByName(_cardOptionlayout, __String::createWithFormat( "AtlasLabelAtkCardType%d", i)->getCString() );
		AtlasLabelAtkCardType->setString( __String::createWithFormat("%d%s", (int)NewDataMgr::getInstance()->getCardAtk(
			NewDataMgr::getInstance()->mRoleConfig.cardArr[i-1].cardID-1, 
			NewDataMgr::getInstance()->mRoleConfig.cardArr[i-1].cardStarLv ), "/")->getCString() );

	}


}

void CardOptionLayout::sortCard(void)
{
	std::list<csvCardAtkPConfig_s> cardlist;

	for (int i = 0; i < GameData_CardNum; i++ )
	{
		int maxStarLv = NewDataMgr::getInstance()->getCardMaxStarLvWithId(i);

		if (maxStarLv)	
		{
			auto cardConfig = NewDataMgr::getInstance()->getCardConfigWithId(i);
			cardlist.push_back(*cardConfig);
		}
	}

	// 排序
	cardlist.sort(CmpareCardAtk());

	auto ScrollViewCardOption = (cocos2d::ui::ScrollView*)GameMainLayer::seekNodeByName(_cardOptionlayout, "ScrollViewCardOption");
	ScrollViewCardOption->removeAllChildrenWithCleanup(true);

	Vec2 firstPoint(55, 345);
	Vec2 offsetPoint(127, 124);
	Vec2 valPoint(0, 0);

	std::list<csvCardAtkPConfig_s>::iterator item = cardlist.begin();

	_cardOptionList.clear();
	for ( int i = 0; item != cardlist.end(); item++, i++ )
	{
		int maxStarLv = NewDataMgr::getInstance()->getCardMaxStarLvWithId(item->id);
		valPoint = Vec2(offsetPoint.x*(i % 5), (i / 5)*offsetPoint.y);

		// 列表按钮
		std::string buttonFilePath = StringUtils::format("%s%d.png", Img_CardHead_FilePath, item->id+1);
		auto buttonItem = Button::create(buttonFilePath, buttonFilePath, buttonFilePath);
		buttonItem->setPosition(Vec2(firstPoint.x + valPoint.x, firstPoint.y - valPoint.y));
		buttonItem->setScale(0.95);
		buttonItem->setTag(item->id);
		buttonItem->setCascadeColorEnabled(true);
		buttonItem->addClickEventListener(CC_CALLBACK_1(CardOptionLayout::callFuncCardOptionEvent, this));
		ScrollViewCardOption->addChild(buttonItem);

		// 按钮框
		auto buttonFram = Sprite::create(StringUtils::format("%s%d.png", Img_CardHeadK_FilePath, maxStarLv));
		buttonFram->setScale(1.05f);
		buttonFram->setPosition(Vec2(49, 44));
		buttonItem->addChild(buttonFram);

		// 类型
		auto cardType = Sprite::create(StringUtils::format("%s%d.png", Img_CardType_FilePath, item->CardType) );
		cardType->setScale(0.7f);
		cardType->setPosition(Vec2(5, 64));
		buttonItem->addChild(cardType);

		_cardOptionList.push_back(buttonItem);

	}
}


/*******************************************
* 卡牌合成界面
*******************************************/
void CardCompoundLayer::showLayout( Node* node, int tag )
{
	if ( node && !node->getChildByName("CardCompoundLayer") )
	{
		auto layout = CardCompoundLayer::create();
		layout->setTag( tag );
		node->addChild( layout, LocalZOrde_500, "CardCompoundLayer" );
	}
}

void CardCompoundLayer::onEnter()
{
	Layer::onEnter();

	initInterface();
}

void CardCompoundLayer::setEventForNode( Button* node, ButtonTag tag )
{
	do 
	{
		CC_BREAK_IF( node == nullptr );

		node->setTag( tag );
		node->addClickEventListener(CC_CALLBACK_1(CardCompoundLayer::callFuncButtonEvent, this));

	} while (0);
}

void CardCompoundLayer::callFuncButtonEvent( Ref* ref )
{
	Button* btn = (Button*)ref;
	int nTag = btn->getTag();


	switch ( nTag )
	{

	case Tag_Back:
		{
			switch (_curCompoundTag)
			{
			case CardCompoundLayer::CompoundTag_1:
				//removeFromParentAndCleanup( true );
				GameInterfaceMgr::getInstance()->closeCurInterface();
				break;
			case CardCompoundLayer::CompoundTag_2:
				changeCardCompound( CompoundTag_1 );
				break;
			case CardCompoundLayer::CompoundTag_3:
				changeCardCompound( CompoundTag_2 );
				break;
			default:
				break;
			}
		}
		break;	
	
	case Tag_1LvCard:
		_curOptionCompoundStarLv = 1;
		updataOptionCompInfo();
		break;

	case Tag_2LvCard:
		_curOptionCompoundStarLv = 2;
		updataOptionCompInfo();
		break;

	case Tag_3LvCard:
		_curOptionCompoundStarLv = 3;
		updataOptionCompInfo();
		break;

	case Tag_4LvCard:
		_curOptionCompoundStarLv = 4;
		updataOptionCompInfo();
		break;

	case Tag_5LvCard:
		_curOptionCompoundStarLv = 5;
		updataOptionCompInfo();
		break;

	case Tag_HeCheng:
		changeCardCompound( CompoundTag_3 );
		break;

	default:
		break;
	}
}

void CardCompoundLayer::initInterface()
{
	_cardCompoundLayer = CSLoader::createNode( Ccs_compoundCardLayer_FilePath );

	if ( !_cardCompoundLayer )
	{
		return;
	}

	addChild( _cardCompoundLayer );

	do 
	{
		auto ButtonBack = (Button*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "ButtonBack" );
		setEventForNode( ButtonBack, Tag_Back );

		_ButtonCardStar[0] = (Button*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "ButtonCardStar1" );
		setEventForNode( _ButtonCardStar[0], Tag_1LvCard );

		_ButtonCardStar[1] = (Button*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "ButtonCardStar2" );
		setEventForNode( _ButtonCardStar[1], Tag_2LvCard );

		_ButtonCardStar[2] = (Button*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "ButtonCardStar3" );
		setEventForNode( _ButtonCardStar[2], Tag_3LvCard );

		_ButtonCardStar[3] = (Button*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "ButtonCardStar4" );
		setEventForNode( _ButtonCardStar[3], Tag_4LvCard );

		_ButtonCardStar[4] = (Button*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "ButtonCardStar5" );
		setEventForNode( _ButtonCardStar[4], Tag_5LvCard );

		auto ButtonHeCheng = (Button*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "ButtonHeCheng" );
		setEventForNode( ButtonHeCheng, Tag_HeCheng );
		ButtonHeCheng->setTouchEnabled( false );
		ButtonHeCheng->setColor( Color3B(74,74,74) );

		

		// 为升级动作界面卡牌添加触摸事件 
		auto imgCard = (Sprite*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "imgCard" );
		auto listener1 = EventListenerTouchOneByOne::create();
		listener1->setSwallowTouches(true);
		setUserObject(listener1);
		listener1->onTouchBegan = [=](Touch* touch, Event* event){
			Vec2 locationInNode = imgCard->convertToNodeSpace(touch->getLocation());
			Size s = imgCard->getContentSize();
			Rect rect = Rect(0, 0, s.width, s.height);

			if (rect.containsPoint(locationInNode) && _compoundFinish)
			{
				_compoundFinish = false;
				NotificationCenter::getInstance()->postNotification(Msg_ShowButtomBar,nullptr);
				changeCardCompound( CompoundTag_2 );

				return true;
			}
			return false;
		};
	

		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, imgCard);
		
		initOptionCardInterface();

	} while (0);

	// 初始首界面
	changeCardCompound( CompoundTag_1 );
}

void CardCompoundLayer::initOptionCardInterface()
{
	// 合成提示
	auto TextCompoundTips = (Text*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "TextCompoundTips" );
	TextCompoundTips->setVisible( true );
	TextCompoundTips->runAction( CCRepeatForever::create(  
		CCSequence::create(
		CCFadeOut::create(0.5f), 
		CCFadeIn::create(0.5f),
		nullptr) ) );

	// 选择升能卡牌及动作
	_optionCardLvFram = (Sprite*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "optionCardLvFram" );
	_optionCardLvFram->setPosition( _ButtonCardStar[0]->getPosition() );
	_optionCardLvFram->runAction(  RepeatForever::create( CCRotateTo::create( 1.0f, -720 )) );
	_optionCardLvFram->setVisible( false );

	for ( int i = 1; i<=5; i++ )
	{
		_cardStarLv[i-1] = (Sprite*)GameMainLayer::seekNodeByName(_cardCompoundLayer, 
			__String::createWithFormat("CardStar%d", i)->getCString() );

		_cardStarLv[i-1]->setVisible( false );

		// 当前等级星数
		auto leftStar = (Sprite*)GameMainLayer::seekNodeByName( _cardCompoundLayer, 
			__String::createWithFormat("leftStar%d", i)->getCString() );
		leftStar->setVisible( false );


		// 将要升级等级星数
		auto rightStar = (Sprite*)GameMainLayer::seekNodeByName( _cardCompoundLayer, 
			__String::createWithFormat("rightStar%d", i)->getCString() );
		rightStar->setVisible( false );
	}

	// 将光隐藏
	auto SpriteCardGuang1 = (Sprite*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "SpriteCardGuang1" );
	auto SpriteCardGuang2 = (Sprite*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "SpriteCardGuang2" );
	auto SpriteCardGuang3 = (Sprite*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "SpriteCardGuang3" );

	SpriteCardGuang1->setOpacity( 0 );
	SpriteCardGuang2->setOpacity( 0 );
	SpriteCardGuang3->setOpacity( 0 );

	// 攻击力
	auto TextLeftAtk = (Text*)GameMainLayer::seekNodeByName( _cardCompoundLayer, "TextLeftAtk" );
	auto TextRightAtk = (Text*)GameMainLayer::seekNodeByName( _cardCompoundLayer, "TextRightAtk" );
	TextLeftAtk->setVisible( false );
	TextRightAtk->setVisible( false );
	
	// 升级效果文字提示
	auto TextDes1 = (Text*)GameMainLayer::seekNodeByName( _cardCompoundLayer, "TextDes1" );
	auto TextDes2 = (Text*)GameMainLayer::seekNodeByName( _cardCompoundLayer, "TextDes2" );
	auto TextDes3 = (Text*)GameMainLayer::seekNodeByName( _cardCompoundLayer, "TextDes3" );
	TextDes1->setVisible( false );
	TextDes2->setVisible( false );
	TextDes3->setVisible( false );
}

void CardCompoundLayer::changeCardCompound( CompoundTag tag )
{
	auto size = Director::getInstance()->getVisibleSize();
	float centreX  = size.width/2;

	auto NodeCompound = (Node*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "NodeCompound" );
	auto NodeOptionCard = (Node*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "NodeOptionCard" );
	auto NodeCompoundEff = (Node*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "NodeCompoundEff" );

	_curCompoundTag = tag;
	switch ( tag )
	{
	case CompoundTag_1:
		addCardToList();
		NodeCompoundEff->setPosition( Vec2( -centreX, NodeCompoundEff->getPosition().y) );
		NodeCompound->setPosition( Vec2( -centreX, NodeCompound->getPosition().y) );
		NodeOptionCard->setPosition( Vec2( centreX, NodeOptionCard->getPosition().y) );
		break;

	case CompoundTag_2:
		initOptionCardInterface();
		updataCompoundNeedNum();
		NodeCompound->setPosition( Vec2( centreX, NodeCompound->getPosition().y) );
		NodeOptionCard->setPosition( Vec2( -centreX, NodeOptionCard->getPosition().y) );
		NodeCompoundEff->setPosition( Vec2( -centreX, NodeCompoundEff->getPosition().y) );
		break;

	case  CompoundTag_3:
		NotificationCenter::getInstance()->postNotification(Msg_HideButtomBar,nullptr);
		NodeCompound->setPosition( Vec2( -centreX, NodeCompound->getPosition().y) );
		NodeOptionCard->setPosition( Vec2( -centreX, NodeOptionCard->getPosition().y) );
		NodeCompoundEff->setPosition( Vec2( centreX, NodeCompoundEff->getPosition().y) );
		playCompoundAction1();
		//playCompoundAction2();
		break;
	default:
		break;

	}
}

void CardCompoundLayer::addCardToList( void )
{
	auto ScrollViewCardOption = (cocos2d::ui::ScrollView*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "ScrollViewCardOption" );
	ScrollViewCardOption->removeAllChildrenWithCleanup( true );

	auto scroll = ScrollViewCardOption->getInnerContainerSize();
	Vec2 firstPoint(40, scroll.height - 70 );
	Vec2 offsetPoint(130, 124);
	Vec2 valPoint(0,0);


	int card1Index = NewDataMgr::getInstance()->mRoleConfig.cardArr[0].cardID-1;
	int card2Index = NewDataMgr::getInstance()->mRoleConfig.cardArr[1].cardID-1;
	int card3Index = NewDataMgr::getInstance()->mRoleConfig.cardArr[2].cardID-1;

	for ( int i = 0, j = 0; i< GameData_CardNum; i++)
	{
		int maxStarLv = NewDataMgr::getInstance()->getCardMaxStarLvWithId( i );
		if ( maxStarLv )
		{
			valPoint = Vec2(offsetPoint.x*(j%5),   (j/5)*offsetPoint.y);j++;

			//std::string buttonFilePath =  __String::createWithFormat("ccsRes/mainLayer/imgcard%d_1.png", (i/3)+1)->getCString();"ccsRes/mainLayer/imgcard1_1.png";
			std::string buttonFilePath =  __String::createWithFormat("%s%d.png", Img_CardHead_FilePath,i+1)->getCString();
			auto buttonItem = Button::create( buttonFilePath, buttonFilePath, buttonFilePath );
			buttonItem->setPosition( Vec2(firstPoint.x + valPoint.x, firstPoint.y - valPoint.y) );
			buttonItem->setScale( 0.95 );
			buttonItem->setTag( i );
			buttonItem->setCascadeColorEnabled( true );
			buttonItem->addClickEventListener(CC_CALLBACK_1(CardCompoundLayer::callFuncCardOptionEvent, this));
			ScrollViewCardOption->addChild( buttonItem );

			auto buttonFram = Sprite::create( __String::createWithFormat("roleImg/CardStarLv%d.png", maxStarLv)->getCString() );
			buttonFram->setScale( 1.05f );
			buttonFram->setPosition( Vec2(49,44) );
			buttonItem->addChild( buttonFram );

			// 使用中
			if ( i == card1Index || i == card2Index || i == card3Index )
			{
				auto userTextTips = Text::create();
				userTextTips->setString( NewDataMgr::getInstance()->getXmlStringWithKey("ueseing_string") );
				userTextTips->setFontSize( 20 );
				userTextTips->enableShadow( Color4B(110,110,110,255), Size(1,-1), 2);
				userTextTips->setPosition( buttonItem->getPosition() );
				userTextTips->setColor( Color3B(255,255,0) );
				ScrollViewCardOption->addChild( userTextTips );
				userTextTips->runAction( CCRepeatForever::create(CCSequence::create(
					CCFadeOut::create(0.4f),
					CCFadeIn::create(0.4f),
					nullptr)));
			}


			_cardOptionList.push_back( buttonItem );

			for ( int j=1; j<=4; j++)
			{
				// 需要升级的数
				int needsCardNum	= getCompoundNeedNumWithLv( j );
				// 当前拥有
				int myCardNum		= NewDataMgr::getInstance()->getCardNumWithStarLv( i, j );

				// 未满足要求的处理
				if ( needsCardNum > myCardNum )
				{
					buttonItem->setColor( Color3B(74,74,74) );
				}
				else
				{
					buttonItem->setColor( Color3B(255,255,255) );
					break;

				}				
			}
		}
	}

}

void CardCompoundLayer::callFuncCardOptionEvent( Ref* ref )
{
	auto btn = (Button*)ref;

	// 存储选择升级的卡牌ID
	_curOptionCompoundID = btn->getTag();

	changeCardCompound( CompoundTag_2 );
}

void CardCompoundLayer::playCompoundAction1( void )
{
	// 更换卡牌框图片
	std::string filePath = __String::createWithFormat("%s%d.png", "ccsRes/cardOptionLayer/CardFrame",  (_curOptionCompoundID+1)/3+1 )->getCString();
	auto CardFream = (Sprite*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "CardFream" );
	CardFream->setTexture( filePath );

	// 更换卡牌图片
	filePath = __String::createWithFormat("%s%d.png", Img_Card_FilePath,  _curOptionCompoundID+1 )->getCString();
	auto imgCard = (Sprite*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "imgCard" );
	imgCard->setTexture( filePath );

	for ( int i = 1; i <= 5; i++ )
	{			
		_cardStarLv[i-1]->setVisible( false );
	}

	auto SpriteCardGuang1 = (Sprite*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "SpriteCardGuang1" );
	auto SpriteCardGuang2 = (Sprite*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "SpriteCardGuang2" );
	auto SpriteCardGuang3 = (Sprite*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "SpriteCardGuang3" );

	SpriteCardGuang1->setOpacity( 255 );
	SpriteCardGuang2->setOpacity( 255 );
	SpriteCardGuang3->setOpacity( 255 );

	SpriteCardGuang3->runAction( CCEaseSineIn::create(CCRotateTo::create(4.0f, -1500)) );

	SpriteCardGuang2->setScale(0.2f);
	SpriteCardGuang2->runAction( Sequence::create( 
		DelayTime::create( 1.5f )
		,CCScaleTo::create(0.5f, 50.0f) 
		,CCCallFunc::create(this, callfunc_selector(CardCompoundLayer::playCompoundAction1_1))
		,DelayTime::create( 2.0f )
		,CCCallFunc::create(this, callfunc_selector(CardCompoundLayer::playCompoundAction2))
		,nullptr));

}

void CardCompoundLayer::playCompoundAction1_1( void )
{
	auto SpriteCardGuang1 = (Sprite*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "SpriteCardGuang1" );
	auto SpriteCardGuang2 = (Sprite*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "SpriteCardGuang2" );
	auto SpriteCardGuang3 = (Sprite*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "SpriteCardGuang3" );

	SpriteCardGuang2->runAction( CCFadeOut::create(0.1f) );
	SpriteCardGuang1->runAction( CCFadeOut::create(1.9f) );
	SpriteCardGuang3->runAction( CCFadeOut::create(1.9f) );
}

void CardCompoundLayer::playCompoundAction1_2( void )
{
	auto SpriteCardGuang3 = (Sprite*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "SpriteCardGuang3" );
	SpriteCardGuang3->runAction( CCRepeatForever::create( CCRotateTo::create(1.5f, -1024) ));
}

void CardCompoundLayer::playCompoundAction2( void )
{
	auto s = Director::getInstance()->getWinSize();
	
	// 更换卡牌图片
	std::string filePath = __String::createWithFormat("%s%d.png", Img_Card_FilePath,  _curOptionCompoundID+1 )->getCString();
	auto imgCard = (Sprite*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "imgCard" );
	imgCard->setTexture( filePath );

	for ( int i = 1; i <= 5; i++ )
	{
		// 播放动作的星星
		if ( i <= _curOptionCompoundStarLv+1 )
		{
			_cardStarLv[i-1]->setVisible( true );
			auto curPos = _cardStarLv[i-1]->getPosition();

			_cardStarLv[i-1]->setVisible( true );
			_cardStarLv[i-1]->setPosition( Vec2(s.width/2, s.height/2) );
			_cardStarLv[i-1]->setScale( 10 );
			_cardStarLv[i-1]->setOpacity( 0 );

			_cardStarLv[i-1]->runAction( CCSequence::create( 
				DelayTime::create(0.5*i), 
				CCSpawn::create(
					EaseSineIn::create(CCMoveTo::create(0.15f, curPos)),
					CCScaleTo::create(0.15f, 1.0f),
					CCFadeIn::create(0.15f),
					nullptr),
				CCCallFunc::create(this,callfunc_selector(CardCompoundLayer::callFuncShake)),
				nullptr) );
		}
		else
		{
			_cardStarLv[i-1]->setVisible( false );
		}
	}

	// 添加一个卡牌数据
	NewDataMgr::getInstance()->addCardNumWithStar( _curOptionCompoundID, _curOptionCompoundStarLv+1, 1);
	NewDataMgr::getInstance()->addCardNumWithStar( _curOptionCompoundID, _curOptionCompoundStarLv, -getCompoundNeedNumWithLv(_curOptionCompoundStarLv));
	// 更新界面
	updataCompoundNeedNum();
	updataOptionCompInfo();

	runAction( CCSequence::create(
		DelayTime::create(0.8*(_curOptionCompoundStarLv + 1)),
		CCCallFunc::create(this,callfunc_selector(CardCompoundLayer::callFuncComPoundFinish)), 
		nullptr));
}

void CardCompoundLayer::callFuncShake( void )
{
	// 卡牌抖动
	ActionInterval* moveby = MoveBy::create(0.01f,Vec2(30,0));
	ActionInterval* moveback = MoveBy::create(0.01f,Vec2(-30,0));
	ActionInterval* seq = Sequence::create( moveby,moveback,nullptr);
	ActionInterval* rep = RepeatForever::create(seq);

	auto imgCard = (Sprite*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "imgCard" );

	imgCard->runAction( seq );
}

void CardCompoundLayer::callFuncComPoundFinish( void )
{
	_compoundFinish = true;
}

void CardCompoundLayer::updataCompoundNeedNum( void )
{
	for ( int i=1; i<=5; i++)
	{
		// 需要升级的数
		int needsCardNum	= getCompoundNeedNumWithLv( i );
		// 当前拥有
		int myCardNum		= NewDataMgr::getInstance()->getCardNumWithStarLv( _curOptionCompoundID, i );

		auto ButtonCardStar = (Button*)GameMainLayer::seekNodeByName( _cardCompoundLayer, 
			__String::createWithFormat("ButtonCardStar%d", i)->getCString() );

		// 更换头像
		std::string buttonFilePath =  __String::createWithFormat("%s%d.png", Img_CardHead_FilePath,_curOptionCompoundID+1)->getCString();
		ButtonCardStar->loadTextures( buttonFilePath, buttonFilePath, buttonFilePath );

		// 未满足要求的处理
		if ( needsCardNum > myCardNum || (5 == i && !myCardNum))
		{
			//ButtonCardStar->setTouchEnabled( false );
			ButtonCardStar->setColor( Color3B(74,74,74) );
		}
		else
		{
			ButtonCardStar->setTouchEnabled( true );
			ButtonCardStar->setColor( Color3B(255,255,255) );

		}

		// 显示数量对比
		auto compoundNum = (TextAtlas*)GameMainLayer::seekNodeByName(_cardCompoundLayer, 
			__String::createWithFormat("CardLv%dNumber", i)->getCString() );

		if ( i == 5 )
			compoundNum->setString( __String::createWithFormat("%d", myCardNum)->getCString() );
		else
			compoundNum->setString( __String::createWithFormat("%d/%d", myCardNum, needsCardNum)->getCString() );
	}
		
}

int CardCompoundLayer::getCompoundNeedNumWithLv( int starLv )
{
	switch ( starLv )
	{
	case 1:
		return CardCompound_StarLv1_To_StarLv2;
		break;
		
	case 2:
		return CardCompound_StarLv2_To_StarLv3;
		break;

	case 3:
		return CardCompound_StarLv3_To_StarLv4;
		break;

	case 4:
		return CardCompound_StarLv4_To_StarLv5;
		break;

	case 5:
		return 1;
		break;
	default:
		break;
	}
}

void CardCompoundLayer::updataOptionCompInfo( void )
{
	_optionCardLvFram->setVisible( true );
	_optionCardLvFram->setPosition( _ButtonCardStar[_curOptionCompoundStarLv-1]->getPosition() );

	int curStarLv	= _curOptionCompoundStarLv;
	int withStarLv	= _curOptionCompoundStarLv+1;

	for ( int i = 1; i<=5; i++ )
	{
		// 当前等级星数
		auto leftStar = (Sprite*)GameMainLayer::seekNodeByName( _cardCompoundLayer, 
			__String::createWithFormat("leftStar%d", i)->getCString() );

		leftStar->setVisible( false );
		if ( i <= curStarLv ) 	leftStar->setVisible( true );


		// 将要升级等级星数
		auto rightStar = (Sprite*)GameMainLayer::seekNodeByName( _cardCompoundLayer, 
			__String::createWithFormat("rightStar%d", i)->getCString() );

		rightStar->setVisible( false );
		if ( i <= withStarLv && curStarLv !=5 ) 	rightStar->setVisible( true );
	}

	// 需要升级的数
	int needsCardNum	= getCompoundNeedNumWithLv( _curOptionCompoundStarLv );
	// 当前拥有
	int myCardNum		= NewDataMgr::getInstance()->getCardNumWithStarLv( _curOptionCompoundID, _curOptionCompoundStarLv );

	// 合成按钮
	auto ButtonHeCheng = (Button*)GameMainLayer::seekNodeByName( _cardCompoundLayer, "ButtonHeCheng" );
	if ( _curOptionCompoundStarLv == 5 || myCardNum < needsCardNum )
	{
		ButtonHeCheng->setTouchEnabled( false );
		ButtonHeCheng->setColor( Color3B(74,74,74) );
	}
	else
	{
		ButtonHeCheng->setTouchEnabled( true );
		ButtonHeCheng->setColor( Color3B(255,255,255) );
	}
	
	// 攻击力
	auto TextLeftAtk = (Text*)GameMainLayer::seekNodeByName( _cardCompoundLayer, "TextLeftAtk" );TextLeftAtk->setVisible( true );
	auto TextRightAtk = (Text*)GameMainLayer::seekNodeByName( _cardCompoundLayer, "TextRightAtk" );TextRightAtk->setVisible( true );
	TextLeftAtk->setString( __String::createWithFormat("%d%s", (int)NewDataMgr::getInstance()->getCardAtk(_curOptionCompoundID ,_curOptionCompoundStarLv ) ,"%")->getCString() );


	// 五星级时不显示右侧数据
	if ( 5 <= _curOptionCompoundStarLv )
		TextRightAtk->setString("");
	else
		TextRightAtk->setString( __String::createWithFormat("%d%s", (int)NewDataMgr::getInstance()->getCardAtk(_curOptionCompoundID ,_curOptionCompoundStarLv+1 ),"%")->getCString() );

	TextLeftAtk->stopAllActions();TextLeftAtk->setScale( 1.3f );TextLeftAtk->runAction( CCScaleTo::create( 0.1f, 1.0f));
	TextRightAtk->stopAllActions();TextRightAtk->setScale( 1.3f );TextRightAtk->runAction( CCScaleTo::create( 0.1f, 1.0f));

	// 升级效果文字提示
	auto TextDes1 = (Text*)GameMainLayer::seekNodeByName( _cardCompoundLayer, "TextDes1" );TextDes1->setVisible( true );
	auto TextDes2 = (Text*)GameMainLayer::seekNodeByName( _cardCompoundLayer, "TextDes2" );TextDes2->setVisible( true );
	auto TextDes3 = (Text*)GameMainLayer::seekNodeByName( _cardCompoundLayer, "TextDes3" );TextDes3->setVisible( true );

	TextDes1->setColor( Color3B(255,255, 255) );TextDes1->stopAllActions();TextDes1->setScale( 1.0f );
	TextDes2->setColor( Color3B(255,255, 255) );TextDes2->stopAllActions();TextDes2->setScale( 1.0f );
	TextDes3->setColor( Color3B(255,255, 255) );TextDes3->stopAllActions();TextDes3->setScale( 1.0f );

	TextDes1->setString( NewDataMgr::getInstance()->getXmlStringWithKey( StringUtils::format("des1to2Star_card%d", _curOptionCompoundID+1 ).c_str() ));
	TextDes2->setString( NewDataMgr::getInstance()->getXmlStringWithKey( StringUtils::format("des3to4Star_card%d", _curOptionCompoundID+1 ).c_str() ));
	TextDes3->setString( NewDataMgr::getInstance()->getXmlStringWithKey( StringUtils::format("des5Star_card%d", _curOptionCompoundID+1 ).c_str() ));

	// 选择星级状态下高亮显示
	Color3B color(46,215, 9);
	if ( 2 >= _curOptionCompoundStarLv)
	{
		TextDes1->setColor( color );
		//TextDes1->runAction(RepeatForever::create( Sequence::create(CCScaleTo::create(0.3f, 1.0, 1.2f),CCScaleTo::create(0.3f, 1.0, 1.0f),nullptr)));
	}
	else if ( 4 >= _curOptionCompoundStarLv)
	{
		TextDes2->setColor( color );
		//TextDes2->runAction(RepeatForever::create( Sequence::create(CCScaleTo::create(0.3f, 1.0, 1.2f),CCScaleTo::create(0.3f, 1.0, 1.0f),nullptr)));
	}
	else if ( 5 >= _curOptionCompoundStarLv)
	{
		TextDes3->setColor( color );
		//TextDes3->runAction(RepeatForever::create( Sequence::create(CCScaleTo::create(0.3f, 1.0, 1.2f),CCScaleTo::create(0.3f, 1.0, 1.0f),nullptr)));
	}

	// 停止升级提示
	auto TextCompoundTips = (Text*)GameMainLayer::seekNodeByName(_cardCompoundLayer, "TextCompoundTips" );
	TextCompoundTips->stopAllActions();
	TextCompoundTips->setVisible( false );
}



/*******************************************
*	幸运卷轴
*******************************************/
void GameRewardLayer::showLayout( Node* node, int tag )
{
	if ( node && !node->getChildByName("GameRewardLayer") )
	{
		auto layout = GameRewardLayer::create();
		layout->setTag( tag );
		node->addChild( layout, LocalZOrde_1000, "GameRewardLayer" );
	}
}

void GameRewardLayer::onEnter()
{
	Layer::onEnter();

	initInterface();

}

void GameRewardLayer::setEventForNode( Button* node, ButtonTag tag )
{
	do 
	{
		CC_BREAK_IF( node == nullptr );

		node->setTag( tag );
		node->addClickEventListener(CC_CALLBACK_1(GameRewardLayer::callFuncButtonEvent, this));

	} while (0);
}

void GameRewardLayer::callFuncButtonEvent( Ref* ref )
{
	Button* btn = (Button*)ref;
	int nTag = btn->getTag();

	switch ( nTag )
	{

	case Tag_Close:
		//removeFromParentAndCleanup( true );
		GameInterfaceMgr::getInstance()->closeCurInterface();
		break;

	case Tag_Start:
		startMoveReward();
		break;

	case Tag_ResetTime:
		NewDataMgr::getInstance()->setScrollUpdateTime( NewDataMgr::getInstance()->convertTimeStrToNumber(CppCallJava::getInstance()->getSystemTime() ) );
		verifyScrollTime();

		createRewardGoodsNode();
		break;

	case Tag_GetReward:
		getGoodsFinish();
		hideReward();
		break;


	default:
		break;
	}
}

void GameRewardLayer::initInterface()
{
	_gameRewardLayer = CSLoader::createNode( Ccs_rewardLayerr_FilePath );

	if ( !_gameRewardLayer )
	{
		return;
	}

	addChild( _gameRewardLayer );

	do 
	{
		// 返回
		auto ButtonBack = (Button*)GameMainLayer::seekNodeByName(_gameRewardLayer, "Buttonclose" );
		setEventForNode( ButtonBack, Tag_Close );

		// 开始
		auto ButtonState = (Button*)GameMainLayer::seekNodeByName(_gameRewardLayer, "ButtonState" );
		setEventForNode( ButtonState, Tag_Start );

		// 刷新
		auto ButtonReset = (Button*)GameMainLayer::seekNodeByName(_gameRewardLayer, "ButtonReset" );
		setEventForNode( ButtonReset, Tag_ResetTime );

		// 领取
		auto ButtonGetReward = (Button*)GameMainLayer::seekNodeByName(_gameRewardLayer, "ButtonGetReward" );
		setEventForNode( ButtonGetReward, Tag_GetReward );
		
		createRewardGoodsNode();

		Size s = _rewardItem[0]->getContentSize();

		_curOptionNode  = Sprite::create( "Effect/RewardOption/jinse_1.png" );
		_rewardItem[0]->getParent()->addChild( _curOptionNode );
		_curOptionNode->setScale( 1.3f, 1.4f );
		_curOptionNode->setPosition(_rewardItem[0]->getPosition() + Vec2(0, 10));

		// 选择框序列动画
		auto animation = Animation::create(); 
		for( int i=0;i<=3;i++) 
		{ 
			char szName[100] = {0}; 
			sprintf(szName, "Effect/RewardOption/jinse_%d.png", i); 
			animation->addSpriteFrameWithFile(szName); 
		} 
		animation->setDelayPerUnit(0.5f / 3.0f); 
		animation->setRestoreOriginalFrame(true); 
		auto action = Animate::create(animation); 
		_curOptionNode->runAction( RepeatForever::create( action ) ); 
		
		// 显示刷新时间
		auto TextScrollTimeTips = (TextAtlas*)GameMainLayer::seekNodeByName(_gameRewardLayer,"TextScrollTimeTips");
		TextScrollTimeTips->setVisible( false );

		verifyScrollTime();

	} while (0);
}

void GameRewardLayer::createRewardGoodsNode( void )
{
	std::vector<int> randGoods;
	// 奖励节点
	for ( int i = 1; i<=6; i++ )
	{
		_rewardItem[i-1] = (Sprite*)GameMainLayer::seekNodeByName(_gameRewardLayer, 
			StringUtils::format("goods%d", i) );
		_rewardItem[i-1]->removeChildByName( "goods" );

		auto goodsName= (Text*)GameMainLayer::seekNodeByName(_gameRewardLayer, 
			StringUtils::format("Textgoods%d", i));

		// 随即奖励物品ID
		int randGoodsID = random((int)RewardKey_1, (int)RewardKey_6);

		while(1)
		{
			if(std::find(randGoods.begin(), randGoods.end(), randGoodsID) != randGoods.end())
			{
				randGoodsID = random((int)RewardKey_1, (int)RewardKey_6);
			}
			else
			{
				auto goodsNode = RewardGoodsNode::create( (RewardGoodsKey)randGoodsID );
				goodsNode->setPosition( _rewardItem[i-1]->getContentSize()/2 );
				_rewardItem[i-1]->addChild( goodsNode, 2, "goods" );
				randGoods.push_back( randGoodsID );

				goodsName->setString( NewDataMgr::getInstance()->getRewardGoodsListWithKey( (RewardGoodsKey)randGoodsID )->GoodsName );

				_rewardItem[i-1]->setTag( randGoodsID );
				break;
			}
		}
	}
}

void GameRewardLayer::startMoveReward( void )
{
	int goodsNum = 0;
	// 奖励节点
	for (int i = 1; i <= 6; i++)
	{
		_rewardItem[i - 1] = (Sprite*)GameMainLayer::seekNodeByName(_gameRewardLayer,
			StringUtils::format("goods%d", i));

		if (0 != _rewardItem[i - 1]->getChildByName("goods")->getTag() )
		{
			goodsNum += 1;
		}
	}
	// 先产生随机的卡
	int randRoleIndex = random(1,6);

	_rotateDaley = 0.80f;

	_MaxMoveNum = 6 * goodsNum + randRoleIndex;
	_MaxMoveNum = (1 == goodsNum) ? 1 : _MaxMoveNum;

	_moveNum = 0;
	_curSelectRole = 0;

	auto ButtonState = (Button*)GameMainLayer::seekNodeByName(_gameRewardLayer, "ButtonState" );
	auto ButtonReset = (Button*)GameMainLayer::seekNodeByName(_gameRewardLayer, "ButtonReset" );
	setButtonTouchState(ButtonState, false);
	setButtonTouchState(ButtonReset, false);

	_curOptionNode->runAction( Sequence::create( 
		DelayTime::create(_rotateDaley), 
		CallFunc::create(this,callfunc_selector(GameRewardLayer::callBackMoveSelect)), 
		nullptr));

}

void GameRewardLayer::callBackMoveSelect()
{

	if ( _moveNum >= _MaxMoveNum)
	{
		_getGoodsID = (RewardGoodsKey)_rewardItem[_curSelectRole-1]->getTag();
		showReward();

		// 抽奖结束
		//_curOptionNode->stopAllActions();
		

		return;
	}

	// 加速
	if (  _moveNum < 7 )
	{
		_rotateDaley -= 0.1f;
	}
	// 减速
	else if( ( _MaxMoveNum - _moveNum ) <= 7 )
	{
		_rotateDaley += 0.1f;
	}
	// 均速
	else
	{
		_rotateDaley = 0.1f;
	}

	_moveNum++;

	if ( _curSelectRole == 6)
	{
		_curSelectRole=0;
	}

	// 更改选择框位置
	do 
	{
		if ( 0 == _rewardItem[_curSelectRole]->getChildByName("goods")->getTag() )
		{
			_curSelectRole += 1;
			_curSelectRole = (_curSelectRole == 6)?0:_curSelectRole;

		}
		else
		{
			break;
		}
	} while (1);
	
	

	_curOptionNode->setPosition( _rewardItem[_curSelectRole]->getPosition()+ Vec2(0, 10) );
	_curSelectRole++;

	_curOptionNode->runAction( Sequence::create( DelayTime::create(_rotateDaley), CallFunc::create(this,callfunc_selector(GameRewardLayer::callBackMoveSelect)), nullptr));

}

void GameRewardLayer::setButtonTouchState( Button* button, bool state )
{
	if ( state )
	{
		button->setColor( Color3B(255,255,255) );
		button->setTouchEnabled( true );
	}
	else
	{
		button->setColor( Color3B(113,113,113) );
		button->setTouchEnabled( false );
	}
}

void GameRewardLayer::getGoodsFinish( void )
{
	auto GoodsNode = (Node*)GameMainLayer::seekNodeByName( _gameRewardLayer, "GoodsNode" );
	auto goods = (Node*)GoodsNode->getChildByName("goods");
	int goodsTag = goods->getTag();
	if ( -1 == goodsTag)
	{
		NewDataMgr::getInstance()->goodsRewardWithKey( (RewardGoodsKey)_getGoodsID );
	}
	else
	{
		NewDataMgr::getInstance()->goodsRewardWithKey( (RewardGoodsKey)_getGoodsID,goodsTag );
	}

	NotificationCenter::getInstance()->postNotification(Msg_ChangeGemGold,nullptr);


	auto ButtonReset = (Button*)GameMainLayer::seekNodeByName(_gameRewardLayer, "ButtonReset" );
	auto ButtonState = (Button*)GameMainLayer::seekNodeByName(_gameRewardLayer, "ButtonState" );
	// 重置选择框位置
	for ( int i = 0; i<6; i++ )
	{
		if ( 0 != _rewardItem[i]->getChildByName("goods")->getTag() )
		{
			_curOptionNode->setPosition( _rewardItem[i]->getPosition()+ Vec2(0, 10) );
			setButtonTouchState( ButtonReset, true );
			setButtonTouchState( ButtonState, true );
			break;
		}

		if ( 5 == i)
		{
			_curOptionNode->setVisible(false);
			setButtonTouchState( ButtonReset, false );
			setButtonTouchState( ButtonState, false );
		}

	}
}

void GameRewardLayer::showReward( void )
{
	auto nodeReward = (Node*)GameMainLayer::seekNodeByName( _gameRewardLayer, "PaneGetReward" );
	auto s = Director::getInstance()->getVisibleSize();
	nodeReward->setPosition( Vec2( s.width/2, nodeReward->getPosition().y) );

	// 光旋转
	auto RewardGuang1 = (Node*)GameMainLayer::seekNodeByName( _gameRewardLayer, "RewardGuang1" );
	auto RewardGuang2 = (Node*)GameMainLayer::seekNodeByName( _gameRewardLayer, "RewardGuang2" );

	RewardGuang1->runAction( CCRepeatForever::create(CCRotateTo::create(6.0f, 720)) );
	RewardGuang2->runAction( CCRepeatForever::create(CCRotateTo::create(6.0f, -720)) );

	// 添加一个抽取的物品节点
	auto goods = (RewardGoodsNode*)_rewardItem[_curSelectRole-1]->getChildByName("goods");
	auto GoodsNode = (Node*)GameMainLayer::seekNodeByName( _gameRewardLayer, "GoodsNode" );
	GoodsNode->removeAllChildrenWithCleanup(true);
	auto rewardGoods = (RewardGoodsNode*)goods->clone();
	rewardGoods->setTag( goods->getTag() );
	rewardGoods->setScale( 0.1f );
	rewardGoods->runAction( EaseBounceOut::create(ScaleTo::create(0.6f, 1.5f)) );
	GoodsNode->addChild( rewardGoods, 10, "goods" );

	// 渐隐物品
	goods->setVisible(false);
	goods->setTag( 0 );

}

void GameRewardLayer::hideReward( void )
{
	auto nodeReward = (Node*)GameMainLayer::seekNodeByName( _gameRewardLayer, "PaneGetReward" );
	auto s = Director::getInstance()->getVisibleSize();
	nodeReward->setPosition( Vec2( -s.width/2, nodeReward->getPosition().y) );
}

void GameRewardLayer::verifyScrollTime( void )
{
	double historyTime = NewDataMgr::getInstance()->getScrollUpdateTime();

	auto ButtonReset = (Button*)GameMainLayer::seekNodeByName(_gameRewardLayer, "ButtonReset" );
	if ( 0 == historyTime)
	{

		setButtonTouchState( ButtonReset , true );
	}
	else
	{
		double curTime = NewDataMgr::getInstance()->convertTimeStrToNumber(CppCallJava::getInstance()->getSystemTime() );
		_replyTime = curTime - historyTime;

		if ( GameScroll_Updta_Time <= _replyTime)
		{
			NewDataMgr::getInstance()->setScrollUpdateTime( 0 );
			setButtonTouchState( ButtonReset , true );
		}
		else
		{
			_replyTime = GameScroll_Updta_Time - _replyTime;
			setButtonTouchState( ButtonReset , false );
			runScrollTime();
		}
	}
}

void GameRewardLayer::runScrollTime( void )
{
	// 显示刷新时间
	auto TextScrollTimeTips = (TextAtlas*)GameMainLayer::seekNodeByName(_gameRewardLayer,"TextScrollTimeTips");
	TextScrollTimeTips->setVisible( true );

	auto ScrollUpdataTime = (TextAtlas*)GameMainLayer::seekNodeByName(_gameRewardLayer,"ScrollUpdataTime");
	ScrollUpdataTime->runAction( RepeatForever::create(CCSequence::create(  CCDelayTime::create(1.0f), CCCallFunc::create(this, callfunc_selector(GameRewardLayer::callbackUpdateTime)), nullptr )));

}

void GameRewardLayer::callbackUpdateTime()
{
	auto ScrollUpdataTime = (TextAtlas*)GameMainLayer::seekNodeByName(_gameRewardLayer,"ScrollUpdataTime");

	if ( 0 >= _replyTime )
	{
		ScrollUpdataTime->stopAllActions();
		NewDataMgr::getInstance()->setScrollUpdateTime( 0 );

		// 隐藏刷新时间
		auto TextScrollTimeTips = (TextAtlas*)GameMainLayer::seekNodeByName(_gameRewardLayer,"TextScrollTimeTips");
		TextScrollTimeTips->setVisible( false );

		verifyScrollTime();

		return;
	}

	std::string lifeTime;

	_replyTime -= 1;
	int min = _replyTime/60;
	int sec = _replyTime%60;

	std::string minStr , secStr;
	if (min/10) 
		minStr = __String::createWithFormat("%d",min )->getCString();
	else
		minStr = __String::createWithFormat("0%d",min )->getCString();

	if (sec/10) 
		secStr = __String::createWithFormat(".%d",sec )->getCString();
	else
		secStr = __String::createWithFormat(".0%d",sec )->getCString();

	minStr.append( secStr );
	ScrollUpdataTime->setString( minStr );

}

/*******************************************
*	无尽模式
*******************************************/
void WuJinModeLayer::showLayout( Node* node, int tag )
{
	if ( node && !node->getChildByName("WuJinModeLayer") )
	{
		auto layout = WuJinModeLayer::create();
		layout->setTag( tag );
		node->addChild( layout, LocalZOrde_1000, "WuJinModeLayer" );
	}
}

void WuJinModeLayer::onEnter()
{
	Layer::onEnter();

	initInterface();
}

void WuJinModeLayer::setEventForNode( Button* node, ButtonTag tag )
{
	do 
	{
		CC_BREAK_IF( node == nullptr );

		node->setTag( tag );
		node->addClickEventListener(CC_CALLBACK_1(WuJinModeLayer::callFuncButtonEvent, this));

	} while (0);
}

void WuJinModeLayer::callFuncButtonEvent( Ref* ref )
{
	Button* btn = (Button*)ref;
	int nTag = btn->getTag();


	switch ( nTag )
	{

	case Tag_Close:
		//removeFromParentAndCleanup( true );
		GameInterfaceMgr::getInstance()->closeCurInterface();
		break;

	case Tag_ResetCard:
		GameInterfaceMgr::getInstance()->closeCurInterface();
		GameInterfaceMgr::getInstance()->addInterfaceToList( GameInterfaceMgr::InterfaceTag_WujinMode, false );
		GameInterfaceMgr::getInstance()->addInterfaceToList(GameInterfaceMgr::InterfaceTag_CardOption);
		break;

	case Tag_BuyGoods:
		buyGoodsProc();
		break;

	case Tag_Start:
		NotificationCenter::getInstance()->postNotification(Msg_ConsumeLife,nullptr);
		GameSceneMgr::getInstance()->changeScene(Scene_Type_PlayILoading);
		break;

	case Tag_GoodsChongCi:
	case Tag_GoodsHuDun:
	case Tag_GoodsDaZhao:
		updateGoodsInfoWithIndex( nTag );
		break;

	case Tag_GetRewarde:
		hideTaskReward();

		NewDataMgr::getInstance()->mGameTaskSystem->getGameTaskListWithID(_getRewardTaskID)->TaskState = 2; 
		NewDataMgr::getInstance()->goodsRewardWithKey( (RewardGoodsKey)(RewardGoodsKey)NewDataMgr::getInstance()->mGameTaskSystem->getGameTaskListWithID(_getRewardTaskID)->TaskRewardID );
		NotificationCenter::getInstance()->postNotification(Msg_ChangeGemGold,nullptr);
		addTaskItemToList();
		break;

	default:
		break;
	}
}

void WuJinModeLayer::initInterface()
{
	_wuJinModeLayer = CSLoader::createNode( Ccs_WujinModeLayer_FilePath );

	if ( !_wuJinModeLayer )
	{
		return;
	}
	
	addChild( _wuJinModeLayer );

	do 
	{
		// 关闭
		auto ButtonClose = (Button*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "ButtonClose" );
		setEventForNode( ButtonClose, Tag_Close );

		// 重置卡牌
		auto ButtonCardReset = (Button*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "ButtonCardReset" );
		setEventForNode( ButtonCardReset, Tag_ResetCard );

		// 购买道具
		auto ButtonPropBuy = (Button*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "ButtonPropBuy" );
		setEventForNode( ButtonPropBuy, Tag_BuyGoods );

		//道具按钮
		auto ButtonProp1 = (Button*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "ButtonProp1" );
		setEventForNode( ButtonProp1, Tag_GoodsChongCi );

		auto ButtonProp2 = (Button*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "ButtonProp2" );
		setEventForNode( ButtonProp2, Tag_GoodsHuDun );

		auto ButtonProp3 = (Button*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "ButtonProp3" );
		setEventForNode( ButtonProp3, Tag_GoodsDaZhao );

		// 购买道具
		auto ButtonStart = (Button*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "ButtonStart" );
		setEventForNode( ButtonStart, Tag_Start );

		// 领取奖励按钮
		auto ButtonGetReward = (Button*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "ButtonGetReward" );
		ButtonGetReward->setVisible( false );
		ButtonGetReward->setTouchEnabled( false );
		setEventForNode( ButtonGetReward, Tag_GetRewarde );
		
		auto AtlasLabelProp1Num = (TextAtlas*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "AtlasLabelProp1Num" );
		auto AtlasLabelProp2Num = (TextAtlas*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "AtlasLabelProp2Num" );
		auto AtlasLabelProp3Num = (TextAtlas*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "AtlasLabelProp3Num" );

		AtlasLabelProp1Num->setString( __String::createWithFormat("%d",NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( BuyListKey::BuyKey_ChongCi )->number )->getCString() );
		AtlasLabelProp2Num->setString( __String::createWithFormat("%d",NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( BuyListKey::BuyKey_HuDun )->number )->getCString() );
		AtlasLabelProp3Num->setString( __String::createWithFormat("%d",NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( BuyListKey::BuyKey_Dazhao )->number )->getCString() );

		updateGoodsInfoWithIndex( Tag_GoodsChongCi );

		addTaskItemToList();


		// 头像处理
		for ( int i = 1; i<=3; i++ )
		{
			for( int j = 1; j<=5; j++ )
			{
				// 星星等级
				auto Lv5Star = (Node*)GameMainLayer::seekNodeByName(_wuJinModeLayer, __String::createWithFormat("LvStar%d_%d", i, j)->getCString() );

				if ( NewDataMgr::getInstance()->mRoleConfig.cardArr[i-1].cardStarLv < j )
					Lv5Star->setVisible( false );
				else
					Lv5Star->setVisible( true );
			}
			// 设置头像
			std::string cardHeadPath = __String::createWithFormat("%s%d.png", Img_CardHead_FilePath ,  NewDataMgr::getInstance()->mRoleConfig.cardArr[i-1].cardID)->getCString();
			auto ButtonCard = (Button*)GameMainLayer::seekNodeByName(_wuJinModeLayer, __String::createWithFormat("ButtonCard%d", i)->getCString());
			ButtonCard->loadTextures( cardHeadPath, cardHeadPath, cardHeadPath );

			// 等级框
			auto StarLvFream = (Sprite*)GameMainLayer::seekNodeByName(_wuJinModeLayer, __String::createWithFormat("StarLvFream_%d", i)->getCString());

			std::string tempName = __String::createWithFormat( "ccsRes/cardOptionLayer/starLv_%d.png", NewDataMgr::getInstance()->mRoleConfig.cardArr[i-1].cardStarLv)->getCString();
			StarLvFream->setTexture( tempName );
		}

		// 更改大招
		const char* filePaht = __String::createWithFormat("roleImg/dazhao%d.png", NewDataMgr::getInstance()->mRoleConfig.roleIndex )->getCString();
		auto SpritePropIco = (Sprite*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "SpritePropIco" );
		SpritePropIco->setTexture( filePaht );

	} while (0);
}

void WuJinModeLayer::updateGoodsInfoWithIndex( int index )
{
	_curOptionGoodsIndex = index;

	auto ButtonProp = (Button*)GameMainLayer::seekNodeByName(_wuJinModeLayer, 
						__String::createWithFormat("ButtonProp%d", index - Tag_BuyGoods)->getCString() );

	// 设置选择框位置
	auto PropOptionFram = (Sprite*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "PropOptionFram" );
	PropOptionFram->setPosition( ButtonProp->getPosition() + Vec2(-2, 5) );

	auto AtlasLabelPropGold = (TextAtlas*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "AtlasLabelPropGold" );

	std::string propGoldNum;

	// 道具说明
	auto TextPropTips = (Text*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "TextPropTips" );
	// 获得道具金币和说明文字
	switch ( index )
	{
	case Tag_GoodsChongCi:
		propGoldNum = __String::createWithFormat("%d",NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( BuyListKey::BuyKey_ChongCi )->Gold )->getCString();
		TextPropTips->setString( NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( BuyListKey::BuyKey_ChongCi )->GoodsDes);
		break;

	case Tag_GoodsHuDun:
		propGoldNum = __String::createWithFormat("%d",NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( BuyListKey::BuyKey_HuDun )->Gold )->getCString();
		TextPropTips->setString( NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( BuyListKey::BuyKey_HuDun )->GoodsDes);
		break;

	case Tag_GoodsDaZhao:
		propGoldNum = __String::createWithFormat("%d",NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( BuyListKey::BuyKey_Dazhao )->Gold )->getCString();
		TextPropTips->setString( NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( BuyListKey::BuyKey_Dazhao )->GoodsDes);
		break;
	default:
		break;
	}

	AtlasLabelPropGold->setString( propGoldNum );

}

void WuJinModeLayer::updateGoodsNumWithIndex( void )
{
	TextAtlas* TextAtlasGoodsNum = nullptr;
	BuyListKey key;
	switch ( _curOptionGoodsIndex )
	{
	case Tag_GoodsChongCi:
		TextAtlasGoodsNum = (TextAtlas*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "AtlasLabelProp1Num" );
		key = BuyListKey::BuyKey_ChongCi;
		break;

	case Tag_GoodsHuDun:
		TextAtlasGoodsNum = (TextAtlas*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "AtlasLabelProp2Num" );
		key = BuyListKey::BuyKey_HuDun;
		break;

	case Tag_GoodsDaZhao:
		TextAtlasGoodsNum = (TextAtlas*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "AtlasLabelProp3Num" );
		key = BuyListKey::BuyKey_Dazhao;
		break;

	default:
		break;
	}

	TextAtlasGoodsNum->setString(StringUtils::format("%d",	NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( key )->number ) );
}

void WuJinModeLayer::buyGoodsProc( void )
{
	int myGoldNum = NewDataMgr::getInstance()->getGold();
	int curGoodsPrice = 0;

	BuyListKey key;

	switch ( _curOptionGoodsIndex )
	{
	case Tag_GoodsChongCi:
		key = BuyListKey::BuyKey_ChongCi;
		break;
	case Tag_GoodsHuDun:
		key = BuyListKey::BuyKey_HuDun;
		break;
	case Tag_GoodsDaZhao:
		key = BuyListKey::BuyKey_Dazhao;
		break;
	default:
		break;
	}
	curGoodsPrice = NewDataMgr::getInstance()->mGameBuySystem->getBuyGoodsDataWithKey( key )->Gold;

	// 金币足够
	if ( NewDataMgr::getInstance()->mGameBuySystem->buyGoodsWithKey( key ) )
	{
		NotificationCenter::getInstance()->postNotification(Msg_BuySucceeEff,nullptr);
		NotificationCenter::getInstance()->postNotification(Msg_ChangeGemGold,nullptr);

		updateGoodsNumWithIndex();
	}
}

void WuJinModeLayer::addTaskItemToList( void )
{
	auto ScrollViewList = (cocos2d::ui::ScrollView*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "ScrollViewTask" );
	ScrollViewList->removeAllChildrenWithCleanup( true );


	// 获得任务表
	std::vector<int> tempTaskList = NewDataMgr::getInstance()->mGameTaskSystem->getDayTaskList();
	std::vector<int>::iterator item = tempTaskList.begin();

	int itemH = 90;
	int itemNum = tempTaskList.size();
	auto scrollSize = ScrollViewList->getInnerContainerSize();
	ScrollViewList->setInnerContainerSize( Size(scrollSize.width, itemH*itemNum) );
	scrollSize = ScrollViewList->getInnerContainerSize();

	int finishTaskNum = 0;

	for ( int i = 0; i<itemNum ; i++, item++)
	{
		auto taskItem = NewDataMgr::getInstance()->mGameTaskSystem->getGameTaskListWithID((*item));
		auto itemNode = Node::create();
		itemNode->setPosition( Vec2(scrollSize.width/2, scrollSize.height - itemH/2 - i*itemH) );
		ScrollViewList->addChild( itemNode );

		// 宝箱
		auto itemBox = Sprite::create( "roleImg/wujin2.png" );
		itemBox->setPosition( Vec2(-250, -8) );
		itemNode->addChild( itemBox );

		auto itemText = Text::create();
		itemText->setAnchorPoint( Vec2(0,0.5f) );
		itemText->setString( taskItem->TaskDes );
		itemText->setPosition( Vec2(-188,0) );
		itemText->setFontSize(25);
		itemText->setFontName( "黑体" );
		itemText->setColor( Color3B(202, 182, 152) );
		itemNode->addChild( itemText );

		auto itemLine = Sprite::create( "roleImg/wujin23.png" );
		itemLine->setPosition( Vec2(0, -45) );
		itemNode->addChild( itemLine );


		// 任务判断
		if ( 0 == taskItem->TaskState )
		{
			auto strTaskPer = __String::createWithFormat( "(%d/%d)", taskItem->TaskProgressNum, taskItem->TaskTargetNum )->getCString();
			auto itemPer = Text::create();
			itemPer->setAnchorPoint( Vec2(1,0.5f) );
			itemPer->setString( strTaskPer );
			itemPer->setPosition( Vec2(275,0) );
			itemPer->setFontSize(25);
			itemPer->setFontName( "黑体" );
			itemPer->setColor( Color3B(43, 231, 14) );
			itemNode->addChild( itemPer );

			//?
			auto imgMark = Sprite::create( "ccsRes/WujinModeLayer/wujin4.png" );
			imgMark->setPosition( itemBox->getContentSize()/2 );
			itemBox->addChild( imgMark );
		}
		else if ( 1 == taskItem->TaskState )
		{
			auto btnFilePath = "ccsRes/Button/wujin18.png";
			auto btnGet = Button::create( btnFilePath, btnFilePath, btnFilePath);
			btnGet->setPosition( Vec2(230,0) );
			btnGet->setTag( (*item) );
			btnGet->addTouchEventListener(CC_CALLBACK_2(WuJinModeLayer::calllFuncGetBtn, this));
			itemNode->addChild( btnGet );

			auto btnStrFilePath = "ccsRes/WujinModeLayer/wujin11.png";
			auto imgStrGet = Sprite::create( btnStrFilePath );
			imgStrGet->setPosition( btnGet->getContentSize()/2 );
			btnGet->addChild( imgStrGet );

			//?
			auto imgMark = Sprite::create( "ccsRes/WujinModeLayer/wujin4.png" );
			imgMark->setPosition( itemBox->getContentSize()/2 );
			itemBox->addChild( imgMark );
		}
		else if ( 2 == taskItem->TaskState )
		{
			auto imgFinish = Sprite::create( "ccsRes/WujinModeLayer/wujin13.png" );
			imgFinish->setPosition( Vec2(230, 0) );
			itemNode->addChild( imgFinish );

			finishTaskNum += 1;
		}

	}
	
	auto AtlasLabelTaskNum = (TextAtlas*)GameMainLayer::seekNodeByName( _wuJinModeLayer, "AtlasLabelTaskNum" );
	AtlasLabelTaskNum->setString( __String::createWithFormat("%d/%d", finishTaskNum, itemNum )->getCString() );
}

void WuJinModeLayer::calllFuncGetBtn( Ref *sender, Widget::TouchEventType controlEvent  )
{
	if (controlEvent == Widget::TouchEventType::ENDED) 
	{
		auto btn = (Button*)sender;

		_getRewardTaskID = btn->getTag();
		showTaskReward();
	}
}

void WuJinModeLayer::showTaskReward( void )
{
	auto nodeReward = (Node*)GameMainLayer::seekNodeByName( _wuJinModeLayer, "PaneGetReward" );

	auto s = Director::getInstance()->getVisibleSize();
	nodeReward->setPosition( Vec2( s.width/2, nodeReward->getPosition().y) );

	playTaskRewardAction();
}

void WuJinModeLayer::hideTaskReward( void )
{
	// 移除奖励节点
	auto rewardGoods = (Node*)GameMainLayer::seekNodeByName( _wuJinModeLayer, "rewardGoods" );
	if ( rewardGoods)
	{
		rewardGoods->removeFromParentAndCleanup( true );
	}

	auto nodeReward = (Node*)GameMainLayer::seekNodeByName( _wuJinModeLayer, "PaneGetReward" );
	auto s = Director::getInstance()->getVisibleSize();
	nodeReward->setPosition( Vec2( -s.width/2, nodeReward->getPosition().y) );
}

void WuJinModeLayer::playTaskRewardAction()
{
	// 光旋转
	auto RewardGuang1 = (Node*)GameMainLayer::seekNodeByName( _wuJinModeLayer, "RewardGuang1" );
	auto RewardGuang2 = (Node*)GameMainLayer::seekNodeByName( _wuJinModeLayer, "RewardGuang2" );

	RewardGuang1->runAction( CCRepeatForever::create(CCRotateTo::create(6.0f, 720)) );
	RewardGuang2->runAction( CCRepeatForever::create(CCRotateTo::create(6.0f, -720)) );

	auto rewardGoods = Sprite::create( "roleImg/wujin2.png" );
	rewardGoods->setPosition( RewardGuang2->getPosition() );
	rewardGoods->setScale( 1.5f );
	rewardGoods->setName( "rewardGoods" );
	RewardGuang2->getParent()->addChild( rewardGoods );

	rewardGoods->runAction( CCSequence::create (
		CCRotateTo::create( 0.1f, -10.0f ),
		CCRotateTo::create( 0.1f, 10.0f ),
		CCRotateTo::create( 0.1f, -10.0f ),
		CCRotateTo::create( 0.1f, 10.0f ),
		CCRotateTo::create( 0.1f, -10.0f ),
		CCRotateTo::create( 0.1f, 10.0f ),
		CCRotateTo::create( 0.1f, -10.0f ),
		CCRotateTo::create( 0.1f, 10.0f ),
		CCRotateTo::create( 0.1f, -10.0f ),
		CCRotateTo::create( 0.1f, 0.0f ),
		CCDelayTime::create( 0.2f ),
		CCCallFuncN::create(this, callfuncN_selector(WuJinModeLayer::callFuncRewardAction1)), 
		NULL
		) );
}

void WuJinModeLayer::callFuncRewardAction1( Node* node )
{
	auto rewardBox = (Sprite*)node;

	auto widgetTitle = (Sprite*)GameMainLayer::seekNodeByName( _wuJinModeLayer, "widgetTitle" );
	widgetTitle->setZOrder( 1 );

	auto rewardGoods = RewardGoodsNode::create( (RewardGoodsKey)NewDataMgr::getInstance()->mGameTaskSystem->getGameTaskListWithID(_getRewardTaskID)->TaskRewardID );
	rewardGoods->setScale( 0.2f );
	rewardGoods->setPosition( rewardBox->getContentSize()/2 );
	rewardBox->addChild( rewardGoods );

	rewardGoods->runAction( CCSequence::create(
		EaseBounceOut::create( CCScaleTo::create(0.5f, 1.3f) ),
		CCCallFuncN::create(this, callfuncN_selector(WuJinModeLayer::callFuncRewardAction2)), 
		nullptr));
}

void WuJinModeLayer::callFuncRewardAction2( Node* node )
{
	// 领取奖励按钮
	auto ButtonGetReward = (Button*)GameMainLayer::seekNodeByName(_wuJinModeLayer, "ButtonGetReward" );
	ButtonGetReward->setVisible( true );
	ButtonGetReward->setTouchEnabled( true );
}


/*******************************************
*	灵石
*******************************************/
void QuickStoneLayer::showLayout(Node* node, int tag)
{
	if (node && !node->getChildByName("QuickStoneLayer"))
	{
		auto layout = QuickStoneLayer::create();
		layout->setTag(tag);
		node->addChild(layout, LocalZOrde_500, "QuickStoneLayer");
	}
}

void QuickStoneLayer::onEnter()
{
	Layer::onEnter();

	initInterface();
}

void QuickStoneLayer::setEventForNode(Button* node, ButtonTag tag)
{
	do
	{
		CC_BREAK_IF(node == nullptr);

		node->setTag(tag);
		node->addClickEventListener(CC_CALLBACK_1(QuickStoneLayer::callFuncButtonEvent, this));

	} while (0);
}

void QuickStoneLayer::callFuncButtonEvent(Ref* ref)
{
	Button* btn = (Button*)ref;
	ButtonTag nTag = (ButtonTag)btn->getTag();

	switch (nTag)
	{
	case QuickStoneLayer::Tag_Back:
		GameInterfaceMgr::getInstance()->closeCurInterface();
		break;

	case QuickStoneLayer::Tag_ImplantLabel:
	case QuickStoneLayer::Tag_CompoundLabel:
		optionLabel(nTag);
		break;

	case QuickStoneLayer::Tag_CloseOptionCard:
		_cardOptionNode->removeFromParentAndCleanup(true);
		break;
	case QuickStoneLayer::Tag_CloseOptionQuickston:
		_quickStonOptionNode->removeFromParentAndCleanup(true);
		break;


	case QuickStoneLayer::Tag_OptionCard:
		showOptionCardWidget();
		break;
	case QuickStoneLayer::Tag_OptionQuickston:
		showOptionQuickStonWidget();
		break;

	case QuickStoneLayer::Tag_StartImplant:
		{
			int implantPrice = NewDataMgr::getInstance()->mQuickStoneSystem->getQuickStoneDataWithKey((QuickStoneKey)_curOptionQuickStonId)->implantPrice;
			if (NewDataMgr::getInstance()->mGameBuySystem->buyGoodsWithPrice(implantPrice))
			{
				setButtonTouchState(btn, false);
				playImplantQuickStone();
			}
		}
		break;
	case QuickStoneLayer::Tag_StartCompount:
		{
			int compoundPrice = NewDataMgr::getInstance()->mQuickStoneSystem->getQuickStoneDataWithKey((QuickStoneKey)(_curOptionCompountQuickSId + 1))->compoundPrice;
			if (NewDataMgr::getInstance()->mGameBuySystem->buyGoodsWithPrice(compoundPrice))
			{
				playCompountQuickStone();
			}
		}
		break;

	default:
		break;
	}
	
}

void QuickStoneLayer::initInterface()
{
	_myLayer = CSLoader::createNode(Ccs_QuickStoneLayer_FilePath);

	if (!_myLayer)
	{
		return;
	}

	addChild(_myLayer);

	do
	{
		// 返回
		auto ButtonBack = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonBack");
		setEventForNode(ButtonBack, Tag_Back);

		// 镶嵌标签
		auto ButtonImplant = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonImplant");
		setEventForNode(ButtonImplant, Tag_ImplantLabel);
		// 镶嵌
		auto ButtonCardImplant = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonCardImplant");
		setEventForNode(ButtonCardImplant, Tag_StartImplant);

		// 合成标签
		auto ButtonCompound = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonCompound");
		setEventForNode(ButtonCompound, Tag_CompoundLabel);
		// 合成
		auto ButtonQuickStCompound = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonQuickStCompound");
		setEventForNode(ButtonQuickStCompound, Tag_StartCompount);

		// 选择灵石
		auto ButtonOptionQuick = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonOptionQuick");
		setEventForNode(ButtonOptionQuick, Tag_OptionQuickston);	
		// 选择卡牌
		auto ButtonOptionCard = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonOptionCard");
		setEventForNode(ButtonOptionCard, Tag_OptionCard);

		// 已选择的卡牌和灵石
		auto ButtonCard = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonCard");
		ButtonCard->setVisible(false);
		auto ButtonQuickSton = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonQuickSton");
		ButtonQuickSton->setVisible(false);

		setButtonTouchState(ButtonCardImplant, false);
		setButtonTouchState(ButtonQuickStCompound, false);

		initImplantInterface();

		//合成提示
		auto PanelCompountTips = (Node*)GameMainLayer::seekNodeByName(_myLayer, "PanelCompountTips");
		PanelCompountTips->setVisible(true);

		auto TextCompountTips = (Text*)GameMainLayer::seekNodeByName(_myLayer, "TextCompountTips");
		TextCompountTips->runAction(CCRepeatForever::create(
			CCSequence::create(
			CCFadeOut::create(0.5f),
			CCFadeIn::create(0.5f),
			DelayTime::create(1.5f),
			nullptr)));

	} while (0);
}

void QuickStoneLayer::initImplantInterface(void)
{
	auto AtlasLabelGoodsAtk = (Text*)GameMainLayer::seekNodeByName(_myLayer, "AtlasLabelGoodsAtk");
	auto AtlasLabelImplantPrice = (TextAtlas*)GameMainLayer::seekNodeByName(_myLayer, "AtlasLabelImplantPrice");

	if (!_curOptionQuickStonId)
	{
		AtlasLabelImplantPrice->setString("");
		AtlasLabelGoodsAtk->setString("");
	}
	else
	{
		AtlasLabelImplantPrice->setString(StringUtils::format("%d", NewDataMgr::getInstance()->mQuickStoneSystem->getQuickStoneDataWithKey((QuickStoneKey)_curOptionQuickStonId)->implantPrice));
		AtlasLabelGoodsAtk->setString(StringUtils::format("+%d%s", NewDataMgr::getInstance()->mQuickStoneSystem->getQuickStoneDataWithKey((QuickStoneKey)_curOptionQuickStonId)->atk, "%"));
	}
}

void QuickStoneLayer::optionLabel(int tag)
{
	auto winSize = Director::getInstance()->getVisibleSize();
	auto LabelOption = (Sprite*)GameMainLayer::seekNodeByName(_myLayer, "LabelOption");
	auto ButtonImplant = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonImplant");
	auto ButtonCompound = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonCompound");

	auto PanelImplant = (Node*)GameMainLayer::seekNodeByName(_myLayer, "PanelImplant");
	auto PanelCompound = (Node*)GameMainLayer::seekNodeByName(_myLayer, "PanelCompound");

	LabelOption->setScale(1.15f);
	auto action = ScaleTo::create(0.3f, 1.0f);
	LabelOption->runAction(action);
	switch ( tag )
	{
	case Tag_CompoundLabel:
		updateQuickStoneCompUI();
		LabelOption->setPosition( ButtonCompound->getPosition() );
		PanelCompound->setPosition(Vec2(0, 0));
		PanelImplant->setPosition(Vec2(winSize.width, 0));
		break;

	case Tag_ImplantLabel:
		initImplantInterface();
		LabelOption->setPosition(ButtonImplant->getPosition());
		PanelImplant->setPosition(Vec2(0, 0));
		PanelCompound->setPosition(Vec2(winSize.width, 0));
		break;
	default:
		break;
	}

}

void QuickStoneLayer::showOptionCardWidget()
{
	_cardOptionNode = CSLoader::createNode(Ccs_QuickStonCardOption_FilePath);

	if (!_cardOptionNode)
	{
		return;
	}

	this->getParent()->addChild(_cardOptionNode, LocalZOrde_1000);

	do
	{
		// 关闭
		auto ButtonClose = (Button*)GameMainLayer::seekNodeByName(_cardOptionNode, "ButtonClose");
		setEventForNode(ButtonClose, Tag_CloseOptionCard);

		addCardToList();

	} while (0);
}

void QuickStoneLayer::showOptionQuickStonWidget()
{
	_quickStonOptionNode = CSLoader::createNode(Ccs_QuickStonOption_FilePath);

	if (!_quickStonOptionNode)
	{
		return;
	}

	this->getParent()->addChild(_quickStonOptionNode, LocalZOrde_1000);

	do
	{
		// 关闭
		auto ButtonClose = (Button*)GameMainLayer::seekNodeByName(_quickStonOptionNode, "ButtonClose");
		setEventForNode(ButtonClose, Tag_CloseOptionQuickston);

		for (int i = 1; i <= 8; i++)
		{
			auto quickStoneItem = NewDataMgr::getInstance()->mQuickStoneSystem->getQuickStoneDataWithKey((QuickStoneKey)i);
			auto ButtonGoods = (Button*)GameMainLayer::seekNodeByName(_quickStonOptionNode, StringUtils::format("ButtonGoods%d", i));
			ButtonGoods->setTag(i);

			if (quickStoneItem->haveNum)
			{
				ButtonGoods->addClickEventListener(CC_CALLBACK_1(QuickStoneLayer::callFuncQuickStonOptionEvent, this));
				ButtonGoods->setColor(Color3B(255, 255, 255));
			}
			else
			{
				ButtonGoods->setColor(Color3B(113, 113, 113));
			}

			auto TextGoodsAtk = (Text*)GameMainLayer::seekNodeByName(_quickStonOptionNode, StringUtils::format("TextGoodsAtk_%d", i));
			TextGoodsAtk->setString(StringUtils::format("%d", quickStoneItem->atk));

			auto GoodsNum = (TextAtlas *)GameMainLayer::seekNodeByName(_quickStonOptionNode, StringUtils::format("GoodsNum_%d", i));
			GoodsNum->setString(StringUtils::format("%d", quickStoneItem->haveNum));
		}

	} while (0);
}

void QuickStoneLayer::addCardToList(void)
{
	auto ScrollViewCardOption = (cocos2d::ui::ScrollView*)GameMainLayer::seekNodeByName(_cardOptionNode, "ScrollViewCardOption");
	ScrollViewCardOption->removeAllChildrenWithCleanup(true);

	Vec2 firstPoint(65, 345);
	Vec2 offsetPoint(155, 124);
	Vec2 valPoint(0, 0);

	for (int i = 0, j = 0; i < GameData_CardNum; i++)
	{
		int maxStarLv = NewDataMgr::getInstance()->getCardMaxStarLvWithId(i);
		if (maxStarLv)
		{
			valPoint = Vec2(offsetPoint.x*(j % 4), (j / 4)*offsetPoint.y); j++;

			// 列表按钮
			std::string buttonFilePath = __String::createWithFormat("%s%d.png", Img_CardHead_FilePath, i + 1)->getCString();
			auto buttonItem = Button::create(buttonFilePath, buttonFilePath, buttonFilePath);
			buttonItem->setPosition(Vec2(firstPoint.x + valPoint.x, firstPoint.y - valPoint.y));
			buttonItem->setScale(0.95);
			buttonItem->setTag(i);
			buttonItem->setCascadeColorEnabled(true);
			buttonItem->addClickEventListener(CC_CALLBACK_1(QuickStoneLayer::callFuncCardOptionEvent, this));
			ScrollViewCardOption->addChild(buttonItem);

			// 按钮框
			auto buttonFram = Sprite::create(__String::createWithFormat("%s%d.png", Img_CardHeadK_FilePath, maxStarLv)->getCString());
			buttonFram->setScale(1.05f);
			buttonFram->setPosition(Vec2(49, 44));
			buttonItem->addChild(buttonFram);

			// 类型
			auto cardType = Sprite::create(__String::createWithFormat("%s%d.png", Img_CardType_FilePath, (i / 3) + 1)->getCString());
			cardType->setScale(0.7f);
			cardType->setPosition(Vec2(5, 64));
			buttonItem->addChild(cardType);

			// 灵石框
			auto quickStoneK = Sprite::create(StringUtils::format("%s%d.png", Img_QuickStonbg_FinePath, (i / 3) + 1));
			quickStoneK->setScale(0.7f);
			quickStoneK->setPosition(Vec2(85, 16));
			buttonItem->addChild(quickStoneK);

			std::string filePath = StringUtils::format("%s%d.png", Img_QuickStonLittle_FinePath, NewDataMgr::getInstance()->getCardConfigWithId(i)->quickStoneId);
			auto quickStone = Sprite::create(filePath);
			if (quickStone)
			{
				quickStone->setScale(0.7f);
				quickStone->setPosition(Vec2(86, 13));
				buttonItem->addChild(quickStone);
			}
		}
	}
}

void QuickStoneLayer::callFuncCardOptionEvent(Ref* ref)
{
	auto node = (Node*)ref;
	int nTag = node->getTag();

	updateCardWithCardId(nTag);
	_cardOptionNode->removeFromParentAndCleanup(true);
}

void QuickStoneLayer::callFuncQuickStonOptionEvent(Ref* ref)
{
	auto node = (Node*)ref;
	int nTag = node->getTag();

	updateQuickStonWithId(nTag);
	initImplantInterface();
	_quickStonOptionNode->removeFromParentAndCleanup(true);

}

void QuickStoneLayer::callOptionCompountQuickst(Ref* ref)
{
	auto node = (Node*)ref;
	int nTag = node->getTag();

	_curOptionCompountQuickSId = nTag;


	auto QuickOptionK = (Sprite*)GameMainLayer::seekNodeByName(_myLayer, "QuickOptionK");
	QuickOptionK->setPosition(node->getPosition() + Vec2(-1, 5));
	QuickOptionK->setVisible(true);

	auto QuickStLeft = (Sprite*)GameMainLayer::seekNodeByName(_myLayer, "QuickStLeft");
	auto QuickStRight = (Sprite*)GameMainLayer::seekNodeByName(_myLayer, "QuickStRight");
	QuickStLeft->setVisible(false);
	QuickStRight->setVisible(false);

	auto ButtonCompound = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonQuickStCompound");
	auto TextNeedNum = (Text*)GameMainLayer::seekNodeByName(_myLayer, "TextNeedNum");
	auto PanelCompountTips = (Node*)GameMainLayer::seekNodeByName(_myLayer, "PanelCompountTips");

	// 灵石是否SS级
	if (nTag >= QuickStoneKey_SS)
	{
		PanelCompountTips->setVisible(true);

		auto TextCompountTips = (Text*)GameMainLayer::seekNodeByName(_myLayer, "TextCompountTips");
		TextCompountTips->stopAllActions();
		TextCompountTips->setOpacity(255);
		TextCompountTips->setString(StringUtils::format("SS级已是最高等级%s", ""));
		return;
	}
	else
	{
		PanelCompountTips->setVisible(false);
	}

	// 判断是否可以合成
	int needNum = NewDataMgr::getInstance()->mQuickStoneSystem->getQuickStoneDataWithKey((QuickStoneKey)(nTag + 1))->compoundNeedNum;
	int haveNum = NewDataMgr::getInstance()->mQuickStoneSystem->getQuickStoneDataWithKey((QuickStoneKey)(nTag))->haveNum;

	//if (haveNum >= needNum && nTag < QuickStoneKey_Max - 1)
	{
		// 灵石是否足够
		if (haveNum >= needNum )
		{ 
			setButtonTouchState(ButtonCompound, true);

			TextNeedNum->setColor(Color3B(255, 255, 0));
			TextNeedNum->setString(StringUtils::format("%d", needNum));
		}
		else
		{
			setButtonTouchState(ButtonCompound, false);

			TextNeedNum->setColor(Color3B(255,0,0));
			TextNeedNum->setString(StringUtils::format("%d(灵石不足)", needNum));
		}

		
		std::string LeftfilePath = StringUtils::format("%s%d.png", Img_QuickSton_FinePath, nTag);
		QuickStLeft->setTexture(LeftfilePath);
		QuickStLeft->setVisible(true);
		QuickStLeft->setOpacity(0);
		QuickStLeft->runAction( CCFadeIn::create(0.1));

		std::string RightfilePath = StringUtils::format("%s%d.png", Img_QuickSton_FinePath, nTag + 1);
		QuickStRight->setTexture(RightfilePath);
		QuickStRight->setVisible(true);
		QuickStRight->setOpacity(0);
		QuickStRight->runAction(CCFadeIn::create(0.1));

		// 合成价格
		auto CompoundPrice = (TextAtlas *)GameMainLayer::seekNodeByName(_myLayer, "CompoundPrice");
		CompoundPrice->setString(StringUtils::format("%d", NewDataMgr::getInstance()->mQuickStoneSystem->getQuickStoneDataWithKey((QuickStoneKey)nTag)->compoundPrice));

		// 合成前后攻击
		auto TextGoodsAtkLeft = (Text *)GameMainLayer::seekNodeByName(_myLayer, "TextAtk1");
		TextGoodsAtkLeft->setString(StringUtils::format("%d%s", NewDataMgr::getInstance()->mQuickStoneSystem->getQuickStoneDataWithKey((QuickStoneKey)nTag)->atk, "%"));
		auto TextGoodsAtkRight = (Text *)GameMainLayer::seekNodeByName(_myLayer, "TextAtk2");
		TextGoodsAtkRight->setString(StringUtils::format("%d%s", NewDataMgr::getInstance()->mQuickStoneSystem->getQuickStoneDataWithKey((QuickStoneKey)(nTag + 1))->atk, "%"));
	}

}

void QuickStoneLayer::updateCardWithCardId(int id)
{
	_curOptionCardId = id +1;

	// 更换卡牌
	std::string filePath = StringUtils::format("%s%d.png", Img_Card_FilePath, id + 1);
	auto ButtonCard = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonCard");
	ButtonCard->loadTextures(filePath, filePath, filePath);
	ButtonCard->setVisible(true);

	// 更换卡牌外框
	filePath = StringUtils::format("%s%d.png", Img_CardTypeK_FinePath, (id / 3) + 1);
	auto cardFream = (Sprite*)GameMainLayer::seekNodeByName(_myLayer, "cardFream");
	cardFream->setTexture( filePath );
	cardFream->setVisible(true);

	// 更换卡牌类型
	filePath = StringUtils::format("%s%d.png", Img_CardType_FilePath, (id / 3) + 1);
	auto CardType = (Sprite*)GameMainLayer::seekNodeByName(_myLayer, "CardType");
	CardType->setTexture(filePath);
	CardType->setVisible(true);

	// 更换灵石背景
	filePath = StringUtils::format("%s%d.png", Img_QuickStonbg_FinePath, (id / 3) + 1);
	auto CardQuickStoneBG = (Sprite*)GameMainLayer::seekNodeByName(_myLayer, "CardQuickStoneBG");
	CardQuickStoneBG->setTexture(filePath);

	// 更换灵石
	auto CardQuickStoneID = (Sprite*)GameMainLayer::seekNodeByName(_myLayer, "CardQuickStoneID");
	int quickStoneId = NewDataMgr::getInstance()->getCardConfigWithId(id)->quickStoneId;
	if (quickStoneId)
	{
		CardQuickStoneID->setVisible(true);
		CardQuickStoneID->setTexture(StringUtils::format("%s%d.png", Img_QuickStonLittle_FinePath, quickStoneId));
		CardQuickStoneID->setScale(1.0f);
	}
	else
	{
		CardQuickStoneID->setVisible(false);
	}

	//
	auto ButtonCardImplant = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonCardImplant");
	auto ButtonQuickStCompound = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonQuickStCompound");
	if ( _curOptionQuickStonId && _curOptionCardId )
	{
		setButtonTouchState(ButtonCardImplant, true);
		setButtonTouchState(ButtonQuickStCompound, true);
	}

}

void QuickStoneLayer::updateQuickStonWithId(int id)
{
	_curOptionQuickStonId = id;

	std::string filePath = StringUtils::format("%s%d.png", Img_QuickSton_FinePath, id);
	auto QuickSton = (Sprite*)GameMainLayer::seekNodeByName(_myLayer, "QuickSton");
	QuickSton->setTexture(filePath);
	QuickSton->setScale(0.8f);
	QuickSton->setOpacity(255);

	auto ButtonQuickSton = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonQuickSton");
	ButtonQuickSton->setVisible(true);

	auto ButtonCardImplant = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonCardImplant");
	auto ButtonQuickStCompound = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonQuickStCompound");
	if (_curOptionQuickStonId && _curOptionCardId)
	{
		setButtonTouchState(ButtonCardImplant, true);
		setButtonTouchState(ButtonQuickStCompound, true);
	}
}

void QuickStoneLayer::updateQuickStoneCompUI(void)
{
	for (int i = 1; i <= 8; i++)
	{
		auto ButtonGoods = (Button*)GameMainLayer::seekNodeByName(_myLayer, StringUtils::format("ButtonGoods%d", i));
		ButtonGoods->addClickEventListener(CC_CALLBACK_1(QuickStoneLayer::callOptionCompountQuickst, this));
		ButtonGoods->setTag(i);

		auto TextGoodsAtk = (Text*)GameMainLayer::seekNodeByName(_myLayer, StringUtils::format("TextGoodsAtk_%d", i));
		TextGoodsAtk->setString(StringUtils::format("%d%s", NewDataMgr::getInstance()->mQuickStoneSystem->getQuickStoneDataWithKey((QuickStoneKey)i)->atk, "%"));

		auto GoodsNum = (TextAtlas *)GameMainLayer::seekNodeByName(_myLayer, StringUtils::format("GoodsNum_%d", i));
		GoodsNum->setString(StringUtils::format("%d%s", NewDataMgr::getInstance()->mQuickStoneSystem->getQuickStoneDataWithKey((QuickStoneKey)i)->haveNum, "%"));

		auto QuickStone = (Sprite*)GameMainLayer::seekNodeByName(_myLayer, StringUtils::format("QuickStone%d", i));
		if (!NewDataMgr::getInstance()->mQuickStoneSystem->getQuickStoneDataWithKey((QuickStoneKey)i)->haveNum)
		{
			QuickStone->setColor(Color3B(113, 113, 113));
		}
		else
		{
			QuickStone->setColor(Color3B(255, 255, 255));
		}
	}

	auto QuickOptionK = (Sprite*)GameMainLayer::seekNodeByName(_myLayer, "QuickOptionK");
	QuickOptionK->setVisible(false);
	//QuickOptionK->runAction(RepeatForever::create(CCRotateTo::create(1.0f, -720)));

	auto ButtonCompound = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonQuickStCompound");
	setButtonTouchState(ButtonCompound, false);

	auto QuickStLeft = (Sprite*)GameMainLayer::seekNodeByName(_myLayer, "QuickStLeft");
	auto QuickStRight = (Sprite*)GameMainLayer::seekNodeByName(_myLayer, "QuickStRight");
	QuickStLeft->setOpacity(0);
	QuickStRight->setOpacity(0);

	// 合成前后攻击
	auto TextGoodsAtkLeft = (Text *)GameMainLayer::seekNodeByName(_myLayer, "TextAtk1");
	TextGoodsAtkLeft->setString(" ");
	auto TextGoodsAtkRight = (Text *)GameMainLayer::seekNodeByName(_myLayer, "TextAtk2");
	TextGoodsAtkRight->setString(" ");

	// 合成价格
	auto CompoundPrice = (TextAtlas *)GameMainLayer::seekNodeByName(_myLayer, "CompoundPrice");
	CompoundPrice->setString("0");

	// 灵石消耗
	auto TextNeedNum = (Text*)GameMainLayer::seekNodeByName(_myLayer, "TextNeedNum");
	TextNeedNum->setString("");
}

void QuickStoneLayer::setButtonTouchState(Button* button, bool state)
{
	if (state)
	{
		button->setColor(Color3B(255, 255, 255));
		button->setTouchEnabled(true);
	}
	else
	{
		button->setColor(Color3B(113, 113, 113));
		button->setTouchEnabled(false);
	}
}

void QuickStoneLayer::playCompountQuickStone(void)
{
	_eventDispatcher->setEnabled(false);
	auto PanelCompound = (Node*)GameMainLayer::seekNodeByName(_myLayer, "PanelCompountBg");

	auto ButtonCompound = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonQuickStCompound");
	setButtonTouchState(ButtonCompound, false);

	auto node1 = Node::create();
	PanelCompound->addChild(node1);

	auto node2 = Node::create();
	PanelCompound->addChild(node2);

	// 粒子
	auto parcile1 = ParticleSystemQuad::create("particla/QuiteStoneCompount/test1.plist");
	node1->addChild(parcile1, -1);
	auto parcile2 = ParticleSystemQuad::create("particla/QuiteStoneCompount/test1.plist");
	node2->addChild(parcile2, -1);

	CCPointArray * array1 = CCPointArray::create(20);
	array1->addControlPoint(ccp(120, 340-60));
	array1->addControlPoint(ccp(120, 400-60));
	array1->addControlPoint(ccp(250, 400-60));
	array1->addControlPoint(ccp(250, 360-60));
	array1->addControlPoint(ccp(360, 360-60));
	array1->addControlPoint(ccp(360, 400-60));
	array1->addControlPoint(ccp(490, 400-60));
	array1->addControlPoint(ccp(490, 340-60));

	CCActionInterval  * CardinalSplineTo = CCCardinalSplineTo::create(1, array1, 0);

	node1->setPosition(ccp(120 + 20, 340 - 60));
	node1->runAction( CCSequence::create(
		CCMoveTo::create(0.1f, ccp(120 + 20, 340 - 60))
		, CCMoveTo::create(0.1f, ccp(120 + 20, 400 - 70))
		, CCMoveTo::create(0.1f, ccp(250 - 10, 400 - 70))
		, CCMoveTo::create(0.1f, ccp(250 - 10, 360 - 80))
		, CCMoveTo::create(0.1f, ccp(360 + 20, 360 - 80))
		, CCMoveTo::create(0.1f, ccp(360 + 10, 400 - 70))
		, CCMoveTo::create(0.1f, ccp(490 - 10, 400 - 70))
		, CCMoveTo::create(0.1f, ccp(490 - 10, 340 - 70))
		, CCDelayTime::create( 0.5f )
		//, CCCallFuncN::create(this, callfuncN_selector(QuickStoneLayer::callFuncplayCompount))
		,nullptr));

	node2->setPosition(ccp(120 + 20, 340 - 60));
	node2->runAction(CCSequence::create(
		CCMoveTo::create(0.1f, ccp(120 + 20, 340 - 60))
		, CCMoveTo::create(0.1f, ccp(120 + 20, 400 - 60 - 120))
		, CCMoveTo::create(0.1f, ccp(250 - 10, 400 - 60 - 120))
		, CCMoveTo::create(0.1f, ccp(250 - 10, 360 - 50 - 50))
		, CCMoveTo::create(0.1f, ccp(360 + 20, 360 - 50 - 50))
		, CCMoveTo::create(0.1f, ccp(360 + 20, 400 - 60 - 120))
		, CCMoveTo::create(0.1f, ccp(490 - 10, 400 - 60 - 120))
		, CCMoveTo::create(0.1f, ccp(490 - 10, 340 - 50))
		, CCDelayTime::create(0.5f)
		, CCCallFuncN::create(this, callfuncN_selector(QuickStoneLayer::callFuncplayCompount))
		, CCDelayTime::create(0.5f)
		, CCCallFuncN::create(this, callfuncN_selector(QuickStoneLayer::callFuncplayCompount2))
		, nullptr));
}

void QuickStoneLayer::callFuncplayCompount(Node* node)
{
	//node->removeFromParentAndCleanup(true);
	NewDataMgr::getInstance()->mQuickStoneSystem->compountQuickStone((QuickStoneKey)(_curOptionCompountQuickSId + 1));
}

void QuickStoneLayer::callFuncplayCompount2(Node* node)
{
	updateQuickStoneCompUI();

	// 翻转
	auto action = Sequence::create(
		CCOrbitCamera::create(10, 1, 0, 0, 90, 0, 0)
		, CCCallFuncN::create(this, callfuncN_selector(QuickStoneLayer::callFuncReversal))
		, CCOrbitCamera::create(10, 1, 0, -90, -90, 0, 0)
		, nullptr);

	auto ButtonGoods = (Button*)GameMainLayer::seekNodeByName(_myLayer, StringUtils::format("ButtonGoods%d", _curOptionCompountQuickSId ));
	auto ButtonGoods2 = (Button*)GameMainLayer::seekNodeByName(_myLayer, StringUtils::format("ButtonGoods%d", _curOptionCompountQuickSId + 1));
	ButtonGoods->runAction(Sequence::create(
		CCOrbitCamera::create(0.2, 1, 0, 0, 90, 0, 0)
		, CCCallFuncN::create(this, callfuncN_selector(QuickStoneLayer::callFuncReversal))
		, CCOrbitCamera::create(0.2, 1, 0, -90, -90, 0, 0)
		, nullptr));
	ButtonGoods2->runAction(Sequence::create(
		CCOrbitCamera::create(0.2, 1, 0, 0, 90, 0, 0)
		, CCCallFuncN::create(this, callfuncN_selector(QuickStoneLayer::callFuncReversal))
		, CCOrbitCamera::create(0.2, 1, 0, -90, -90, 0, 0)
		, nullptr));

	_eventDispatcher->setEnabled(true);
}

void QuickStoneLayer::playImplantQuickStone(void)
{
	_eventDispatcher->setEnabled(false);

	auto PanelImplant = (Node*)GameMainLayer::seekNodeByName(_myLayer, "PanelImplant");
	auto ButtonCard = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonCard");

	// 灵石放大
	auto CardQuickStoneID = (Sprite*)GameMainLayer::seekNodeByName(_myLayer, "CardQuickStoneID");
	CardQuickStoneID->setVisible(true);
	CardQuickStoneID->setTexture(StringUtils::format("%s%d.png", Img_QuickStonLittle_FinePath, _curOptionQuickStonId));
	CardQuickStoneID->setScale(0.1f);
	CardQuickStoneID->setOpacity(255);
	CardQuickStoneID->runAction(CCScaleTo::create(3.0f, 1.0f));

	// 粒子
	auto node1 = Node::create();
	node1->setPosition(CardQuickStoneID->getPosition() + Vec2(0, -65) );
	ButtonCard->addChild(node1);
	auto parcile1 = ParticleSystemQuad::create("particla/QuiteStoneCompount/test2.plist");
	parcile1->setScale(1.5f);
	node1->addChild(parcile1, -1);


	// 将灵石放大消失
	auto QuickSton = (Sprite*)GameMainLayer::seekNodeByName(_myLayer, "QuickSton");
	QuickSton->runAction(CCSpawn::create(ScaleTo::create(0.2, 3), CCFadeOut::create(0.2f), nullptr));

	ButtonCard->runAction(CCSequence::create(
		CCDelayTime::create(0.5f)
		, CCCallFuncN::create(this, callfuncN_selector(QuickStoneLayer::callFuncplayImplant2))
		, CCDelayTime::create(4.0f)
		, CCCallFuncN::create(this, callfuncN_selector(QuickStoneLayer::callFuncplayImplant1))
		, nullptr));
}

void QuickStoneLayer::callFuncplayImplant1(Node* node)
{
	auto ButtonQuickSton = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonQuickSton");
	ButtonQuickSton->setVisible(false);

	auto ButtonCardImplant = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonCardImplant");
	setButtonTouchState(ButtonCardImplant, false);

	NewDataMgr::getInstance()->mQuickStoneSystem->implantQuickStone((_curOptionCardId / 3) + 1, (QuickStoneKey)_curOptionQuickStonId);
	_curOptionQuickStonId = 0;

	_eventDispatcher->setEnabled(true);
}

void QuickStoneLayer::callFuncplayImplant2(Node* node)
{

	auto ButtonCard = (Button*)GameMainLayer::seekNodeByName(_myLayer, "ButtonCard");
	auto CardQuickStoneID = (Sprite*)GameMainLayer::seekNodeByName(_myLayer, "CardQuickStoneID");
	// 粒子
	auto node1 = Node::create();
	node1->setPosition(CardQuickStoneID->getPosition() + Vec2(0, -65));
	ButtonCard->addChild(node1);
	auto parcile1 = ParticleSystemQuad::create("particla/QuiteStoneCompount/test2.plist");
	parcile1->setScale(1.5f);
	node1->addChild(parcile1, -1);
}

void QuickStoneLayer::callFuncReversal(Node* node)
{
	node->setScaleX(-1);
}
