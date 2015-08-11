#ifndef __COMMON_WIDGET_H__
#define __COMMON_WIDGET_H__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../GameCommon/NewDataMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocostudio::timeline;
using namespace cocos2d::ui;


/************************************************************************/
/* 公共小弹框                                                           */
/************************************************************************/

class CommonWidget : public Layer
{
public:
	CREATE_FUNC(CommonWidget);

	CommonWidget()
		:_commonWidget(nullptr)
		,eventListener(nullptr)
		,callFuncBtnExit(nullptr)
		,callFuncBtn1(nullptr)
		,callFuncBtn2(nullptr)
	{

	}
	~CommonWidget()
	{

	}
	enum ButtonTag
	{
		Tag_ExitWidget,			// 退出
		Tag_Btn1,
		Tag_Btn2,
	};

	enum class EventKey
	{
		EventKey_Exit,
		EventKey_Btn1,
		EventKey_Btn2,
	};

	typedef std::function<void(void)> ccCommonWidgetBtnCallBack;
	typedef std::function<void(CommonWidget::EventKey)> ccCommonWidgetBtnEvent;


	// setting widget content string
	void setContent( const char* content1 = nullptr, const char* content2 = nullptr );

	// add click event listener
	void addEventListener(const ccCommonWidgetBtnEvent& callback );
	void addBtnExitClickEventListener( const ccCommonWidgetBtnCallBack& callback );
	void addBtn1ClickEventListener( const ccCommonWidgetBtnCallBack& callback );
	void addBtn2ClickEventListener( const ccCommonWidgetBtnCallBack& callback );

private:

	virtual bool init();
	virtual void onEnter();
	virtual void onExit(void);
	
	// show interface
	void initInterface( void );

	// Even Callfunc
	void callFuncButtonEvent( Ref* ref );

private:
	Node* _commonWidget;

	// 按钮回调方法
	ccCommonWidgetBtnEvent	eventListener;
	ccCommonWidgetBtnCallBack callFuncBtnExit;	// exit
	ccCommonWidgetBtnCallBack callFuncBtn1;		// button1
	ccCommonWidgetBtnCallBack callFuncBtn2;		// button2

};


/*******************************************
*	商店
*******************************************/


class ShopBuyLayer :public Layer
{
public:

	enum GoodsType
	{
		GoodsType_Gold = 1,
		GoodsType_Gem = 2,
		GoodsType_Life = 3,
	};

	enum ButtonTag
	{
		Tag_Back,			// 返回

		Tag_LabelGold,		// 金币
		Tag_LabelGem,		// 宝石
		Tag_LabelWing,		// 翅膀

	};
	ShopBuyLayer()
		:_widgetLayer(nullptr)
		,_optionLabel(Tag_LabelGold)
	{

	}

	~ShopBuyLayer ()
	{

	}

	static void showLayout( Node* node, int tag, int labelTag );
	virtual void onEnter();

	CREATE_FUNC( ShopBuyLayer );

	CC_SYNTHESIZE(int,_optionLabel,optionLabel);			//标签页面

private:
	void setEventForNode( Button* node, ButtonTag tag );
	void callFuncButtonEvent( Ref* ref );

	void callFuncBuyGoods( Ref* ref ,Widget::TouchEventType type );
	void callFuncViewPage( Ref*, PageView::EventType );

	void initInterface();
	void optionLabel( ButtonTag tag );

	void delayOptionLabel( void );


private:
	Node*	_widgetLayer;

};


/*******************************************
*	奖励物品节点
*******************************************/

class RewardGoodsNode : public Node, public Clonable
{
public:
	
	static RewardGoodsNode* create( RewardGoodsKey key );

	RewardGoodsNode()
		:_nodeTag(0)
	{

	}
	~RewardGoodsNode()
	{

	}
	
	virtual RewardGoodsNode* clone() const;

private:

	// show interface
	bool initWithKey( RewardGoodsKey key );


private:
	RewardGoodsKey _goodeskey;

	int _nodeTag;

};

#endif
