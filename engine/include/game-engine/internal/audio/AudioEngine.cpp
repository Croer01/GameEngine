//
// Created by adria on 29/11/2018.
//

#include <algorithm>
#include "AudioEngine.hpp"
#include <game-engine/internal/utils.hpp>

namespace GameEngine {
namespace Internal {
    AudioEngine::AudioEngine() {
        mute_ = false;
    }

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

        const std::shared_ptr<AudioSource> &source = std::make_shared<AudioSource>(buffer, this);
        return source;
    }

    void AudioEngine::muteAll(bool mute) {
        mute_ = mute;
        if(mute) {
            for(AudioSource* source : sources_)
                source->stop();
        }
    }

    bool AudioEngine::muteAll() const {
        return mute_;
    }

    void AudioEngine::registerSource(AudioSource *source) {
        sources_.push_back(source);
    }

    void AudioEngine::unregisterSource(AudioSource *source) {
        auto it = std::find(sources_.begin(), sources_.end(), source);
        if (it != sources_.end())
            sources_.erase(it);
    }
}
}
