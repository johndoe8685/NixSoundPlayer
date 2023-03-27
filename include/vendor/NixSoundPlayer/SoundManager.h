#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <vendor/NixSoundPlayer/vendor/NixTools/system.h>
#include "vendor/NixSoundPlayer/SoundBuffer.h"
#include "vendor/NixSoundPlayer/QueuedSoundBuffer.h"
#include "vendor/NixSoundPlayer/SoundSource.h"
class SoundManager : public System
{
private:

	struct pointList
	{
		bool isQueued;
		size_t index;
	};
	
	struct SoundBufferData
	{
		std::shared_ptr<SoundBuffer> data;
		bool isPlaying;
	};

	struct QueuedSoundBufferData
	{
		std::shared_ptr<QueuedSoundBuffer> data;
		bool isPlaying;
		bool isFileClosed;
	};

	std::vector<SoundBufferData> m_SoundBufferVector;
	std::vector<QueuedSoundBufferData> m_QueuedSoundBufferVector;

	std::unordered_map<std::string, pointList> m_PointMap;

	static SoundManager s_Instance;
public:
	SoundManager();
	~SoundManager();

	void Update();

	void AddSound(std::string componentName, std::string filePath);
	void PlaySound(SoundSource* source, std::string componentName);
	void StopSound(SoundSource* source, std::string componentName);

	ALuint getBuffer(std::string componentName);

	static SoundManager Get();
};