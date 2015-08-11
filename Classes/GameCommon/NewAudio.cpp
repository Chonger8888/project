#include "NewAudio.h"



NewAudio::NewAudio()
{
	_SoundEffState = false;
}

void NewAudio::playBgMusicForce( const std::string& file)
{
	bool isplay = isBGMPlaying(file);
	if ( isplay )
	{
		return;
	}
	//删除列表中正在播放的该背景音乐
	for (auto it = m_MusicVec.begin(); it != m_MusicVec.end(); it++)
	{
		MusicPro& mp = (*it);
		
		AudioEngine::stop(mp.id);
	}
	m_MusicVec.clear();


	MusicPro mp;

	if ( !_mucicState )
		mp.id = 0;
	else
		mp.id = AudioEngine::play2d(file,true,0.5f,nullptr);

	mp.Musicfile = file;

	m_MusicVec.push_back(mp);
}

bool NewAudio::isBGMPlaying( const std::string& file)
{
	bool b = false;
	for (auto it = m_MusicVec.begin(); it != m_MusicVec.end(); it++)
	{
		MusicPro& mp = (*it);
		if (mp.Musicfile == file)
		{
			b = true;
			break;;
		}
	}

	return b;
}

void NewAudio::playEff(const std::string& file )
{
	if ( !_SoundEffState )
		return;

	int id = AudioEngine::play2d(file);
}

void NewAudio::playEffWithPar( const std::string& filePath, bool loop /*= false*/, float volume /*= 1.0f*/, const AudioProfile *profile /*= nullptr*/ )
{
	if( _SoundEffState )
	{
		int id = AudioEngine::play2d(filePath,loop,volume,profile);
	}
}

void NewAudio::setMusicState( bool state )
{
	_mucicState = state;
#ifdef WIN32 
	_mucicState = false;
#endif

	if ( !_mucicState )
	{
		//删除列表中正在播放的该背景音乐
		for (auto it = m_MusicVec.begin(); it != m_MusicVec.end(); it++)
		{
			MusicPro& mp = (*it);

			AudioEngine::stop(mp.id);
		}
	}
	else
	{
		if ( !m_MusicVec.empty() )
		{
			auto it = m_MusicVec.begin();
			it->id = AudioEngine::play2d( it->Musicfile );
		}
	}
}

bool NewAudio::getMusicState( void )
{
	return _mucicState;
}
