#ifndef __GAME_FIRST_SCENE_H__
#define __GAME_FIRST_SCENE_H__


#include "cocos2d.h"
USING_NS_CC;


/************************************************************************/
/* 游戏首界面                                                           */
/************************************************************************/

class GameFristScene : public Layer
{
public:
	GameFristScene()
	{

	}
	~GameFristScene()
	{

	}

public:
	CREATE_FUNC(GameFristScene);
	static Scene* createScene();

	virtual void onEnter();
	virtual void onExit(void);
	virtual void update(float delta);
	// touch event
	virtual bool onTouchBegan(Touch *touch, Event *unusedEvent) override;
	virtual void onTouchMoved(Touch *touch, Event *unusedEvent) override;
	virtual void onTouchEnded(Touch *touch, Event *unusedEvent) override;

	// show interface
	void initInterface( void );
	void callFuncPlayEffect(Node* node);
private:

};



#endif