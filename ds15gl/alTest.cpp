#include <alut.h>
#include <iostream>
#include <ctime>
#include <conio.h>

// �洢��������.  
ALuint Buffer;  

// ���ڲ�������.  
ALuint Source;  

// Դ������λ��.  
ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };  

// Դ�������ٶ�.  
ALfloat SourceVel[] = { 0.0, 0.0, 0.1 };  

// ���ߵ�λ��.  
ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };  

// ���ߵ��ٶ�  
ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };  

// ���ߵķ��� (first 3 elements are "at", second 3 are "up")  
ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };  
//��һ������һ��Ψһ�Ĳ�ͬ��Դ�ٶȵĸı䣬���ġ�Z��������0.1.  

ALboolean LoadALData()  
{  
	// �������  
	//ALenum format;  
	//ALsizei size;  
	//ALvoid* data;  
	//ALsizei freq;  
	//ALboolean loop;  

	//// ����WAV����.  

	//alGenBuffers(1, &Buffer);  

	//if (alGetError() != AL_NO_ERROR)  
	//	return AL_FALSE;  
	//signed char* fileName="wavdata/Footsteps.wav";
	//alutLoadWAVFile("Footsteps.wav", &format, &data, &size, &freq, &loop);  
	//alBufferData(Buffer, format, data, size, freq);  
	//alutUnloadWAV(format, data, size, freq);  
	Buffer = alutCreateBufferFromFile("data/sound/Footsteps.wav");
	// ����Դ  

	alGenSources(1, &Source);  

	if (alGetError() != AL_NO_ERROR)  
		return AL_FALSE;  

	alSourcei (Source, AL_BUFFER, Buffer );  
	alSourcef (Source, AL_PITCH, 1.0f );  
	alSourcef (Source, AL_GAIN, 1.0f );  
	alSourcefv(Source, AL_POSITION, SourcePos);  
	alSourcefv(Source, AL_VELOCITY, SourceVel);  
	alSourcei (Source, AL_LOOPING, AL_TRUE );  

	// �������Ⲣ����  

	if (alGetError() != AL_NO_ERROR)  
		return AL_FALSE;  

	return AL_TRUE;  
}  

//����һ�����������ı䣬�����ǵ��롰FOOTSTES��WAV��������Դ��AL_LOOPING��  
//Ϊ��AL_TRUE��������ζ��Դ����ֱ��ֹͣʱ�������������ϵ�ѭ�����š�  
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
//����û�иı䡣  
int main1(int argc, char *argv[])  
{  
	// ��ʼOPENAL��������ֽ�  
	alutInit(NULL,0);  
	alGetError();  

	// ����WAV����.  
	if (LoadALData() == AL_FALSE)  
		return 0;  

	SetListenervalues();  

	// �����˳�����.  
	atexit(KillALData);  

	// ��ʼԴ�Ĳ���.  
	alSourcePlay(Source);  

	//ѭ��  
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

//����Ψһ�ĸı���������һ��ѭ�����������沥�ź�ֹͣ��ť��  
//We do this by slowly incrementing the position by it's velocity over  
//	time. The time is sampled by checking the system clock which gives  
//	us a tick count. It shouldn't be necessary to change this, but if the   
//	audio clip fades too fast you might want to change 50 to some higher n  
//	umber. Pressing any key will end the loop  