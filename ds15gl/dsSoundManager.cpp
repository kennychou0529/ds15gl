#include "dsSoundManager.h"

DSSoundManager* DSSoundManager::sm = nullptr;

DSSoundManager* DSSoundManager::getSoundManager(){
	if (sm==nullptr)
	{
		return sm=new DSSoundManager;
	}
	return sm;
}


DSSoundManager::DSSoundManager(void)
{
	device = alcOpenDevice(NULL); 
	if (device){
		context = alcCreateContext(device, NULL);
		alcMakeContextCurrent(context);
	}
	g_bEAX = alIsExtensionPresent("EAX2.0");

	loadSounds();
}


DSSoundManager::~DSSoundManager(void)
{
	context=alcGetCurrentContext();
	device=alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);

	sourceMap.clear();
}

ALuint DSSoundManager::addSound(unsigned int id, char* fileName,ALint n){
	alGetError();

	ALuint sourceIndex = 0;
	
	//����buffuer
	ALuint* tempBuffer = new ALuint[n];
	alGenBuffers(n,tempBuffer);
	if((alError = alGetError())!= AL_NO_ERROR){
		displayALError("alGenBuffers:",alError);
		
		return 0;
	}
	for(int i=0;i<n;i++){
		ALenum format;
		void*data;
		ALsizei	size;
		ALsizei	frequency;
		ALboolean loop;
		alutLoadWAVFile(fileName,&format,&data,&size,&frequency,&loop);
		alBufferData(tempBuffer[i],format,data,size,frequency);
		//tempBuffer = alutCreateBufferHelloWorld();


		if((alError = alGetError())!= AL_NO_ERROR){
			//displayALError("alutCreateBufferFromFIle:",alError);
			alDeleteBuffers(1,tempBuffer+i);
			return 0;
		}
		if(tempBuffer==AL_NONE){
			printf_s("LoadFileFailure\n");
			return 0;
		}

		//����source
		ALuint tempSource;
		alGenSources(1,&tempSource);
		if((alError = alGetError())!= AL_NO_ERROR){
			displayALError("alGenSources:",alError);
			return 0;
		}

		alSourcei(tempSource,AL_BUFFER,tempBuffer[i]);

		if((alError = alGetError())!= AL_NO_ERROR){
			displayALError("alSourcei:",alError);
			alDeleteSources(1,&tempSource);
			alDeleteBuffers(1,tempBuffer+i);
			return 0;
		}

		//alSourcePlay(tempSource);

		if (sourceIndex==0)	{
			sourceIndex = tempSource;
		}
		sourceMap.insert(SourceMap::value_type(id,tempSource));
	}
	

	return sourceIndex;
}

void DSSoundManager::playSound(unsigned int id){
	SourceMap::iterator it = sourceMap.find(id);
	ALint sourceIndex=0;

	while(it!=sourceMap.end()){
		ALint value;
		alGetSourcei(it->second,AL_SOURCE_STATE,&value);

		//��Ҫ��һ��û���ڲ��ŵ�source
		if (value != AL_PLAYING)
		{
			sourceIndex=it->second;
			break;
		}
		it++;	
	}


	//���û�����Id, ������source���ڲ���
	if (sourceIndex==0)	return;	

	ALCfloat sourcePos[]={2,0,2};
	ALCfloat sourceVel[]={1,10,1};
	alSourcefv(sourceIndex,AL_POSITION,sourcePos);
	alSourcefv(sourceIndex,AL_VELOCITY,sourceVel);
	alSourcei(sourceIndex,AL_LOOPING,AL_TRUE);
	alSourcef(sourceIndex,AL_GAIN,1);
	alSourcef(sourceIndex,AL_PITCH,1);

	alSourcePlay(sourceIndex);
	if((alError = alGetError())!= AL_NO_ERROR){
		displayALError("alSourcePlay:",alError);
	}
	
}


void DSSoundManager::setListenerPosition(ALfloat x, ALfloat y, ALfloat z){
	ALCfloat listenerPos[]={x,y,z};
	ALCfloat listenerVel[]={0,0,0};
	ALCfloat listenerOri[]={0,0,-1,0,1,0};
	alListenerfv(AL_POSITION,listenerPos);
	alListenerfv(AL_VELOCITY,listenerVel);
	alListenerfv(AL_ORIENTATION,listenerOri);
}



void DSSoundManager::displayALError(char* func,ALenum alError){
	printf_s(func);
	printf_s(alGetString(alError));
}


void DSSoundManager::loadSounds(){
/* ��ʱ�����ñ������֣�wav �ı������ֻ�ǳ���?
	backgroundSound = addSound(0,"");
	alSourcePlay(backgroundSound);*/

	addSound(1,"data/sound/Footsteps.wav",5);

	//����
	for (int i=0;i<3;i++)
	{
		playSound(1);
		alutSleep(1);
	}
}

//int main(int argc,char** argv){
//	//alutInit(&argc,argv);
//	DSSoundManager* sm = DSSoundManager::getSoundManager();
//	sm->addSound(1,"data/sound/Footsteps.wav",5);
//	sm->setListenerPosition(15,20);
//	for (int i=0;i<5;i++){
//		sm->playSound(1);
//		alutSleep(1);
//	}
//	alutSleep(10);
//}