#pragma once

#include <alut.h>
#include <stdio.h>
#include <windows.h>
#include <thread>

#include "mpg123/mpg123.h"
#include <conio.h>

#include <map>
#include <vector>

#pragma comment (lib, "./mpg123/libmpg123.lib")
#pragma comment (lib, "./openal/lib/OpenAL32.lib")
#pragma comment (lib, "./openal/lib/alut.lib")

// 注释待添加

class Clip{
public:
	Clip(char* fileName);
	~Clip();
	void append();
	void play();

	void playMP3(bool* running);

private:
	char* fileName;
	std::vector<ALuint> sources;
	unsigned short type;
	bool isPlaying;

	ALenum alError;
};

typedef std::map<unsigned int,Clip*> ClipMap; 

class DSSoundManager
{
public:
	static DSSoundManager* getSoundManager();

	DSSoundManager(void);
	~DSSoundManager(void);
	// 注释待添加
	void addSound(unsigned int id, char* fileName);
	// 注释待添加
	void playSound(unsigned int id);
	// 注释待添加
	void loadSounds();
	
	void setListenerPosition(ALfloat x, ALfloat y, ALfloat z=8.f);
	// 注释待添加
	static void displayALError(char* func,ALenum alError);

	

private:
	static DSSoundManager *sm;

	ALCdevice *device;
	ALCcontext *context;
	
	mpg123_handle *mpg123 ;

	ClipMap clipMap;

	ALuint backgroundSound;

	ALboolean g_bEAX;
	ALenum alError;
	int iMpg123_error;
};

