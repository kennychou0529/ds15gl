#include "dsSoundManager.h"

void Clip::append(){
	alGetError();

	ALuint sourceIndex = 0;

	//����buffuer
	ALuint tempBuffer ;
	alGenBuffers(1,&tempBuffer);
	if((alError = alGetError())!= AL_NO_ERROR){
		DSSoundManager::displayALError("alGenBuffers:",alError);
		return ;
	}
		if(strlen(fileName)>4&&strcmp(fileName+strlen(fileName)-3,"mp3")==0){
			//tempBuffer=DSSoundManager::createBufferFromMp3File(fileName);
			type=1;
		}else{
			type=0;
			ALenum format;
			void*data;
			ALsizei	size;
			ALsizei	frequency;
			ALboolean loop;
			alutLoadWAVFile(fileName,&format,&data,&size,&frequency,&loop);
			alBufferData(tempBuffer,format,data,size,frequency);
			//tempBuffer = alutCreateBufferHelloWorld();


			if((alError = alGetError())!= AL_NO_ERROR){
				//displayALError("alutCreateBufferFromFIle:",alError);
				alDeleteBuffers(1,&tempBuffer);
				return;
			}
		}
		if(tempBuffer==AL_NONE){
			printf_s("LoadFileFailure\n");
			return ;
		}

		//����source
		ALuint tempSource;
		alGenSources(1,&tempSource);
		if((alError = alGetError())!= AL_NO_ERROR){
			DSSoundManager::displayALError("alGenSources:",alError);
			return ;
		}

		alSourcei(tempSource,AL_BUFFER,tempBuffer);

		if((alError = alGetError())!= AL_NO_ERROR){
			DSSoundManager::displayALError("alSourcei:",alError);
			alDeleteSources(1,&tempSource);
			alDeleteBuffers(1,&tempBuffer);
			return ;
		}
	if (tempSource!=AL_NONE)
	{
		sources.push_back(tempSource);
	}
}

Clip::Clip(char* fileName){

	this->fileName = fileName;

	append();
	
}
Clip::~Clip(){
	isPlaying=false;
}
void Clip::play(){
	ALuint sourceIndex=0;
	if(type=0){
		if(sources.size()==0)
			return;

		for(int i=0;i<sources.size();i++){
			ALint value;
			alGetSourcei(sources[i],AL_SOURCE_STATE,&value);


			if (value != AL_PLAYING)
			{
				sourceIndex=sources[i];
				break;
			}
		}
		if(sourceIndex==AL_NONE){
			append();
			play();
			return;
		}
		ALCfloat sourcePos[]={2,0,2};
		ALCfloat sourceVel[]={1,10,1};
		alSourcefv(sourceIndex,AL_POSITION,sourcePos);
		alSourcefv(sourceIndex,AL_VELOCITY,sourceVel);
		alSourcei(sourceIndex,AL_LOOPING,AL_TRUE);
		alSourcef(sourceIndex,AL_GAIN,1);
		alSourcef(sourceIndex,AL_PITCH,1);

		alSourcePlay(sourceIndex);
		if((alError = alGetError())!= AL_NO_ERROR){
			DSSoundManager::displayALError("alSourcePlay:",alError);
		}
	}else if(type=1){
		int playMP3(char*,bool*);
		this->isPlaying=true;
		std::thread t(playMP3,fileName,&(this->isPlaying));
		t.detach();
	}	
}

#define NUM_BUFFERS 4

