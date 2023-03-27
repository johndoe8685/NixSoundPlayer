#include "vendor/NixSoundPlayer/SoundSource.h"

SoundSource::SoundSource()
	:System("SoundSource"), m_State(AL_INITIAL)
{
	alGenSources(1, &m_Source);
	alSourcef(m_Source, AL_PITCH, m_Pitch);
	alSourcef(m_Source, AL_GAIN, m_Gain);
	alSource3f(m_Source, AL_POSITION, m_Position[0], m_Position[1], m_Position[2]);
	alSource3f(m_Source, AL_VELOCITY, m_Velocity[0], m_Velocity[1], m_Velocity[2]);
	alSourcei(m_Source, AL_LOOPING, m_LoopSound);
	alSourcei(m_Source, AL_BUFFER, m_Buffer);
}

SoundSource::~SoundSource()
{
	alSourcei(m_Source, AL_BUFFER, NULL);
	alDeleteSources(1, &m_Source);
}

void SoundSource::Play(const ALuint buffer_to_play)
{
	alGetSourcei(m_Source, AL_SOURCE_STATE, &m_State);

	if (m_State != AL_PLAYING)
	{
		if (buffer_to_play != m_Buffer)
		{
			m_Buffer = buffer_to_play;
			alSourcei(m_Source, AL_BUFFER, (ALint)m_Buffer);
		}

		alSourcePlay(m_Source);
	}
}

void SoundSource::Play(ALuint* buffers, ALsizei number_of_buffers)
{
	// clear any al errors
	alGetError();

	//Rewind the source position and clear the buffer queue
	alSourceRewind(m_Source);
	alSourcei(m_Source, AL_BUFFER, 0);

	//Queue and start play
	alSourceQueueBuffers(m_Source, number_of_buffers, buffers);
	alSourcePlay(m_Source);
}