/********************************************************************
������:		2014/12/25  14:56	
�ļ���: 	E:/Cocos3.3_Project/MyGame/Classes/Buff.h 
��  ��:		alex(alex)
�޸��ڣ�	2014/12/25  14:56	

Ŀ  ��:		�ɻ���Ԫ��BUFF
*********************************************************************/
#ifndef __Buff_h__
#define __Buff_h__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "../GameCommon/GameDefine.h"

USING_NS_CC;
USING_NS_CC_EXT;

class BuffManager;
class BaseBuff : public Node
{
public:
	static BaseBuff* create(BuffManager* buffmanager);

	/**
	@���ܣ�����BUFF�������ǰ�д�BUFF����ôʱ������
	@������1.Node��Ҫ��BUFF�Ľڵ㣻
			2.BuffId��Ҫ�����buff��Id
			3.float��BUFF������ʱ��
	@���أ���
	*/
	void ActiveBuff(Node* node, const BuffId& sid, const float& duration);

	/**
	@���ܣ�����BUFFδ��ʧ֮ǰ���ֳ�������ͬBUFF����ô���ø�BUFF��ʱ��Ϊ���ֵ
	@��������
	@���أ���
	*/
	void resetTime();

	//ȡ�õ�ǰʣ��ʱ��
	float getLeftTime();

	//ȡ�õ�ǰBUFF��ID
	BuffId getBuffId();

	//BUFFʱ�䵽��ɾ��
	void removeBuff();

private:
	//BUFFʱ�䵹��
	void CutDown();

	/**
	@���ܣ���BUFF�������������
	@������BuffManager��BUFF������
	@���أ���
	*/
	void setManager(BuffManager* buffmanager);

	/**
	@���ܣ�����BUFF��idѡ�����BUFFЧ������ʾ,���Ҵ���BUFF������
	@������BuffId: BUFF��ID
	@���أ���
	*/
	void addBUffById(const BuffId& buffid);

	/**
	@���ܣ�����BUFF��idѡ��Ҫ�Ƴ���BUFFЧ������ʾ�����Ҵ�BUFF������ɾ��
	@������BuffId: BUFF��ID
	@���أ���
	*/
	void removeBuffById(const BuffId& buffid);

private:

	Node*		m_buffNode;		//BUFFʩ�ӵĶ���
	BuffId		m_buffid;
	float		m_LeftTime;
	float		m_maxTime;		

	BuffManager* m_buffManager;
};


/************************************************************************/
/* 
BUFF�����������ܹ�����BUFF�Ľڵ���̳У�������BUFF
*/
/************************************************************************/
class BuffManager 
{
public:

	/**
	@���ܣ��ж�ĳ��buff�Ƿ񼤻�
	@������BuffId��	BUFF��id
	@���أ�bool������Ǽ���״̬����true�����򷵻�false
	*/
	bool isBuffActive(const BuffId& buffid);

	/**
	@���ܣ�����ĳ��BUFF��ʱ��
	@������BuffId��BUFF��id
	@���أ�
	*/
	void resetBuff(const BuffId& buffid);

	//����BUFF
	void addBuff(BaseBuff* buff);

	//ɾ����ӦBUFF
	void DelBuff(BaseBuff* buff);

private:

	std::vector<BaseBuff*>		m_buffVec;

};

#endif // Buff_h__