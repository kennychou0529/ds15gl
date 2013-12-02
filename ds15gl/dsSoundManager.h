#pragma once

#include <alut.h>
#include <stdio.h>
#include <windows.h>
#include <thread>

#include "mpg123/mpg123.h"
#include <conio.h>

#include <map>
#include <vector>

#include "tinyxml2.h"
#include "dsSoldier.h"

#pragma comment (lib, "./mpg123/libmpg123.lib")
#pragma comment (lib, "./openal/lib/OpenAL32.lib")
#pragma comment (lib, "./openal/lib/alut.lib")

// 音频source的容器

class Clip {
public:
    Clip(const char* fileName, bool loop);
    ~Clip();
    void append();
    //x,y,z 声源位置
    //vx,vy,vz 声源速度
    ALuint play(float x, float y, float z, float vx = 0, float vy = 0, float vz = 0);
	//void playMP3(bool* running);
private:
    char fileName[256];
    std::vector<ALuint> sources;
    unsigned short type;
    bool isPlaying;
    bool loop;
    ALenum alError;
	bool stopped;
	friend class DSSoundManager;
};

typedef std::map<unsigned int, Clip*> ClipMap;

//必须在alutinit之后实例化，否则会出问题（导入音频失败）
class DSSoundManager {
public:

    //就用这个
    DSSoundManager(void);
    //析构
    ~DSSoundManager(void);

    // 按文件名后缀区分MP3文件和wav文件
    // wav文件默认载入两次备用，不够用时自动扩展
    // mp3文件只保存文件名，播放时读取
    // 暂时只能载入一个MP3作为背景音乐
    void addSound(unsigned int id, const char* fileName, bool loop = true);

	//重新播放背景音乐
	void replay(){
		clipMap[backGroundSoundIndex]->isPlaying = false;
		clipMap[backGroundSoundIndex]->play(0,0,0);
	}
    // x,y,z 声源位置
    // vx,vy,vz 声源速度
    // 返回所播放声源的索引
    // 若为mp3文件，或id不存在，返回 0；
    ALuint playSound(unsigned int id, float x, float y, float z, float vx = 0, float vy = 0, float vz = 0);

    static void changePosition(ALuint source, float x, float y, float z = 0.f);

    // sourceIndex 请使用 plaSound的返回值
    void stop(ALuint sourceIndex);

    // 在Frame::Initialize中调用
    void loadSounds();

    void setListenerPosition(ALfloat x, ALfloat y, ALfloat z = 8.f);
    // 注释待添加
    static void displayALError(char* func, ALenum alError);

	std::map<std::string,SoundIDandSource> soundgroups;
	static SoundIDandSource defaultSoundGroup;

private:
    /* ALCdevice* device;
     ALCcontext* context;*/

    mpg123_handle* mpg123 ;

    ClipMap clipMap;

    //ALuint backgroundSound;

    ALboolean g_bEAX;
    ALenum alError;
    int iMpg123_error;
	string backGroundSoundFile;
	int backGroundSoundIndex;
};

