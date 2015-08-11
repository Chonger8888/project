#ifndef __ACTION_RECORD_H_
#define __ACTION_RECORD_H_


#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"

USING_NS_CC;
using namespace cocos2d::network;

/************************************************************************/
/* 用户行为记录点                                                       */
/************************************************************************/

// 单件声明模板
#define SINGLETON_DECLEAR( className )	public:\
	static className	*getInstance(){ if( inst == NULL ) { inst = new (std::nothrow) className(); }	return inst; }\
	static void			purgeInstance(){ if( inst != NULL ) { CC_SAFE_DELETE(inst);	inst = NULL; } }\
										private:\
										static className	*inst;


// 单件定义模板。在cpp开始调用											
#define SINGLETON_DEFINE( className )	className	*className::inst = NULL;


#define strConversion(s)	#s

#define ActionRecord_Url	"http://guest.candy.weme-game.com/plane.analysis/dispatch.php?v_class=1&v_cmd=0&batch_tracker="

typedef enum
{
	RECODING_MSG_ACTIONID_1001, 			// 飞机升级		
	RECODING_MSG_ACTIONID_1002, 			// 飞机解锁		
	RECODING_MSG_ACTIONID_1003, 			// 飞机出战次数		
	RECODING_MSG_ACTIONID_1004, 			// 无尽关进入次数		
	RECODING_MSG_ACTIONID_1005, 			// 普通关过关状态

	RECODING_MSG_NUM	  	
}RecordingMsgType;



class userAction
{
	SINGLETON_DECLEAR( userAction );

public:

	userAction();
	~userAction();
	
	CC_SYNTHESIZE(std::string, devicesId, DevicesId);

	// 角色出战统计
	CC_SYNTHESIZE(int, _Role1FightCount, Role1FightCount);
	CC_SYNTHESIZE(int, _Role2FightCount, Role2FightCount);
	CC_SYNTHESIZE(int, _Role3FightCount, Role3FightCount);

	void sendRecordingMsg(RecordingMsgType type, ccHttpRequestCallback	pSelector, const char* string1=NULL,const char *string2=NULL, 
						const char *string3=NULL,const char *string4=NULL,const char *string5=NULL);

private:
	void setRecordHeadMsg(RecordingMsgType type );
	void setRecordMsgAppend(const  char* string);
	void sendMsg(const char *url,ccHttpRequestCallback	pSelector );
private:
	std::string				_Recordingdata;



};



#endif // !__ACTION_RECORD_H_
