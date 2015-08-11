#ifndef CloudLayer_h__
#define CloudLayer_h__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CloudLayer : public Layer
{
public:
	
	CREATE_FUNC(CloudLayer);

	virtual void onEnter();

	void addOneCloud();

	//º”ÀŸ
	void SpeedUp();

private:
	Size		m_ws;
	bool		m_isCloudSpeedUp;
};
#endif // CloudLayer_h__