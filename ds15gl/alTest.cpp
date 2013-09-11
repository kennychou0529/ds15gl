#include <alut.h>
#include <iostream>
#include <ctime>
#include <conio.h>

// 存储声音数据.  
ALuint Buffer;  

// 用于播放声音.  
ALuint Source;  

// 源声音的位置.  
ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };  

// 源声音的速度.  
ALfloat SourceVel[] = { 0.0, 0.0, 0.1 };  

// 听者的位置.  
ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };  

// 听者的速度  
ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };  

// 听者的方向 (first 3 elements are "at", second 3 are "up")  
ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };  
//这一章与上一章唯一的不同是源速度的改变，他的‘Z’现在是0.1.  

ALboolean LoadALData()  
{  
	// 载入变量  
	//ALenum format;  
	//ALsizei size;  
	//ALvoid* data;  
	//ALsizei freq;  
	//ALboolean loop;  

	//// 载入WAV数据.  

	//alGenBuffers(1, &Buffer);  

	//if (alGetError() != AL_NO_ERROR)  
	//	return AL_FALSE;  
	//signed char* fileName="wavdata/Footsteps.wav";
	//alutLoadWAVFile("Footsteps.wav", &format, &data, &size, &freq, &loop);  
	//alBufferData(Buffer, format, data, size, freq);  
	//alutUnloadWAV(format, data, size, freq);  
	Buffer = alutCreateBufferFromFile("data/sound/Footsteps.wav");
	// 捆绑源  

	alGenSources(1, &Source);  

	if (alGetError() != AL_NO_ERROR)  
		return AL_FALSE;  

	alSourcei (Source, AL_BUFFER, Buffer );  
	alSourcef (Source, AL_PITCH, 1.0f );  
	alSourcef (Source, AL_GAIN, 1.0f );  
	alSourcefv(Source, AL_POSITION, SourcePos);  
	alSourcefv(Source, AL_VELOCITY, SourceVel);  
	alSourcei (Source, AL_LOOPING, AL_TRUE );  

	// 做错误检测并返回  

	if (alGetError() != AL_NO_ERROR)  
		return AL_FALSE;  

	return AL_TRUE;  
}  

//在这一节中有两处改变，首先是导入“FOOTSTES。WAV”，设置源‘AL_LOOPING’  
//为‘AL_TRUE’。这意味着源播放直到停止时结束。他将不断的循环播放。  
void SetListenervalues()  
{  
	alListenerfv(AL_POSITION, ListenerPos);  
	alListenerfv(AL_VELOCITY, ListenerVel);  
	alListenerfv(AL_ORIENTATION, ListenerOri);  
}  

void KillALData()  
{  
	alDeleteBuffers(1, &Buffer);  
	alDeleteSources(1, &Source);  
	alutExit();  
}  
//这里没有改变。  
int main1(int argc, char *argv[])  
{  
	// 初始OPENAL并清错误字节  
	alutInit(NULL,0);  
	alGetError();  

	// 载入WAV数据.  
	if (LoadALData() == AL_FALSE)  
		return 0;  

	SetListenervalues();  

	// 设置退出函数.  
	atexit(KillALData);  

	// 开始源的播放.  
	alSourcePlay(Source);  

	//循环  
	ALint time = 0;  
	ALint elapse = 0;  

	while (!kbhit())  
	{  
		elapse += clock() - time;  
		time += elapse;  

		if (elapse > 50)  
		{  
			elapse = 0;  

			SourcePos[0] += SourceVel[0];  
			SourcePos[1] += SourceVel[1];  
			SourcePos[2] += SourceVel[2];  

			alSourcefv(Source, AL_POSITION, SourcePos);  
		}  
	}  


	return 0;  
}  

//这里唯一的改变是增加了一个循环。他将代替播放和停止按钮。  
//We do this by slowly incrementing the position by it's velocity over  
//	time. The time is sampled by checking the system clock which gives  
//	us a tick count. It shouldn't be necessary to change this, but if the   
//	audio clip fades too fast you might want to change 50 to some higher n  
//	umber. Pressing any key will end the loop  