//
// Created by adria on 30/11/2018.
//

#include "AudioSource.hpp"
#include "../utils.hpp"
#include <exception>
#include <chrono>
#include <iostream>


AudioSource::AudioSource(const std::shared_ptr<AudioBuffer> &buffer) {
    currentChunk_ = 0;
    looping_ = false;
    buffer_ = buffer;
    streamRunning_ = false;

    alGenSources(1, &sourceId_);
    CheckAlError();

    alGenBuffers(AUDIOSOURCE_BUFFERS, streamBuffers_);
}

AudioSource::~AudioSource() {
    alDeleteSources(1, &sourceId_);
    alDeleteBuffers(AUDIOSOURCE_BUFFERS, streamBuffers_);
    streamRunning_ = false;
    streamThread_.join();
}

void AudioSource::play() {
    //prepare data to stream sound
    currentChunk_ = 0;

    //unqueue all buffers
    int buffersQueued = 0;
    alGetSourcei(sourceId_, AL_BUFFERS_QUEUED, &buffersQueued);

    ALuint *tmpBuffer = (ALuint *)malloc(buffersQueued * sizeof(ALuint));
    alSourceUnqueueBuffers(sourceId_, buffersQueued, tmpBuffer);
    free(tmpBuffer);

    // Fill all the buffers with audio data from buffer_
    bool needStream = true;
    for(int i = 0; i < AUDIOSOURCE_BUFFERS; i++)
    {
        if(!buffer_->fillBuffer(streamBuffers_[i], currentChunk_++)){
            needStream = false;
            break;
        }
        alSourceQueueBuffers(sourceId_, 1, &streamBuffers_[i]);
    }

    alSourcePlay(sourceId_);

    //start the thread in charge of streaming the audio
    if(needStream) {
        if (streamThread_.joinable())
            streamThread_.join();
        streamThread_ = std::thread(&AudioSource::processStream, this);
    }

    CheckAlError();
}

bool AudioSource::isPlaying() const {
    ALenum state;
    alGetSourcei(sourceId_, AL_SOURCE_STATE, &state);

    return state == AL_PLAYING;
}

void AudioSource::stop() {
    alSourceStop(sourceId_);
    streamRunning_ = false;
}

void AudioSource::setLooping(bool loop) {
    looping_ = loop;
}

void AudioSource::processStream() {
    using namespace std::chrono_literals;
    streamRunning_ = true;

    while (streamRunning_) {

        ALint state = AL_INITIAL;
        alGetSourcei(sourceId_, AL_SOURCE_STATE, &state);

        streamRunning_ = state == AL_PLAYING;

        ALint processedBuffers = 0;
        alGetSourcei(sourceId_, AL_BUFFERS_PROCESSED, &processedBuffers);

        while(processedBuffers > 0)
        {
            ALuint buffer;
            alSourceUnqueueBuffers(sourceId_, 1, &buffer);

            if(looping_) {
                bool isEndOfBuffer = !buffer_->fillBuffer(buffer, currentChunk_++);

                if(isEndOfBuffer) {
                    currentChunk_ = 0;
                    buffer_->fillBuffer(buffer, currentChunk_++);
                }
            }
            else{
                streamRunning_ = buffer_->fillBuffer(buffer, currentChunk_++);
            }

            alSourceQueueBuffers(sourceId_, 1, &buffer);
            processedBuffers--;
        }

        std::this_thread::sleep_for(100ms);
    }

    std::cout << "the stream finish successfully" << std::endl;
}
