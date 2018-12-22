//
// Created by adria on 29/11/2018.
//

#include "AudioEngine.hpp"
#include "../utils.hpp"

void AudioEngine::init() {
    device_ = alcOpenDevice(nullptr);
    if (!device_)
        CheckAlError();

    context_ = alcCreateContext(device_, nullptr);
    if (!alcMakeContextCurrent(context_))
        CheckAlError();
}

AudioEngine::~AudioEngine() {
    alcCloseDevice(device_);
}
