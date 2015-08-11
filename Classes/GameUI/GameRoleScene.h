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
/* �������
*******************************************/
class GameInterfaceMgr
{
public:
	enum InterfaceTag
	{
		InterfaceTag_RoleOption=1888,		// ��ɫѡ��
		InterfaceTag_CardOption,			// ����ѡ��
		InterfaceTag_CompoundCard,			// ��������
		InterfaceTag_Reward,				// �齱
		InterfaceTag_WujinMode,				// �޾�ģʽ
		InterfaceTag_ShopBuy,				// �̵�
		InterfaceTag_QuickStone,			// ��ʯ
		InterfaceTag_QuickStoneCardOption,	// ��ʯ����ѡ��
		InterfaceTag_QuickStoneOption,		// ��ʯѡ��
	};
	
	static GameInterfaceMgr* getInstance(){
		static GameInterfaceMgr nGameInterfaceMgr;
		return &nGameInterfaceMgr;
	}
	// ���/���ø��ڵ�
	void setParentNode( Node* node );
	Node* getParentNode( void );
	// �����һ������
	void addInterfaceToList( InterfaceTag tag , bool isShow = true, void* data = nullptr );
	// ��ʾ��һ������
	void showNextInterface( void );
	// �زŵ�ǰ����
	void closeCurInterface( void );
	// clean���н���
	void closeAllInterface( void );

private:

private:
	// ���ڵ�
	Node*	_node;

	// ��������
	void*	_data;	

	std::list<InterfaceTag>	_interfaceVector;
};

/*******************************************
* ������
*******************************************/
class GameRoleScene : public Layer
{
public:
	typedef enum ButtonTag
	{
		Tag_wjMode,				// �޾�ģʽ
		Tag_gkMode,				// �ؿ�ģʽ
		Tag_tzMode,				// ��սģʽ

		Tag_Card,				// ����

		Tag_SignIn,				// ǩ��
		Tag_RoleOption,			// ��ɫѡ��

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

	// ��������
	void playUpgradeAnim( void );
	void removeUpgradeAnim( Node* node );

	// ����ɹ���Ч
	void buySucceedAction( void );
	void callFuncbuySucceed( Node* node );


	// Even Callfunc
	void setEventForNode( Button* node, ButtonTag tag );
	void callFuncButtonEvent( Ref* ref );
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event); 
	void callFuncExitGame( void );
	
	// ���¿���ͷ���
	void updataCardHead( Ref* sender );
	// �����涯��
	void playMainSceneAction( Ref* sender );

	
	void showPaySuccee( Ref* sender );
	void updataChangeRole( Ref* sender );

	void LoadArmatureCallback(float dt = 0);


private:
	static GameRoleScene* _gameRoleScene;

	Size	_winSize;
	// ��ɫ����node
	Node* _roleRootNode;

	// �����һҳ��idx
	int _lastPageIndex;

	bool _scrollTag;


	LoadingBar*	_barLife;			// ����ֵ
	LoadingBar* _barRoleAtk;		// ������
	LoadingBar* _barRoleSpeed;		// �����ٶ�

	Sprite*	_barLifeRight;			// ����ֵ
	Sprite* _barRoleAtkRight;		// ������
	Sprite* _barRoleSpeedRight;		// �����ٶ�

};

