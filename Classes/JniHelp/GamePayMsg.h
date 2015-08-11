#ifndef __GAMEPAY_MSG_H__
#define __GAMEPAY_MSG_H__

#include "cocos2d.h"

USING_NS_CC;


/************************************************************************/
/* ��Ϸ֧������                                                        */
/************************************************************************/

enum PayKay
{
	PayKay_Test1,						// ֧������
	PayKay_Test2,						// ֧������
	PayKay_Test3,						// ֧������
	PayKay_Test4,						// ֧������
	PayKay_Test5,						// ֧������
	PayKay_Test6,						// ֧������
	PayKay_Test7,						// ֧������
	PayKay_Test8,						// ֧������
	PayKay_Test9,						// ֧������

	Paykey_End
};

// �Ʒ���Ϣ
struct PayInfoStr
{
	std::string		payCode;	// �ƷѴ���
	std::string		payName;	// �Ʒ�����
	int				payPrice;	// �Ʒѽ��
};


class GamePayMsg
{
public:
	static GamePayMsg* getInstance()
	{
		static GamePayMsg Instance;
		return &Instance;
	}

	GamePayMsg();
	~GamePayMsg();

	void GamePay( PayKay key );

	void JniCallJavaPay( std::string  payInfo );

private:

};





#endif