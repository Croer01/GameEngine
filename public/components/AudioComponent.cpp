//
// Created by adria on 27/12/2018.
//

#include <game-engine/components/AudioComponent.hpp>
#include "../private/audio/AudioEngine.hpp"

namespace GameEngine {
    void AudioComponent::init() {
        if (playOnInit_)
            play();
        setLooping(loopOnInit_);
    }

    PropertySetBase *AudioComponent::configureProperties() {
        auto *properties = new PropertySet<AudioComponent>(this);

        properties->add(new Property<AudioComponent, std::string>(
                "filePath",
                this,
                &AudioComponent::filepath,
                &AudioComponent::filepath,
                "",
                true));
        properties->add(new Property<AudioComponent, bool>(
                "playOnInit",
                this,
                &AudioComponent::playOnInit,
                &AudioComponent::playOnInit,
                false));
        properties->add(new Property<AudioComponent, bool>(
                "loopOnInit",
                this,
                &AudioComponent::loopOnInit,
                &AudioComponent::loopOnInit,
                false));

        return properties;
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
        if(path.empty())
            source_.reset();
        else
            source_ = Internal::AudioEngine::GetInstance().getAudio(path);
        filePath_ = path;
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
}