int playMP3(char* fileName, bool* running){
	ALuint g_Buffers[NUM_BUFFERS];
	ALuint uiSource;
	ALuint uiBuffer;
	ALCdevice * pDevice = NULL;
	ALCcontext *pContext = NULL;
	ALboolean g_bEAX;
	ALenum error, eBufferFormat;
	ALint iDataSize, iFrequency;
	ALuint ulFormat;
	int iLoop,iBuffersProcessed, iTotalBuffersProcessed;
	unsigned long ulDataSize = 0;
	unsigned long ulFrequency = 0;
	unsigned long ulBufferSize;
	size_t ulBytesWritten;
	long lRate;
	int iEncoding, iChannels;
	int encoding;
	void *        pData = NULL;
	WAVEFORMATEX *pwfex;
	int iState, iQueuedBuffers;
	mpg123_handle *mpg123 ;
	int iMpg123_error;
	//init mpg123 library
	if(MPG123_OK != (iMpg123_error = mpg123_init()))
	{
		printf("failed to init mpg123\n");
		return -1;
	}

	//open a default mpg123 decoder
	mpg123 = mpg123_new(mpg123_decoders()[0], &iMpg123_error);

	//Generate openal  Buffers
	alGetError();  //clear all error code

	alGenBuffers(NUM_BUFFERS, g_Buffers);

	if((error = alGetError()) != AL_NO_ERROR)
	{
		fprintf(stderr, "alGenBuffers :%s", error);
	}

	alGenSources(1, &uiSource);

	//try to open a mp3 file, modify to your own mp3 files
	if(MPG123_OK != (iMpg123_error = mpg123_open(mpg123,fileName)))
	{
		fprintf(stderr,"error in open mp3 file\n");
		return -1;
	}
	//get mp3 format infomation
	mpg123_getformat(mpg123, &lRate, &iChannels,  &iEncoding);

	//there only parse stereo mp3 file
	if(iChannels ==2)
	{
		ulFormat = alGetEnumValue("AL_FORMAT_STEREO16");
		ulBufferSize = lRate;  //set buffer to 250ms
		ulBufferSize -= (ulBufferSize % 4);  //set pcm Block align
		ulFrequency = lRate;  //set pcm sample rate

	}
	else
	{
		printf("channels info%i\n", iChannels);
		return -3;
	}




	pData = malloc(ulBufferSize);
	//feed data to openal buffer
	for(iLoop= 0; iLoop < 4; iLoop++)
	{
		mpg123_read(mpg123, (unsigned char *)pData, ulBufferSize,&ulBytesWritten);
		alBufferData(g_Buffers[iLoop], ulFormat, pData, ulBytesWritten, ulFrequency);
		alSourceQueueBuffers(uiSource, 1, &g_Buffers[iLoop]);
	}
	alSourcePlay(uiSource);
	iTotalBuffersProcessed = 0;
	printf("playing\n"); 
	//wait for key press
	while(*running) 
	{
		Sleep(20);
		iBuffersProcessed = 0;
		alGetSourcei(uiSource, AL_BUFFERS_PROCESSED, &iBuffersProcessed);

		iTotalBuffersProcessed += iBuffersProcessed;
		printf("Buffers total Processed %d\r", iTotalBuffersProcessed);
		while(iBuffersProcessed)
		{
			uiBuffer = 0;
			alSourceUnqueueBuffers(uiSource, 1, &uiBuffer);

			mpg123_read(mpg123, (unsigned char *)pData, ulBufferSize, &ulBytesWritten);
			if(ulBytesWritten)
			{
				alBufferData(uiBuffer, ulFormat, pData, ulBytesWritten, ulFrequency);
				alSourceQueueBuffers(uiSource, 1, &uiBuffer);
			}				iBuffersProcessed--;

		}
		alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
		if(iState != AL_PLAYING)
		{
			alGetSourcei(uiSource, AL_BUFFERS_QUEUED, &iQueuedBuffers);

			if(iQueuedBuffers)
			{
				alSourcePlay(uiSource);//buffers have data, play it
			}
			else
			{
				//there is no data any more
				break;
			}
		}
	}
	//close mpg123
	mpg123_close(mpg123);
	//stop the source and clear the queue
	alSourceStop(uiSource);
	alSourcei(uiSource, AL_BUFFER, 0);

	free(pData);
	pData = NULL;

	return 0;

}


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

	//init mpg123 library
	if(MPG123_OK != (iMpg123_error = mpg123_init()))
	{
		printf("failed to init mpg123\n");
		return;
	}

	

	loadSounds();
}


DSSoundManager::~DSSoundManager(void)
{
	context=alcGetCurrentContext();
	device=alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);

	clipMap.clear();
}

void DSSoundManager::addSound(unsigned int id, char* fileName){
	Clip *clip=new Clip(fileName);
	clipMap.insert(ClipMap::value_type(id,clip));
}
	


void DSSoundManager::playSound(unsigned int id){
	ClipMap::iterator it = clipMap.find(id);
	if(it!=clipMap.end())
		(it->second)->play();
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
	/**/
///* ��ʱ�����ñ������֣�wav �ı������ֻ�ǳ���?
//	*/
//	addSound(0,"data/sound/test.mp3");
//	/*alSourcePlay(backgroundSound);*/
//
//	addSound(1,"data/sound/Footsteps.wav");
//
//	//����
//	
//		playSound(0);
//		playSound(1);
//		//alutSleep(1);

	
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