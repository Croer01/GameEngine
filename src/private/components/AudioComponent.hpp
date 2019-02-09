//
// Created by adria on 27/12/2018.
//

#ifndef SPACEINVADERS_AUDIOCOMPONENT_HPP
#define SPACEINVADERS_AUDIOCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include "../audio/AudioSource.hpp"
namespace GameEngine {
namespace Internal {
    class AudioComponent : public geComponent {
        std::string filePath_;
        std::shared_ptr<AudioSource> source_;
        bool playOnInit_;
        bool loopOnInit_;
    public:
        void init() override;

        //AudioComponent public API
        void play();

        bool isPlaying();

        void stop();

        void setLooping(bool loop);
    };
}
}

#endif //SPACEINVADERS_AUDIOCOMPONENT_HPP
