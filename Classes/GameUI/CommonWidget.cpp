#include "CommonWidget.h"
#include "GameMainLayer.h"
#include "GameCommon/CommonRes.h"
#include "GameRoleScene.h"
#include "../GameCommon/GameDefine.h"


bool CommonWidget::init()
{
	if ( !Layer::init() )
	{
		return false;
	}

	initInterface();

	return true;
}

void CommonWidget::onEnter()
{
	Layer::onEnter();

}

void CommonWidget::onExit( void )
{
	Layer::onExit();
}

void CommonWidget::initInterface( void )
{
	_commonWidget = CSLoader::createNode( Ccs_GameWidgetLayer_FilePath );

	if ( !_commonWidget )
	{
		return;
	}
	addChild( _commonWidget );

	do 
	{
		auto butExit = (Button*)GameMainLayer::seekNodeByName(_commonWidget,"ButtonClose");
		CC_BREAK_IF( butExit == nullptr );
		butExit->setTag( Tag_ExitWidget );
		butExit->addClickEventListener(CC_CALLBACK_1(CommonWidget::callFuncButtonEvent, this));

		auto butBuy = (Button*)GameMainLayer::seekNodeByName(_commonWidget,"ButtonConfirm");
		CC_BREAK_IF( butBuy == nullptr );
		butBuy->setTag( Tag_Btn1 );
		butBuy->addClickEventListener(CC_CALLBACK_1(CommonWidget::callFuncButtonEvent, this));

		auto textContene1			= (Text*)GameMainLayer::seekNodeByName(_commonWidget, "TextWidgetTips" );
		textContene1->setString( "" );

		auto textContene2			= (Text*)GameMainLayer::seekNodeByName(_commonWidget, "TextWidgetTips2" );
		textContene2->setString( "" );

	} while (0);

}

void CommonWidget::setContent(const  char* content1 /*= nullptr*/, const char* content2 /*= nullptr*/ )
{
	auto textContene1			= (Text*)GameMainLayer::seekNodeByName(_commonWidget, "TextWidgetTips" );
	if ( textContene1 && content1 )
	{
		textContene1->setString( content1 );
	}

	auto textContene2			= (Text*)GameMainLayer::seekNodeByName(_commonWidget, "TextWidgetTips2" );
	if ( textContene2 && content2 )
	{
		textContene2->setString( content2 );
	}
}

void CommonWidget::addBtn1ClickEventListener( const ccCommonWidgetBtnCallBack& callback )
{
	this->callFuncBtn1 = callback;
}

void CommonWidget::addBtn2ClickEventListener( const ccCommonWidgetBtnCallBack& callback )
{
	this->callFuncBtn2 = callback;
}

void CommonWidget::addBtnExitClickEventListener( const ccCommonWidgetBtnCallBack& callback )
{
	this->callFuncBtnExit = callback;
}

void CommonWidget::callFuncButtonEvent( Ref* ref )
{
	Button* btn = (Button*)ref;
	int nTag = btn->getTag();

	switch ( nTag )
	{
		// 退出
	case Tag_ExitWidget:
		{
			if ( !callFuncBtnExit )
			{
				removeFromParentAndCleanup(true);
			}
			else
			{
				callFuncBtnExit();
				removeFromParentAndCleanup(true);
			}
		}
		break;

	case Tag_Btn1:
		{
			if ( !callFuncBtn1 )
			{
				removeFromParentAndCleanup(true);
			}
			else
			{
				callFuncBtn1();
				removeFromParentAndCleanup(true);
			}
		}
		break;

	case Tag_Btn2:
		{
			if ( !callFuncBtn2 )
			{
				removeFromParentAndCleanup(true);
			}
			else
			{
				callFuncBtn2();
				removeFromParentAndCleanup(true);
			}
		}

	default:
		break;
	}


}


// 商店
void ShopBuyLayer::showLayout( Node* node, int tag, int labelTag )
{
	if ( node && !node->getChildByName("ShopBuyLayer") )
	{
		auto layout = ShopBuyLayer::create();
		layout->setTag( tag );
		layout->setoptionLabel( labelTag );
		node->addChild( layout, LocalZOrde_500, "ShopBuyLayer" );
	}
	else
	{
		auto layout = (ShopBuyLayer*)node->getChildByName("ShopBuyLayer");
		layout->optionLabel( (ButtonTag)labelTag );
	}
}

