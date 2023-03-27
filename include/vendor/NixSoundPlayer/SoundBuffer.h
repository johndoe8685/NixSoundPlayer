#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <vendor/NixSoundPlayer/vendor/AL/al.h>
#include <vendor/NixSoundPlayer/vendor/AL/alext.h>
#include <vendor/NixSoundPlayer/vendor/sndfile/sndfile.h>
#include <vendor/NixSoundPlayer/vendor/NixTools/system.h>

class SoundBuffer : public System
{
private:
	ALint m_format;
	sf_count_t m_num_of_frames;
	ALsizei m_num_of_byte;
	SNDFILE* m_file;
	SF_INFO m_sfinfo;
	short* m_memory_buffer;

	ALuint m_ALbuffer;
public:
	SoundBuffer(std::string componentName, SNDFILE* file, SF_INFO sfinfo, ALint format);
	~SoundBuffer();

	ALuint getBuffer() { return m_ALbuffer; }
};