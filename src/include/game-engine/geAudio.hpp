//
// Created by adria on 16/03/2019.
//

#ifndef SPACEINVADERS_GEAUDIO_HPP
#define SPACEINVADERS_GEAUDIO_HPP

#include <game-engine/api.hpp>

namespace GameEngine{
    class PUBLICAPI geAudio {
    public:
        void muteAll(bool mute);
        bool muteAll() const;
    };
}


#endif //SPACEINVADERS_GEAUDIO_HPP