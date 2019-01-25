//
// Created by adria on 29/11/2018.
//

#ifndef SPACEINVADERS_AUDIOENGINE_HPP
#define SPACEINVADERS_AUDIOENGINE_HPP


#include "../Singleton.hpp"
#include "AudioSource.hpp"
#include <Al/al.h>
#include <Al/alc.h>
#include <memory>
#include <map>

namespace GameEngine {
namespace Internal {
    class AudioEngine : public Singleton<AudioEngine> {
        ALCcontext *context_;
        ALCdevice *device_;
        std::map<std::string, std::shared_ptr<AudioBuffer>> buffers_;
    public:
        void init();

        std::shared_ptr<AudioSource> getAudio(const std::string &filename);

        virtual ~AudioEngine();
    };
}
}


#endif //SPACEINVADERS_AUDIOENGINE_HPP
