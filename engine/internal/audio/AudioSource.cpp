//
// Created by adria on 30/11/2018.
//

#include <game-engine/internal/audio/AudioSource.hpp>
#include <game-engine/internal/utils.hpp>
#include <game-engine/internal/audio/AudioEngine.hpp>
#include <exception>
#include <chrono>
#include <iostream>
#include <assert.h>

namespace GameEngine {
namespace Internal {
    AudioSource::AudioSource(const std::shared_ptr<AudioBuffer> &buffer, AudioEngine *engine) {
        currentChunk_ = 0;
        looping_ = false;
        buffer_ = buffer;
        streamRunning_ = false;

        alGenSources(1, &sourceId_);
        CheckAlError();

        alGenBuffers(AUDIOSOURCE_BUFFERS, streamBuffers_);

        assert(engine != nullptr);
        engine_ = engine;
        engine_->registerSource(this);
    }

    AudioSource::~AudioSource() {
        alDeleteSources(1, &sourceId_);
        alDeleteBuffers(AUDIOSOURCE_BUFFERS, streamBuffers_);
        streamRunning_ = false;
        if(streamThread_.joinable())
            streamThread_.join();

        assert(engine_ != nullptr);
        engine_->unregisterSource(this);
    }

    void AudioSource::play() {
        assert(engine_ != nullptr);
        if(engine_->muteAll())
            return;

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
        // if I needn't stream and looping is enable, use OpenAl loop system
        } else if(looping_) {
            alSourcei(sourceId_, AL_LOOPING, 1);
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
        //always disable the OpenAl loop system to avoid weird playbacks
        alSourcei(sourceId_, AL_LOOPING, 0);
    }

    void AudioSource::setLooping(bool loop) {
        looping_ = loop;
    }

    void AudioSource::processStream() {
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

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::cout << "the stream finish successfully" << std::endl;
    }
}
}
