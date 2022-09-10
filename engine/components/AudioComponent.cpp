//
// Created by adria on 27/12/2018.
//

#include <game-engine/components/AudioComponent.hpp>
#include <game-engine/Game.hpp>

namespace GameEngine {
    void AudioComponent::init() {
        setPropertyObserver<std::string>("filePath",[this](){
            std::string path = getPropertyValue<std::string>("filePath");
            if(path.empty())
                source_.reset();
            else
                loadSource(path);
        });

        loadSource(getPropertyValue<std::string>("filePath"));
        if (getPropertyValue<bool>("playOnInit"))
            play();
        setLooping(getPropertyValue<bool>("loopOnInit"));
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

void AudioComponent::loadSource(const std::string &filePath)
{
    if(gameObject() != nullptr)
    {
        Game *game = gameObject()->game();
        if(game)
            source_ = game->audioEngine()->getAudio(filePath);
    }
}

}