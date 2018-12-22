//
// Created by adria on 29/11/2018.
//

#ifndef SPACEINVADERS_AUDIOENGINE_HPP
#define SPACEINVADERS_AUDIOENGINE_HPP


#include "../Singleton.hpp"
#include <Al/al.h>
#include <Al/alc.h>

class AudioEngine : public Singleton<AudioEngine> {
    ALCcontext *context_;
    ALCdevice *device_;
public:
    void init();

    virtual ~AudioEngine();
};


#endif //SPACEINVADERS_AUDIOENGINE_HPP
