//
// Created by adria on 27/12/2018.
//

#ifndef SPACEINVADERS_AUDIOCOMPONENT_HPP
#define SPACEINVADERS_AUDIOCOMPONENT_HPP

#include <game-engine/api.hpp>
#include <game-engine/geComponent.hpp>
#include <game-engine/internal/audio/AudioSource.hpp>
#include <game-engine/components/ComponentData.hpp>

namespace GameEngine
{
class PUBLICAPI AudioComponentData : public ComponentData
{
public:
    AudioComponentData()
    {
        createProperty<std::string>("filePath","", true);
        createProperty<bool>("playOnInit", false);
        createProperty<bool>("loopOnInit", false);
    }
};

    class PUBLICAPI AudioComponent : public geComponentInstantiable<AudioComponent, AudioComponentData> {
        std::shared_ptr<Internal::AudioSource> source_;

        void loadSource(const std::string &filePath);
    public:
        void init() override;

        //AudioComponent public API
        void play();

        bool isPlaying();

        void stop();

        void setLooping(bool loop);
    };
}

#endif //SPACEINVADERS_AUDIOCOMPONENT_HPP
