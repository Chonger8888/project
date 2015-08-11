#ifndef __GAME_MAIN_LAYER_H__
#define __GAME_MAIN_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../GameUI/CommonWidget.h"
#include "GameCommon/NewDataMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocostudio::timeline;
using namespace cocos2d::ui;

/************************************************************************/
/* 游戏战斗主界面                                                        */
/************************************************************************/

class GameMainLayer : public Layer
{
public:
	GameMainLayer()
		:_mainLayerRootNode(nullptr)
//		,_loadingBarEnergy(nullptr)
		,_countTime(0)
		,_countAddLifeNum(0)
		,_maxLifeBar(0)
		,_curMaxLifeBar(0)
		,_decayInterval(0)
		,_decayCountT(0)
		,m_HpLimit(100.0f)
		,_decayIng(false)	// 默认非衰减中
		,m_isDead(false)
		,m_isCanBeHurt(true)
	{
		int curRole = NewDataMgr::getInstance()->mRoleConfig.roleIndex;
		m_HpLimit = NewDataMgr::getInstance()->getRoleHpWithId(curRole);
		m_CurHp = m_HpLimit;
	}
	~GameMainLayer()
	{

	}

	enum ButtonTag
	{
		Tag_Pause,			// 关闭
	};

	CREATE_FUNC(GameMainLayer);

	
	// 更新能量条
	void updateEnergyBar( float num );
	void energyBarAction1( Node* node );

	// 能量条衰减
	void decayEnergyBar( void );
	void energyBarAction2( Node* node );

	// 根据角色获得生命条衰减总时间
	float getDecayCountT( int type );
	void setdecayConfig( float interval, float t );
	// 获得衰减状态
	bool getDecayState( void );

	// 获得收集物品节点
	Node* getCollectNode( void );

	// 更新金币
	void updateUIGold( int goldnum );
	// 更新成绩
	void updateUIScore( int score );
	// 更新Km
	void updataGamedistance( int km );

	// 生命衰减
	void decayLifeBar( void );
	void callbackDecayLIfe( float t );

	//血量持续减少
	void HpDecrease(float t);
	void AddHp(float num);
	
	//在受到伤害之后的一段时间内，不会受到伤害
	void WudiSometime(float dur = 1.0f);
	void WudiOver(Node* node);

	void GameFailed();
	bool isRealDead(){return m_isDead;}
	void SetRealDead(bool b);

	void showBuyFuHuoWidget( void );
	void showSettingWidget( void );

	static Node * seekNodeByName(Node* node, const std::string &name );
	static void DelayPuse(Node* node);
	void GamePause();


	// 播放远征成功
	void playPassSucceed( void );
	void callFuncGuang1( Node* node );
	void callFuncNodeMove( Node* node );
	void setDaZhaoState( bool state );
	void startDaZhaoSD( void );
private:

	virtual void onEnter();
	virtual bool init();

	// 初始界面数据
	void initInterface( void );

	Node* createEnergyFullAnim( Vec2 ps );
	void removeEnergyAnim( Node* node );
	void callFuncRandAnim( float t );

	void playLifeBarWarning( void );

	void buyFuhuoBtnEvent(CommonWidget::EventKey key);
	void buyFuhuoCancel( void );
	void BuyFuHuoConfirm( void );

	void callbackPause( Ref* ref );
	// 大招
	void callbackDazhao( Ref* ref );
	void playDaZhaoCD( void );
	void callFuncDaZhaoCD( Node* node );
	void callFuncWidgetBuyDz( void );
	void callFuncWidgetBuyDzExit( void );
	void callWidgetGoldTips( void );

private:

//	LoadingBar* _loadingBarEnergy;


	Node*	_mainLayerRootNode;
	float	_countAddLifeNum;
	int		_countTime;
	float	_curMaxLifeBar;
	float	_maxLifeBar;
	
	float	_decayInterval;			// 衰减延时
	float	_decayCountT;			// 衰减总时长
	bool	_decayIng;				// 衰减状态 ture衰减中

	float	m_HpLimit;				//血上限
	float	m_CurHp;				//当前血量值
	bool	m_isDead;				//是否打死
	bool	m_isCanBeHurt;
};



// 复活
class BuyFuHuo : public Layer
{
public:

	enum ButtonTag
	{
		Tag_BuyFuhuo,		// 购买
		Tag_Close,		// 购买

	};

	BuyFuHuo()
		:_fuHuoLayout(nullptr)
	{

	}

	~BuyFuHuo()
	{

	}

	static void showLayout( Node* node );

	virtual void onEnter();

	CREATE_FUNC( BuyFuHuo );


private:

	void initInterface();

	// Even Callfunc
	void setEventForNode( Button* node, ButtonTag tag );
	void callFuncButtonEvent( Ref* ref );

	void buyFuhuoCancel( void );
	void BuyFuHuoConfirm( void );

	void callFuncAction1( Node* node );
private:

	Node* _fuHuoLayout;


};



// 设置
class SettingLayer : public Layer
{
public:

	enum ButtonTag
	{
		Tag_ExitGame,		// 退出游戏
		Tag_ContinueGame,	// 继续游戏

	};

	SettingLayer()
		:_settingayout(nullptr)
	{

	}

	~SettingLayer()
	{

	}

	static void showLayout( Node* node );

	virtual void onEnter();

	CREATE_FUNC( SettingLayer );


private:

	void initInterface();

	// Even Callfunc
	void setEventForNode( Button* node, ButtonTag tag );
	void callFuncButtonEvent( Ref* ref );

	// 音乐音效
	void selectedMusicEvent(Ref* pSender,CheckBox::EventType type);
	void selectedSoundEvent(Ref* pSender,CheckBox::EventType type);
	
private:

	Node* _settingayout;


};

#endif
