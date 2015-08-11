#include "GameLevelScene.h"
#include "GameSceneMgr.h"
#include "../GameCommon/NewAudio.h"
#include "../GameCommon/ActionRecord.h"
#include "GameMainLayer.h"



Scene* GameLevelScene::createScene()
{
	auto scene = Scene::create();

	auto layer = GameLevelScene::create();
	scene->addChild(layer);

	return scene;
}

void GameLevelScene::onEnter()
{
	Layer::onEnter();

	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(GameLevelScene::onKeyReleased,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	initInterface();

	NewAudio::getInstance()->playBgMusicForce(Music_Bg);

}

bool GameLevelScene::onTouchBegan( Touch *touch, Event *unusedEvent )
{
	bool ret = true;

	return ret;
}

void GameLevelScene::onTouchMoved( Touch *touch, Event *unusedEvent )
{

}

void GameLevelScene::onTouchEnded( Touch *touch, Event *unusedEvent )
{

}

void GameLevelScene::initInterface( void )
{
	_levelRootNode = CSLoader::createNode("ccsres/GameLevelScene.csb");

	if ( !_levelRootNode )
	{
		return;
	}
	addChild( _levelRootNode );


	do 
	{
		// ·µ»Ø°´Å¥
		Button* buttonPlay = (Button*)GameMainLayer::seekNodeByName(_levelRootNode, "ButtonReturn" );
		CC_BREAK_IF( buttonPlay == nullptr );

		buttonPlay->setTag( Tag_Return );
		buttonPlay->addClickEventListener(CC_CALLBACK_1(GameLevelScene::callFuncButtonEvent, this));

		for (int i = 1; i<=10; i++)
		{
			auto buttonLevel = (Button*)GameMainLayer::seekNodeByName(_levelRootNode, __String::createWithFormat("ButtonLevel%d", i)->getCString() );
			CC_BREAK_IF( buttonLevel == nullptr );
			buttonLevel->setVisible( false );
			buttonLevel->setTouchEnabled( false );
			if( i<= 10 )
			{
				buttonLevel->setVisible( true );
				buttonLevel->setTouchEnabled( true );
				buttonLevel->setTag( i );
				buttonLevel->addClickEventListener(CC_CALLBACK_1(GameLevelScene::callFuncButtonEvent, this));

				Vec2 pos = buttonLevel->getPosition();
				buttonLevel->runAction(RepeatForever::create(Sequence::create(
					MoveTo::create(0.5f, Vec2(pos.x, pos.y + 10) ),
					MoveTo::create(0.5f, Vec2(pos.x, pos.y) ),
					nullptr)));
			}
		}

	} while ( 0 );
}

void GameLevelScene::callFuncButtonEvent( Ref* ref )
{
	static bool isShowLoading = true;
	Button* btn = (Button*)ref;
	int nTag = btn->getTag();


	switch ( nTag )
	{
	case Tag_Return:
		{
			GameSceneMgr::getInstance()->changeScene( Scene_Type_RoleInterface );
		}
		break;

	default:
		{
			GameSceneMgr::getInstance()->changeScene( Scene_Type_PlayILoading );
		}
		break;
	}
}

void GameLevelScene::onKeyReleased( EventKeyboard::KeyCode keyCode, Event* event )
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE
		|| keyCode == EventKeyboard::KeyCode::KEY_ESCAPE )
	{
		GameSceneMgr::getInstance()->changeScene( Scene_Type_RoleInterface );
	}
}
