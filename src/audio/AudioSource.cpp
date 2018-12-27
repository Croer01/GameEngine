//
// Created by adria on 30/11/2018.
//

#include "AudioSource.hpp"
#include "../utils.hpp"
#include <exception>
#include <vector>


AudioSource::AudioSource(const std::shared_ptr<AudioBuffer> &buffer) {
    buffer_ = buffer;

    alGenSources(1, &sourceId_);
    CheckAlError();

    alSourcei(sourceId_, AL_BUFFER, buffer_->getId());
}

AudioSource::~AudioSource() {
    alDeleteSources(1, &sourceId_);
}

void AudioSource::play() const {
    alSourcePlay(sourceId_);
    CheckAlError();
}
