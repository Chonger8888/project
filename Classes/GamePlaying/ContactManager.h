/********************************************************************
������:		2015/01/05  10:19	
�ļ���: 	E:\Cocos3.3_Project\MyGame\Classes\ContactManager.h 
��  ��:		alex(alex)
�޸��ڣ�	2015/01/05  10:19	

Ŀ  ��:		���������������ײ�¼����¼�����ַ����������
*********************************************************************/
#ifndef __ContactManager_h__
#define __ContactManager_h__


#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

/************************************************************************/
/* 
��ײ������
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