void ShopBuyLayer::onEnter()
{
	Layer::onEnter();

	initInterface();
}

void ShopBuyLayer::setEventForNode( Button* node, ButtonTag tag )
{
	do 
	{
		CC_BREAK_IF( node == nullptr );

		node->setTag( tag );
		node->addClickEventListener(CC_CALLBACK_1(ShopBuyLayer::callFuncButtonEvent, this));

	} while (0);
}

void ShopBuyLayer::callFuncButtonEvent( Ref* ref )
{
	Button* btn = (Button*)ref;
	int nTag = btn->getTag();


	switch ( nTag )
	{
	case Tag_Back:
		//removeFromParentAndCleanup( true );
		GameInterfaceMgr::getInstance()->closeCurInterface();
		break;

	case Tag_LabelGold:
	case Tag_LabelGem:
	case Tag_LabelWing:
		optionLabel( (ButtonTag)nTag );
		break;

	default:
		break;
	}
}

void ShopBuyLayer::initInterface()
{
	_widgetLayer = CSLoader::createNode( Ccs_ShopLayer_FilePath );

	if ( !_widgetLayer )
	{
		return;
	}

	addChild( _widgetLayer );

	do 
	{
		//返回
		auto ButtonBack = (Button*)GameMainLayer::seekNodeByName(_widgetLayer, "ButtonBack" );
		setEventForNode( ButtonBack, Tag_Back );

		//金币
		auto ButtonGold = (Button*)GameMainLayer::seekNodeByName(_widgetLayer, "ButtonGold" );
		setEventForNode( ButtonGold, Tag_LabelGold );
		//宝石
		auto ButtonGem = (Button*)GameMainLayer::seekNodeByName(_widgetLayer, "ButtonGem" );
		setEventForNode( ButtonGem, Tag_LabelGem );
		//翅膀
		auto ButtonWing = (Button*)GameMainLayer::seekNodeByName(_widgetLayer, "ButtonWing" );
		setEventForNode( ButtonWing, Tag_LabelWing );
		
		auto PageViewBuy = (PageView*)GameMainLayer::seekNodeByName(_widgetLayer, "PageViewBuy" );
		PageViewBuy->addEventListener( CC_CALLBACK_2(ShopBuyLayer::callFuncViewPage, this) );

		for ( int i = 1, j = 1, m = 1, n = 1; i<=CSV_MGR()->GetCsvShopList()->GetAll()->size(); i++ )
		{
			auto item =  NewDataMgr::getInstance()->getShopListWithID(i);

			switch ( item->BuyType )
			{
			case GoodsType_Gold:
				{
					auto ButtonGoldItem = (Button*)GameMainLayer::seekNodeByName(_widgetLayer, __String::createWithFormat("ButtonGoldItem%d", j)->getCString() );
					if ( ButtonGoldItem )
					{
						ButtonGoldItem->setTag( i );
						ButtonGoldItem->addTouchEventListener(CC_CALLBACK_2(ShopBuyLayer::callFuncBuyGoods, this));

						auto ALNumGoldItem		= (TextAtlas*)GameMainLayer::seekNodeByName(_widgetLayer, __String::createWithFormat("ALNumGoldItem%d", j)->getCString() );
						auto ALGemNumGoldItem	= (TextAtlas*)GameMainLayer::seekNodeByName(_widgetLayer, __String::createWithFormat("ALGemNumGoldItem%d", j)->getCString() );

						ALNumGoldItem->setString( __String::createWithFormat("%d", item->BuyNumber)->getCString() );
						ALGemNumGoldItem->setString( __String::createWithFormat("%d", item->BuyPrice)->getCString() );

						j++;
					}
				}
				break;
			case GoodsType_Gem:
				{
					auto ButtonGemItem = (Button*)GameMainLayer::seekNodeByName(_widgetLayer, __String::createWithFormat("ButtonGemItem%d", m)->getCString() );
					if ( ButtonGemItem )
					{
						ButtonGemItem->setTag( i );
						ButtonGemItem->addTouchEventListener(CC_CALLBACK_2(ShopBuyLayer::callFuncBuyGoods, this));

						auto ALNumGemItem		= (TextAtlas*)GameMainLayer::seekNodeByName(_widgetLayer, __String::createWithFormat("ALNumGemItem%d", m)->getCString() );
						auto ALMonNumGoldItem	= (TextAtlas*)GameMainLayer::seekNodeByName(_widgetLayer, __String::createWithFormat("ALMonNumGoldItem%d", m)->getCString() );

						ALNumGemItem->setString( __String::createWithFormat("%d", item->BuyNumber)->getCString() );
						ALMonNumGoldItem->setString( __String::createWithFormat("%d", item->BuyPrice)->getCString() );

						m++;
					}
				}
				break;
			case GoodsType_Life:
				{
					auto ButtonLifeItem = (Button*)GameMainLayer::seekNodeByName(_widgetLayer, __String::createWithFormat("ButtonLifeItem%d", n)->getCString() );
					if ( ButtonLifeItem )
					{
						ButtonLifeItem->setTag( i );
						ButtonLifeItem->addTouchEventListener(CC_CALLBACK_2(ShopBuyLayer::callFuncBuyGoods, this));

						auto ALNumLifeItem		= (TextAtlas*)GameMainLayer::seekNodeByName(_widgetLayer, __String::createWithFormat("ALNumLifeItem%d", n)->getCString() );
						auto ALGemNumLifeItem	= (TextAtlas*)GameMainLayer::seekNodeByName(_widgetLayer, __String::createWithFormat("ALGemNumLifeItem%d", n)->getCString() );

						ALNumLifeItem->setString( __String::createWithFormat("%d", item->BuyNumber)->getCString() );
						ALGemNumLifeItem->setString( __String::createWithFormat("%d", item->BuyPrice)->getCString() );

						n++;
					}
				}
				break;
			default:
				break;
			}
			
		}

		runAction( CCSequence::create( DelayTime::create(0.1f), CallFunc::create(this, callfunc_selector(ShopBuyLayer::delayOptionLabel) ), nullptr));
		//optionLabel( (ButtonTag)getoptionLabel() );

	} while (0);
}

