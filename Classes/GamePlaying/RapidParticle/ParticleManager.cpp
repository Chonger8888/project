#include "ParticleManager.h"



RapidParticle* ParticleManager::getParticle( const std::string& file )
{
	RapidParticle* particle = nullptr;
	RapidParticle* exsitParticle = nullptr;
	auto key = file;

	bool isExsit = false;
	if (m_ParticleFileMap.empty())
	{
		isExsit = false;
	}
	else
	{
		Map<std::string, RapidParticle*>::iterator it;
		it = m_ParticleFileMap.find(key);
		if (it == m_ParticleFileMap.end())
		{
			//没找到
			isExsit = false;
		}
		else
		{
			isExsit = true;
			exsitParticle = (*it).second;
		}
	}

	//判断文件是否读取过，否，则读取存储;是，则返回值
	if (!isExsit)
	{
		auto plistData = FileUtils::getInstance()->getValueMapFromFile(file);
		particle = RapidParticle::create(file);
		particle->retain();

		m_ParticleFileMap.insert(key, particle);
	}
	else
	{
		particle = RapidParticle::createWithExsitParticle(exsitParticle);
	}

	return particle;
}

void ParticleManager::purge()
{
	releaseAllParticle();
}

void ParticleManager::releaseAllParticle()
{
	Map<std::string, RapidParticle*>::iterator it = m_ParticleFileMap.begin();
	for (it; it != m_ParticleFileMap.end(); it++)
	{
		RapidParticle* particle = (*it).second;
		particle->release();
	}
	m_ParticleFileMap.clear();
}
