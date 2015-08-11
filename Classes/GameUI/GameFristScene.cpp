#include "GameFristScene.h"
#include "AudioEngine.h"
#include "../GameCommon/NewAudio.h"
#include "GameSceneMgr.h"
#include "AppDelegate.h"
#include "GameCommon/NewDataMgr.h"
#include "GamePlaying/RapidParticle/ParticleManager.h"
USING_NS_CC;

Scene* GameFristScene::createScene()
{
	auto scene = Scene::create();

	auto layer = GameFristScene::create();
	scene->addChild(layer);

	return scene;
}

void GameFristScene::onEnter()
{
	Layer::onEnter();

	// Register Touch Event
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);

	listener->onTouchBegan = CC_CALLBACK_2(GameFristScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameFristScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameFristScene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	initInterface();

	NewAudio::getInstance()->playBgMusicForce(Music_Bg);
	
	scheduleUpdate();


}

bool GameFristScene::onTouchBegan( Touch *touch, Event *unusedEvent )
{
	bool ret = true;

	return ret;
}

void GameFristScene::onTouchMoved( Touch *touch, Event *unusedEvent )
{

}

void GameFristScene::onTouchEnded( Touch *touch, Event *unusedEvent )
{
	// 
// 	for (int i = 1; i <= 5000; i++ )
// 	{
// 		auto node = Node::create();
// 		node->setPosition(Vec2(random(0, 720), random(1, 1280)));
// 		addChild(node);
// 
// // 		auto parcile = ParticleSystemQuad::create("particla/BuySucceed/test2.plist");
// // 		node->addChild(parcile);
// 
// 		auto parcile = ParticleManager::getInstance()->getParticle("particla/BuySucceed/test2.plist");
// 		node->addChild(parcile);
// 
// 		GameParticleMsg::getInstance()->playParticleWithType(node, 1);
// 	}


	GameSceneMgr::getInstance()->changeScene( Scene_Type_RoleInterface );

}