void ShopBuyLayer::optionLabel( ButtonTag tag )
{
	auto SpritebutOption = (Sprite*)GameMainLayer::seekNodeByName(_widgetLayer, "SpritebutOption" );

	auto ButtonGold = (Button*)GameMainLayer::seekNodeByName(_widgetLayer, "ButtonGold" );
	auto ButtonGem = (Button*)GameMainLayer::seekNodeByName(_widgetLayer, "ButtonGem" );
	auto ButtonWing = (Button*)GameMainLayer::seekNodeByName(_widgetLayer, "ButtonWing" );

	auto PageViewBuy = (PageView*)GameMainLayer::seekNodeByName(_widgetLayer, "PageViewBuy" );

	SpritebutOption->setScale( 1.15f );
	auto action = ScaleTo::create(0.3f, 1.0f);
	SpritebutOption->runAction( action );
	switch ( tag )
	{
	case Tag_LabelGem:
		SpritebutOption->setPosition( ButtonGem->getPosition() );
		PageViewBuy->scrollToPage(1);
		break;

	case Tag_LabelGold:
		SpritebutOption->setPosition( ButtonGold->getPosition() );
		PageViewBuy->scrollToPage(0);
		break;

	case Tag_LabelWing:
		SpritebutOption->setPosition( ButtonWing->getPosition() );
		PageViewBuy->scrollToPage(2);
		break;
	default:
		break;
	}

}

