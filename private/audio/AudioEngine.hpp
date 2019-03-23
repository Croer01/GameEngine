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
        std::vector<AudioSource*> sources_;
        bool mute_;
    public:
        void init();

    protected:
        void onCreateInstance() override;

    public:
        std::shared_ptr<AudioSource> getAudio(const std::string &filename);

        void registerSource(AudioSource *source);
        void unregisterSource(AudioSource *source);

        void muteAll(bool mute);
        bool muteAll() const;

        virtual ~AudioEngine();
    };
}
}


#endif //SPACEINVADERS_AUDIOENGINE_HPP
