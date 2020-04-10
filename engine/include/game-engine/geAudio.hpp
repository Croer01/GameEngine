//
// Created by adria on 16/03/2019.
//

#ifndef GAMEENGINE_GEAUDIO_HPP
#define GAMEENGINE_GEAUDIO_HPP

#include <game-engine/api.hpp>

namespace GameEngine{
    class PUBLICAPI geAudio {
    public:
        virtual ~geAudio() = 0;
        virtual void muteAll(bool mute) = 0;
        virtual bool muteAll() const = 0;
    };
}


#endif //GAMEENGINE_GEAUDIO_HPP
