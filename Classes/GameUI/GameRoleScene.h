#ifndef __GAME_ROLE_SCENE_H__
#define __GAME_ROLE_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../GameCommon/NewDataMgr.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocostudio::timeline;
using namespace cocos2d::ui;

#define LocalZOrde_1000		1000
#define LocalZOrde_500		500

/*******************************************
/* 界面管理
*******************************************/
class GameInterfaceMgr
{
public:
	enum InterfaceTag
	{
		InterfaceTag_RoleOption=1888,		// 角色选择
		InterfaceTag_CardOption,			// 卡牌选择
		InterfaceTag_CompoundCard,			// 升级卡牌
		InterfaceTag_Reward,				// 抽奖
		InterfaceTag_WujinMode,				// 无尽模式
		InterfaceTag_ShopBuy,				// 商店
		InterfaceTag_QuickStone,			// 灵石
		InterfaceTag_QuickStoneCardOption,	// 灵石卡牌选择
		InterfaceTag_QuickStoneOption,		// 灵石选择
	};
	
	static GameInterfaceMgr* getInstance(){
		static GameInterfaceMgr nGameInterfaceMgr;
		return &nGameInterfaceMgr;
	}
	// 获得/设置父节点
	void setParentNode( Node* node );
	Node* getParentNode( void );
	// 添加下一个界面
	void addInterfaceToList( InterfaceTag tag , bool isShow = true, void* data = nullptr );
	// 显示下一个界面
	void showNextInterface( void );
	// 关才当前界面
	void closeCurInterface( void );
	// clean所有界面
	void closeAllInterface( void );

private:

private:
	// 父节点
	Node*	_node;

	// 附加数据
	void*	_data;	

	std::list<InterfaceTag>	_interfaceVector;
};

/*******************************************
* 主界面
*******************************************/
class GameRoleScene : public Layer
{
public:
	typedef enum ButtonTag
	{
		Tag_wjMode,				// 无尽模式
		Tag_gkMode,				// 关卡模式
		Tag_tzMode,				// 挑战模式

		Tag_Card,				// 卡牌

		Tag_SignIn,				// 签到
		Tag_RoleOption,			// 角色选择

	};

	GameRoleScene();
	~GameRoleScene();

	CREATE_FUNC(GameRoleScene);
	static Scene* createScene();


	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
    virtual void onEnterTransitionDidFinish();

	static GameRoleScene* getGameRoleScene();
	void initInterface( void );


	void playPassSucceed( void );
	void callFuncGuang1( Node* node );
	void callFuncNodeMove( Node* node );

	// 升级动画
	void playUpgradeAnim( void );
	void removeUpgradeAnim( Node* node );

	// 购买成功特效
	void buySucceedAction( void );
	void callFuncbuySucceed( Node* node );


	// Even Callfunc
	void setEventForNode( Button* node, ButtonTag tag );
	void callFuncButtonEvent( Ref* ref );
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event); 
	void callFuncExitGame( void );
	
	// 更新卡牌头像框
	void updataCardHead( Ref* sender );
	// 主界面动画
	void playMainSceneAction( Ref* sender );

	
	void showPaySuccee( Ref* sender );
	void updataChangeRole( Ref* sender );

	void LoadArmatureCallback(float dt = 0);


private:
	static GameRoleScene* _gameRoleScene;

	Size	_winSize;
	// 角色界面node
	Node* _roleRootNode;

	// 存放上一页面idx
	int _lastPageIndex;

	bool _scrollTag;


	LoadingBar*	_barLife;			// 生命值
	LoadingBar* _barRoleAtk;		// 攻击力
	LoadingBar* _barRoleSpeed;		// 攻击速度

	Sprite*	_barLifeRight;			// 生命值
	Sprite* _barRoleAtkRight;		// 攻击力
	Sprite* _barRoleSpeedRight;		// 攻击速度

};

/*******************************************
*	角色选择界面
*******************************************/
class RoleOptionlayer : public Layer
{
public:
	RoleOptionlayer()
		:_curOptionRoleIndex(1)
		,_roleUpgradeInterface( nullptr )
		,_curOptionRockIndex(1)
		,_curHaveExp(0)
		,_consumeExp(0)
		,_isUpgradeing(false)
	{
	}
	
	~RoleOptionlayer()
	{
	}

	typedef enum ButtonTag
	{
		Tag_Back,				// 返回
		Tag_Unlock,				// 解锁

		Tag_Role1,				// 角色1
		Tag_Role2,				// 角色2
		Tag_Role3,				// 角色3

		Tag_UpgradeClose,		// 升级关闭
		Tag_Upgrade,			// 角色升级

		Tag_Pebble1,				// 小水晶石
		Tag_Pebble2,				// 中水晶石
		Tag_Pebble3,				// 大水晶石

		Tag_RoleStartCompound,				// 角色开始升级 吸收水晶
	};

