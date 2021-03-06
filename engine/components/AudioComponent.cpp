//
// Created by adria on 27/12/2018.
//

#include <game-engine/components/AudioComponent.hpp>
#include <game-engine/internal/audio/AudioEngine.hpp>
#include <game-engine/Game.hpp>

namespace GameEngine {
    void AudioComponent::init() {
        loadSource();
        if (playOnInit_)
            play();
        setLooping(loopOnInit_);
    }

    void AudioComponent::play() {
        stop();
        source_->play();
    }

    bool AudioComponent::isPlaying() {
        return source_->isPlaying();
    }

    void AudioComponent::stop() {
        if (isPlaying())
            source_->stop();
    }

    void AudioComponent::setLooping(bool loop) {
        source_->setLooping(loop);
    }

    void AudioComponent::filepath(const std::string &path) {
        filePath_ = path;
        if(path.empty())
            source_.reset();
        else
            loadSource();
    }

    std::string AudioComponent::filepath() const {
        return filePath_;
    }

    void AudioComponent::playOnInit(const bool &play){
        playOnInit_ = play;
    }

    void AudioComponent::loopOnInit(const bool &loop){
        loopOnInit_ = loop;
    }

    bool AudioComponent::playOnInit() const {
        return playOnInit_;
    }

    bool AudioComponent::loopOnInit() const {
        return loopOnInit_;
    }

PropertySetBase *AudioComponent::getProperties() const
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

void AudioComponent::loadSource()
{
    if(gameObject() != nullptr)
    {
        auto game = dynamic_cast<Game*>(gameObject()->game());
        if(game)
            source_ = game->audioEngine()->getAudio(filePath_);
    }
}

}