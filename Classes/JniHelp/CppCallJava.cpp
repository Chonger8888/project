#include "CppCallJava.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)//预编译 判断是否是android平台
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif


void CppCallJava::vibrate( long time )
{
	return;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo minfo;
	
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/weme/game/JniHelp/CppCallJava","vibrate","(I)V");
	if (isHave)
	{
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, time);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
	else 
	{
		CCLog("### java func vibrate() is not exist ### ");
	}

#endif
}

void CppCallJava::vibrateWithPattern()
{
	return;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo minfo;

	bool isHave = JniHelper::getStaticMethodInfo(minfo,"org/weme/game/JniHelp/CppCallJava","vibrateWithPattern","()V");
	if (isHave)
	{
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, time);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
	else 
	{
		CCLog("### java func vibrateWithPattern() is not exist ### ");
	}

#endif
}

std::string  CppCallJava::getSystemTime( void )
{
	std::string curTime;

	//CCLog("### java func getSystemTime()### ");
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )

	time_t curtime=time(0); 
	tm tim =*localtime(&curtime); 
	int  day, mon, year, hour, min, sec; 
	day = tim.tm_mday;
	mon = tim.tm_mon;
	year = tim.tm_year;
	hour = tim.tm_hour;
	min = tim.tm_min;
	sec = tim.tm_sec;
	
	char curTimeBuffer[256];
	sprintf_s( curTimeBuffer, sizeof(curTimeBuffer), "%d||%d||%d||%d||%d||%d||", year, mon, day, hour, min, sec );
	curTime = curTimeBuffer;

#elif ( CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
	JniMethodInfo minfo;

	CCLog("### java func getSystemTime()### ");
	if ( JniHelper::getStaticMethodInfo(minfo,"org/weme/game/JniHelp/CppCallJava","getCurSystemTime","()Ljava/lang/String;"))
	{
		CCLog("### java func getSystemTime()### ");
		jstring time = (jstring)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		curTime = (char*)minfo.env->GetStringUTFChars(time, NULL);
		minfo.env->DeleteLocalRef(minfo.classID);
	}
	else 
	{
		CCLog("### java func getSystemTime() is not exist ### ");
	}


#endif
	
	CCLOG("CppCallJava::getSystemTime time:%s", curTime.c_str());
	return curTime;
}

