#ifndef __LABELNUM_ACTION_H__
#define __LABELNUM_ACTION_H__


/************************************************************************/
/* ���ֱ�ǩ��������                                                     */
/************************************************************************/
#include "cocos2d.h"
#include "2d/CCActionInterval.h"//����ϵͳ��ʱ�ද��ͷ�ļ�  
#include "cocos-ext.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d::ui;

class  LabelNumAction : public CCActionInterval
{
public:
	/** creates the action */
	static LabelNumAction* create(float d, int targetnum);

	//
	// Overrides
	//
	virtual void update(float time);

private:
	void initWithTimeTarget(float t,int targetnum);

private:
	Node*		_nodeTarget;
	float		_runTime;
	int			_targetNum;

	int			_countT;
	float		_curTime;

};


#endif // !__LABELNUM_ACTION_H__
