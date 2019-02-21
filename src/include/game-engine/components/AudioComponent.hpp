//
// Created by adria on 27/12/2018.
//

#ifndef SPACEINVADERS_AUDIOCOMPONENT_HPP
#define SPACEINVADERS_AUDIOCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include "../private/audio/AudioSource.hpp"
namespace GameEngine {
    class AudioComponent : public geComponent {
        std::string filePath_;
        std::shared_ptr<Internal::AudioSource> source_;
        bool playOnInit_;
        bool loopOnInit_;
    public:
        void init() override;

        //AudioComponent public API
        void play();

        bool isPlaying();

        void playOnInit(const bool &play);
        bool playOnInit() const;
        void loopOnInit(const bool &loop);
        bool loopOnInit() const;

        void stop();

        void setLooping(bool loop);

        void filepath(const std::string &path);
        std::string filepath() const;
    protected:
        PropertySetBase *configureProperties() override;

        geComponentRef instantiate() const override;
    };
}

#endif //SPACEINVADERS_AUDIOCOMPONENT_HPP
