﻿#include "dsSoundManager.h"

void Clip::append() {
    alGetError();

    if (strlen(fileName) > 4 && strcmp(fileName + strlen(fileName) - 3, "mp3") == 0) {
        //tempBuffer=DSSoundManager::createBufferFromMp3File(fileName);
        type = 1;
    } else {
        type = 0;

        ALuint sourceIndex = 0;
        ALuint tempBuffer ;

        tempBuffer = alutCreateBufferFromFile(fileName);

        if ((alError = alGetError()) != AL_NO_ERROR) {
            DSSoundManager::displayALError("alutCreateBufferFromFIle:", alError);
            alDeleteBuffers(1, &tempBuffer);
            return;
        }

        if (tempBuffer == AL_NONE) {
            printf_s("LoadFileFailure\n");
            return ;
        }

        // 注释待添加
        ALuint tempSource;
        alGenSources(1, &tempSource);
        if ((alError = alGetError()) != AL_NO_ERROR) {
            DSSoundManager::displayALError("alGenSources:", alError);
            return ;
        }

        alSourcei(tempSource, AL_BUFFER, tempBuffer);

        if ((alError = alGetError()) != AL_NO_ERROR) {
            DSSoundManager::displayALError("alSourcei:", alError);
            alDeleteSources(1, &tempSource);
            alDeleteBuffers(1, &tempBuffer);
            return ;
        }

        if (tempSource != AL_NONE) {
            //循环播放
            if (loop) {
                alSourcei(tempSource, AL_LOOPING, AL_TRUE);
            }
            alSourcef(tempSource, AL_GAIN, 50.0f);
            alSourcef(tempSource, AL_PITCH, 1.0f);
            sources.push_back(tempSource);
        }
    }
}

Clip::Clip(const char* fileName, bool loop) {
    this->loop = loop;
	stopped = true;
	strcpy_s(this->fileName , fileName);
    append();
    //append();

}
Clip::~Clip() {
    isPlaying = false;
    if (type == 0) {
        for (unsigned int i = 0; i < sources.size(); i++) {
            //这是怎么回事？sources[i]居然报错。
            ALuint source = sources.operator[](i);
            if (!alIsSource(source)) {
                continue;
            }
            alSourceStop(source);
            ALint buffer = 0;
            alGetSourcei(source, AL_BUFFER, &buffer);
            if (alIsBuffer(buffer)) {
                alDeleteBuffers(1, (ALuint*)&buffer);
            }
            buffer = 0;
            alDeleteSources(1, &source);
        }
    }
    sources.clear();
}
ALuint Clip::play(float x, float y, float z, float vx, float vy, float vz) {
    ALuint sourceIndex = 0;

    //为 wav 格式文件创建的源
    if (type == 0) {
        if (sources.size() == 0) {
            return 0;
        }

        for (unsigned int i = 0; i < sources.size(); i++) {
            ALint value;
            alGetSourcei(sources[i], AL_SOURCE_STATE, &value);

            if (value != AL_PLAYING) {
                sourceIndex = sources[i];
                break;
            }
        }

        if (sourceIndex == AL_NONE) {
            append();
            return play(x, y, z, vx, vy, vz);
        }

        ALCfloat sourcePos[] = {x, y, z};
        ALCfloat sourceVel[] = {vx, vy, vz};
        alSourcefv(sourceIndex, AL_POSITION, sourcePos);
        alSourcefv(sourceIndex, AL_VELOCITY, sourceVel);



        alSourcePlay(sourceIndex);
        if ((alError = alGetError()) != AL_NO_ERROR) {
            DSSoundManager::displayALError("alSourcePlay:", alError);
        }
        return sourceIndex;
    } else if (type == 1) {

        //播放mp3音乐
        int playMP3(char*, bool*, bool*);
        //this->isPlaying = true;
        std::thread t(playMP3, fileName, &(this->isPlaying), &(this->stopped));
        t.detach();
    }
    return 0;
}

#define NUM_BUFFERS 4