	static void showLayout( Node* node, int tag );

	virtual void onEnter();

	CREATE_FUNC(RoleOptionlayer);


private:
	void setEventForNode( Button* node, ButtonTag tag );
	void callFuncButtonEvent( Ref* ref );
	void selected10ContinuonsEvent(Ref* pSender,CheckBox::EventType type);

	void initInterface();

	void changeOptionCard( void );
	void updataRoleLock( void );
	void changeOptionRock( int index );

	void showRoleUpgrade( void );
	void hideRoleUpgrade( void );

	// 更新升级角色界面
	void startUpgradePre( void );
	void updataRoleUpgrade( void );
	void callFuncUpgrade( Node* node );
	void palyRoleUpgrade( void );
	void callfunPlayUpgradeAction( void );


	BuyListKey getOptionPebbleKey( void );
private:
	bool _isUpgradeing;
	Node*	_roleOptionlayout;

	// 升级角色界面
	Node*	_roleUpgradeInterface;

	// 当前选择的升级石头ID
	int		_curOptionRockIndex;

	// 当前选择的角色ID
	int		_curOptionRoleIndex;

	// 当前选择灵石获得的经验
	int		_curHaveExp;
	// 当前选择灵石已消耗经验
	int		_consumeExp;
};

/*******************************************
* 主界面顶部和底部框
*******************************************/
class MainSceneTaskBar : public Layer
{
public:
	typedef enum ButtonTag
	{
		Tag_HzOption,			// 徽章
		Tag_HCOption,			// 合成
		Tag_LsOption,			// 灵石
		Tag_MainInOption,		// 主界面
		Tag_JzOption,			// 卷轴

		Tag_BgMusic,			// 背景音乐
		Tag_SoundEff,			// 音效

		Tag_BuyGole,
		Tag_BuygGem,
		Tag_BuyWing,

	};
	MainSceneTaskBar();
	~MainSceneTaskBar();



	static void showLayout(Node* node, bool parm = true );
	virtual void onEnter();

	CREATE_FUNC(MainSceneTaskBar);

private:
	void setEventForNode( Button* node, ButtonTag tag );
	void callFuncButtonEvent( Ref* ref );
	// 音乐音效
	void selectedMusicEvent(Ref* pSender,CheckBox::EventType type);
	void selectedSoundEvent(Ref* pSender,CheckBox::EventType type);

	void initInterface();
	void updataShowData( void );

	// 校准生命回复时间
	void verifyLifeTime( void );
	// 生命回复时间处理
	void replyLifePro( void );
	void runLifeTime( void );

	void callbackLifeTime();

	void MsgProShowButtomBar( Ref* sender );
	void MsgProHideButtomBar( Ref* sender );
	void MsgProConsumeLife( Ref* sender );
	void MsgProChangeGemGold( Ref* sender );

private:
	Node*	_mainSceneTaskBar;

	long int _lifeReplyTime;

	bool _isShowButtomBar;

};

/*******************************************
* 卡牌合成界面
*******************************************/
class CardCompoundLayer :public Layer
{
public:
	enum CompoundTag
	{
		CompoundTag_1,		// 选择升级卡牌
		CompoundTag_2,		// 升级卡牌星级确定 
		CompoundTag_3,		// 升级特效界面 
	};

	typedef enum ButtonTag
	{
		Tag_Back,			// 返回

		Tag_1LvCard,		// 1星card
		Tag_2LvCard,		// 2星card
		Tag_3LvCard,		// 3星card
		Tag_4LvCard,		// 4星card
		Tag_5LvCard,		// 5星card

		Tag_HeCheng,		// 合成

	};
	CardCompoundLayer ()
		:_cardCompoundLayer(nullptr)
		,_optionCardLvFram(nullptr)
		,_curOptionCompoundID(0)
		,_compoundFinish(false)
	{

	}

	~CardCompoundLayer ()
	{

	}

	static void showLayout( Node* node, int tag );
	virtual void onEnter();

	CREATE_FUNC(CardCompoundLayer);

private:
	void setEventForNode( Button* node, ButtonTag tag );
	void callFuncButtonEvent( Ref* ref );
	void callFuncCardOptionEvent( Ref* ref );

	void initInterface();
	void initOptionCardInterface();
	// 切换升级界面
	void changeCardCompound( CompoundTag tag );
	// 更新升级选星界面
	void updataCompoundNeedNum( void );
	// 更新选择星级升级信息
	void updataOptionCompInfo( void );

	// 添加节点到列表
	void addCardToList( void );

