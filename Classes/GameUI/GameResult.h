/********************************************************************
创建于:		2014/12/11  10:39	
文件名: 	E:\Cocos3.3_Project\MyGame\Classes\GameResult.h 
作  者:		alex(alex)
修改于：	2014/12/11  10:39	

目  的:		游戏结果
*********************************************************************/
#ifndef _GAME_RESULT_H_
#define _GAME_RESULT_H_


#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GamePlaying/Layer/UIController.h"

USING_NS_CC;
USING_NS_CC_EXT;


using namespace cocostudio::timeline;
using namespace cocos2d::ui;

class UIController;
class GameResult : public Layer
{
public:

	enum ButtonTag
	{
		Tag_Back,
		Tag_Again,
	};

	typedef enum InterfaceTag
	{
		Interface_Tag_Resout,
		Interface_Tag_ReWard,
	}InterfaceTag;

	CREATE_FUNC(GameResult);

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void onTouchEnded( Ref *sender, Widget::TouchEventType controlEvent );

	// 显示结算界面 by eason
	static void showResultWidget( bool  );
	// touch event

	void initInterface( void );
	void initRewardInterface( void );
	void updateRewardInterface( void );
	void playRewardAction( void );

	// Even Callfunc
	void setEventForNode( Button* node, ButtonTag tag );
	void callFuncButtonEvent( Ref* ref );

	// 抽奖翻牌回调 
	void callFuncReversal( Node* node );
	void callFuncRewardFinish(Node* node);
	void callFuncFlashAction(Node* node);

private:
	// 切换结算界面 1 成绩界面 2 抽奖界面
	void changeResoultInterface( InterfaceTag tag );

	void getLabelNumAction( Node* node, void* data );
	void callFuncLabelNumAction( Node* node, void* data );

	// 是否允许购买奖励
	bool isBuyReward();

private:

	// 翻动中
	bool _crossing;

	Node* _resultWidget;

	Sprite* _rewardCard[3];

	EventListenerTouchOneByOne *_touchListener;

	int _rollCurScore;
	// 计数翻取奖励数
	int _countGetRewardNum;

	InterfaceTag _curInterfaceTag;



};


#endif