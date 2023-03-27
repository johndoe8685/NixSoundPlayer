#pragma once
#include <vendor/NixSoundPlayer/vendor/AL/al.h>
#include <vendor/NixSoundPlayer/vendor/NixTools/system.h>
#include <conio.h>

class SoundSource : public System
{
private:
	ALuint m_Source;
	float m_Pitch = 1.f;
	float m_Gain = 0.5f;
	float m_Position[3] = { 0,0,0 };
	float m_Velocity[3] = { 0,0,0 };
	bool m_LoopSound = false;
	ALuint m_Buffer = 0;
	ALint m_State;
public:
	SoundSource();
	~SoundSource();

	void Play(const ALuint buffer_to_play);
	void Play(ALuint* buffers, ALsizei number_of_buffers);

	void Stop() { alSourceStop(m_Source); alSourcei(m_Source, AL_BUFFER, NULL);  }
	ALuint GetSource() { return m_Source; }
};