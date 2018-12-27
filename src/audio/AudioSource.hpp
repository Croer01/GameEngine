//
// Created by adria on 30/11/2018.
//

#ifndef SPACEINVADERS_AUDIOSOURCE_HPP
#define SPACEINVADERS_AUDIOSOURCE_HPP


#include <AL/al.h>
#include <string>
#include <memory>
#include <sndfile.h>
#include "AudioBuffer.hpp"

class AudioSource {

    ALuint sourceId_;
    std::shared_ptr<AudioBuffer> buffer_;
public:
    explicit AudioSource(const std::shared_ptr<AudioBuffer> &buffer);
    ~AudioSource();

    void play() const;

protected:
};


#endif //SPACEINVADERS_AUDIOSOURCE_HPP
