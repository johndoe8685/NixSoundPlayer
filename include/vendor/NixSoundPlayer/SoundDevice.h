#pragma once
#include <vendor/NixSoundPlayer/vendor/AL/alc.h>
#include <memory>
#include <vendor/NixSoundPlayer/vendor/NixTools/system.h>

class SoundDevice : public System
{
private:
	ALCdevice* m_ALDevice;
	ALCcontext* m_ALContext;
public:
	SoundDevice();
	~SoundDevice();
};