	// 播放升级界面动作
	void playCompoundAction1( void );
	void playCompoundAction2( void );
	void playCompoundAction1_1( void );
	void playCompoundAction1_2( void );
	void callFuncShake( void );
	void callFuncComPoundFinish( void );

	// 获得升级所需数量
	int getCompoundNeedNumWithLv( int starLv );



private:
	Node*	_cardCompoundLayer;

	Sprite* _cardStarLv[5];
	Sprite* _optionCardLvFram;
	Button* _ButtonCardStar[5];
	
	// 当前升级界面
	CompoundTag					_curCompoundTag;

	std::vector<Button*>		_cardOptionList;	

	// 当前选择要升级的卡牌ID
	int	_curOptionCompoundID;
	// 当前选择要升级的卡牌星级
	int _curOptionCompoundStarLv;
	// 是否完成升级
	bool _compoundFinish;


};

/*******************************************
* 卡牌选择界面
*******************************************/
class CardOptionLayout : public Layer
{
public:
	enum ButtonTag
	{
		Tag_Back,			// 返回
		Tag_Card1,			// 卡牌1
		Tag_Card2,			// 卡牌2
		Tag_Card3,			// 卡牌3

		Tag_CardStarLv1,	// 星1卡牌
		Tag_CardStarLv2,	// 星2卡牌
		Tag_CardStarLv3,	// 星3卡牌
		Tag_CardStarLv4,	// 星4卡牌
		Tag_CardStarLv5,	// 星5卡牌

		Tag_CloseOption,	// 关闭选择界面

		Tag_CardSort,		// 排序
	};

	CardOptionLayout()
		:OptionCardTouchListener(nullptr)
		,_touchLayer(nullptr)
		,_curSelectCardType(0)
		, _lastSelectCardType(0)
		, _isOption(false)
	{

	}

	~CardOptionLayout()
	{

	}

	static void showLayout( Node* node, int tag );

	virtual void onEnter();
	virtual bool onTouchBegan( Touch *touch, Event *unused_event );

	CREATE_FUNC(CardOptionLayout);


private:

	void initInterface();


	// Even Callfunc
	void setEventForNode( Button* node, ButtonTag tag );
	void callFuncButtonEvent( Ref* ref );
	void callFuncCardOptionEvent( Ref* ref );

	void runButtonAction( Node* node );

	// 排序卡牌
	void sortCard( void ); 

	// 添加卡牌类型等级到列表
	void addCardToList();
	Button* getCardItem( void );

	// 更新选择的卡牌
	void updataChangeCard( void );
	// 更新选择的卡牌类型状态 非同类灰暗
	void updataCardTypeState( int cardtype );
	void updataCardListState( int optionid );
	void updataStarLvInterface( int id );

	// 显示隐藏选择卡牌星级界面
	void showConfirmOption( void );
	void hideConfirmOption( void );



private:

	Layer*	_touchLayer;

	Node* _cardOptionlayout;


	std::vector<Button*>		_cardOptionList;	

	Button* _cardBtn[3];

	// 确认卡牌选择界面触摸
	EventListenerTouchOneByOne *OptionCardTouchListener;

	int		_lastSelectCardType;	//	1-3
	int		_curSelectCardType;		//	1-3
	int		_curSelectCardID;		//	1-9	
	int		_curSelectCardStarLv;	//	1-5	

	bool	_isOption;
};

class CmpareCardAtk
{
public:
	bool operator()(const csvCardAtkPConfig_s item1, const csvCardAtkPConfig_s  item2) const
	{
		int item1CardStarLv = NewDataMgr::getInstance()->getCardMaxStarLvWithId(item1.CardId);
		auto item1Atk = NewDataMgr::getInstance()->getCardAtk(item1.CardId, item1CardStarLv);

		int item2CardStarLv = NewDataMgr::getInstance()->getCardMaxStarLvWithId(item2.CardId);
		auto item2Atk = NewDataMgr::getInstance()->getCardAtk(item2.CardId, item2CardStarLv);
		return item1Atk > item2Atk;
	}
};

/*******************************************
*	幸运卷轴
*******************************************/
class GameRewardLayer :public Layer
{
public:
	
	typedef enum ButtonTag
	{
		Tag_Close,			// 关闭

		Tag_ResetTime,		// 重置刷新时间
		Tag_Start,			// 开始
		Tag_GetReward,		// 领取

	};
	GameRewardLayer ()
		:_gameRewardLayer(nullptr)
		,_curOptionNode(nullptr)
		,_replyTime(0)
	{

	}

	~GameRewardLayer ()
	{

	}

	static void showLayout( Node* node, int tag );
	virtual void onEnter();


	CREATE_FUNC( GameRewardLayer );

private:
	void setEventForNode( Button* node, ButtonTag tag );
	void callFuncButtonEvent( Ref* ref );

