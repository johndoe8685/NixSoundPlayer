#include <iostream>
#include <vendor/NixSoundPlayer/vendor/NixTools/debugger.h>
#include "vendor/NixSoundPlayer/SoundDevice.h"
#include "vendor/NixSoundPlayer/SoundManager.h"
#include "vendor/NixSoundPlayer/SoundSource.h"

int main()
{
	Debugger debugger("Default", "SoundPlayer");
	debugger.giveMessage(Debugger::Info, "Hello from SoundPlayer!");

	SoundDevice snd_device;
	SoundManager snd_manager;
	SoundSource snd_source;
	
	snd_manager.AddSound("Halcyon", "/res/Orbital - Halcyon On and On.ogg");
	snd_manager.AddSound("Technicolor", "/res/Technicolor.ogg");
	snd_manager.AddSound("DMTAngel", "/res/Ooxygen - Light.ogg");
	
	snd_manager.PlaySound(&snd_source, "Technicolor");
	//snd_manager.PlaySound(&snd_source, "Arcade");
	//snd_manager.PlaySound(&snd_source, "DMTAngel");

	ALint state = AL_PLAYING;
	while (state == AL_PLAYING)
	{
		alGetSourcei(snd_source.GetSource(), AL_SOURCE_STATE, &state);
		debugger.giveMessage(Debugger::Info, "Currently playing sound. Press any key to stop.");
		if (_kbhit())
		{
			break;
		}
		snd_manager.Update();
	}
	_getch();
	snd_manager.StopSound(&snd_source, "Technicolor");
	debugger.giveMessage(Debugger::Info, "Done playing sound.");
	snd_manager.PlaySound(&snd_source, "Halcyon");
	state = AL_PLAYING;
	while (state == AL_PLAYING)
	{
		alGetSourcei(snd_source.GetSource(), AL_SOURCE_STATE, &state);
		debugger.giveMessage(Debugger::Info, "Currently playing sound. Press any key to stop.");
		if (_kbhit())
		{
			break;
		}
		snd_manager.Update();
	}
	debugger.giveMessage(Debugger::Info, "Done playing sound.");
	snd_manager.StopSound(&snd_source, "Halcyon");
	return 0;
}