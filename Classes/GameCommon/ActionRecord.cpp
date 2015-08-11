#include "ActionRecord.h"


SINGLETON_DEFINE( userAction );

char* RecordingMsgId[RECODING_MSG_NUM] = 
{
	(char*)"1001",						// �ɻ�����		
	(char*)"1002",						// �ɻ�����		
	(char*)"1003",						// �ɻ���ս����	
	(char*)"1004",						// �޾��ؽ������
	(char*)"1005",						// ��ͨ�ؽ���
};


userAction::userAction()
{
#ifdef WIN32
	setDevicesId("54351551");
#endif
}

userAction::~userAction()
{
}

void userAction::setRecordHeadMsg(RecordingMsgType type )																							
{				

	_Recordingdata.clear();																															
	_Recordingdata.append( ActionRecord_Url );								
	_Recordingdata.append( "1" );								
	_Recordingdata.append( "^" );																							
	_Recordingdata.append( RecordingMsgId[type] );																									
	_Recordingdata.append( "^" );																							
	_Recordingdata.append( getDevicesId().c_str() );																				
}

void userAction::setRecordMsgAppend( const char* string )
{
	_Recordingdata.append( "^" );
	if(string)
		_Recordingdata.append( string );
}

void userAction::sendRecordingMsg( RecordingMsgType type, ccHttpRequestCallback	pSelector, const char* string1/*=NULL*/,const char *string2/*=NULL*/, const char *string3/*=NULL*/,const char *string4/*=NULL*/,const char *string5/*=NULL*/ )
{
	if(type <0 || type>=RECODING_MSG_NUM)
		return ;

	setRecordHeadMsg(type);
	setRecordMsgAppend(string1);
	setRecordMsgAppend(string2);
	setRecordMsgAppend(string3);
	setRecordMsgAppend(string4);
	setRecordMsgAppend(string5);
	
	CCLOG("send message data[%s]", _Recordingdata.c_str() );
	sendMsg( _Recordingdata.c_str() , pSelector );
}

void userAction::sendMsg(const char *url, ccHttpRequestCallback	pSelector )
{
	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl( url );
	request->setRequestType( HttpRequest::Type::GET );
	request->setResponseCallback( pSelector);
// 	HttpClient::getInstance()->setTimeoutForConnect(30);
// 	HttpClient::getInstance()->setTimeoutForRead(30);
	HttpClient::getInstance()->send(request);
	request->release();
}