void ShopBuyLayer::callFuncViewPage( Ref* ref, PageView::EventType event )
{
	auto PageViewBuy = (PageView*)GameMainLayer::seekNodeByName(_widgetLayer, "PageViewBuy" );
	auto SpritebutOption = (Sprite*)GameMainLayer::seekNodeByName(_widgetLayer, "SpritebutOption" );

	auto ButtonGold = (Button*)GameMainLayer::seekNodeByName(_widgetLayer, "ButtonGold" );
	auto ButtonGem = (Button*)GameMainLayer::seekNodeByName(_widgetLayer, "ButtonGem" );
	auto ButtonWing = (Button*)GameMainLayer::seekNodeByName(_widgetLayer, "ButtonWing" );

	switch ( PageViewBuy->getCurPageIndex() )
	{
	case 0:
		SpritebutOption->setPosition( ButtonGold->getPosition() );
		break;
	case 1:
		SpritebutOption->setPosition( ButtonGem->getPosition() );
		break;
	case 2:
		SpritebutOption->setPosition( ButtonWing->getPosition() );
		break;
	default:
		break;
	}
}

void ShopBuyLayer::callFuncBuyGoods( Ref* ref ,Widget::TouchEventType type )
{
	if ( Widget::TouchEventType::ENDED != type  ) return;

	Button* btn = (Button*)ref;
	int nTag = btn->getTag();

	auto item =  NewDataMgr::getInstance()->getShopListWithID(nTag);


	switch ( item->BuyType )
	{
	case GoodsType_Gold:
		{
			if ( NewDataMgr::getInstance()->getGem() >= item->BuyPrice )
			{
				NewDataMgr::getInstance()->addGold( item->BuyNumber );
				NewDataMgr::getInstance()->addGem( -item->BuyPrice );

				NotificationCenter::getInstance()->postNotification(Msg_BuySucceeEff,nullptr);
				NotificationCenter::getInstance()->postNotification(Msg_ChangeGemGold,nullptr);
			}
			else
			{
				auto widget = CommonWidget::create();
				widget->setContent( "宝石不足" );
				addChild( widget );
			}
		}
		break;

	case GoodsType_Gem:
		{
			NewDataMgr::getInstance()->addGem( item->BuyNumber );

			NotificationCenter::getInstance()->postNotification(Msg_BuySucceeEff,nullptr);
			NotificationCenter::getInstance()->postNotification(Msg_ChangeGemGold,nullptr);
		}
		break;

	case GoodsType_Life:
		{
			if ( NewDataMgr::getInstance()->getGem() >= item->BuyPrice )
			{
				NewDataMgr::getInstance()->addUserLife( item->BuyNumber );
				NewDataMgr::getInstance()->addGem( -item->BuyPrice );

				NotificationCenter::getInstance()->postNotification(Msg_BuySucceeEff,nullptr);
				NotificationCenter::getInstance()->postNotification(Msg_ChangeGemGold,nullptr);
			}
			else
			{
				auto widget = CommonWidget::create();
				widget->setContent( "宝石不足" );
				addChild( widget );
			}
		}
		break;
	default:
		break;
	}

}

void ShopBuyLayer::delayOptionLabel( void )
{
	optionLabel( (ButtonTag)getoptionLabel() );
}





// 奖励节点
RewardGoodsNode* RewardGoodsNode::create( RewardGoodsKey key )
{
	RewardGoodsNode *node = new (std::nothrow) RewardGoodsNode();
	if (node && node->initWithKey( key ))
	{
		node->autorelease();
		return node;
	}
	CC_SAFE_DELETE(node);
	return nullptr;
}
RewardGoodsNode* RewardGoodsNode::clone() const
{
	RewardGoodsNode *node = new (std::nothrow) RewardGoodsNode();
	if ( node )
	{
		node->_nodeTag = this->_nodeTag;
		node->initWithKey( _goodeskey );
		node->autorelease();
		return node;
	}
	CC_SAFE_DELETE(node);
	return nullptr;
}