int playMP3(char* fileName, bool* running, bool* stopped) {
    while (!*stopped) {
        Sleep(100);
    }
	*running = true;
    *stopped = false;
    ALuint g_Buffers[NUM_BUFFERS];
    ALuint uiSource;
    ALuint uiBuffer;
    //ALboolean g_bEAX;
    ALenum error;//, eBufferFormat;
    //ALint iDataSize, iFrequency;
    ALuint ulFormat;
    int iLoop, iBuffersProcessed, iTotalBuffersProcessed;
    unsigned long ulDataSize = 0;
    unsigned long ulFrequency = 0;
    unsigned long ulBufferSize;
    size_t ulBytesWritten;
    long lRate;
    int iEncoding, iChannels;
    //int encoding;
    void*         pData = NULL;
    //WAVEFORMATEX* pwfex;
    int iState, iQueuedBuffers;
    mpg123_handle* mpg123 ;
    int iMpg123_error;
    //init mpg123 library
    if (MPG123_OK != (iMpg123_error = mpg123_init())) {
        printf("failed to init mpg123\n");
        return -1;
    }

    //open a default mpg123 decoder
    mpg123 = mpg123_new(mpg123_decoders()[0], &iMpg123_error);

    //Generate openal  Buffers
    alGetError();  //clear all error code

    alGenBuffers(NUM_BUFFERS, g_Buffers);

    if ((error = alGetError()) != AL_NO_ERROR) {
        fprintf(stderr, "alGenBuffers :%s", error);
    }

    alGenSources(1, &uiSource);

    //try to open a mp3 file, modify to your own mp3 files
    if (MPG123_OK != (iMpg123_error = mpg123_open(mpg123, fileName))) {
        fprintf(stderr, "error in open mp3 file\n");
        return -1;
    }
    //get mp3 format infomation
    mpg123_getformat(mpg123, &lRate, &iChannels,  &iEncoding);

    //there only parse stereo mp3 file
    if (iChannels == 2) {
        ulFormat = alGetEnumValue("AL_FORMAT_STEREO16");
        ulBufferSize = lRate;  //set buffer to 250ms
        ulBufferSize -= (ulBufferSize % 4);  //set pcm Block align
        ulFrequency = lRate;  //set pcm sample rate

    } else {
        printf("channels info%i\n", iChannels);
        return -3;
    }




    pData = malloc(ulBufferSize);
    //feed data to openal buffer
    for (iLoop = 0; iLoop < 4; iLoop++) {
        mpg123_read(mpg123, (unsigned char*)pData, ulBufferSize, &ulBytesWritten);
        alBufferData(g_Buffers[iLoop], ulFormat, pData, ulBytesWritten, ulFrequency);
        alSourceQueueBuffers(uiSource, 1, &g_Buffers[iLoop]);
    }

    alSourcef(uiSource, AL_GAIN, 0.5f);
    alSourcePlay(uiSource);
    iTotalBuffersProcessed = 0;
    printf("playing\n");
    //wait for key press
    while (*running) {
        Sleep(20);
        iBuffersProcessed = 0;
        alGetSourcei(uiSource, AL_BUFFERS_PROCESSED, &iBuffersProcessed);

        iTotalBuffersProcessed += iBuffersProcessed;
        //printf("Buffers total Processed %d", iTotalBuffersProcessed);
        while (iBuffersProcessed) {
            uiBuffer = 0;
            alSourceUnqueueBuffers(uiSource, 1, &uiBuffer);

            mpg123_read(mpg123, (unsigned char*)pData, ulBufferSize, &ulBytesWritten);
            if (ulBytesWritten) {
                alBufferData(uiBuffer, ulFormat, pData, ulBytesWritten, ulFrequency);
                alSourceQueueBuffers(uiSource, 1, &uiBuffer);
            }
            iBuffersProcessed--;

        }
        alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
        if (iState != AL_PLAYING) {
            alGetSourcei(uiSource, AL_BUFFERS_QUEUED, &iQueuedBuffers);

            if (iQueuedBuffers) {
                alSourcePlay(uiSource);//buffers have data, play it
            } else {
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
    *stopped = true;
    return 0;
}



DSSoundManager::DSSoundManager(void) {
    alutInit(NULL, 0);

    //init mpg123 library
    if (MPG123_OK != (iMpg123_error = mpg123_init())) {
        printf("failed to init mpg123\n");
        return;
    }

    //loadSounds();
}


DSSoundManager::~DSSoundManager(void) {
    ClipMap::iterator it = clipMap.begin();
    while (it != clipMap.end()) {
        delete it->second;
        it++;
    }

    alutExit();
    clipMap.clear();
}

void DSSoundManager::addSound(unsigned int id, const char* fileName, bool loop) {
    Clip* clip = new Clip(fileName, loop);
    clipMap.insert(ClipMap::value_type(id, clip));
}

void DSSoundManager::stop(ALuint sourceIndex) {
    alGetError();
    if (!alIsSource(sourceIndex)) {
        return;
    }
    alSourceStop(sourceIndex);
    if (alError = alGetError() != AL_NO_ERROR) {
        DSSoundManager::displayALError("stop", alError);
    }
}

ALuint DSSoundManager::playSound(unsigned int id, float x, float y, float z, float vx , float vy , float vz) {
    ClipMap::iterator it = clipMap.find(id);
    if (it != clipMap.end()) {
        return (it->second)->play(x, y, z, vx, vy, vz);
    }
    return 0;
}


void DSSoundManager::changePosition(ALuint source, float x, float y, float z) {
    if (alIsSource(source)) {
        alSource3f(source, AL_POSITION, x, y, z);
    }

}

void DSSoundManager::setListenerPosition(ALfloat x, ALfloat y, ALfloat z) {
    ALCfloat listenerPos[] = {x, y, z};
    //ALCfloat listenerVel[]={0,0,0};
    //ALCfloat listenerOri[]={0,0,-1,0,1,0};
    alListenerfv(AL_POSITION, listenerPos);
    //alListenerfv(AL_VELOCITY,listenerVel);
    //alListenerfv(AL_ORIENTATION,listenerOri);
}



void DSSoundManager::displayALError(char* func, ALenum alError) {
    printf_s(func);
    printf_s(alGetString(alError));
}


SoundIDandSource DSSoundManager::defaultSoundGroup = {1, 0, 2, 2, 2};

void DSSoundManager::loadSounds() {
    ////背景音乐
    //   addSound(0, "data/sound/test.mp3");
    ////默认音效
    //   addSound(1, "data/sound/footsteps.wav");
    //addSound(2, "data/sound/hited.wav",false);
    tinyxml2::XMLDocument doc;
    doc.LoadFile("sounds.xml");
    auto root = doc.FirstChildElement();
    auto background = root->FirstChildElement("sound");
    backGroundSoundFile = background->GetText();
    backGroundSoundIndex = atoi(background->Attribute("id"));
    if (background != nullptr) {
        addSound(backGroundSoundIndex, backGroundSoundFile.c_str());
    }

    auto group = root->FirstChildElement("sound_group");
    for (; group != nullptr; group = group->NextSiblingElement("sound_group")) {
        SoundIDandSource* pSound;
        std::string id = group->Attribute("id");
        if (id == "default") {
            pSound = &defaultSoundGroup;
        } else {
            pSound = new SoundIDandSource(defaultSoundGroup);
        }
        auto sound = group->FirstChildElement("sound");
        for (; sound != nullptr; sound = sound->NextSiblingElement("sound")) {
            std::string soundType = sound->Attribute("type");
            if (soundType == "move") {
                pSound->_run = atoi(sound->Attribute("id"));
                addSound(pSound->_run, sound->GetText());
            }
            if (soundType == "pain") {
                pSound->_pain = atoi(sound->Attribute("id"));
                addSound(pSound->_pain, sound->GetText(), false);
            }
            if (soundType == "fight") {
                pSound->_fight = atoi(sound->Attribute("id"));
                addSound(pSound->_fight, sound->GetText(), false);
            }
            if (soundType == "dying") {
                pSound->_dying = atoi(sound->Attribute("id"));
                addSound(pSound->_dying, sound->GetText(), false);
            }
            soundgroups[id] = *pSound;
        }
    }

    //播放背景音乐
    playSound(0, 0, 0, 0);

}

