#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <vendor/NixSoundPlayer/vendor/AL/al.h>
#include <vendor/NixSoundPlayer/vendor/AL/alext.h>
#include <vendor/NixSoundPlayer/vendor/sndfile/sndfile.h>
#include <vendor/NixSoundPlayer/vendor/NixTools/system.h>

class QueuedSoundBuffer : public System
{
private:
	ALint m_format;
	sf_count_t m_num_of_frames;
	ALsizei m_num_of_byte;
	SF_INFO m_sfinfo;
	short* m_memory_buffer;
	std::string m_FilePath;
	SNDFILE* m_file;

	ALuint m_current_source;

	static const int BUFFER_FRAME_SIZE = 8192;
	static const int BUFFER_AMOUNT = 4;

	ALuint m_ALbuffers[BUFFER_AMOUNT];
public:

	struct bufferTuple
	{
		ALuint* buffers;
		ALsizei size;
	};

	QueuedSoundBuffer(std::string componentName, std::string filePath, SF_INFO sfinfo, ALint format);

	void UpdateBufferStream();
	bufferTuple LoadPlay(ALuint current_source);
	void CloseFile() { sf_close(m_file); }

	~QueuedSoundBuffer();
};