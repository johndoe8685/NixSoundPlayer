#include "vendor/NixSoundPlayer/QueuedSoundBuffer.h"

QueuedSoundBuffer::QueuedSoundBuffer(std::string componentName, std::string filePath, SF_INFO sfinfo, ALint format)
	:System("QueuedSoundBuffer"), m_FilePath(filePath), m_sfinfo(sfinfo), m_format(format)
{
	directory.changePath(filePath);
	m_file = sf_open(directory.getPath().c_str(), SFM_READ, &m_sfinfo);

	int error;
	std::stringstream error_str;

	//Allocate the part of the audio file into a buffer.
	m_num_of_frames = ((size_t)BUFFER_FRAME_SIZE * (size_t)m_sfinfo.channels) * sizeof(short);
	m_memory_buffer = static_cast<short*>(malloc(m_num_of_frames));

	//Check sample size.
	if (m_num_of_frames < 1)
	{
		free(m_memory_buffer);
		error_str.str(std::string()); // Flush stringstream
		error_str << m_num_of_frames;
		debugger.giveMessage(Debugger::Error, "Failed to read samples", error_str.str());
	}

	//Create OpenAL buffer.
	alGenBuffers(BUFFER_AMOUNT, m_ALbuffers);
}

void QueuedSoundBuffer::UpdateBufferStream()
{
	ALint processed, state;

	alGetSourcei(m_current_source, AL_SOURCE_STATE, &state);
	alGetSourcei(m_current_source, AL_BUFFERS_PROCESSED, &processed);
	if (alGetError() != AL_NO_ERROR)
	{
		debugger.giveMessage(Debugger::Error, "Error checking Music state");
	}

	/* Unqueue and handle each processed buffer */
	while (processed > 0)
	{
		ALuint bufid;
		sf_count_t slen;

		alSourceUnqueueBuffers(m_current_source, 1, &bufid);
		processed--;

		/* Read the next chunk of data, refill the buffer, and queue it
		 * back on the source */
		ALsizei size = sf_readf_short(m_file, m_memory_buffer, BUFFER_FRAME_SIZE);
		if (size > 0)
		{
			size *= m_sfinfo.channels * (sf_count_t)sizeof(short);
			alBufferData(bufid, m_format, m_memory_buffer, size,
				m_sfinfo.samplerate);
			alSourceQueueBuffers(m_current_source, 1, &bufid);
		}
		if (alGetError() != AL_NO_ERROR)
		{
			debugger.giveMessage(Debugger::Error, "Error buffering music data");
		}
	}

	/* Make sure the source hasn't underrun */
	if (state != AL_PLAYING && state != AL_PAUSED)
	{
		ALint queued;

		/* If no buffers are queued, playback is finished */
		alGetSourcei(m_current_source, AL_BUFFERS_QUEUED, &queued);
		if (queued == 0)
			return;

		alSourcePlay(m_current_source);
		if (alGetError() != AL_NO_ERROR)
		{
			debugger.giveMessage(Debugger::Error, "Error restarting music playback");
		}
	}
}

QueuedSoundBuffer::bufferTuple QueuedSoundBuffer::LoadPlay(ALuint current_source)
{
	m_current_source = current_source;
	m_file = sf_open(directory.getPath().c_str(), SFM_READ, &m_sfinfo);

	//Load the buffers and give buffer array to the SoundSpeaker
	ALsizei i;
	for (i = 0; i < BUFFER_AMOUNT; i++)
	{
		sf_count_t size = sf_readf_short(m_file, m_memory_buffer, BUFFER_FRAME_SIZE);
		if (size < 1) break;
		size *= m_sfinfo.channels * (sf_count_t)sizeof(short);
		
		alBufferData(m_ALbuffers[i], m_format, m_memory_buffer, size, m_sfinfo.samplerate);
	}

	return { m_ALbuffers, i };
}

QueuedSoundBuffer::~QueuedSoundBuffer()
{
	alDeleteBuffers(BUFFER_AMOUNT, m_ALbuffers);
}