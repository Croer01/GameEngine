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
    buffer_ = buffer;

    alGenSources(1, &sourceId_);
    CheckAlError();

    alGenBuffers(AUDIOSOURCE_BUFFERS, streamBuffers_);
}

AudioSource::~AudioSource() {
    alDeleteSources(1, &sourceId_);
    alDeleteBuffers(AUDIOSOURCE_BUFFERS, streamBuffers_);
}

void AudioSource::play() {
    //prepare data to stream sound
    currentChunk_ = 0;

    int buffersQueued = 0;
    alGetSourcei(sourceId_, AL_BUFFERS_QUEUED, &buffersQueued);

    ALuint *tmpBuffer = (ALuint *)malloc(buffersQueued * sizeof(ALuint));
    alSourceUnqueueBuffers(sourceId_, buffersQueued, tmpBuffer);
    free(tmpBuffer);

    // Fill all the buffers with audio data from buffer_
    for(int i = 0; i < AUDIOSOURCE_BUFFERS; i++)
    {
        if(currentChunk_ >= buffer_->getChunkCount())
            break;

        buffer_->fillBuffer(streamBuffers_[i], currentChunk_++);
        alSourceQueueBuffers(sourceId_, 1, &streamBuffers_[i]);
    }

    alSourcePlay(sourceId_);

    //start the thread in charge of streaming the audio
    if(streamThread_.joinable())
        streamThread_.join();
    streamThread_ = std::thread(&AudioSource::processStream, this);
    // 1 - start queue first OpenAL buffers from buffer_.
    // 2 - play audio.
    // 3 - start new thread to queue/unqueue OpenAlBuffers from buffer_.
    // 4 - If source stops there are 2 options:
    //     - If loop is enable, continue from 1 (modifying some steps but quiet similar).
    //     - If loop is disable, destroy thread and prepare for a future play action.

    CheckAlError();
}

bool AudioSource::isPlaying() const {
    ALenum state;
    alGetSourcei(sourceId_, AL_SOURCE_STATE, &state);

    return state == AL_PLAYING;
}

void AudioSource::stop() {
    alSourceStop(sourceId_);
}

void AudioSource::setLooping(bool loop) {
//    alSourcei(sourceId_, AL_LOOPING, loop? 1 : 0);
}

void AudioSource::processStream() {
    using namespace std::chrono_literals;
    bool running = true;

    while (running) {

        ALint state = AL_INITIAL;
        alGetSourcei(sourceId_, AL_SOURCE_STATE, &state);

        running = state == AL_PLAYING && currentChunk_ < buffer_->getChunkCount();

        ALint processedBuffers = 0;
        alGetSourcei(sourceId_, AL_BUFFERS_PROCESSED, &processedBuffers);

        while(processedBuffers > 0)
        {
            if(currentChunk_ >= buffer_->getChunkCount())
                break;

            ALuint buffer;
            alSourceUnqueueBuffers(sourceId_, 1, &buffer);

            buffer_->fillBuffer(buffer, currentChunk_++);
            alSourceQueueBuffers(sourceId_, 1, &buffer);
            processedBuffers--;
        }

        std::this_thread::sleep_for(100ms);
    }

    std::cout << "the stream finish successfully chunks: " << std::to_string(currentChunk_) << "/" << std::to_string(buffer_->getChunkCount()) << std::endl;
}
