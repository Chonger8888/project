/********************************************************************
创建于:		2015/01/05  10:19	
文件名: 	E:\Cocos3.3_Project\MyGame\Classes\ContactManager.h 
作  者:		alex(alex)
修改于：	2015/01/05  10:19	

目  的:		监听物理世界的碰撞事件，事件处理分发给具体对象
*********************************************************************/
#ifndef __ContactManager_h__
#define __ContactManager_h__


#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

/************************************************************************/
/* 
碰撞处理类
*/
/************************************************************************/

class ContactManager : public Layer
{
public:
	CREATE_FUNC(ContactManager);
	virtual void onEnter();
	virtual void onExit();
	bool onContactBegin(const PhysicsContact& contact);

};


#endif // ContactManager_h__