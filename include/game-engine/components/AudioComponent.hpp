//
// Created by adria on 27/12/2018.
//

#ifndef SPACEINVADERS_AUDIOCOMPONENT_HPP
#define SPACEINVADERS_AUDIOCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/properties/PropertiesRegister.hpp>
#include "../private/audio/AudioSource.hpp"
namespace GameEngine {
    class PUBLICAPI AudioComponent : public geComponentInstantiable<AudioComponent> {
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
    };

    template <>
    class PropertyInstantiator<AudioComponent>
    {
    public:
        static std::string getTargetName()
        {
            return "AudioComponent";
        }

        static PropertySetBase* instantiate()
        {
            auto *properties = new PropertySet<AudioComponent>();

            properties->add(new Property<AudioComponent, std::string>(
                "filePath",
                &AudioComponent::filepath,
                &AudioComponent::filepath,
                "",
                true));
            properties->add(new Property<AudioComponent, bool>(
                "playOnInit",
                &AudioComponent::playOnInit,
                &AudioComponent::playOnInit,
                false));
            properties->add(new Property<AudioComponent, bool>(
                "loopOnInit",
                &AudioComponent::loopOnInit,
                &AudioComponent::loopOnInit,
                false));

            return properties;
        }
    };
}

#endif //SPACEINVADERS_AUDIOCOMPONENT_HPP
