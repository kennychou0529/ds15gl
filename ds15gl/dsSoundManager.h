#pragma once

#include <alut.h>
#include <stdio.h>
#include <map>

//multimap�ļ������ظ�
typedef std::multimap<unsigned int,ALuint> SourceMap; 

//��������࣬Ŀǰ��֧��wav

class DSSoundManager
{
public:
	static DSSoundManager* getSoundManager();

	DSSoundManager(void);
	~DSSoundManager(void);
	//���ļ��е������������n��ʾ��Ҫn��������Դ�Ա�ʹ��,���ص�һ��source��Index
	ALuint addSound(unsigned int id, char* fileName,ALint n = 1);
	//����ָ��ID��Դ����Ӧָ��Դ��λ�ú��ٶ�
	//idӦΪ�����ߵ����ͺ��������͵����?
	void playSound(unsigned int id);
	//������Ƶ,���尴ʲô���ش���
	void loadSounds();
	
	void setListenerPosition(ALfloat x, ALfloat y, ALfloat z=8.f);
	//������
	void displayALError(char* func,ALenum alError);


private:
	static DSSoundManager *sm;

	ALCdevice *device;
	ALCcontext *context;

	SourceMap sourceMap;

	ALuint backgroundSound;

	ALboolean g_bEAX;
	ALenum alError;
};

