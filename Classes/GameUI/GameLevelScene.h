#ifndef __GAME_LEVEL_SCENE_H__
#define __GAME_LEVEL_SCENE_H__


#include "cocos2d.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocos2d::ui;


/************************************************************************/
/* 游戏关卡界面                                                         */
/************************************************************************/

class GameLevelScene : public Layer
{
public:
	GameLevelScene()
		:_levelRootNode(nullptr)
	{

	}
	~GameLevelScene()
	{

	}

	enum ButtonTag
	{
		Tag_Return,
		Tag_Level,
	};

	CREATE_FUNC(GameLevelScene);
	static Scene* createScene();

private:

	virtual void onEnter();

	// touch event
	virtual bool onTouchBegan(Touch *touch, Event *unusedEvent) override;
	virtual void onTouchMoved(Touch *touch, Event *unusedEvent) override;
	virtual void onTouchEnded(Touch *touch, Event *unusedEvent) override;
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event); 


	void initInterface( void );

	// Even Callfunc
	void callFuncButtonEvent( Ref* ref );

private:
	Node* _levelRootNode;

};



#endif
