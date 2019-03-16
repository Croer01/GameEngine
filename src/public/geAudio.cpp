//
// Created by adria on 16/03/2019.
//

#include <game-engine/geAudio.hpp>
#include "../private/audio/AudioEngine.hpp"

namespace GameEngine{
    void geAudio::muteAll() {
        GameEngine::Internal::AudioEngine::GetInstance().muteAll(true);
    }

    void geAudio::unmuteAll() {
        GameEngine::Internal::AudioEngine::GetInstance().muteAll(false);
    }
}
