/********************************************************************
创建于:		2015/03/05  14:57	
文件名: 	E:\Cocos3.3_Project\PlaneGame\Classes\Common\NewAudio.h 
作  者:		alex()
修改于：	2015/03/05  14:57	

目  的:		
*********************************************************************/
#ifndef __NewAudio_H__
#define __NewAudio_H__


#include "cocos2d.h"
#include "cocos-ext.h"
#include "../../cocos2d/cocos/audio/include/AudioEngine.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace experimental;

#define Music_Bg					"Music/Music_Bg.ogg"
#define Music_Bg_Play				"Music/Music_Bg_Play.ogg"
#define Music_BallContact1			"Music/Music_BallContact1.mp3"
#define Music_BallContact2			"Music/Music_BallContact2.mp3"
#define Music_BallContact3			"Music/Music_BallContact3.mp3"
#define Music_BossIn				"Music/Music_BossIn.mp3"		//OK
#define Music_getDrops				"Music/Music_getDrops.mp3"		//OK
#define Music_getGolds				"Music/Music_getGolds.mp3"		//OK
#define Music_shootAmmo1			"Music/Music_shootAmmo1.mp3"	//OK
#define Music_shootAmmo2			"Music/Music_shootAmmo2.mp3"	//OK
#define Music_shootAmmo3			"Music/Music_shootAmmo3.mp3"	//OK
#define Music_shootAmmo4			"Music/Music_shootAmmo4.mp3"	//OK
#define Music_mainPlDead			"Music/Music_mainPlDead.mp3"	//OK
#define Music_MainPlDeadVoice1		"Music/Music_MainPlDeadVoice1.mp3"	//OK
#define Music_MainPlDeadVoice2		"Music/Music_MainPlDeadVoice2.mp3"	//OK
#define Music_MainPlDeadVoice3		"Music/Music_MainPlDeadVoice3.mp3"	//OK
#define Music_PlDead1				"Music/Music_PlDead1.mp3"		//OK
#define Music_PlDead2				"Music/Music_PlDead2.mp3"		//OK
#define Music_PlDead3				"Music/Music_PlDead3.mp3"		//OK
#define Music_PlDead4				"Music/Music_PlDead4.mp3"		//OK
#define Music_PlDead5				"Music/Music_PlDead5.mp3"		//OK
#define Music_Boss1Ready			"Music/Boss1Ready.mp3"			//OK
#define Music_Boss1Shoot1			"Music/Boss1Shoot1.mp3"			//OK
#define Music_Boss1Shoot2			"Music/Boss1Shoot2.mp3"			//OK
#define Music_Boss1Dead				"Music/Music_Boss1Dead.mp3"		
#define Music_SpeedOver				"Music/Music_SpeedOver.mp3"		//OK

#define Music_Menu					"Music/Music_Menu.mp3"
#define Music_Buy					"Music/Music_Buy.mp3"

//特殊子弹
#define Music_Cat					"Music/Music_Cat.mp3"			//OK
#define Music_Dragon				"Music/Music_Dragon.mp3"		//OK
#define Music_Feibiao				"Music/Music_Feibiao.mp3"		//OK
#define Music_Flower				"Music/Music_Flower.mp3"		//OK
#define Music_IceMissile			"Music/Music_IceMissile.mp3"	//OK
#define Music_Laser					"Music/Music_Laser.mp3"			//OK



typedef struct MusicPro
{
	int			id;
	std::string	Musicfile;

	MusicPro()
	{
		id = -1;
		Musicfile = "";
	}
}MusicPro;


class NewAudio
{
public:
	static NewAudio* getInstance()
	{
		static NewAudio instance;
		return &instance;
	}


	CC_SYNTHESIZE(bool,_SoundEffState,SoundEffState);		//音效
	
	void setMusicState( bool state );//音乐
	bool getMusicState( void );

	//如果当前播放的音乐就是即将要播放的音乐，那么不再播放，否则强制停止当前背景音乐，播放新的背景音乐
	void playBgMusicForce(const std::string& file);

	//是否正在播放
	bool isBGMPlaying(const std::string& file);

	void playEff(const std::string& file);
	void playEffWithPar(const std::string& filePath, bool loop = false, float volume = 1.0f, const AudioProfile *profile = nullptr);
private:
	NewAudio();
	std::vector<MusicPro>		m_MusicVec;

	bool		_mucicState;
};


#endif // __NewAudio_H__