/*******************************************
*	��ɫѡ�����
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
		Tag_Back,				// ����
		Tag_Unlock,				// ����

		Tag_Role1,				// ��ɫ1
		Tag_Role2,				// ��ɫ2
		Tag_Role3,				// ��ɫ3

		Tag_UpgradeClose,		// �����ر�
		Tag_Upgrade,			// ��ɫ����

		Tag_Pebble1,				// Сˮ��ʯ
		Tag_Pebble2,				// ��ˮ��ʯ
		Tag_Pebble3,				// ��ˮ��ʯ

		Tag_RoleStartCompound,				// ��ɫ��ʼ���� ����ˮ��
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

	// ����������ɫ����
	void startUpgradePre( void );
	void updataRoleUpgrade( void );
	void callFuncUpgrade( Node* node );
	void palyRoleUpgrade( void );
	void callfunPlayUpgradeAction( void );


	BuyListKey getOptionPebbleKey( void );
private:
	bool _isUpgradeing;
	Node*	_roleOptionlayout;

	// ������ɫ����
	Node*	_roleUpgradeInterface;

	// ��ǰѡ�������ʯͷID
	int		_curOptionRockIndex;

	// ��ǰѡ��Ľ�ɫID
	int		_curOptionRoleIndex;

	// ��ǰѡ����ʯ��õľ���
	int		_curHaveExp;
	// ��ǰѡ����ʯ�����ľ���
	int		_consumeExp;
};

/*******************************************
* �����涥���͵ײ���
*******************************************/
class MainSceneTaskBar : public Layer
{
public:
	typedef enum ButtonTag
	{
		Tag_HzOption,			// ����
		Tag_HCOption,			// �ϳ�
		Tag_LsOption,			// ��ʯ
		Tag_MainInOption,		// ������
		Tag_JzOption,			// ����

		Tag_BgMusic,			// ��������
		Tag_SoundEff,			// ��Ч

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
	// ������Ч
	void selectedMusicEvent(Ref* pSender,CheckBox::EventType type);
	void selectedSoundEvent(Ref* pSender,CheckBox::EventType type);

	void initInterface();
	void updataShowData( void );

	// У׼�����ظ�ʱ��
	void verifyLifeTime( void );
	// �����ظ�ʱ�䴦��
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
* ���ƺϳɽ���
*******************************************/
class CardCompoundLayer :public Layer
{
public:
	enum CompoundTag
	{
		CompoundTag_1,		// ѡ����������
		CompoundTag_2,		// ���������Ǽ�ȷ�� 
		CompoundTag_3,		// ������Ч���� 
	};

	typedef enum ButtonTag
	{
		Tag_Back,			// ����

		Tag_1LvCard,		// 1��card
		Tag_2LvCard,		// 2��card
		Tag_3LvCard,		// 3��card
		Tag_4LvCard,		// 4��card
		Tag_5LvCard,		// 5��card

		Tag_HeCheng,		// �ϳ�

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
	// �л���������
	void changeCardCompound( CompoundTag tag );
	// ��������ѡ�ǽ���
	void updataCompoundNeedNum( void );
	// ����ѡ���Ǽ�������Ϣ
	void updataOptionCompInfo( void );

	// ��ӽڵ㵽�б�
	void addCardToList( void );

	// �����������涯��
	void playCompoundAction1( void );
	void playCompoundAction2( void );
	void playCompoundAction1_1( void );
	void playCompoundAction1_2( void );
	void callFuncShake( void );
	void callFuncComPoundFinish( void );

	// ���������������
	int getCompoundNeedNumWithLv( int starLv );



private:
	Node*	_cardCompoundLayer;

	Sprite* _cardStarLv[5];
	Sprite* _optionCardLvFram;
	Button* _ButtonCardStar[5];
	
	// ��ǰ��������
	CompoundTag					_curCompoundTag;

	std::vector<Button*>		_cardOptionList;	

	// ��ǰѡ��Ҫ�����Ŀ���ID
	int	_curOptionCompoundID;
	// ��ǰѡ��Ҫ�����Ŀ����Ǽ�
	int _curOptionCompoundStarLv;
	// �Ƿ��������
	bool _compoundFinish;


};

/*******************************************
* ����ѡ�����
*******************************************/
class CardOptionLayout : public Layer
{
public:
	enum ButtonTag
	{
		Tag_Back,			// ����
		Tag_Card1,			// ����1
		Tag_Card2,			// ����2
		Tag_Card3,			// ����3

		Tag_CardStarLv1,	// ��1����
		Tag_CardStarLv2,	// ��2����
		Tag_CardStarLv3,	// ��3����
		Tag_CardStarLv4,	// ��4����
		Tag_CardStarLv5,	// ��5����

		Tag_CloseOption,	// �ر�ѡ�����

		Tag_CardSort,		// ����
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

	// ������
	void sortCard( void ); 

	// ��ӿ������͵ȼ����б�
	void addCardToList();
	Button* getCardItem( void );

	// ����ѡ��Ŀ���
	void updataChangeCard( void );
	// ����ѡ��Ŀ�������״̬ ��ͬ��Ұ�
	void updataCardTypeState( int cardtype );
	void updataCardListState( int optionid );
	void updataStarLvInterface( int id );

	// ��ʾ����ѡ�����Ǽ�����
	void showConfirmOption( void );
	void hideConfirmOption( void );



private:

	Layer*	_touchLayer;

	Node* _cardOptionlayout;


	std::vector<Button*>		_cardOptionList;	

	Button* _cardBtn[3];

	// ȷ�Ͽ���ѡ����津��
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
*	���˾���
*******************************************/
class GameRewardLayer :public Layer
{
public:
	
	typedef enum ButtonTag
	{
		Tag_Close,			// �ر�

		Tag_ResetTime,		// ����ˢ��ʱ��
		Tag_Start,			// ��ʼ
		Tag_GetReward,		// ��ȡ

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

	// �����ȡ����
	void getGoodsFinish( void );
	// ����/��ʾ��ȡ����
	void showReward( void );
	void hideReward( void );

	// ��ư�ť״̬
	void setButtonTouchState( Button* button, bool state );

	// ˢ��ʱ�����
	void verifyScrollTime( void );
	void replyScrollPro( void );
	void runScrollTime( void );
	void callbackUpdateTime();

private:
	Node*	_gameRewardLayer;

	Sprite*	_curOptionNode;
	Sprite* _rewardItem[6];

	// ˢ��ʱ��
	int _replyTime;

	int _curSelectRole;
	int _MaxMoveNum;
	int	_moveNum;
	float _rotateDaley;
	int	_getGoodsID;

};

/*******************************************
*	�޾�ģʽ
*******************************************/
class WuJinModeLayer :public Layer
{
public:

	typedef enum ButtonTag
	{
		Tag_Close,			// �ر�

		Tag_ResetCard,		// ���ÿ���
		Tag_BuyGoods,		// �������

		Tag_GoodsChongCi,			// ����1
		Tag_GoodsHuDun,			// ����2
		Tag_GoodsDaZhao,			// ����3

		Tag_Start,			// ��ʼ

		Tag_GetRewarde,			// ��ȡ����
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
*	��ʯ
*******************************************/
class QuickStoneLayer :public Layer
{
public:
	enum ButtonTag
	{
		Tag_Back,			// ����

		Tag_ImplantLabel,	// ��Ƕ��ǩ
		Tag_CompoundLabel,	// �ϳɱ�ǩ

		Tag_OptionCard,			// ѡ����
		Tag_OptionQuickston,	//ѡ����ʯ
		Tag_CloseOptionCard,	// ѡ���ƹر�
		Tag_CloseOptionQuickston,//ѡ����ʯ�ر�

		Tag_StartImplant,	// ��ʼ��Ƕ
		Tag_StartCompount,	// ��ʼ�ϳ�

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

	// ѡ�����ص�
	void callFuncCardOptionEvent(Ref* ref);
	void callFuncQuickStonOptionEvent(Ref* ref);

	void setEventForNode(Button* node, ButtonTag tag);
	void callFuncButtonEvent(Ref* ref);
	void callOptionCompountQuickst(Ref* ref);



	void updateQuickStoneCompUI(void);
	// ����ѡ�������Ƕ����
	void updateCardWithCardId(int id);
	void updateQuickStonWithId(int id);

	// ��ư�ť״̬
	void setButtonTouchState(Button* button, bool state);

	// ��ʼ�ϳ���ʯ
	void playCompountQuickStone(void);
	void callFuncplayCompount(Node* node);
	void callFuncplayCompount2(Node* node);
	void callFuncReversal(Node* node);

	// ��ʼ��Ƕ��ʯ
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