	void initInterface();
	void createRewardGoodsNode( void );

	void startMoveReward( void );
	void callBackMoveSelect();

	// 完成领取动作
	void getGoodsFinish( void );
	// 隐藏/显示领取界面
	void showReward( void );
	void hideReward( void );

	// 设计按钮状态
	void setButtonTouchState( Button* button, bool state );

	// 刷新时间相关
	void verifyScrollTime( void );
	void replyScrollPro( void );
	void runScrollTime( void );
	void callbackUpdateTime();

private:
	Node*	_gameRewardLayer;

	Sprite*	_curOptionNode;
	Sprite* _rewardItem[6];

	// 刷新时间
	int _replyTime;

	int _curSelectRole;
	int _MaxMoveNum;
	int	_moveNum;
	float _rotateDaley;
	int	_getGoodsID;

};

/*******************************************
*	无尽模式
*******************************************/
class WuJinModeLayer :public Layer
{
public:

	typedef enum ButtonTag
	{
		Tag_Close,			// 关闭

		Tag_ResetCard,		// 重置卡牌
		Tag_BuyGoods,		// 购买道具

		Tag_GoodsChongCi,			// 道具1
		Tag_GoodsHuDun,			// 道具2
		Tag_GoodsDaZhao,			// 道具3

		Tag_Start,			// 开始

		Tag_GetRewarde,			// 领取奖励
	};
	WuJinModeLayer ()
		:_wuJinModeLayer(nullptr)
	{

	}

	~WuJinModeLayer ()
	{

	}

	static void showLayout( Node* node, int tag );
	virtual void onEnter();

	CREATE_FUNC( WuJinModeLayer );

private:
	void setEventForNode( Button* node, ButtonTag tag );
	void callFuncButtonEvent( Ref* ref );
	

	void initInterface();

	void updateGoodsInfoWithIndex( int index );
	void updateGoodsNumWithIndex( void );
	void buyGoodsProc( void );

	void addTaskItemToList( void );
	void calllFuncGetBtn( Ref *sender, Widget::TouchEventType controlEvent  );
	
	void showTaskReward( void );
	void hideTaskReward( void );

	void playTaskRewardAction();
	void callFuncRewardAction1(Node* node);
	void callFuncRewardAction2(Node* node);


private:
	Node*	_wuJinModeLayer;

	int		_curOptionGoodsIndex;

	int		_getRewardTaskID;

};


/*******************************************
*	灵石
*******************************************/
class QuickStoneLayer :public Layer
{
public:
	enum ButtonTag
	{
		Tag_Back,			// 返回

		Tag_ImplantLabel,	// 镶嵌标签
		Tag_CompoundLabel,	// 合成标签

		Tag_OptionCard,			// 选择卡牌
		Tag_OptionQuickston,	//选择灵石
		Tag_CloseOptionCard,	// 选择卡牌关闭
		Tag_CloseOptionQuickston,//选择灵石关闭

		Tag_StartImplant,	// 开始镶嵌
		Tag_StartCompount,	// 开始合成

	};

	QuickStoneLayer()
		:_curOptionCardId(0)
		, _curOptionQuickStonId(0)
		, _curOptionCompountQuickSId(0)
	{}
	~QuickStoneLayer(){}

	static void showLayout(Node* node, int tag);
	virtual void onEnter();

	CREATE_FUNC(QuickStoneLayer);

private:
	void initInterface();
	void optionLabel(int tag);
	void initImplantInterface(void);

	void showOptionCardWidget();
	void showOptionQuickStonWidget();

	void addCardToList(void);

	// 选择界面回调
	void callFuncCardOptionEvent(Ref* ref);
	void callFuncQuickStonOptionEvent(Ref* ref);

	void setEventForNode(Button* node, ButtonTag tag);
	void callFuncButtonEvent(Ref* ref);
	void callOptionCompountQuickst(Ref* ref);



	void updateQuickStoneCompUI(void);
	// 根据选择更新镶嵌界面
	void updateCardWithCardId(int id);
	void updateQuickStonWithId(int id);

	// 设计按钮状态
	void setButtonTouchState(Button* button, bool state);

	// 开始合成灵石
	void playCompountQuickStone(void);
	void callFuncplayCompount(Node* node);
	void callFuncplayCompount2(Node* node);
	void callFuncReversal(Node* node);

	// 开始镶嵌灵石
	void playImplantQuickStone(void);
	void callFuncplayImplant1(Node* node);
	void callFuncplayImplant2(Node* node);
private:
	Node*	_myLayer;
	Node*	_cardOptionNode;
	Node*	_quickStonOptionNode;

	int	_curOptionCardId;
	int _curOptionQuickStonId;

	int _curOptionCompountQuickSId;
};

#endif
