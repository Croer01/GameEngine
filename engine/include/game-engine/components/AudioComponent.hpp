//
// Created by adria on 27/12/2018.
//

#ifndef SPACEINVADERS_AUDIOCOMPONENT_HPP
#define SPACEINVADERS_AUDIOCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include "../private/audio/AudioSource.hpp"

namespace GameEngine {
    class PUBLICAPI AudioComponent : public geComponentInstantiable<AudioComponent> {
        std::string filePath_;
        std::shared_ptr<Internal::AudioSource> source_;
        bool playOnInit_;
        bool loopOnInit_;
    public:
        void init() override;
        virtual PropertySetBase *getProperties() const;

        //AudioComponent public API
        void play();

        bool isPlaying();

        void playOnInit(const bool &play);
        bool playOnInit() const;
        void loopOnInit(const bool &loop);
        bool loopOnInit() const;

        void stop();

        void setLooping(bool loop);

        void loadSource();

        void filepath(const std::string &path);
        std::string filepath() const;
    };
}

#endif //SPACEINVADERS_AUDIOCOMPONENT_HPP
