/********************************************************************
������:		2015/02/28  14:09	
�ļ���: 	E:\Cocos3.3_Project\PlaneGame\Classes\Util\CppCallJava.h 
��  ��:		alex()
�޸��ڣ�	2015/02/28  14:09	

Ŀ  ��:		C++����JAVA�Ĺ��ܴ���
*********************************************************************/
#ifndef __CppCallJava_H__
#define __CppCallJava_H__


#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CppCallJava : public Ref
{
public:
	static CppCallJava* getInstance()
	{
		static CppCallJava instance;
		return &instance;
	}

	//�ֻ���,����ʱ��
	void vibrate(long time);

	//�ֻ��𶯣���2��
	void vibrateWithPattern();

	//���ϵͳʱ��
	std::string getSystemTime( void );
	
};

#endif // __CppCallJava_H__