bool RewardGoodsNode::initWithKey( RewardGoodsKey key )
{
	if ( 0 == _nodeTag) setTag( -1 );
	_goodeskey = key;

	switch ( key )
	{
	case RewardKey_1:
		{
			int randCardIndex = random(1,9);

			if ( 0 != _nodeTag)	randCardIndex = _nodeTag;
			setTag( randCardIndex );

			std::string cardFilePath = __String::createWithFormat("%s%d.png", Img_Card_FilePath, randCardIndex )->getCString();
			auto goods = Sprite::create( cardFilePath );
			goods->setAnchorPoint( Vec2(0.5, 0.5) );
			goods->setScale( 0.40f );
			goods->setPosition( Vec2(0, 25 ));
			goods->setName( "goods" );
			goods->setCascadeOpacityEnabled(true);
			addChild(goods);

			auto goodsK = Sprite::create( StringUtils::format("ccsRes/cardOptionLayer/CardFrame%d.png", (randCardIndex-1)/3+1) );
			goodsK->setScale( 1.7f);
			goodsK->setPosition(goods->getContentSize() / 2);
			goodsK->setCascadeOpacityEnabled(true);
			goods->addChild(goodsK);

			for ( int i = 0; i<4; i++ )
			{
				auto goodsStar = Sprite::create( "ccsRes/CompoundCardLayer/hecheng14.png" );
				goodsStar->setPosition( Vec2( 309 - i*50, 408) );
				goodsStar->setCascadeOpacityEnabled(true);
				goods->addChild(goodsStar);
			}
		}
		break;

	case RewardKey_2:
		{
			int randCardIndex = random(1,9);
			if ( 0 != _nodeTag)	randCardIndex = _nodeTag;
			setTag( randCardIndex );

			std::string cardFilePath = __String::createWithFormat("%s%d.png", Img_Card_FilePath, randCardIndex )->getCString();
			auto goods = Sprite::create( cardFilePath );
			goods->setAnchorPoint( Vec2(0.5, 0.5) );
			goods->setScale( 0.40f );
			goods->setPosition( Vec2(0, 25 ));
			goods->setName( "goods" );
			goods->setCascadeOpacityEnabled(true);
			addChild(goods);

			auto goodsK = Sprite::create( StringUtils::format("ccsRes/cardOptionLayer/CardFrame%d.png", (randCardIndex-1)/3+1) );
			goodsK->setScale( 1.7f);
			goodsK->setPosition( goods->getContentSize()/2 );
			goodsK->setCascadeOpacityEnabled(true);
			goods->addChild(goodsK);

			for ( int i = 0; i<3; i++ )
			{
				auto goodsStar = Sprite::create( "ccsRes/CompoundCardLayer/hecheng14.png" );
				goodsStar->setPosition( Vec2( 309 - i*50, 408) );
				goodsStar->setCascadeOpacityEnabled(true);
				goods->addChild(goodsStar);
			}

		}
		break;

	case RewardKey_3:
	case RewardKey_24:
		{
			auto goods = Sprite::create( "ccsRes/roleOptionLayer/juese5.png" );
			goods->setName( "goods" );
			goods->setPosition( Vec2(0, 10) );
			goods->setScale( 1.3f );
			addChild( goods );
		}
		break;

	case RewardKey_4:
	case RewardKey_5:
	case RewardKey_14:
	case RewardKey_21:
	case RewardKey_22:
	case RewardKey_23:
		{
			auto goods = Sprite::create( "ccsRes/ShopLayer/goumai10.png" );
			goods->setPosition( Vec2(0, 5) );
			goods->setName( "goods" );
			addChild( goods );
		}
		break;

	case RewardKey_6:
		{
			auto goods = Sprite::create( "ccsRes/ShopLayer/goumai21.png" );
			goods->setPosition( Vec2(0, 15) );
			goods->setName( "goods" );
			addChild( goods );
		}
		break;

	case RewardKey_7:
	case RewardKey_15:
		{
			int randCardIndex = random(1,9);
			if ( 0 != _nodeTag)	randCardIndex = _nodeTag;
			setTag( randCardIndex );

			std::string cardFilePath = __String::createWithFormat("%s%d.png", Img_Card_FilePath, randCardIndex )->getCString();
			auto goods = Sprite::create( cardFilePath );
			goods->setAnchorPoint( Vec2(0.5, 0.5) );
			goods->setScale( 0.40f );
			goods->setPosition( Vec2(0, 25 ));
			goods->setName( "goods" );
			goods->setCascadeOpacityEnabled(true);
			addChild(goods);

			auto goodsK = Sprite::create( StringUtils::format("ccsRes/cardOptionLayer/CardFrame%d.png", (randCardIndex-1)/3+1) );
			goodsK->setScale( 1.7f);
			goodsK->setPosition( goods->getContentSize()/2 );
			goodsK->setCascadeOpacityEnabled(true);
			goods->addChild(goodsK);

			for ( int i = 0; i<1; i++ )
			{
				auto goodsStar = Sprite::create( "ccsRes/CompoundCardLayer/hecheng14.png" );
				goodsStar->setPosition( Vec2( 309 - i*50, 408) );
				goodsStar->setCascadeOpacityEnabled(true);
				goods->addChild(goodsStar);
			}

		}
		break;

	case RewardKey_8:
	case RewardKey_19:
	case RewardKey_20:
		{
			int randCardIndex = random(1,9);
			if ( 0 != _nodeTag)	randCardIndex = _nodeTag;
			setTag( randCardIndex );

			std::string cardFilePath = __String::createWithFormat("%s%d.png", Img_Card_FilePath, randCardIndex )->getCString();
			auto goods = Sprite::create( cardFilePath );
			goods->setAnchorPoint( Vec2(0.5, 0.5) );
			goods->setScale( 0.40f );
			goods->setPosition( Vec2(0, 25 ));
			goods->setName( "goods" );
			goods->setCascadeOpacityEnabled(true);
			addChild(goods);

			auto goodsK = Sprite::create( StringUtils::format("ccsRes/cardOptionLayer/CardFrame%d.png", (randCardIndex-1)/3+1) );
			goodsK->setScale( 1.7f);
			goodsK->setPosition( goods->getContentSize()/2 );
			goodsK->setCascadeOpacityEnabled(true);
			goods->addChild(goodsK);

			for ( int i = 0; i<2; i++ )
			{
				auto goodsStar = Sprite::create( "ccsRes/CompoundCardLayer/hecheng14.png" );
				goodsStar->setPosition( Vec2( 309 - i*50, 408) );
				goodsStar->setCascadeOpacityEnabled(true);
				goods->addChild(goodsStar);
			}
		}
		break;

	case RewardKey_9:
		{
			auto goods = Sprite::create( "ccsRes/roleOptionLayer/juese3.png" );
			goods->setPosition( Vec2(0, 10) );
			goods->setName( "goods" );
			goods->setScale( 1.3f );
			addChild( goods );
		}
		break;

	case RewardKey_10:
	case RewardKey_16:
	case RewardKey_17:
	case RewardKey_18:
		{
			auto goods = Sprite::create( "ccsRes/roleOptionLayer/juese4.png" );
			goods->setPosition( Vec2(0, 10) );
			goods->setScale( 1.3f );
			goods->setName( "goods" );
			addChild( goods );
		}
		break;

	case RewardKey_11:
		{
			auto goods = Sprite::create( "ccsRes/WujinModeLayer/goodschongci.png" );
			goods->setPosition( Vec2(0, 25) );
			goods->setName( "goods" );
			addChild( goods );
		}
		break;

	case RewardKey_12:
		{
			auto goods = Sprite::create( "ccsRes/WujinModeLayer/goodsHudun.png" );
			goods->setPosition( Vec2(0, 25) );
			goods->setName( "goods" );
			addChild( goods );
		}
		break;

	case RewardKey_13:
		{
			auto goods = Sprite::create( "ccsRes/WujinModeLayer/dazhao.png" );
			goods->setPosition( Vec2(0, 25) );
			goods->setName( "goods" );
			addChild( goods );
		}
		break;

	default:
		break;
	}

	_nodeTag = getTag();

	// 数量
// 	auto goodsNum = TextAtlas::create( __String::createWithFormat(".%d", NewDataMgr::getInstance()->getRewardGoodsListWithKey( key )->GoodsNum )->getCString(), 
// 		"ccsRes/AtlasLabel/jiesuan22.png", 20,28, ".");
// 	goodsNum->setPosition(Vec2(0, -60));
// 	addChild( goodsNum );

	return true;
}
