/********************************************************************
������:		2014/11/17  16:00	
�ļ���: 	E:\Cocos3.3_Project\MyGame\Classes\MenuLayer.h 
��  ��:		alex(alex)
�޸��ڣ�	2014/11/17  16:00	

Ŀ  ��:		�˵���
*********************************************************************/
#ifndef _MENU_LAYER_H_
#define _MENU_LAYER_H_


#include "cocos2d.h"
#include "cocos-ext.h"
#include "GUI/CCControlExtension/CCControlSlider.h"

USING_NS_CC;
USING_NS_CC_EXT;

class TimeCutDown;
class MenuLayer : public Layer
{
public:
	CREATE_FUNC(MenuLayer);
	
	virtual bool init();
	virtual void onEnter();

	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	//һϵ�в��԰�ť
	void TestMenus();

	void addTestMenu(const char* info, const Point& pos, SEL_MenuHandler callback);

	void MenuCB1(Ref* sender);
	void MenuCB2(Ref* sender);
	void MenuCB3(Ref* sender);
	void MenuCB4(Ref* sender);
	void MenuCB5(Ref* sender);
	void MenuCB6(Ref* sender);
	void MenuCB7(Ref* sender);
	void MenuCB8(Ref* sender);
	void MenuCB9(Ref* sender);
	void MenuCB10(Ref* sender);
	void MenuCB11(Ref* sender);
	void MenuCB12(Ref* sender);
	
	void menuSwitch();
	bool isTouchInArea(const Vec2& amiPos, const Vec2& curPos, float radius);

	void ChangeListeningState();


private:
	Size				m_winSize;
	std::list<Menu*>	m_menuList;
	
	CC_SYNTHESIZE(bool, m_isShowMenu, isShowMenu);

	bool				m_isListening;

	int					m_Pos_1_State;
	int					m_Pos_2_State;
	int					m_Pos_3_State;
	int					m_Pos_4_State;
	int					m_bulletId;
};



#endif