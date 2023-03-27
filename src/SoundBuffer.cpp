#include "vendor/NixSoundPlayer/SoundBuffer.h"

SoundBuffer::SoundBuffer(std::string componentName, SNDFILE* file, SF_INFO sfinfo, ALint format)
	:System(componentName, "SoundBuffer"), m_file(file), m_sfinfo(sfinfo), m_format(format), m_ALbuffer(0)
{
	debugger.giveMessage(Debugger::Info, "Hello from soundbuffer");

	int error;
	std::stringstream error_str;

	//Decode the whole audio file to a buffer.
	m_memory_buffer = static_cast<short*>(malloc((size_t)(m_sfinfo.frames * m_sfinfo.channels) * sizeof(short)));
	m_num_of_frames = sf_readf_short(file, m_memory_buffer, m_sfinfo.frames);

	//Check sample size.
	if (m_num_of_frames < 1)
	{
		free(m_memory_buffer);
		sf_close(file);
		error_str.str(std::string()); // Flush stringstream
		error_str << m_num_of_frames;
		debugger.giveMessage(Debugger::Error, "Failed to read samples", error_str.str());
	}

	m_num_of_byte = (ALsizei)(m_num_of_frames * m_sfinfo.channels) * (ALsizei)sizeof(short);

	//Create OpenAL buffer.
	alGenBuffers(1, &m_ALbuffer);
	alBufferData(m_ALbuffer, m_format, m_memory_buffer, m_num_of_byte, m_sfinfo.samplerate);

	free(m_memory_buffer);
	sf_close(file);

	error = 0;
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		debugger.giveMessage(Debugger::Error, "OpenAL Error", alGetString(error));
		if (m_ALbuffer && alIsBuffer(m_ALbuffer))
		{
			alDeleteBuffers(1, &m_ALbuffer);
		}
	}
}

SoundBuffer::~SoundBuffer()
{
	alDeleteBuffers(1, &m_ALbuffer);
}