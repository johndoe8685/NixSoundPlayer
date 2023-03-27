#include "vendor/NixSoundPlayer/SoundDevice.h"
#include <iostream>

SoundDevice::SoundDevice()
	:System("Default", "SoundDevice")
{
	m_ALDevice = alcOpenDevice(nullptr);
	if (!m_ALDevice)
	{
		debugger.giveMessage(Debugger::Error, "m_ALDevice", "OpenAL device was not created!!");
		return;
	}
	m_ALContext = alcCreateContext(m_ALDevice, nullptr);
	if (!m_ALContext)
	{
		debugger.giveMessage(Debugger::Error, "m_ALContext", "OpenAL context was not created!!");
		return;
	}
	alcMakeContextCurrent(m_ALContext);
	
	const ALCchar* name = alcGetString(m_ALDevice, ALC_DEVICE_SPECIFIER);
	debugger.giveMessage(Debugger::Info, "Device created", name);
}

SoundDevice::~SoundDevice()
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(m_ALContext);
	alcCloseDevice(m_ALDevice);
}