//
// Created by adria on 29/11/2018.
//

#ifndef SPACEINVADERS_AUDIOENGINE_HPP
#define SPACEINVADERS_AUDIOENGINE_HPP


#include <game-engine/internal/audio/AudioSource.hpp>
#include <al.h>
#include <alc.h>
#include <memory>
#include <map>
#include <game-engine/geAudio.hpp>

namespace GameEngine {
namespace Internal {
    class AudioEngine : public geAudio {
        ALCcontext *context_;
        ALCdevice *device_;
        std::map<std::string, std::shared_ptr<AudioBuffer>> buffers_;
        std::vector<AudioSource*> sources_;
        bool mute_;
    public:
        AudioEngine();
        void init();
        std::shared_ptr<AudioSource> getAudio(const std::string &filename);

        void registerSource(AudioSource *source);
        void unregisterSource(AudioSource *source);

        virtual void muteAll(bool mute);
        virtual bool muteAll() const;

        virtual ~AudioEngine();
    };
}
}


#endif //SPACEINVADERS_AUDIOENGINE_HPP
