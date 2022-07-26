//
// Created by adria on 30/11/2018.
//

#ifndef SPACEINVADERS_AUDIOSOURCE_HPP
#define SPACEINVADERS_AUDIOSOURCE_HPP


#include <game-engine/api.hpp>
#include <al.h>
#include <string>
#include <memory>
#include <sndfile.h>
#include <thread>
#include <atomic>
#include <game-engine/internal/audio/AudioBuffer.hpp>

namespace GameEngine {
namespace Internal {
    #define AUDIOSOURCE_BUFFERS 4

    class AudioEngine;

    class PUBLICAPI AudioSource {

        ALuint sourceId_;
        std::shared_ptr<AudioBuffer> buffer_;
        ALuint streamBuffers_[AUDIOSOURCE_BUFFERS];
        int currentChunk_;
        bool looping_;
        std::thread streamThread_;
        std::atomic<bool> streamRunning_;
        AudioEngine *engine_;

        void processStream();
    public:
        AudioSource(const std::shared_ptr<AudioBuffer> &buffer, AudioEngine *engine);
        ~AudioSource();

        void play();
        bool isPlaying() const;
        void stop();
        void setLooping(bool loop);
    };
}
}


#endif //SPACEINVADERS_AUDIOSOURCE_HPP
