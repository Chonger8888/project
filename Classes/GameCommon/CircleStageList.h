#ifndef __CIRCLE_STAGE_LIST_H__
#define __CIRCLE_STAGE_LIST_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;

using namespace cocostudio;


//����ڵ㡪����ѡ��ɫ
class CandidateRole :public CCNode{
public:
	CandidateRole();
	~CandidateRole();
	virtual bool init(const char* name);
	static CandidateRole* create(const char* name);
	CandidateRole* mpNextRole; //�ڵ������ɫ��ָ��
	CandidateRole* mpForeRole;  //..��..........     

	void setPositionIndex(int );
	int getPositionIndex();   

	int mDistance;     //����Բ��͵�ľ���
	int mPositionIndex;     //�ڵ��������е����
	int mNextPositionX;    
	int mNextPositionY;
	int mColorR;
	int mColorG;
	int mColorB;
	Sprite* mpArmature;
	bool mbIsSelected;   // �Ƿ�ѡ��

};


//�ɺ�ѡ��ɫ��ɵ���������Ļ�ϳ���Բ�ֲ�
class CircleStage :public CCNode
{

public:
	typedef std::function<void(int)> onCircleEndCallBack;

	CircleStage();
	~CircleStage();
	virtual bool init();

	CREATE_FUNC(CircleStage);
	void addRole(CandidateRole *);    //���һ����ɫ������
	void changeToNext( float t = 0.4f );    //˳ʱ����ת
	void changeToFore( float t = 0.4f );    //��ʱ����ת

	void callbackSetCurrentRole1( void );
	void callbackSetCurrentRole2( void );
	void setCurrentRole( int index );
	bool isOnAction();
	void actionEnd();

public:
	onCircleEndCallBack callFuncCircleEnd;	// end


private:
	int mCountActionEndNum;
	bool mbOnAction;
	int mRoleNum;
	CandidateRole* mpCurrentRole;     //���������õ�ָ��
	CandidateRole* mpHead;
	CandidateRole* mpLast;
	CandidateRole* mpSelectRole;    //��ѡ�еĽ�ɫ������Բ��͵�
	int mCircleA;    //��Բ��a
	int mCircleB;    //��Բ��b
	void updateColor();    //����ÿ���ڵ��������ԽԶԽ��
	void updateZorders();    //�����ڸǹ�ϵ
	void updateZordersCallBack();
	void updateDistances();    //����ÿ���ڵ㵽����ڵ�ľ���
	void initAppearance();    //��ʼ����ɫ������

	int mTargetIndex;

};




#endif
