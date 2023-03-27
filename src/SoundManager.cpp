#include "vendor/NixSoundPlayer/SoundManager.h"

SoundManager::SoundManager()
    :System("SoundManager")
{
}

SoundManager::~SoundManager()
{
	for (size_t i = 0; i < m_SoundBufferVector.size(); i++)
	{
		m_SoundBufferVector[i].data.get()->~SoundBuffer();
	}

	for (size_t i = 0; i < m_QueuedSoundBufferVector.size(); i++)
	{
		m_QueuedSoundBufferVector[i].data.get()->~QueuedSoundBuffer();
	}
}

void SoundManager::Update()
{
	for (size_t i = 0; i < m_QueuedSoundBufferVector.size(); i++)
	{
		if (m_QueuedSoundBufferVector[i].isPlaying)
		{
			m_QueuedSoundBufferVector[i].data.get()->UpdateBufferStream();
		}
		else if(!m_QueuedSoundBufferVector[i].isFileClosed)
		{
			m_QueuedSoundBufferVector[i].data.get()->CloseFile();
			m_QueuedSoundBufferVector[i].isFileClosed = true;
		}
	}
}

void SoundManager::AddSound(std::string componentName, std::string filePath)
{
    directory.changePath(filePath);
	SF_INFO sfinfo;
	ALint format;
    
    //Try to open the file and check that is usable
    SNDFILE* file = sf_open(directory.getPath().c_str(), SFM_READ, &sfinfo);

	//Check file is working
	int error = sf_error(file);
	std::stringstream error_str;
	error_str << error;
	if (error != 0)
	{
		debugger.giveMessage(Debugger::Error, "SndFile Error value", error_str.str());
		return;
	}

	//Check Frame is appropiate
	error_str.str(std::string()); // Flush stringstream
	error_str << sfinfo.frames;
	if (sfinfo.frames < 1)
	{
		debugger.giveMessage(Debugger::Error, "Bad sample count", error_str.str());
		return;
	}

	//Check if file is vorbis encoded
	if ((sfinfo.format & SF_FORMAT_SUBMASK) != SF_FORMAT_VORBIS)
	{
		debugger.giveMessage(Debugger::Error, "SoundPlayer only work with vorbis encoding");
		return;
	}

	//Check channel format
	switch (sfinfo.channels)
	{
	case(1):
		format = AL_FORMAT_MONO16;
		break;
	case(2):
		format = AL_FORMAT_STEREO16;
		break;
	case(3):
		if (sf_command(file, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT2D_16;
		break;
	case(4):
		if (sf_command(file, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT3D_16;
		break;
	default:
		debugger.giveMessage(Debugger::Error, "Sound channels are not supported!");
		sf_close(file);
		return;
	}

	//If file is bigger than 512kb than put it in QueuedSoundBuffer otherwise put it in SoundBuffer
	size_t size_in_bytes = (size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short);
	if (size_in_bytes < 524288)
	{
		//Put it in SoundBuffer
		SoundBufferData result;

		result.data = std::make_shared<SoundBuffer>(componentName, file, sfinfo, format);
		result.isPlaying = false;

		m_SoundBufferVector.emplace_back(result);

		size_t index = m_SoundBufferVector.size() - 1;
		m_PointMap[componentName] = { false, index };
	}
	else
	{
		//Put it in QueuedSoundBuffer
		QueuedSoundBufferData result;

		result.data = std::make_shared<QueuedSoundBuffer>(componentName, filePath, sfinfo, format);
		result.isPlaying = false;

		m_QueuedSoundBufferVector.emplace_back(result);

		size_t index = m_QueuedSoundBufferVector.size() - 1;
		m_PointMap[componentName] = { true, index };
	}
	
	sf_close(file);
}

void SoundManager::PlaySound(SoundSource* source, std::string componentName)
{
	pointList temp = m_PointMap[componentName];
	
	if (temp.isQueued)
	{
		QueuedSoundBuffer::bufferTuple result = m_QueuedSoundBufferVector[temp.index].data.get()->LoadPlay(source->GetSource());
		m_QueuedSoundBufferVector[temp.index].isPlaying = true;
		m_QueuedSoundBufferVector[temp.index].isFileClosed = false;
		source->Play(result.buffers, result.size);
	}
	else
	{
		ALuint buffer = 0;
		buffer = m_SoundBufferVector[temp.index].data.get()->getBuffer();
		m_SoundBufferVector[temp.index].isPlaying = true;
		source->Play(buffer);
	}
}

void SoundManager::StopSound(SoundSource* source, std::string componentName)
{
	pointList temp = m_PointMap[componentName];
	if (temp.isQueued)
	{
		m_QueuedSoundBufferVector[temp.index].isPlaying = false;
		m_QueuedSoundBufferVector[temp.index].isFileClosed = false;
		source->Stop();
	}
	else
	{
		m_SoundBufferVector[temp.index].isPlaying = false;
		source->Stop();
	}
}

ALuint SoundManager::getBuffer(std::string componentName)
{
	pointList temp = m_PointMap[componentName];
	if (temp.isQueued)
	{
		debugger.giveMessage(Debugger::Error, "WIP", "This feature didnt implemented for QueuedSoundBuffers dont use!");
		return -1;
	}
	else
	{
		 return m_SoundBufferVector[temp.index].data.get()->getBuffer();
	}
}

SoundManager SoundManager::Get()
{
    return SoundManager();
}