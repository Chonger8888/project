/********************************************************************
������:		2014/12/30  15:19	
�ļ���: 	E:/Cocos3.3_Project\MyGame\Classes\GameHelp.h 
��  ��:		alex(alex)
�޸��ڣ�	2014/12/30  15:19	

Ŀ  ��:		��Ϸ����/��ʾ
*********************************************************************/
#ifndef __GameHelp_h__
#define __GameHelp_h__


#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

/************************************************************************/
/* 
��Ϸָ��
*/
/************************************************************************/
class GameHelp : public Sprite
{
public:
	static GameHelp* create();

	


private:
	void addMenu();
	void confirmCallback(Ref* sender);
};


#endif // GameHelp_h__