#pragma once
#include "pch.h"
class Audio
{
private:
	bool m_IsMusic;
	bool m_Paused;
	bool m_Loop;
	int id;
	const char* name;
	const char* fileName;
public:
	Audio(const char* SoundName, const char* FileName, bool LoopFlag, bool IsMeMusic);
	Audio();
	~Audio();
	bool IsPlaying();
	void Play();
	void Stop();
	void Pause();
	void SetVolume(int Volume); //0 -> -10000
	void SetPan(int Pan); //-10,000 -> 0 -> 10,000 
	void SetLoopFlag(bool LoopFlag);
	void Id(int);
	int  Id();
	void Name(const char*);
	const char* Name();

};

class AudioManager
{
private:
	std::vector<Audio> m_soundList;
	static AudioManager* m_pInstance; // Singleton pointer
	AudioManager();
	~AudioManager();
	AudioManager& operator=(AudioManager& other);
	void Init();
public:

	void AddSound(const char* SoundName, const char* FileName, bool LoopFlag, bool IsMeMusic);
	Audio* GetReference(const char* name);
	Audio* GetReference(int id);

};