void GameFristScene::initInterface( void )
{
	auto glview = Director::getInstance()->getOpenGLView();

	auto backgroudSpr = Sprite::create( "FirstUIRes/firstBg.png" );
	backgroudSpr->setAnchorPoint( Vec2(0.0f, 0.0f) );
	addChild( backgroudSpr );


	auto spriteWidgetLeft1 = Sprite::create("FirstUIRes/spriteWidget3.png");
	spriteWidgetLeft1->setAnchorPoint(Vec2(1, 0.5));
	spriteWidgetLeft1->setFlipX(true);
	spriteWidgetLeft1->setScale(0.85);
	spriteWidgetLeft1->setOpacity(0);
	spriteWidgetLeft1->setPosition(Vec2(backgroudSpr->getContentSize().width*0.27, backgroudSpr->getContentSize().height*0.29));
	spriteWidgetLeft1->setRotation(-120);
	addChild(spriteWidgetLeft1);
	spriteWidgetLeft1->runAction(CCSequence::create(
		DelayTime::create(0.9),
		CCFadeIn::create(0.05),
		CCEaseBackIn::create(CCRotateTo::create(0.4f, 0)),
		nullptr));

	auto spriteWidgetLeft2 = Sprite::create("FirstUIRes/spriteWidget2.png");
	spriteWidgetLeft2->setAnchorPoint(Vec2(1, 0.5));
	spriteWidgetLeft2->setFlipX(true);
	spriteWidgetLeft2->setScale(0.85);
	spriteWidgetLeft2->setOpacity(0);
	spriteWidgetLeft2->setPosition(Vec2(backgroudSpr->getContentSize().width*0.40, backgroudSpr->getContentSize().height*0.28));
	addChild(spriteWidgetLeft2);
	spriteWidgetLeft2->setRotation(-120);
	spriteWidgetLeft2->runAction(CCSequence::create(
		DelayTime::create(0.85),
		CCFadeIn::create(0.05),
		CCEaseBackIn::create(CCRotateTo::create(0.4f, 0)),
		nullptr));

	auto spriteWidgetRight1 = Sprite::create("FirstUIRes/spriteWidget3.png");
	spriteWidgetRight1->setAnchorPoint(Vec2(0, 0.5));
	spriteWidgetRight1->setScale(0.85);
	spriteWidgetRight1->setRotation(120);
	spriteWidgetRight1->setOpacity(0);
	spriteWidgetRight1->setPosition(Vec2(backgroudSpr->getContentSize().width*0.72, backgroudSpr->getContentSize().height*0.29));
	addChild(spriteWidgetRight1);
	spriteWidgetRight1->runAction(CCSequence::create(
		DelayTime::create(0.9),
		CCFadeIn::create(0.05),
		CCEaseBackIn::create(CCRotateTo::create(0.4f, 0)),
		nullptr));

	auto spriteWidgetRight2 = Sprite::create("FirstUIRes/spriteWidget2.png");
	spriteWidgetRight2->setAnchorPoint(Vec2(0, 0.5));
	spriteWidgetRight2->setScale(0.85);
	spriteWidgetRight2->setRotation(120);
	spriteWidgetRight2->setOpacity(0);
	spriteWidgetRight2->setPosition(Vec2(backgroudSpr->getContentSize().width*0.59, backgroudSpr->getContentSize().height*0.28));
	addChild(spriteWidgetRight2);
	spriteWidgetRight2->runAction(CCSequence::create(
		DelayTime::create(0.85),
		CCFadeIn::create(0.05),
		CCEaseBackIn::create(CCRotateTo::create(0.4f, 0)),
		nullptr));


	auto spriteWidget1 = Sprite::create("FirstUIRes/spriteWidget1.png");
	spriteWidget1->setPosition(Vec2(backgroudSpr->getContentSize().width*0.5,backgroudSpr->getContentSize().height*0.25));
	spriteWidget1->setScale(3.0);
	spriteWidget1->setOpacity(0);
	addChild(spriteWidget1);
	spriteWidget1->runAction(CCSequence::create(
		CCFadeIn::create( 0.3f ),
		CCEaseBackOut::create(CCScaleTo::create(0.3f, 0.8)),
		CCDelayTime::create(0.8),
		CCCallFuncN::create(this, callfuncN_selector(GameFristScene::callFuncPlayEffect)),
		nullptr));

	

	auto tips = Sprite::create("Bg/tips.png");
	tips->setPosition(Vec2(backgroudSpr->getContentSize().width*0.5,
		backgroudSpr->getContentSize().height*0.25));
	backgroudSpr->addChild(tips);
	
	ActionInterval* fadein = FadeIn::create(0.5f);
	ActionInterval* fadeout = FadeOut::create(0.5f);
	ActionInterval* seq = Sequence::create(fadein,fadeout,nullptr);
	ActionInterval* rep = RepeatForever::create(seq);
	//tips->runAction(rep);

	/*auto touchTips = LabelTTF::create( "60", "Helvetica", 40 );
	touchTips->setColor( Color3B(255,0,0) );
	touchTips->setPosition( Vec2(glview->getVisibleSize().width/2, glview->getVisibleSize().height/5) );
	touchTips->setString( "点击屏幕继续" );
	addChild( touchTips );*/

}

void GameFristScene::onExit( void )
{
	Layer::onExit();
}

void GameFristScene::update( float delta )
{
	//AudioMgr::getInstance()->updataAudio();
}

void GameFristScene::callFuncPlayEffect(Node* node)
{
	auto parcile1 = ParticleSystemQuad::create("particla/firstUI/test1.plist");
	auto parcile2 = ParticleSystemQuad::create("particla/firstUI/test2.plist");
	parcile1->setPosition(node->getPosition());
	parcile2->setPosition(node->getPosition());
	parcile1->setScale(2.5f);
	parcile2->setScale(2.5f);
	addChild(parcile1);
	addChild(parcile2);
}


