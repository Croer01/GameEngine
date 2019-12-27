//
// Created by adria on 16/03/2019.
//

#include <game-engine/geAudio.hpp>
#include "../private/audio/AudioEngine.hpp"

namespace GameEngine{
    void geAudio::muteAll(bool mute) {
        GameEngine::Internal::AudioEngine::GetInstance().muteAll(mute);
    }
    bool geAudio::muteAll() const {
        return GameEngine::Internal::AudioEngine::GetInstance().muteAll();
    }
}
