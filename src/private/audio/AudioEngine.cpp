//
// Created by adria on 29/11/2018.
//

#include "AudioEngine.hpp"
#include "../utils.hpp"

namespace GameEngine {
namespace Internal {
    void AudioEngine::init() {
        device_ = alcOpenDevice(nullptr);
        if (!device_)
            CheckAlError();

        context_ = alcCreateContext(device_, nullptr);
        if (!alcMakeContextCurrent(context_))
            CheckAlError();
    }

    AudioEngine::~AudioEngine() {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context_);
        alcCloseDevice(device_);
    }

    std::shared_ptr<AudioSource> AudioEngine::getAudio(const std::string &filename) {
        std::shared_ptr<AudioBuffer> buffer = buffers_[filename];
        if(!buffer){
            buffer = std::make_shared<AudioBuffer>(filename);
            buffers_[filename] = buffer;
        }

        return std::make_shared<AudioSource>(buffer);
    }
}
}
