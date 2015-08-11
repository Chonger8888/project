/********************************************************************
������:		2014/12/11  10:39	
�ļ���: 	E:\Cocos3.3_Project\MyGame\Classes\GameResult.h 
��  ��:		alex(alex)
�޸��ڣ�	2014/12/11  10:39	

Ŀ  ��:		��Ϸ���
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

	// ��ʾ������� by eason
	static void showResultWidget( bool  );
	// touch event

	void initInterface( void );
	void initRewardInterface( void );
	void updateRewardInterface( void );
	void playRewardAction( void );

	// Even Callfunc
	void setEventForNode( Button* node, ButtonTag tag );
	void callFuncButtonEvent( Ref* ref );

	// �齱���ƻص� 
	void callFuncReversal( Node* node );
	void callFuncRewardFinish(Node* node);
	void callFuncFlashAction(Node* node);

private:
	// �л�������� 1 �ɼ����� 2 �齱����
	void changeResoultInterface( InterfaceTag tag );

	void getLabelNumAction( Node* node, void* data );
	void callFuncLabelNumAction( Node* node, void* data );

	// �Ƿ���������
	bool isBuyReward();

private:

	// ������
	bool _crossing;

	Node* _resultWidget;

	Sprite* _rewardCard[3];

	EventListenerTouchOneByOne *_touchListener;

	int _rollCurScore;
	// ������ȡ������
	int _countGetRewardNum;

	InterfaceTag _curInterfaceTag;



};


#endif